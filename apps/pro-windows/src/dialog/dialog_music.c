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

static VOID OnInit(HWND hWnd)
{
	const char *pszText;

	UNUSED_PARAMETER(hWnd);

	/* File */
	pszText = get_string_param(MUSIC_PARAM_FILE);
	if (strcmp(pszText, "stop") == 0)
	{
		SendMessage(GetDlgItem(hWnd, IDC_CHECK_STOP), (UINT)BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
		EnableWindow(GetDlgItem(hWnd, IDC_TEXT_FILE1), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHECK_ONCE), FALSE);
	}
	else
	{
		SetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), conv_utf8_to_utf16(pszText));
	}

	/* Once */
	pszText = get_string_param(MUSIC_PARAM_ONCE);
	if (strcmp(pszText, "once") == 0)
		SendMessage(GetDlgItem(hWnd, IDC_CHECK_ONCE), (UINT)BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
}

static VOID OnCommand(HWND hWnd, UINT nID)
{
	if (nID == IDC_BUTTON_FILE1)
	{
		const wchar_t *pwszFile;
		pwszFile = SelectFile(BGM_DIR);
		if (pwszFile != NULL)
		{
			SetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), pwszFile);
		}
		return;
	}

	if (nID == IDC_CHECK_STOP)
	{
		if (!SendMessage(GetDlgItem(hWnd, IDC_CHECK_STOP), (UINT)BM_GETCHECK, (WPARAM)0, (LPARAM)0))
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_STOP), (UINT)BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
			EnableWindow(GetDlgItem(hWnd, IDC_TEXT_FILE1), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_CHECK_ONCE), FALSE);
		}
		else
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_STOP), (UINT)BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
			EnableWindow(GetDlgItem(hWnd, IDC_TEXT_FILE1), TRUE);
			EnableWindow(GetDlgItem(hWnd, IDC_CHECK_ONCE), TRUE);
		}
		return;
	}

	if (nID == IDC_CHECK_ONCE)
	{
		if (!SendMessage(GetDlgItem(hWnd, IDC_CHECK_ONCE), (UINT)BM_GETCHECK, (WPARAM)0, (LPARAM)0))
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_ONCE), (UINT)BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
		else
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_ONCE), (UINT)BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
		return;
	}
}

static BOOL OnFinish(HWND hWnd)
{
	wchar_t wszText[1024];
	char szCmd[4096];

	strncpy(szCmd, "@music", sizeof(szCmd) - 1);

	/* File */
	if (!SendMessage(GetDlgItem(hWnd, IDC_CHECK_STOP), BM_GETCHECK, (WPARAM)0, (LPARAM)0))
	{
		GetWindowText(GetDlgItem(hWnd, IDC_TEXT_FILE1), wszText, sizeof(wszText) / sizeof(wchar_t));
		if (wcscmp(wszText, L"") == 0)
		{
			MessageBox(hWnd,
					   bEnglish ?
					   L"Fill the music file name." :
					   L"音楽ファイル名を指定してください。",
					   DIALOG_TITLE,
					   MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
		strncat(szCmd, " file=", sizeof(szCmd) - 1);
		strncat(szCmd, conv_utf16_to_utf8(wszText), sizeof(szCmd) - 1);
	}
	else
	{
		strncat(szCmd, " stop", sizeof(szCmd) - 1);

		/* Update the script model. */
		update_script_line(get_expanded_line_num(), szCmd);
		return TRUE;
	}

	/* Once */
	if (SendMessage(GetDlgItem(hWnd, IDC_CHECK_ONCE), BM_GETCHECK, (WPARAM)0, (LPARAM)0))
		strncat(szCmd, " once", sizeof(szCmd) - 1);

	/* Update the script model. */
	update_script_line(get_expanded_line_num(), szCmd);
	return TRUE;
}

BOOL CALLBACK DlgMusicWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
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
