#ifndef OPENNOVEL_DIALOG_DIALOG_H
#define OPENNOVEL_DIALOG_DIALOG_H

#include <windows.h>

/* winpro.c */
extern BOOL bEnglish;
extern HWND hWndMain;
const wchar_t *conv_utf8_to_utf16(const char *utf8_message);
const char *conv_utf16_to_utf8(const wchar_t *utf16_message);
const wchar_t *SelectFile(const char *pszDir);

/* dialog_*.c */
BOOL CALLBACK DlgBgWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

#endif
