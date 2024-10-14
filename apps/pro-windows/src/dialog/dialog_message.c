/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * Dialog procedure for the @bg command.
 */

#include "dialog.h"
#include "resource.h"

#include "opennovel.h"

static VOID OnInit(HWND hWnd)
{
	HWND hWndEscape;

	UNUSED_PARAMETER(hWnd);

	/* Name/Voice/Message */
	if (get_command_type() == COMMAND_SERIF)
	{
		/* Serif */
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_NAME), conv_utf8_to_utf16(get_string_param(SERIF_PARAM_NAME)));
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), conv_utf8_to_utf16(get_string_param(SERIF_PARAM_VOICE)));
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_MESSAGE), conv_utf8_to_utf16(get_string_param(SERIF_PARAM_MESSAGE)));
	}
	else
	{
		/* Message */
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_MESSAGE), conv_utf8_to_utf16(get_string_param(MESSAGE_PARAM_MESSAGE)));
	}

	/* Escape sequence */
	hWndEscape = GetDlgItem(hWnd, IDC_COMBO_ESCAPE);
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(bEnglish ? L"Centering" : L"中央揃え"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)1, (LPARAM)(bEnglish ? L"Rightify" : L"右揃え"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)2, (LPARAM)(bEnglish ? L"Leftify" : L"左揃え"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)3, (LPARAM)(bEnglish ? L"Return" : L"改行"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)4, (LPARAM)(bEnglish ? L"Font 1" : L"フォント1"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)5, (LPARAM)(bEnglish ? L"Font 2" : L"フォント2"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)6, (LPARAM)(bEnglish ? L"Font 3" : L"フォント3"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)7, (LPARAM)(bEnglish ? L"Font 4" : L"フォント4"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)8, (LPARAM)(bEnglish ? L"Outline On" : L"ふちどりオン"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)9, (LPARAM)(bEnglish ? L"Outline Off" : L"ふちどりオフ"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)10, (LPARAM)(bEnglish ? L"Font Color" : L"フォント色"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)11, (LPARAM)(bEnglish ? L"Font Color (Default)" : L"フォント色(デフォルト)"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)12, (LPARAM)(bEnglish ? L"Font Size" : L"フォントサイズ"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)13, (LPARAM)(bEnglish ? L"Font Size (Force)" : L"フォントサイズ(強制)"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)14, (LPARAM)(bEnglish ? L"Wait" : L"ウェイト"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)15, (LPARAM)(bEnglish ? L"Pen Move" : L"ペン移動"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)16, (LPARAM)(bEnglish ? L"Emoticon" : L"エモーティコン"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)17, (LPARAM)(bEnglish ? L"Ruby" : L"ルビ"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)18, (LPARAM)(bEnglish ? L"Line Height" : L"行間"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)19, (LPARAM)(bEnglish ? L"Origin" : L"原点"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)20, (LPARAM)(bEnglish ? L"Background Color On" : L"背景色オン"));
	SendMessage(hWndEscape, (UINT)CB_ADDSTRING, (WPARAM)21, (LPARAM)(bEnglish ? L"Background Color Of" : L"背景色オフ"));
	SendMessage(hWndEscape, (UINT)CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}

static VOID OnCommand(HWND hWnd, UINT nID)
{
	if (nID == IDC_BUTTON_FILE1)
	{
		const wchar_t *pwszFile;
		pwszFile = SelectFile(CV_DIR);
		if (pwszFile != NULL)
		{
			SetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), pwszFile);
		}
		return;
	}

	if (nID == IDC_BUTTON_INSERT)
	{
		wchar_t wszText[4096];
		int nIndex;
		const wchar_t *esc[] = {
			L"\\c",
			L"\\r",
			L"\\l",
			L"\\n",
			L"\\f{1}",
			L"\\f{2}",
			L"\\f{3}",
			L"\\f{4}",
			L"\\o{+}",
			L"\\o{-}",
			L"\\#{RRGGBB}",
			L"\\#{DEF}",
			L"\\@{32}",
			L"\\@{!32}",
			L"\\w{1.0}",
			L"\\p{0,0}",
			L"\\e{heart}",
			L"\\^{ルビ}",
			L"\\L{32}",
			L"\\M{0,0}",
			L"\\k{RRGGBB}",
			L"\\k{OFF}",
		};

		nIndex = SendMessage(GetDlgItem(hWnd, IDC_COMBO_ESCAPE), (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
		GetWindowText(GetDlgItem(hWnd, IDC_TEXT_MESSAGE), wszText, sizeof(wszText) / sizeof(wchar_t));
		wcsncat(wszText, esc[nIndex], sizeof(wszText) / sizeof(wchar_t) - 1);
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_MESSAGE), wszText);
		return;
	}
}

static BOOL OnFinish(HWND hWnd)
{
	wchar_t wszText[4096];
	char szCmd[4096*4];
	wchar_t *pwszText;

	/* Check name. */
	GetWindowText(GetDlgItem(hWnd, IDC_TEXT_NAME), wszText, sizeof(wszText) / sizeof(wchar_t));
	if (wcscmp(wszText, L"") != 0)
	{
		/* Serif */
		strncpy(szCmd, "*", sizeof(szCmd) - 1);
		strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);
		strncat(szCmd, "*", sizeof(szCmd) - 1);

		/* Voice */
		GetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), wszText, sizeof(wszText) / sizeof(wchar_t));
		if (wcscmp(wszText, L"") != 0)
		{
			strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);
			strncat(szCmd, "*", sizeof(szCmd) - 1);
		}

		/* Message */
		GetWindowText(GetDlgItem(hWnd, IDC_TEXT_MESSAGE), wszText, sizeof(wszText) / sizeof(wchar_t));
		pwszText = &wszText[0];
		while (*pwszText)
		{
			if (*pwszText == L'\r')
				*pwszText = L'\\';
			else if (*pwszText == L'\n')
				*pwszText = L'n';
			pwszText++;
		}
		strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);
	}
	else
	{
		/* Message */
		GetWindowText(GetDlgItem(hWnd, IDC_TEXT_MESSAGE), wszText, sizeof(wszText) / sizeof(wchar_t));
		pwszText = &wszText[0];
		while (*pwszText)
		{
			if (*pwszText == L'\r')
				*pwszText = L'\\';
			else if (*pwszText == L'\n')
				*pwszText = L'n';
			pwszText++;
		}
		strncpy(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);
	}

	/* Update the script model. */
	update_script_line(get_expanded_line_num(), szCmd);

	return TRUE;
}

BOOL CALLBACK DlgMessageWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
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
	default:
		break;
	}

	return FALSE;
}
