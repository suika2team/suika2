/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * Dialog procedure for the @time command.
 */

#include "dialog.h"
#include "resource.h"

#include "opennovel.h"

static VOID OnInit(HWND hWnd)
{
	const char *pszText;
	float fTime;

	UNUSED_PARAMETER(hWnd);

	/* Time */
	pszText = get_string_param(TIME_PARAM_SPAN);
	if (strcmp(pszText, "") == 0)
		pszText = "0";
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), conv_utf8_to_utf16(pszText));
	fTime = (float)atof(pszText);
	SendMessage(GetDlgItem(hWnd, IDC_TRACK_TIME), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(int)(fTime * 10.0f));
}

static VOID OnCommand(HWND hWnd, UINT nID)
{
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

	strncpy(szCmd, "@time", sizeof(szCmd) - 1);

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

BOOL CALLBACK DlgTimeWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
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
		OnCommand(hWnd, IDC_TRACK_TIME);
		return TRUE;
	default:
		break;
	}

	return FALSE;
}
