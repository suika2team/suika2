#ifndef OPENNOVEL_DIALOG_DIALOG_H
#define OPENNOVEL_DIALOG_DIALOG_H

#include <windows.h>
#include <commctrl.h>

#define DIALOG_TITLE	L"OpenNovel"

/* winpro.c */
extern BOOL bEnglish;
extern HWND hWndMain;
const wchar_t *conv_utf8_to_utf16(const char *utf8_message);
const char *conv_utf16_to_utf8(const wchar_t *utf16_message);
const wchar_t *SelectFile(const char *pszDir);

/* dialog_*.c */
BOOL CALLBACK DlgMessageWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgBgWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgChWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgChchWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgMusicWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgSoundWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgVolumeWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgVideoWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

#endif
