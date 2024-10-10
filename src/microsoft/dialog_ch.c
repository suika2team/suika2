/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * Dialog procedure for the @ch command.
 */

#include "dialog.h"
#include "resource.h"

#include "../opennovel.h"

static VOID OnInit(HWND hWnd)
{
	HWND hWndPos, hWndFade;
	const wchar_t *pwszText;
	const char *pszText;
	float fTime;
	int i;

	struct chpos {
		const char *name;
		int index;
	} chpos[] = {
		{"", 0},
		{"center", 0},
		{"c", 0},
		{"left", 1},
		{"l", 1},
		{"right", 2},
		{"r", 2},
		{"left-center", 3},
		{"lc", 3},
		{"right-center", 4},
		{"rc", 4},
		{"back", 5},
		{"b", 5},
	};

	struct fade {
		const char *name;
		int index;
	} fade[] = {
		{"", 0},
		{"normal", 0},
		{"n", 0},
		{"curtain", 1},
		{"curtain-right", 1},
		{"cr", 1},
		{"c", 1},
		{"curtain-left", 2},
		{"cl", 2},
		{"curtain-up", 3},
		{"cu", 3},
		{"curtain-down", 4},
		{"cd", 4},
		{"slide-right", 5},
		{"sr", 5},
		{"slide-left", 6},
		{"sl", 6},
		{"slide-up", 7},
		{"su", 7},
		{"slide-down", 8},
		{"sd", 8},
		{"shutter-right", 9},
		{"sr", 9},
		{"shutter-left", 10},
		{"sl", 10},
		{"shutter-up", 11},
		{"su", 11},
		{"shutter-down", 12},
		{"sd", 12},
		{"eye-open", 13},
		{"eye-close", 14},
		{"eye-open-v", 15},
		{"eye-close-v", 16},
		{"slit-open", 17},
		{"slit-close", 18},
		{"slit-open-v", 19},
		{"slit-close-v", 20},
	};

	UNUSED_PARAMETER(hWnd);

	/* Position */
	hWndPos = GetDlgItem(hWnd, IDC_COMBO_CHPOS);
	SendMessage(hWndPos, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(bEnglish ? L"Center" : L"中央"));
	SendMessage(hWndPos, (UINT)CB_ADDSTRING, (WPARAM)1, (LPARAM)(bEnglish ? L"Left" : L"左"));
	SendMessage(hWndPos, (UINT)CB_ADDSTRING, (WPARAM)2, (LPARAM)(bEnglish ? L"Right" : L"右"));
	SendMessage(hWndPos, (UINT)CB_ADDSTRING, (WPARAM)3, (LPARAM)(bEnglish ? L"Left Center" : L"左中央"));
	SendMessage(hWndPos, (UINT)CB_ADDSTRING, (WPARAM)4, (LPARAM)(bEnglish ? L"Right Center" : L"右中央"));
	SendMessage(hWndPos, (UINT)CB_ADDSTRING, (WPARAM)5, (LPARAM)(bEnglish ? L"Bak" : L"背面中央"));
	pszText = get_string_param(CH_PARAM_POS);
	for (i = 0; i < (int)(sizeof(chpos) / sizeof(struct chpos)); i++)
	{
		if (strcmp(pszText, chpos[i].name) == 0)
		{
			SendMessage(hWndPos, CB_SETCURSEL, (WPARAM)chpos[i].index, (LPARAM)0);
			break;
		}
	}

	/* File */
	pwszText = conv_utf8_to_utf16(get_string_param(CH_PARAM_FILE));
	if (wcscmp(pwszText, L"none") == 0)
	{
		SendMessage(GetDlgItem(hWnd, IDC_CHECK_NONE), (UINT)BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
		EnableWindow(GetDlgItem(hWnd, IDC_TEXT_FILE1), FALSE);
	}
	else
	{
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), pwszText);
	}

	/* Time */
	pwszText = conv_utf8_to_utf16(get_string_param(CH_PARAM_SPAN));
	if (wcscmp(pwszText, L"") == 0)
		pwszText = L"0";
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), pwszText);
	fTime = wcstof(pwszText, NULL);
	SendMessage(GetDlgItem(hWnd, IDC_TRACK_TIME), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(int)(fTime * 10.0f));

	/* Fade */
	hWndFade = GetDlgItem(hWnd, IDC_COMBO_FADE);
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(bEnglish ? L"Normal" : L"標準"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)1, (LPARAM)(bEnglish ? L"Curtain Right" : L"カーテン右"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)2, (LPARAM)(bEnglish ? L"Curtain Left" : L"カーテン左"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)3, (LPARAM)(bEnglish ? L"Curtain Up" : L"カーテン上"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)4, (LPARAM)(bEnglish ? L"Curtain Down" : L"カーテン下"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)5, (LPARAM)(bEnglish ? L"Slide Right" : L"スライド右"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)6, (LPARAM)(bEnglish ? L"Slide Left" : L"スライド左"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)7, (LPARAM)(bEnglish ? L"Slide Up" : L"スライド上"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)8, (LPARAM)(bEnglish ? L"Slide Down" : L"スライド下"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)9, (LPARAM)(bEnglish ? L"Shutter Right" : L"シャッター右"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)10, (LPARAM)(bEnglish ? L"Shutter Left" : L"シャッター左"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)11, (LPARAM)(bEnglish ? L"Shutter Up" : L"シャッター上"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)12, (LPARAM)(bEnglish ? L"Shutter Down" : L"シャッター下"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)13, (LPARAM)(bEnglish ? L"Eye Open" : L"目を開く"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)14, (LPARAM)(bEnglish ? L"Eye Close" : L"目をつぶる"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)15, (LPARAM)(bEnglish ? L"Eye Open Vertical" : L"目を開く(縦)"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)16, (LPARAM)(bEnglish ? L"Eye Close Vertical" : L"目をつぶる(縦)"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)17, (LPARAM)(bEnglish ? L"Slit Open" : L"スリットを開く"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)18, (LPARAM)(bEnglish ? L"Slit Close" : L"スリットを閉じる"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)19, (LPARAM)(bEnglish ? L"Slit Open Vertical" : L"スリットを開く(縦)"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)20, (LPARAM)(bEnglish ? L"Slit Close Vertical" : L"スリットを閉じる(縦)"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)21, (LPARAM)(bEnglish ? L"Rule Image 1-bit" : L"ルール画像(1bit)"));
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)22, (LPARAM)(bEnglish ? L"Rule Image 8-bit" : L"ルール画像(8bit)"));
	pszText = get_string_param(CH_PARAM_METHOD);
	for (i = 0; i < (int)(sizeof(fade) / sizeof(struct fade)); i++)
	{
		if (strcmp(pszText, fade[i].name) == 0)
		{
			SendMessage(hWndFade, CB_SETCURSEL, (WPARAM)fade[i].index, (LPARAM)0);
				break;
		}
	}
	if (strncmp(pszText, "rule:", 5) == 0)
	{
		const int RULE = 21;
		SendMessage(hWndFade, CB_SETCURSEL, (WPARAM)RULE, (LPARAM)0);
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_RULE), conv_utf8_to_utf16(pszText + 5));
	}
	else if (strncmp(pszText, "melt:", 5) == 0)
	{
		const int MELT = 22;
		SendMessage(hWndFade, CB_SETCURSEL, (WPARAM)MELT, (LPARAM)0);
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_RULE), conv_utf8_to_utf16(pszText + 5));
	}
	else
	{
		EnableWindow(GetDlgItem(hWnd, IDC_TEXT_RULE), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_RULE), FALSE);
	}

	/* X-Offset */
	pwszText = conv_utf8_to_utf16(get_string_param(CH_PARAM_OFFSET_X));
	if (wcscmp(pwszText, L"") == 0)
		pwszText = L"0";
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OFSX), pwszText);

	/* Y-Offset */
	pwszText = conv_utf8_to_utf16(get_string_param(CH_PARAM_OFFSET_Y));
	if (wcscmp(pwszText, L"") == 0)
		pwszText = L"0";
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OFSY), pwszText);
}

