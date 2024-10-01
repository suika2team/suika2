/* -*- tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, OpenNovel.Org. All rights reserved.
 */

/*
 * OpenNovel HAL Implementation for ALSA
 */

/* OpenNovel Base */
#if !defined(USE_QT)
#include "../opennovel.h"
#else
#include "opennovel.h"
#endif

/* POSIX */
#include <pthread.h>

/* ALSA */
#include <alsa/asoundlib.h>

/*
 * Format
 */
#define SAMPLING_RATE	(44100)
#define CHANNELS	(2)
#define DEPTH		(16)
#define FRAME_SIZE	(4)

/*
 * Sound Buffer
 */
#define BUF_FRAMES		(SAMPLING_RATE / 8)
#define BUF_SIZE		(BUF_FRAMES * FRAME_SIZE)
#define PERIODS			(4)
#define PERIOD_FRAMES		(BUF_FRAMES / PERIODS)
#define PERIOD_SIZE		(BUF_SIZE / PERIODS)
#define PERIOD_FRAMES_PAD	((PERIOD_SIZE + 63) / 64 * 64 - PERIOD_SIZE)

/*
 * Stream Data
 */

/* ALSA Devices */
static snd_pcm_t *pcm[MIXER_STREAMS];

/* Input Streams */
static struct wave *wave[MIXER_STREAMS];

/* Sound Threads */
static pthread_t thread[MIXER_STREAMS];

/* Mutex Objects (mutually exclude between the main thread and a sound thread) */
static pthread_mutex_t mutex[MIXER_STREAMS];

/* Exit Requst */
static bool exit_req;

/* Buffers */
SIMD_ALIGNED_MEMBER(static uint32_t period_buf[MIXER_STREAMS][PERIOD_FRAMES + PERIOD_FRAMES_PAD]);

/* Volume Values */
static float volume[MIXER_STREAMS];

/* Finish Flags */
static bool finish[MIXER_STREAMS];

/*
 * Forward Declarations
 */
static bool init_pcm(int n);
static void *sound_thread(void *p);
static bool playback_period(int n);
static void scale_samples(uint32_t *buf, int frames, float vol);

/*
 * Initialize ALSA.
 */
bool init_asound(void)
{
	int n, ret;

	exit_req = false;

	for (n = 0; n < MIXER_STREAMS; n++) {
		/* Initialize per stream data. */
		pcm[n] = NULL;
		wave[n] = NULL;
		volume[n] = 1.0f;
		finish[n] = false;

		/* Initialize a device. */
		if (!init_pcm(n))
			return false;

		/* Create a mutex object. */
		pthread_mutex_init(&mutex[n], NULL);

		/* Start a sound thread. */
		ret = pthread_create(&thread[n], NULL, sound_thread, (void *)(intptr_t)n);
		if (ret != 0) {
			pthread_mutex_unlock(&mutex[n]);
			return false;
		}
	}

	return true;
}

/*
 * Cleanup ALSA.
 */
void cleanup_asound(void)
{
	void *p1;
	int n;

	exit_req = true;

	for (n = 0; n < MIXER_STREAMS; n++) {
		if (pcm[n] == NULL)
			continue;

		stop_sound(n);

		/* Wait for an exit of the sound thread. */
		pthread_join(thread[n], &p1);

		/* Close a device. */
		if (pcm[n] != NULL)
			snd_pcm_close(pcm[n]);

		/* Destroy a mutex. */
		pthread_mutex_destroy(&mutex[n]);
	}

	/* Free caches for Valgrind check. */
	snd_config_update_free_global();
}

/*
 * Start sound playback.
 */
bool play_sound(int n, struct wave *w)
{
	assert(n < MIXER_STREAMS);
	assert(w != NULL);

	/* If ALSA is not available, just return. */
	if (pcm[n] == NULL)
		return true;

	pthread_mutex_lock(&mutex[n]);
	{
		/* Drop the current buffer. */
		snd_pcm_drop(pcm[n]);

		/* Set a PCM stream. */
		wave[n] = w;

		/* Reset a finish flag. */
		finish[n] = false;
	}
	pthread_mutex_unlock(&mutex[n]);

	return true;
}

/*
 * Stop sound playback on a stream.
 */
bool stop_sound(int n)
{
	assert(n < MIXER_STREAMS);

	/* If ALSA is not available, just return. */
	if (pcm[n] == NULL)
		return true;

	pthread_mutex_lock(&mutex[n]);
	{
		if (wave[n] != NULL) {
			/* Cancel playback status. */
			wave[n] = NULL;

			/* Stop an in-flight buffer. */
			snd_pcm_drop(pcm[n]);
		}
	}
	pthread_mutex_unlock(&mutex[n]);

	return true;
}

/*
 * Set a sound volume for a stream.
 */
bool set_sound_volume(int n, float vol)
{
	assert(n < MIXER_STREAMS);
	assert(vol >= 0 && vol <= 1.0f);

	volume[n] = vol;

	/* For relaxed consistencies. Not needed for x86 and arm processors. */
	__sync_synchronize();

	return true;
}

/*
 * Check if a sound stream is finished.
 */
bool is_sound_finished(int n)
{
	/* If ALSA is not available, just return. */
	if (pcm[n] == NULL)
		return true;

	/* For relaxed consistencies. Not needed for x86 and arm processors. */
	__sync_synchronize();

	if (!finish[n])
		return false;

	return true;
}

