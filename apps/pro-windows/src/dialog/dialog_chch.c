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

#include "opennovel.h"

enum {
	COMBO_STAY = 0,
	COMBO_CHANGE = 1,
	COMBO_NONE = 2,
};

enum {
	COMBO_ASIS = 0,
	COMBO_NORMAL = 1,
	COMBO_DARK = 2,
};

static VOID InitFor(HWND hWnd, int nFileParam, int nAlphaParam, int nXOfsParam, int nYOfsParam, int nDimParam, int nStateID, int nFileID, int nAlphaID, int nXOfsID, int nYOfsID, int nDimID);
static VOID InitTimeAndFade(HWND hWnd);
static VOID OnComboState(HWND hWnd, int nComboID, int nTextID, int nButtonID);
static VOID OnFileButton(HWND hWnd, const char *pszDir, int nTextID);
static VOID OnComboFade(HWND hWnd);
static VOID OnButtonRule(HWND hWnd);
static VOID OnTrackbar(HWND hWnd);
static BOOL FinalizeItem(HWND hWnd, char *pszCmd, size_t nLen, const char *pszFile, const char *pszAlpha, const char *pszX, const char *pszY, const char *pszDim, int nComboFileID, int nTextFileID, int nTextAlphaID, int nTextXID, int nTextYID, int nComboDimID);

static VOID OnInit(HWND hWnd)
{
	InitFor(hWnd, CHCH_PARAM_C,  CHCH_PARAM_CA,  CHCH_PARAM_CX,  CHCH_PARAM_CY,  CHCH_PARAM_CD,  IDC_COMBO_C,  IDC_TEXT_C,  IDC_TEXT_CA,  IDC_TEXT_CX,  IDC_TEXT_CY,  IDC_COMBO_CD);
	InitFor(hWnd, CHCH_PARAM_L,  CHCH_PARAM_LA,  CHCH_PARAM_LX,  CHCH_PARAM_LY,  CHCH_PARAM_LD,  IDC_COMBO_L,  IDC_TEXT_L,  IDC_TEXT_LA,  IDC_TEXT_LX,  IDC_TEXT_LY,  IDC_COMBO_LD);
	InitFor(hWnd, CHCH_PARAM_R,  CHCH_PARAM_RA,  CHCH_PARAM_RX,  CHCH_PARAM_RY,  CHCH_PARAM_RD,  IDC_COMBO_R,  IDC_TEXT_R,  IDC_TEXT_RA,  IDC_TEXT_RX,  IDC_TEXT_RY,  IDC_COMBO_RD);
	InitFor(hWnd, CHCH_PARAM_LC, CHCH_PARAM_LCA, CHCH_PARAM_LCX, CHCH_PARAM_LCY, CHCH_PARAM_LCD, IDC_COMBO_LC, IDC_TEXT_LC, IDC_TEXT_LCA, IDC_TEXT_LCX, IDC_TEXT_LCY, IDC_COMBO_LCD);
	InitFor(hWnd, CHCH_PARAM_RC, CHCH_PARAM_RCA, CHCH_PARAM_RCX, CHCH_PARAM_RCY, CHCH_PARAM_RCD, IDC_COMBO_RC, IDC_TEXT_RC, IDC_TEXT_RCA, IDC_TEXT_RCX, IDC_TEXT_RCY, IDC_COMBO_RCD);
	InitFor(hWnd, CHCH_PARAM_B,  CHCH_PARAM_BA,  CHCH_PARAM_BX,  CHCH_PARAM_BY,  CHCH_PARAM_BD,  IDC_COMBO_B,  IDC_TEXT_B,  IDC_TEXT_BA,  IDC_TEXT_BX,  IDC_TEXT_BY,  IDC_COMBO_BD);
	InitFor(hWnd, CHCH_PARAM_BG, CHCH_PARAM_BGA, CHCH_PARAM_BGX, CHCH_PARAM_BGY, -1,             IDC_COMBO_BG, IDC_TEXT_BG, IDC_TEXT_BGA, IDC_TEXT_BGX, IDC_TEXT_BGY, -1);
	InitTimeAndFade(hWnd);
}

