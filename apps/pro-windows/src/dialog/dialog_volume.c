/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * Dialog procedure for the @volume command.
 */

#include "dialog.h"
#include "resource.h"

#include "opennovel.h"

static VOID OnInit(HWND hWnd)
{
	HWND hWndTrack;
	const char *pszText;
	float fVolume, fTime;

	UNUSED_PARAMETER(hWnd);

	/* Track */
	hWndTrack = GetDlgItem(hWnd, IDC_COMBO_TRACK);
	SendMessage(hWndTrack, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(bEnglish ? L"Music (BGM)" : L"音楽(BGM)"));
	SendMessage(hWndTrack, (UINT)CB_ADDSTRING, (WPARAM)1, (LPARAM)(bEnglish ? L"Sound (SE)" : L"音声(SE)"));
	SendMessage(hWndTrack, (UINT)CB_ADDSTRING, (WPARAM)2, (LPARAM)(bEnglish ? L"Voice (CV)" : L"ボイス"));
	pszText = get_string_param(VOLUME_PARAM_STREAM);
	if (strcmp(pszText, "bgm") == 0)
		SendMessage(hWndTrack, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	else if (strcmp(pszText, "se") == 0)
		SendMessage(hWndTrack, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);
	else
		SendMessage(hWndTrack, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

	/* Volume */
	pszText = get_string_param(VOLUME_PARAM_VOL);
	if (strcmp(pszText, "") == 0)
		pszText = "0";
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_VOLUME), conv_utf8_to_utf16(pszText));
	fVolume = (float)atof(pszText);
	SendMessage(GetDlgItem(hWnd, IDC_TRACK_VOLUME), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(int)(fVolume * 100.0f));

	/* Time */
	pszText = get_string_param(VOLUME_PARAM_SPAN);
	if (strcmp(pszText, "") == 0)
		pszText = "0";
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), conv_utf8_to_utf16(pszText));
	fTime = (float)atof(pszText);
	SendMessage(GetDlgItem(hWnd, IDC_TRACK_TIME), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(int)(fTime * 10.0f));
}

static VOID OnCommand(HWND hWnd, UINT nID)
{
	if (nID == IDC_TRACK_VOLUME)
	{
		int nPos;
		wchar_t wszText[128];
		nPos = SendMessage(GetDlgItem(hWnd, IDC_TRACK_VOLUME), TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		swprintf(wszText, sizeof(wszText) / sizeof(wchar_t), L"%0.1f", nPos / 100.0);
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_VOLUME), wszText);
		return;
	}

	if (nID == IDC_TRACK_TIME)
	{
		int nPos;
		wchar_t wszText[128];
		nPos = SendMessage(GetDlgItem(hWnd, IDC_TRACK_TIME), TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		swprintf(wszText, sizeof(wszText) / sizeof(wchar_t), L"%0.1f", nPos / 10.0);
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), wszText);
		return;
	}
}

static BOOL OnFinish(HWND hWnd)
{
	wchar_t wszText[1024];
	char szCmd[4096];
	int nIndex;

	strncpy(szCmd, "@volume", sizeof(szCmd) - 1);

	/* Position */
	nIndex = SendMessage(GetDlgItem(hWnd, IDC_COMBO_TRACK), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	strncat(szCmd, " track=", sizeof(szCmd) - 1);
	if (nIndex == 0)
		strncat(szCmd, "bgm", sizeof(szCmd) - 1);
	else if (nIndex == 1)
		strncat(szCmd, "se", sizeof(szCmd) - 1);
	else if (nIndex == 2)
		strncat(szCmd, "voice", sizeof(szCmd) - 1);

	/* Volume */
	GetWindowText(GetDlgItem(hWnd, IDC_TEXT_VOLUME), wszText, sizeof(wszText) / sizeof(wchar_t));
	if (wcscmp(wszText, L"") == 0)
		wcscpy(wszText, L"0");
	strncat(szCmd, " vol=", sizeof(szCmd) - 1);
	strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);

	/* Time */
	GetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), wszText, sizeof(wszText) / sizeof(wchar_t));
	if (wcscmp(wszText, L"") == 0)
		wcscpy(wszText, L"0");
	strncat(szCmd, " t=", sizeof(szCmd) - 1);
	strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);

	/* Update the script model. */
	update_script_line(get_expanded_line_num(), szCmd);

	return TRUE;
}

BOOL CALLBACK DlgVolumeWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	UINT nID;

	UNUSED_PARAMETER(hWnd);
	UNUSED_PARAMETER(lParam);

	nID = LOWORD(wParam);
	switch (nMsg)
	{
	case WM_INITDIALOG:
		OnInit(hWnd);
		return TRUE;
	case WM_COMMAND:
		if (nID == IDCANCEL)
		{
			PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_PROPERTY_UPDATE, 0);
		}
		else if (nID == IDOK)
		{
			if (OnFinish(hWnd))
				PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_PROPERTY_UPDATE, 0);
		}
		else
		{
			OnCommand(hWnd, nID);
		}
		return TRUE;
	case WM_HSCROLL:
		if ((HWND)lParam == GetDlgItem(hWnd, IDC_TRACK_VOLUME))
			OnCommand(hWnd, IDC_TRACK_VOLUME);
		else if ((HWND)lParam == GetDlgItem(hWnd, IDC_TRACK_TIME))
			OnCommand(hWnd, IDC_TRACK_TIME);
		return TRUE;
	default:
		break;
	}

	return FALSE;
}
