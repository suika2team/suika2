/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, OpenNovel.Org. All rights reserved.
 */

/*
 * DirectSound Output (DirectSound 5.0)
 */

#ifndef OPENNOVEL_DSOUND_H
#define OPENNOVEL_DSOUND_H

#include <windows.h>

BOOL DSInitialize(HWND hWnd);
VOID DSCleanup();

#endif