static VOID OnCommand(HWND hWnd, UINT nID)
{
	if (nID == IDC_BUTTON_FILE1)
	{
		const wchar_t *pwszFile;
		pwszFile = SelectFile(CH_DIR);
		if (pwszFile != NULL)
		{
			SetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), pwszFile);
		}
		return;
	}

	if (nID == IDC_CHECK_NONE)
	{
		if (!SendMessage(GetDlgItem(hWnd, IDC_CHECK_NONE), (UINT)BM_GETCHECK, (WPARAM)0, (LPARAM)0))
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_NONE), (UINT)BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
			EnableWindow(GetDlgItem(hWnd, IDC_TEXT_FILE1), FALSE);
		}
		else
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_NONE), (UINT)BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
			EnableWindow(GetDlgItem(hWnd, IDC_TEXT_FILE1), TRUE);
		}
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

	if (nID == IDC_COMBO_FADE)
	{
		const int RULE = 21;
		if (SendMessage(GetDlgItem(hWnd, IDC_COMBO_FADE), (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0) >= RULE)
		{
			EnableWindow(GetDlgItem(hWnd, IDC_TEXT_RULE), TRUE);
			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_RULE), TRUE);
		}
		else
		{
			EnableWindow(GetDlgItem(hWnd, IDC_TEXT_RULE), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_RULE), FALSE);
		}
	}

	if (nID == IDC_BUTTON_RULE)
	{
		const wchar_t *pwszFile;
		pwszFile = SelectFile(RULE_DIR);
		if (pwszFile != NULL)
		{
			SetWindowText(GetDlgItem(hWnd, IDC_TEXT_RULE), pwszFile);
		}
		return;
	}
}

