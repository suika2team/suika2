/*
 * OpenNovel
 * Copyright (C) 2024, OpenNovel.Org. All rights reserved.
 */

/*
 * Text to speech.
 */

#ifndef OPENNOVEL_TTS_SAPI_H
#define OPENNOVEL_TTS_SAPI_H

#ifdef __cplusplus
extern "C" {
#endif

void InitSAPI(void);
void SpeakSAPI(const wchar_t *text);

#ifdef __cplusplus
};
#endif

#endif
