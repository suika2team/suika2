/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * ScriptView
 */

#ifndef WINPRO_SCRIPTVIEW_H
#define WINPRO_SCRIPTVIEW_H

#include <windows.h>

HWND ScriptView_Create(HWND hWndParent);
void ScriptView_OnChange(void);
void ScriptView_ApplyFont(void);
int ScriptView_GetCursorPosition(void);
int ScriptView_GetSelectedLen(void);
void ScriptView_SetCursorPosition(int nCursor);
void ScriptView_SetSelectedRange(int nLineStart, int nLineLen);
int ScriptView_GetCursorLine(void);
wchar_t *ScriptView_GetText(void);
void ScriptView_SetTextColorForAllLines(void);
void ScriptView_SetTextColorForLine(const wchar_t *pText, int nLineStartCR, int nLineStartCRLF, int nLineLen);
void ScriptView_ClearFormatAll(void);
void ScriptView_ClearBackgroundColorAll(void);
void ScriptView_SetBackgroundColorForNextExecuteLine(void);
void ScriptView_SetBackgroundColorForCurrentExecuteLine(void);
void ScriptView_GetLineStartAndLength(int nLine, int *nLineStart, int *nLineLen);
void ScriptView_SetTextColorForSelectedRange(COLORREF cl);
void ScriptView_SetBackgroundColorForSelectedRange(COLORREF cl);
void ScriptView_AutoScroll(void);
void ScriptView_GetLineStartAndLength(int nLine, int *nLineStart, int *nLineLen);
BOOL ScriptView_SearchNextError(int nStart, int nEnd);
void ScriptView_SetTextByScriptModel(void);
void ScriptView_UpdateScriptModelFromText(void);
void ScriptView_InsertText(const wchar_t *pLine, ...);
void ScriptView_InsertTextAtEnd(const wchar_t *pszText);
void ScriptView_UpdateTheme(void);
void ScriptView_DelayedHighligth(void);

BOOL ScriptView_IsDarkMode(void);
void ScriptView_SetDarkMode(BOOL bEnabled);
BOOL ScriptView_IsHighlightMode(void);
void ScriptView_SetHighlightMode(BOOL bEnabled);
const wchar_t *ScriptView_GetFontName(void);
void ScriptView_SetFontName(const wchar_t *pwszFontName);
int ScriptView_GetFontSize(void);
void ScriptView_SetFontSize(int nSize);

void ScriptView_UpdateTimeStamp(void);

void __stdcall ScriptView_OnFormatTimer(HWND hWnd, UINT nID, UINT_PTR uTime, DWORD dwParam);

#endif