static VOID InitFor(
	HWND hWnd,
	int nFileParam,
	int nAlphaParam,
	int nXOfsParam,
	int nYOfsParam,
	int nDimParam,
	int nStateID,
	int nFileID,
	int nAlphaID,
	int nXOfsID,
	int nYOfsID,
	int nDimID)
{
	HWND hWndCombo;
	const char *pszText;
	const int COMBO_STAY = 0;
	const int COMBO_CHANGE = 1;
	const int COMBO_ERASE = 2;

	/* File */
	pszText = get_string_param(nFileParam);
	hWndCombo = GetDlgItem(hWnd, nStateID);
	SendMessage(hWndCombo, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(bEnglish ? L"As is" : L"変更なし"));
	SendMessage(hWndCombo, (UINT)CB_ADDSTRING, (WPARAM)1, (LPARAM)(bEnglish ? L"Change" : L"変更あり"));
	SendMessage(hWndCombo, (UINT)CB_ADDSTRING, (WPARAM)2, (LPARAM)(bEnglish ? L"Erase" : L"消去"));
	if (strcmp(pszText, "none") == 0)
	{
		SendMessage(hWndCombo, CB_SETCURSEL, (WPARAM)COMBO_ERASE, (LPARAM)0);
		EnableWindow(GetDlgItem(hWnd, nFileID), FALSE);
	}
	else if (strcmp(pszText, "stay") == 0 || strcmp(pszText, "") == 0)
	{
		SendMessage(hWndCombo, CB_SETCURSEL, (WPARAM)COMBO_STAY, (LPARAM)0);
		EnableWindow(GetDlgItem(hWnd, nFileID), FALSE);
	}
	else
	{
		SendMessage(hWndCombo, CB_SETCURSEL, (WPARAM)COMBO_CHANGE, (LPARAM)0);
		SetWindowText(GetDlgItem(hWnd, nFileID), conv_utf8_to_utf16(pszText));
	}

	/* Alpha */
	pszText = get_string_param(nAlphaParam);
	SetWindowText(GetDlgItem(hWnd, nAlphaID), conv_utf8_to_utf16(pszText));

	/* X Offset */
	pszText = get_string_param(nXOfsParam);
	if (strcmp(pszText, "") == 0)
		pszText = "0";
	SetWindowText(GetDlgItem(hWnd, nXOfsID), conv_utf8_to_utf16(pszText));

	/* Y Offset */
	pszText = get_string_param(nYOfsParam);
	if (strcmp(pszText, "") == 0)
		pszText = "0";
	SetWindowText(GetDlgItem(hWnd, nYOfsID), conv_utf8_to_utf16(pszText));

	/* Dim */
	if (nDimParam != -1)
	{
		hWndCombo = GetDlgItem(hWnd, nDimID);
		SendMessage(hWndCombo, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(bEnglish ? L"As is" : L"変更なし"));
		SendMessage(hWndCombo, (UINT)CB_ADDSTRING, (WPARAM)1, (LPARAM)(bEnglish ? L"Normal" : L"標準"));
		SendMessage(hWndCombo, (UINT)CB_ADDSTRING, (WPARAM)2, (LPARAM)(bEnglish ? L"Dark" : L"暗く"));
		pszText = get_string_param(nDimParam);
		if (strcmp(pszText, "yes") == 0 || strcmp(pszText, "dark") == 0)
			SendMessage(hWndCombo, CB_SETCURSEL, (WPARAM)COMBO_DARK, (LPARAM)0);
		else if (strcmp(pszText, "no") == 0 || strcmp(pszText, "light") == 0)
			SendMessage(hWndCombo, CB_SETCURSEL, (WPARAM)COMBO_NORMAL, (LPARAM)0);
		else
			SendMessage(hWndCombo, CB_SETCURSEL, (WPARAM)COMBO_ASIS, (LPARAM)0);
	}
}

