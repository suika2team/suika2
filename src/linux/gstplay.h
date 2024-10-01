/* -*- c-basic-offset: 2; indent-tabs-mode: nil; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, OpenNovel.Org. All rights reserved.
 */

/*
 * GStreamer video playback
 */

#ifndef OPENNOVEL_GSTPLAY_H
#define OPENNOVEL_GSTPLAY_H

#include <X11/Xlib.h>

void
gstplay_init (int argc, char *argv[]);

void
gstplay_play (const char *fname, Window window);

void
gstplay_stop (void);

int
gstplay_is_playing (void);

void
gstplay_loop_iteration (void);

#endif
