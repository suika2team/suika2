/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, The Authors. All rights reserved.
 */

/*
 * Dialog procedure for the @bg command.
 */

#include "dialog.h"
#include "resource.h"

#include "../opennovel.h"

static VOID OnInit(HWND hWnd)
{
	const char *pszFade;
	HWND hWndFade;
	const wchar_t *pwszText;
	int i;

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

	/* File */
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), conv_utf8_to_utf16(get_string_param(BG_PARAM_FILE)));

	/* Time */
	pwszText = conv_utf8_to_utf16(get_string_param(BG_PARAM_SPAN));
	if (wcscmp(pwszText, L"") == 0)
		pwszText = L"0";
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), pwszText);

	/* Fade */
	hWndFade = GetDlgItem(hWnd, IDC_COMBO_FADE);
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"標準");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)1, (LPARAM)L"カーテン右");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)2, (LPARAM)L"カーテン左");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)3, (LPARAM)L"カーテン上");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)4, (LPARAM)L"カーテン下");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)5, (LPARAM)L"スライド右");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)6, (LPARAM)L"スライド左");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)7, (LPARAM)L"スライド上");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)8, (LPARAM)L"スライド下");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)9, (LPARAM)L"シャッター右");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)10, (LPARAM)L"シャッター左");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)11, (LPARAM)L"シャッター上");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)12, (LPARAM)L"シャッター下");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)13, (LPARAM)L"目を開く");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)14, (LPARAM)L"目をつぶる");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)15, (LPARAM)L"目を開く(縦)");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)16, (LPARAM)L"目をつぶる(縦)");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)17, (LPARAM)L"スリットを開く");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)18, (LPARAM)L"スリットを閉じる");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)19, (LPARAM)L"スリットを開く(縦)");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)20, (LPARAM)L"スリットを閉じる(縦)");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)21, (LPARAM)L"ルール画像(1bit)");
	SendMessage(hWndFade, (UINT)CB_ADDSTRING, (WPARAM)22, (LPARAM)L"ルール画像(8bit)");
	pszFade = get_string_param(BG_PARAM_METHOD);
	for (i = 0; i < (int)(sizeof(fade) / sizeof(struct fade)); i++)
	{
		if (strcmp(pszFade, fade[i].name) == 0)
		{
			SendMessage(hWndFade, CB_SETCURSEL, (WPARAM)fade[i].index, (LPARAM)0);
				break;
		}
	}
	if (strcmp(pszFade, "rule:") == 0)
	{
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_RULE), conv_utf8_to_utf16(pszFade + 5));
	}
}

static VOID OnFinish(HWND hWnd)
{
	UNUSED_PARAMETER(hWnd);
}

static VOID OnCommand(HWND hWnd, UINT nID)
{
	if (nID == IDC_BUTTON_FILE1)
	{
		const wchar_t *pwszFile;
		pwszFile = SelectFile(BG_DIR);
		if (pwszFile != NULL)
		{
			SetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), pwszFile);
		}
		return;
	}
}

BOOL CALLBACK DlgBgWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
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
			OnFinish(hWnd);
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
