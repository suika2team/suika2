/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * DirectShow video playback
 */

#ifndef OPENNOVEL_DSVIDEO_H
#define OPENNOVEL_DSVIDEO_H

#include "../opennovel.h"

#include <windows.h>

#define WM_GRAPHNOTIFY	(WM_APP + 13)

BOOL DShowPlayVideo(HWND hWnd, const char *pszFileName, int nOfsX, int nOfsY, int nWidth, int nHeight);
VOID DShowStopVideo(void);
BOOL DShowProcessEvent(void);

#endif