static VOID InitTimeAndFade(HWND hWnd)
{
	HWND hWndFade;
	const char *pszText;
	float fTime;
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

	/* Time */
	pszText = get_string_param(CHCH_PARAM_SPAN);
	if (strcmp(pszText, "") == 0)
		pszText = "0";
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), conv_utf8_to_utf16(pszText));
	fTime = (float)atof(pszText);
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
	pszText = get_string_param(CHCH_PARAM_METHOD);
	for (i = 0; i < (int)(sizeof(fade) / sizeof(struct fade)); i++)
	{
		if (strcmp(pszText, fade[i].name) == 0)
		{
			SendMessage(hWndFade, (UINT)CB_SETCURSEL, (WPARAM)fade[i].index, (LPARAM)0);
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
}

static VOID OnCommand(HWND hWnd, UINT nID)
{
	switch (nID)
	{
	case IDC_COMBO_C:
		OnComboState(hWnd, IDC_COMBO_C, IDC_TEXT_C, IDC_BUTTON_C);
		return;
	case IDC_COMBO_L:
		OnComboState(hWnd, IDC_COMBO_L, IDC_TEXT_L, IDC_BUTTON_L);
		return;
	case IDC_COMBO_R:
		OnComboState(hWnd, IDC_COMBO_R, IDC_TEXT_R, IDC_BUTTON_R);
		return;
	case IDC_COMBO_LC:
		OnComboState(hWnd, IDC_COMBO_LC, IDC_TEXT_LC, IDC_BUTTON_LC);
		return;
	case IDC_COMBO_RC:
		OnComboState(hWnd, IDC_COMBO_RC, IDC_TEXT_RC, IDC_BUTTON_RC);
		return;
	case IDC_COMBO_B:
		OnComboState(hWnd, IDC_COMBO_B, IDC_TEXT_B, IDC_BUTTON_B);
		return;
	case IDC_COMBO_BG:
		OnComboState(hWnd, IDC_COMBO_BG, IDC_TEXT_BG, IDC_BUTTON_BG);
		return;
	case IDC_BUTTON_C:
		OnFileButton(hWnd, CH_DIR, IDC_TEXT_C);
		return;
	case IDC_BUTTON_L:
		OnFileButton(hWnd, CH_DIR, IDC_TEXT_L);
		return;
	case IDC_BUTTON_R:
		OnFileButton(hWnd, CH_DIR, IDC_TEXT_R);
		return;
	case IDC_BUTTON_LC:
		OnFileButton(hWnd, CH_DIR, IDC_TEXT_LC);
		return;
	case IDC_BUTTON_RC:
		OnFileButton(hWnd, CH_DIR, IDC_TEXT_RC);
		return;
	case IDC_BUTTON_B:
		OnFileButton(hWnd, CH_DIR, IDC_TEXT_B);
		return;
	case IDC_BUTTON_BG:
		OnFileButton(hWnd, BG_DIR, IDC_TEXT_BG);
		return;
	case IDC_TRACK_TIME:
		OnTrackbar(hWnd);
		return;
	case IDC_COMBO_FADE:
		OnComboFade(hWnd);
		return;
	case IDC_BUTTON_RULE:
		OnButtonRule(hWnd);
		return;
	default:
		break;
	}
}

static VOID OnFileButton(HWND hWnd, const char *pszDir, int nTextID)
{
	const wchar_t *pwszFile;

	pwszFile = SelectFile(pszDir);
	if (pwszFile != NULL)
		SetWindowText(GetDlgItem(hWnd, nTextID), pwszFile);
}

static VOID OnComboState(HWND hWnd, int nComboID, int nTextID, int nButtonID)
{
	int nIndex;

	nIndex = SendMessage(GetDlgItem(hWnd, nComboID), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (nIndex == COMBO_STAY)
	{
		EnableWindow(GetDlgItem(hWnd, nTextID), FALSE);
		EnableWindow(GetDlgItem(hWnd, nButtonID), FALSE);
	}
	else if (nIndex == COMBO_CHANGE)
	{
		EnableWindow(GetDlgItem(hWnd, nTextID), TRUE);
		EnableWindow(GetDlgItem(hWnd, nButtonID), TRUE);
	}
	else
	{
		EnableWindow(GetDlgItem(hWnd, nTextID), FALSE);
		EnableWindow(GetDlgItem(hWnd, nButtonID), FALSE);
	}
}

static VOID OnTrackbar(HWND hWnd)
{
	wchar_t wszText[128];
	int nPos;

	nPos = SendMessage(GetDlgItem(hWnd, IDC_TRACK_TIME), TBM_GETPOS, (WPARAM)0, (LPARAM)0);
	swprintf(wszText, sizeof(wszText) / sizeof(wchar_t), L"%0.1f", nPos / 10.0);
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_TIME), wszText);
}

