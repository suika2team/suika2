/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * Dialog procedure for the @shake command.
 */

#include "dialog.h"
#include "resource.h"

#include "opennovel.h"

static VOID OnInit(HWND hWnd)
{
	HWND hWndCombo;
	const char *pszText;
	int nValue;
	float fValue;

	UNUSED_PARAMETER(hWnd);

	/* Direction */
	hWndCombo = GetDlgItem(hWnd, IDC_COMBO_DIRECTION);
	SendMessage(hWndCombo, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(bEnglish ? L"Horizontal" : L"横"));
	SendMessage(hWndCombo, (UINT)CB_ADDSTRING, (WPARAM)1, (LPARAM)(bEnglish ? L"Vertical" : L"縦"));
	pszText = get_string_param(SHAKE_PARAM_MOVE);
	if (strcmp(pszText, "horizontal") == 0 || strcmp(pszText, "h") == 0)
		SendMessage(hWndCombo, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	else
		SendMessage(hWndCombo, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);

	/* Amount */
	pszText = get_string_param(SHAKE_PARAM_AMOUNT);
	nValue = atoi(pszText);
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_PIXEL), conv_utf8_to_utf16(pszText));
	SendMessage(GetDlgItem(hWnd, IDC_TRACK_PIXEL), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)nValue);

	/* Count */
	pszText = get_string_param(SHAKE_PARAM_TIMES);
	nValue = atoi(pszText);
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_COUNT), conv_utf8_to_utf16(pszText));
	SendMessage(GetDlgItem(hWnd, IDC_TRACK_COUNT), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(nValue * 10));

	/* Time */
	pszText = get_string_param(SHAKE_PARAM_SPAN);
	fValue = (float)atof(pszText);
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), conv_utf8_to_utf16(pszText));
	SendMessage(GetDlgItem(hWnd, IDC_TRACK_TIME), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(fValue * 10.0f));
}

static VOID OnCommand(HWND hWnd, UINT nID)
{
	if (nID == IDC_TRACK_PIXEL)
	{
		int nPos;
		wchar_t wszText[128];
		nPos = SendMessage(GetDlgItem(hWnd, IDC_TRACK_PIXEL), TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		swprintf(wszText, sizeof(wszText) / sizeof(wchar_t), L"%d", nPos);
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_PIXEL), wszText);
		return;
	}

	if (nID == IDC_TRACK_COUNT)
	{
		int nPos;
		wchar_t wszText[128];
		nPos = SendMessage(GetDlgItem(hWnd, IDC_TRACK_COUNT), TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		swprintf(wszText, sizeof(wszText) / sizeof(wchar_t), L"%d", nPos / 10);
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_COUNT), wszText);
		return;
	}

	if (nID == IDC_TRACK_TIME)
	{
		int nPos;
		wchar_t wszText[128];
		nPos = SendMessage(GetDlgItem(hWnd, IDC_TRACK_TIME), TBM_GETPOS, (WPARAM)0, (LPARAM)0);
		swprintf(wszText, sizeof(wszText) / sizeof(wchar_t), L"%d", nPos / 10);
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), wszText);
		return;
	}
}

static BOOL OnFinish(HWND hWnd)
{
	wchar_t wszText[1024];
	char szCmd[4096];
	int nIndex;

	strncpy(szCmd, "@shake", sizeof(szCmd) - 1);

	/* Direction */
	nIndex = SendMessage(GetDlgItem(hWnd, IDC_COMBO_DIRECTION), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	strncat(szCmd, " dir=", sizeof(szCmd) - 1);
	if (nIndex == 0)
		strncat(szCmd, "horizontal", sizeof(szCmd) - 1);
	else
		strncat(szCmd, "vertical", sizeof(szCmd) - 1);

	/* Time */
	GetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), wszText, sizeof(wszText) / sizeof(wchar_t));
	if (wcscmp(wszText, L"") == 0)
		wcscpy(wszText, L"0");
	strncat(szCmd, " t=", sizeof(szCmd) - 1);
	strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);

	/* Count */
	GetWindowText(GetDlgItem(hWnd, IDC_TEXT_COUNT), wszText, sizeof(wszText) / sizeof(wchar_t));
	if (wcscmp(wszText, L"") == 0)
		wcscpy(wszText, L"0");
	strncat(szCmd, " count=", sizeof(szCmd) - 1);
	strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);

	/* Amount */
	GetWindowText(GetDlgItem(hWnd, IDC_TEXT_PIXEL), wszText, sizeof(wszText) / sizeof(wchar_t));
	if (wcscmp(wszText, L"") == 0)
		wcscpy(wszText, L"0");
	strncat(szCmd, " amp=", sizeof(szCmd) - 1);
	strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);

	/* Update the script model. */
	update_script_line(get_expanded_line_num(), szCmd);

	return TRUE;
}

BOOL CALLBACK DlgShakeWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
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
		if ((HWND)lParam == GetDlgItem(hWnd, IDC_TRACK_TIME))
			OnCommand(hWnd, IDC_TRACK_TIME);
		else if ((HWND)lParam == GetDlgItem(hWnd, IDC_TRACK_COUNT))
			OnCommand(hWnd, IDC_TRACK_COUNT);
		else if ((HWND)lParam == GetDlgItem(hWnd, IDC_TRACK_PIXEL))
			OnCommand(hWnd, IDC_TRACK_PIXEL);
		return TRUE;
	default:
		break;
	}

	return FALSE;
}