/* Initialize a device for a stream. */
static bool init_pcm(int n)
{
	/* Open a device. */
	int ret;
	ret = snd_pcm_open(&pcm[n], "default", SND_PCM_STREAM_PLAYBACK, 0);
	if (ret < 0) {
		log_api_error("snd_pcm_open");
		return false;
	}

	/* Set the format (44.1kHz, stereo, 16-bit signed little endian) */
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;
	snd_pcm_hw_params_alloca(&params);
	ret = snd_pcm_hw_params_any(pcm[n], params);
	if (ret < 0) {
		log_api_error("snd_pcm_hw_params_any");
		return false;
	}
	if (snd_pcm_hw_params_set_access(pcm[n], params,
					 SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
		log_api_error("snd_pcm_hw_params_set_access");
		return false;
	}
	if (snd_pcm_hw_params_set_format(pcm[n], params, SND_PCM_FORMAT_S16_LE) < 0) {
		log_api_error("snd_pcm_hw_params_set_format");
		return false;
	}
	if (snd_pcm_hw_params_set_rate(pcm[n], params, SAMPLING_RATE, 0) < 0) {
		log_api_error("snd_pcm_hw_params_set_rate");
		return false;
	}
	if (snd_pcm_hw_params_set_channels(pcm[n], params, 2) < 0) {
		log_api_error("snd_pcm_hw_params_set_channels");
		return false;
	}
	if (snd_pcm_hw_params_set_periods(pcm[n], params, PERIODS, 0) < 0) {
		log_api_error("snd_pcm_hw_params_set_periods");
		return false;
	}
	if (snd_pcm_hw_params_set_buffer_size(pcm[n], params, BUF_FRAMES) < 0) {
		frames = BUF_FRAMES;
		if (snd_pcm_hw_params_set_buffer_size_near(pcm[n], params, &frames) < 0) {
			log_api_error("snd_pcm_hw_params_set_buffer_size_near");
			return false;
		}
	}
	if (snd_pcm_hw_params(pcm[n], params) < 0) {
		log_api_error("snd_pcm_hw_params");
		return false;
	}

	return true;
}

/*
 * Sound Threads
 */

/* The entrypoint of a per-device sound thread. */
static void *sound_thread(void *p)
{
	int n = (int)(intptr_t)p;

	while (!exit_req) {
		/* Run a playback loop. */
		while (playback_period(n)) {
#if defined(__linux__)
			/*
			 * [Important]
			 *  - We execute context-switch explicitly on Linux by sleep(0).
			 *  - If we do not call sleep(0), the main thread stops.
			 *  - We have confirmed this issue on Linux 4.4.0 x86_64.
			 *  - As we understand, sched_yield() does not solve this issue.
			 */
			sleep(0);
#elif defined(__NetBSD__)
			/*
			 * [Important]
			 *  - We execute context-switch explicitly on NetBSD by sched_yield().
			 *  - If we do not call sched_yield(), the main thread cannot acquire a mutex.
			 *  - We have confirmed this issue on NetBSD 9.1 amd64.
			 *  - As we understand, sleep(0) does not solve this issue.
			 */
			sched_yield();
#endif
		}
	}

	return (void *)0;
}

/* Write to a buffer. */
static bool playback_period(int n)
{
	int size;

	pthread_mutex_lock(&mutex[n]);
	{
		/* Return false if the main thread stopped a playback. */
		if (wave[n] == NULL) {
			pthread_mutex_unlock(&mutex[n]);
			return false;
		}

		/* Get PCM samples. */
		size = get_wave_samples(wave[n], period_buf[n], PERIOD_FRAMES);

		/* Fill the remaining samples by zeros for a case where we have reached an end-of-stream. */
		if (size < PERIOD_FRAMES)
			memset(period_buf[n] + size, 0, (size_t)(PERIOD_FRAMES - size) * FRAME_SIZE);

		/* Scale samples by a volume value. */
		scale_samples(period_buf[n], PERIOD_FRAMES, volume[n]);

		/* Write to the device (repeat while under-running) */
		while (snd_pcm_writei(pcm[n], period_buf[n], PERIOD_FRAMES) < 0)
			snd_pcm_prepare(pcm[n]);

		/* Return false if we reached an end-of-stream. */
		if (is_wave_eos(wave[n])) {
			wave[n] = NULL;
			finish[n] = true;
			pthread_mutex_unlock(&mutex[n]);
			return false;
		}
	}
	pthread_mutex_unlock(&mutex[n]);

	return true;
}

/* Apply a volume value. */
static void scale_samples(uint32_t *buf, int frames, float vol)
{
	float scale;
	uint32_t frame;
	int32_t il, ir;	/* intermediate L/R */
	int16_t sl, sr;	/* source L/R*/
	int16_t dl, dr;	/* destination L/R */
	int i;

	/* For relaxed consistencies. Not needed for x86 and arm processors. */
	__sync_synchronize();

	/* Convert a scale factor to an exponential value. */
	scale = (powf(10.0f, vol) - 1.0f) / (10.0f - 1.0f);

	/* Scale samples. */
	for (i = 0; i < frames; i++) {
		frame = buf[i];

		sl = (int16_t)(uint16_t)frame;
		sr = (int16_t)(uint16_t)(frame >> 16);

		il = (int)(sl * scale);
		ir = (int)(sr * scale);

		il = il > 32767 ? 32767 : il;
		il = il < -32768 ? -32768 : il;
		ir = ir > 32767 ? 32767 : ir;
		ir = ir < -32768 ? -32768 : ir;

		dl = (int16_t)il;
		dr = (int16_t)ir;

		buf[i] = ((uint32_t)(uint16_t)dl) | (((uint32_t)(uint16_t)dr) << 16);
	}
}
