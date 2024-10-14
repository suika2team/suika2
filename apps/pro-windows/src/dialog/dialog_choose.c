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
	UNUSED_PARAMETER(hWnd);

	/* Opt 1-10 */
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT1), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_TEXT1)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT2), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_TEXT2)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT3), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_TEXT3)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT4), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_TEXT4)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT5), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_TEXT5)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT6), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_TEXT6)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT7), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_TEXT7)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT8), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_TEXT8)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT9), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_TEXT9)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT10), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_TEXT10)));

	/* Label 1-10 */
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL1), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_LABEL1)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL2), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_LABEL2)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL3), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_LABEL3)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL4), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_LABEL4)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL5), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_LABEL5)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL6), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_LABEL6)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL7), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_LABEL7)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL8), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_LABEL8)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL9), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_LABEL9)));
	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL10), conv_utf8_to_utf16(get_string_param(CHOOSE_PARAM_LABEL10)));
}

static VOID OnCommand(HWND hWnd, UINT nID)
{
	UNUSED_PARAMETER(hWnd);
	UNUSED_PARAMETER(nID);
}

static BOOL OnFinish(HWND hWnd)
{
	wchar_t wszOpt[1024];
	wchar_t wszLabel[1024];
	char szCmd[4096];
	int nIndex;

	strncpy(szCmd, "@choose", sizeof(szCmd) - 1);

	/* Opt1 */
	for (nIndex = 0; nIndex < 10; nIndex++)
	{
		GetWindowText(GetDlgItem(hWnd, IDC_TEXT_OPT1 + nIndex), wszOpt, sizeof(wszOpt) / sizeof(wchar_t));
		GetWindowText(GetDlgItem(hWnd, IDC_TEXT_LABEL1 + nIndex), wszLabel, sizeof(wszLabel) / sizeof(wchar_t));

		if (wcscmp(wszOpt, L"") == 0 && wcscmp(wszLabel, L"") == 0)
			continue;
		if (wcscmp(wszOpt, L"") == 0 && wcscmp(wszLabel, L"") != 0)
		{
			MessageBox(hWnd,
					   bEnglish ?
					   L"Fill the option text." :
					   L"選択肢テキストを指定してください。",
					   DIALOG_TITLE,
					   MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
		if (wcscmp(wszOpt, L"") != 0 && wcscmp(wszLabel, L"") == 0)
		{
			MessageBox(hWnd,
					   bEnglish ?
					   L"Fill the label text." :
					   L"ラベルテキストを指定してください。",
					   DIALOG_TITLE,
					   MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		strncat(szCmd, " \"", sizeof(szCmd) - 1);
		strncat(szCmd, conv_utf16_to_utf8(wszLabel), sizeof(szCmd) - 1);
		strncat(szCmd, "\" \"", sizeof(szCmd) - 1);
		strncat(szCmd, conv_utf16_to_utf8(wszOpt), sizeof(szCmd) - 1);
		strncat(szCmd, "\"", sizeof(szCmd) - 1);
	}

	/* Update the script model. */
	update_script_line(get_expanded_line_num(), szCmd);

	return TRUE;
}

BOOL CALLBACK DlgChooseWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
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
