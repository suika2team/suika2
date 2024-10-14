/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

#ifndef WINPRO_WINPRO_H
#define WINPRO_WINPRO_H

#include <windows.h>

int App_GetDpiForWindow(HWND hWnd);
BOOL App_IsEnglishMode(void);

BOOL App_IsRunning(void);
void App_SetExecLineChanged(BOOL bVal);
void App_SetChangedLine(int nLine);
void App_SetScriptOpened(BOOL bVal);

const wchar_t *conv_utf8_to_utf16(const char *utf8_message);
const char *conv_utf16_to_utf8(const wchar_t *utf16_message);

#endif