static VOID OnComboFade(HWND hWnd)
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

static VOID OnButtonRule(HWND hWnd)
{
	const wchar_t *pwszFile;

	pwszFile = SelectFile(RULE_DIR);
	if (pwszFile != NULL)
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_RULE), pwszFile);
}

static BOOL OnFinish(HWND hWnd)
{
	wchar_t wszText[1024];
	char szCmd[4096];
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

	strncpy(szCmd, "@chch", sizeof(szCmd) - 1);

	/* Images */
	if (!FinalizeItem(hWnd, szCmd, sizeof(szCmd), "c", "ca", "cx", "cy", "cd", IDC_COMBO_C, IDC_TEXT_C, IDC_TEXT_CA, IDC_TEXT_CX, IDC_TEXT_CY, IDC_COMBO_CD))
		return FALSE;
	if (!FinalizeItem(hWnd, szCmd, sizeof(szCmd), "l", "la", "lx", "ly", "ld", IDC_COMBO_L, IDC_TEXT_L, IDC_TEXT_LA, IDC_TEXT_LX, IDC_TEXT_LY, IDC_COMBO_LD))
		return FALSE;
	if (!FinalizeItem(hWnd, szCmd, sizeof(szCmd), "r", "ra", "rx", "ry", "rd", IDC_COMBO_R, IDC_TEXT_R, IDC_TEXT_RA, IDC_TEXT_RX, IDC_TEXT_RY, IDC_COMBO_RD))
		return FALSE;
	if (!FinalizeItem(hWnd, szCmd, sizeof(szCmd), "lc", "lca", "lcx", "lcy", "lcd", IDC_COMBO_LC, IDC_TEXT_LC, IDC_TEXT_LCA, IDC_TEXT_LCX, IDC_TEXT_LCY, IDC_COMBO_LCD))
		return FALSE;
	if (!FinalizeItem(hWnd, szCmd, sizeof(szCmd), "rc", "rca", "rcx", "rcy", "rcd", IDC_COMBO_RC, IDC_TEXT_RC, IDC_TEXT_RCA, IDC_TEXT_RCX, IDC_TEXT_RCY, IDC_COMBO_RCD))
		return FALSE;
	if (!FinalizeItem(hWnd, szCmd, sizeof(szCmd), "b", "ba", "bx", "by", "bd", IDC_COMBO_B, IDC_TEXT_B, IDC_TEXT_BA, IDC_TEXT_BX, IDC_TEXT_BY, IDC_COMBO_BD))
		return FALSE;
	if (!FinalizeItem(hWnd, szCmd, sizeof(szCmd), "bg", NULL, "bgx", "bgy", NULL, IDC_COMBO_BG, IDC_TEXT_BG, -1, IDC_TEXT_BGX, IDC_TEXT_BGY, -1))
		return FALSE;

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

	/* Update the script model. */
	update_script_line(get_expanded_line_num(), szCmd);

	return TRUE;
}