static BOOL OnFinish(HWND hWnd)
{
	wchar_t wszText[1024];
	char szCmd[4096];
	BOOL bXOfs, bYOfs;
	const char *chpos[] = {
		"center",
		"left",
		"right",
		"left-center",
		"right-center",
		"back",
	};
	const char *fade[] = {
		"normal",
		"curtain-right",
		"curtain-left",
		"curtain-up",
		"curtain-down",
		"slide-right",
		"slide-left",
		"slide-up",
		"slide-down",
		"shutter-right",
		"shutter-left",
		"shutter-up",
		"shutter-down",
		"eye-open",
		"eye-close",
		"eye-open-v",
		"eye-close-v",
		"slit-open",
		"slit-close",
		"slit-open-v",
		"slit-close-v",
		"rule:",
		"melt:",
	};
	int nIndex;
	const int RULE = 21;
	const int MELT = 22;

	strncpy(szCmd, "@ch", sizeof(szCmd) - 1);

	/* Position */
	nIndex = SendMessage(GetDlgItem(hWnd, IDC_COMBO_CHPOS), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	strncat(szCmd, " pos=", sizeof(szCmd) - 1);
	strncat(szCmd, chpos[nIndex], sizeof(szCmd) - 1);

	/* File */
	if (!SendMessage(GetDlgItem(hWnd, IDC_CHECK_NONE), BM_GETCHECK, (WPARAM)0, (LPARAM)0))
	{
		GetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), wszText, sizeof(wszText) / sizeof(wchar_t));
		if (wcscmp(wszText, L"") == 0)
		{
			MessageBox(hWnd,
					   bEnglish ?
					   L"Fill the background file name." :
					   L"背景ファイル名を指定してください。",
					   DIALOG_TITLE,
					   MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
		strncat(szCmd, " file=", sizeof(szCmd) - 1);
		strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);
	}
	else
	{
		strncat(szCmd, " file=none", sizeof(szCmd) - 1);
	}

	/* Time */
	GetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), wszText, sizeof(wszText) / sizeof(wchar_t));
	if (wcscmp(wszText, L"") == 0)
		wcscpy(wszText, L"0");
	strncat(szCmd, " t=", sizeof(szCmd) - 1);
	strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);

	/* Fade */
	nIndex = SendMessage(GetDlgItem(hWnd, IDC_COMBO_FADE), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	strncat(szCmd, " effect=", sizeof(szCmd) - 1);
	strncat(szCmd, fade[nIndex], sizeof(szCmd) - 1);

	/* Rule file */
	if (nIndex == RULE || nIndex == MELT)
	{
		GetWindowText(GetDlgItem(hWnd, IDC_TEXT_RULE), wszText, sizeof(wszText) / sizeof(wchar_t));
		if (wcscmp(wszText, L"") == 0)
		{
			MessageBox(hWnd,
					   bEnglish ?
					   L"Fill the rule file name." :
					   L"ルールファイル名を指定してください。",
					   DIALOG_TITLE,
					   MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
		strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);
	}

	/* X-Y Offsets */
	GetWindowText(GetDlgItem(hWnd, IDC_TEXT_OFSX), wszText, sizeof(wszText) / sizeof(wchar_t));
	bXOfs = wcscmp(wszText, L"") != 0 && wcscmp(wszText, L"0") != 0 ;
	GetWindowText(GetDlgItem(hWnd, IDC_TEXT_OFSY), wszText, sizeof(wszText) / sizeof(wchar_t));
	bYOfs = wcscmp(wszText, L"") != 0 && wcscmp(wszText, L"0") != 0 ;
	if (bXOfs || bYOfs)
	{
		/* X Offset */
		if (!bXOfs)
			wcscpy(wszText, L"0");
		else
			GetWindowText(GetDlgItem(hWnd, IDC_TEXT_OFSX), wszText, sizeof(wszText) / sizeof(wchar_t));
		strncat(szCmd, " right=", sizeof(szCmd) - 1);
		strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);

		/* Y Offset */
		if (!bYOfs)
			wcscpy(wszText, L"0");
		else
			GetWindowText(GetDlgItem(hWnd, IDC_TEXT_OFSX), wszText, sizeof(wszText) / sizeof(wchar_t));
		GetWindowText(GetDlgItem(hWnd, IDC_TEXT_OFSY), wszText, sizeof(wszText) / sizeof(wchar_t));
		strncat(szCmd, " down=", sizeof(szCmd) - 1);
		strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);
	}

	/* Update the script model. */
	update_script_line(get_expanded_line_num(), szCmd);

	return TRUE;
}

BOOL CALLBACK DlgChWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
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
