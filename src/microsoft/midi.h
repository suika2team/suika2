/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * MIDI playback
 */

#ifndef OPENNOVEL_D3D_H
#define OPENNOVEL_D3D_H

#include "../opennovel.h"

#include <windows.h>

void init_midi(void);
void cleanup_midi(void);
bool play_midi(const char *dir, const char *fname);
void stop_midi(void);
void sync_midi(void);

#endif