static BOOL
FinalizeItem(
	HWND hWnd,
	char *pszCmd,
	size_t nLen,
	const char *pszFile,
	const char *pszAlpha,
	const char *pszX,
	const char *pszY,
	const char *pszDim,
	int nComboFileID,
	int nTextFileID,
	int nTextAlphaID,
	int nTextXID,
	int nTextYID,
	int nComboDimID)
{
	wchar_t wszText[1024];
	int nIndex;

	/* File */
	nIndex = SendMessage(GetDlgItem(hWnd, nComboFileID), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (nIndex == COMBO_CHANGE)
	{
		GetWindowText(GetDlgItem(hWnd, nTextFileID), wszText, sizeof(wszText) / sizeof(wchar_t));
		if (wcscmp(wszText, L"") == 0)
		{
			MessageBox(hWnd,
					   bEnglish ?
					   L"Fill the center file name." :
					   L"中央ファイル名を指定してください。",
					   DIALOG_TITLE,
					   MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
		strncat(pszCmd, " ", nLen - 1);
		strncat(pszCmd, pszFile, nLen - 1);
		strncat(pszCmd, "=", nLen - 1);
		strncat(pszCmd, conv_utf16_to_utf8(wszText), nLen - 1);
	}
	else if (nIndex == COMBO_NONE)
	{
		strncat(pszCmd, " c=none", nLen - 1);
	}

	/* Alpha */
	if (pszAlpha != NULL)
	{
		GetWindowText(GetDlgItem(hWnd, nTextAlphaID), wszText, sizeof(wszText) / sizeof(wchar_t));
		if (wcscmp(wszText, L"") != 0)
		{
			strncat(pszCmd, " ", nLen - 1);
			strncat(pszCmd, pszAlpha, nLen - 1);
			strncat(pszCmd, "=", nLen - 1);
			strncat(pszCmd, conv_utf16_to_utf8(wszText), nLen - 1);
		}
	}

	/* X */
	GetWindowText(GetDlgItem(hWnd, nTextXID), wszText, sizeof(wszText) / sizeof(wchar_t));
	if (wcscmp(wszText, L"") != 0 && wcscmp(wszText, L"0") != 0)
	{
		strncat(pszCmd, " ", nLen - 1);
		strncat(pszCmd, pszX, nLen - 1);
		strncat(pszCmd, "=", nLen - 1);
		strncat(pszCmd, conv_utf16_to_utf8(wszText), nLen - 1);
	}

	/* Y */
	GetWindowText(GetDlgItem(hWnd, nTextYID), wszText, sizeof(wszText) / sizeof(wchar_t));
	if (wcscmp(wszText, L"") != 0 && wcscmp(wszText, L"0") != 0)
	{
		strncat(pszCmd, " ", nLen - 1);
		strncat(pszCmd, pszY, nLen - 1);
		strncat(pszCmd, "=", nLen - 1);
		strncat(pszCmd, conv_utf16_to_utf8(wszText), nLen - 1);
	}

	/* Dim */
	if (nComboDimID != -1)
	{
		nIndex = SendMessage(GetDlgItem(hWnd, nComboDimID), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
		if (nIndex == COMBO_NORMAL)
		{
			strncat(pszCmd, " ", nLen - 1);
			strncat(pszCmd, pszDim, nLen - 1);
			strncat(pszCmd, "=no", nLen - 1);
		}
		else if (nIndex == COMBO_DARK)
		{
			strncat(pszCmd, " ", nLen - 1);
			strncat(pszCmd, pszDim, nLen - 1);
			strncat(pszCmd, "=yes", nLen - 1);
		}
	}

	return TRUE;
}

BOOL CALLBACK DlgChchWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
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
