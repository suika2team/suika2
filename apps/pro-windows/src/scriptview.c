/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * ScriptView
 */

/* Base */
#include "opennovel.h"

/* Editor */
#include "app.h"
#include "scriptview.h"

/* Windows */
#include <windows.h>
#include <richedit.h>		/* RichEdit */
#include "resource.h"

/*
 * Constants
 */

/* Light Color Theme */
#define LIGHT_BG_DEFAULT	0x00ffffff
#define LIGHT_FG_DEFAULT	0x00000000
#define LIGHT_COMMENT		0x00808080
#define LIGHT_LABEL			0x00ff0000
#define LIGHT_ERROR			0x000000ff
#define LIGHT_COMMAND_NAME	0x00ff0000
#define LIGHT_CIEL_COMMAND	0x00cba55d
#define LIGHT_PARAM_NAME	0x00c0f0c0
#define LIGHT_NEXT_EXEC		0x00ffc0c0
#define LIGHT_CURRENT_EXEC	0x00c0c0ff
#define LIGHT_SELECTED		0x0033ccff

/* Dark Color Theme */
#define DARK_BG_DEFAULT		0x00282828
#define DARK_FG_DEFAULT		0x00ffffff
#define DARK_COMMENT		0x00808080
#define DARK_LABEL			0x006200ee
#define DARK_ERROR			0x000000ff
#define DARK_COMMAND_NAME	0x0060a0a0
#define DARK_CIEL_COMMAND	0x00ecd790
#define DARK_PARAM_NAME		0x00e0acac
#define DARK_NEXT_EXEC		0x002d623a
#define DARK_CURRENT_EXEC	0x00353562
#define DARK_SELECTED		0x00282828

/* Timer */
#define ID_TIMER_FORMAT		(1)

/* The font name for the script view. */
#define SCRIPT_FONT_JP		L"BIZ UDゴシック"
#define SCRIPT_FONT_EN		L"Courier New"

/*
 * Variables
 */

/* HWND for the ScriptView. */
static HWND hWndScriptView;

/* The font name for the ScriptView. */
static wchar_t wszFontName[128];

/* The font size for the ScriptView. */
static int nFontSize = 10;

/* A flag to show whether we are using the highlight mode or not. */
static BOOL bHighlightMode;

/* A flag to show whether we are using the dark mode or not. */
static BOOL bDarkMode;

/* Color Theme */
static DWORD dwColorBgDefault = LIGHT_BG_DEFAULT;
static DWORD dwColorFgDefault = LIGHT_FG_DEFAULT;
static DWORD dwColorComment = LIGHT_COMMENT;
static DWORD dwColorLabel = LIGHT_LABEL;
static DWORD dwColorError = LIGHT_ERROR;
static DWORD dwColorCommandName = LIGHT_COMMAND_NAME;
static DWORD dwColorCielCommand = LIGHT_CIEL_COMMAND;
static DWORD dwColorParamName = LIGHT_PARAM_NAME;
static DWORD dwColorNextExec = LIGHT_NEXT_EXEC;
static DWORD dwColorCurrentExec = LIGHT_CURRENT_EXEC;

/* Tempolaliry ignore any changes to the RichEdit control. */
static BOOL bIgnoreChange;

/* A timestamp of the current editing file. */
static FILETIME ftTimeStamp;

/*
 * Forward Declarations
 */

static void __stdcall OnFormatTimer(HWND hWnd, UINT nID, UINT_PTR uTime, DWORD dwParam);

/*
 * This function creates a ScriptView.
 */
HWND ScriptView_Create(HWND hWndParent)
{
	wchar_t wszCls[128];
	int nDpi;

	nDpi = App_GetDpiForWindow(hWndParent);

	/* We use RichEdit50W. */
	LoadLibrary(L"Msftedit.dll");

	/* First, we try to create a RichEdit50W control shipped with Windows 7 or later. */
	hWndScriptView = CreateWindowEx(
		0,
		MSFTEDIT_CLASS, /* RichEdit50W */
		L"Script",
		ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | ES_AUTOVSCROLL,
		MulDiv(10, nDpi, 96),
		MulDiv(100, nDpi, 96),
		MulDiv(420, nDpi, 96),
		MulDiv(400, nDpi, 96),
		hWndParent,
		(HMENU)ID_RICHEDIT,
		GetModuleHandle(NULL),
		NULL);

	/* If failed, we create a RichEdit30W control for Windows XP. */
	if (hWndScriptView == NULL)
	{
		hWndScriptView = CreateWindowEx(
			0,
			RICHEDIT_CLASS, /* RichEdit30W */
			L"Script",
			ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | ES_AUTOVSCROLL,
			MulDiv(10, nDpi, 96),
			MulDiv(100, nDpi, 96),
			MulDiv(420, nDpi, 96),
			MulDiv(400, nDpi, 96),
			hWndParent,
			(HMENU)ID_RICHEDIT,
			GetModuleHandle(NULL),
			NULL);
	}

	/*
	 * In case we use the non-Office version of RichEdit.
	 *  - Windows has a RichEdit DLL by default.
	 *  - However, if Microsoft Office is installed, RichEdit is replaced by the Office version.
	 *  - We handle a difference between the two versions.
	 */
	GetClassName(hWndScriptView, wszCls, sizeof(wszCls) / sizeof(wchar_t));
	if (wcscmp(wszCls, L"RICHEDIT50W") == 0)
	{
		/* If we use the non-Office version, we don't use auto-scrolling. */
		LONG style = GetWindowLong(hWndScriptView, GWL_STYLE);
		style &= ~ES_AUTOVSCROLL;
		SetWindowLong(hWndScriptView, GWL_STYLE, style);
	}

	SendMessage(hWndScriptView, EM_SHOWSCROLLBAR, (WPARAM)SB_VERT, (LPARAM)TRUE);
	SendMessage(hWndScriptView, EM_SETEVENTMASK, 0, (LPARAM)ENM_CHANGE);
	SendMessage(hWndScriptView, EM_SETBKGNDCOLOR, (WPARAM)0, (LPARAM)dwColorBgDefault);

	wcscpy(wszFontName, App_IsEnglishMode() ? SCRIPT_FONT_EN : SCRIPT_FONT_JP);
	ScriptView_ApplyFont();

	EnableWindow(hWndScriptView, FALSE);

	return hWndScriptView;
}

/*
 * This function returns the HWND for the ScriptView.
 */
HWND ScriptView_GetHWND(void)
{
	return hWndScriptView;
}

/*
 * This function processes the notification to ScriptView.
 */
void ScriptView_OnChange(void)
{
	int nCursor;

	if (bIgnoreChange)
	{
		bIgnoreChange = FALSE;
		return;
	}

	/* カーソル位置を取得する */
	nCursor = ScriptView_GetCursorPosition();

	SetFocus(NULL);

	/* フォントを設定する */
	ScriptView_ApplyFont();

	/* 実行行の背景色を設定する */
	if (App_IsRunning())
		ScriptView_SetBackgroundColorForCurrentExecuteLine();
	else
		ScriptView_SetBackgroundColorForNextExecuteLine();

	/*
	 * カーソル位置を設定する
	 *  - 色付けで選択が変更されたのを修正する
	 */
	ScriptView_SetCursorPosition(nCursor);

	SetFocus(hWndScriptView);

	/* 全体のテキスト色を変更する(遅延) */
	ScriptView_DelayedHighligth();
}

/* リッチエディットのフォントを設定する */
void ScriptView_ApplyFont(void)
{
	CHARFORMAT2W cf;

	bIgnoreChange = TRUE;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_FACE | CFM_SIZE;
	cf.yHeight = nFontSize * 20;
	wcscpy(&cf.szFaceName[0], wszFontName);
	SendMessage(hWndScriptView, EM_SETCHARFORMAT, (WPARAM)SCF_ALL, (LPARAM)&cf);
}

/* リッチエディットのカーソル位置を取得する */
int ScriptView_GetCursorPosition(void)
{
	CHARRANGE cr;

	/* カーソル位置を取得する */
	SendMessage(hWndScriptView, EM_EXGETSEL, 0, (LPARAM)&cr);

	return cr.cpMin;
}

/* リッチエディットの選択範囲の長さを取得する */
int ScriptView_GetSelectedLen(void)
{
	CHARRANGE cr;

	/* カーソル位置を取得する */
	SendMessage(hWndScriptView, EM_EXGETSEL, 0, (LPARAM)&cr);

	return cr.cpMax - cr.cpMin;
}

/* リッチエディットのカーソル位置を設定する */
void ScriptView_SetCursorPosition(int nCursor)
{
	CHARRANGE cr;

	bIgnoreChange = TRUE;
	cr.cpMin = nCursor;
	cr.cpMax = nCursor;
	SendMessage(hWndScriptView, EM_EXSETSEL, 0, (LPARAM)&cr);
}

/* リッチエディットの範囲を選択する */
void ScriptView_SetSelectedRange(int nLineStart, int nLineLen)
{
	CHARRANGE cr;

	bIgnoreChange = TRUE;
	memset(&cr, 0, sizeof(cr));
	cr.cpMin = nLineStart;
	cr.cpMax = nLineStart + nLineLen;
	SendMessage(hWndScriptView, EM_EXSETSEL, 0, (LPARAM)&cr);
}

/* リッチエディットのカーソル行の行番号を取得する */
int ScriptView_GetCursorLine(void)
{
	wchar_t *pWcs, *pCRLF;
	int nTotal, nCursor, nLineStartCharCR, nLineStartCharCRLF, nLine;

	pWcs = ScriptView_GetText();
	nTotal = (int)wcslen(pWcs);
	nCursor = ScriptView_GetCursorPosition();
	nLineStartCharCR = 0;
	nLineStartCharCRLF = 0;
	nLine = 0;
	while (nLineStartCharCRLF < nTotal)
	{
		pCRLF = wcswcs(pWcs + nLineStartCharCRLF, L"\r\n");
		int nLen = (pCRLF != NULL) ?
			(int)(pCRLF - (pWcs + nLineStartCharCRLF)) :
			(int)wcslen(pWcs + nLineStartCharCRLF);
		if (nCursor >= nLineStartCharCR && nCursor <= nLineStartCharCR + nLen)
			break;
		nLineStartCharCRLF += nLen + 2; /* +2 for CRLF */
		nLineStartCharCR += nLen + 1; /* +1 for CR */
		nLine++;
	}
	free(pWcs);

	return nLine;
}

/* リッチエディットのテキストを取得する */
wchar_t *ScriptView_GetText(void)
{
	wchar_t *pText;
	int nTextLen;

	/* リッチエディットのテキストの長さを取得する */
	nTextLen = (int)SendMessage(hWndScriptView, WM_GETTEXTLENGTH, 0, 0);
	if (nTextLen == 0)
	{
		pText = wcsdup(L"");
		if (pText == NULL)
		{
			log_memory();
			abort();
		}
	}

	/* テキスト全体を取得する */
	pText = malloc((size_t)(nTextLen + 1) * sizeof(wchar_t));
	if (pText == NULL)
	{
		log_memory();
		abort();
	}
	SendMessage(hWndScriptView, WM_GETTEXT, (WPARAM)(nTextLen + 1), (LPARAM)pText);
	pText[nTextLen] = L'\0';

	return pText;
}

/* リッチエディットのテキストすべてについて、行の内容により色付けを行う */
void ScriptView_SetTextColorForAllLines(void)
{
	wchar_t *pText, *pLineStop;
	int i, nLineStartCRLF, nLineStartCR, nLineLen;

	pText = ScriptView_GetText();
	nLineStartCRLF = 0;		/* WM_GETTEXTは改行をCRLFで返す */
	nLineStartCR = 0;		/* EM_EXSETSELでは改行はCRの1文字 */
	for (i = 0; i < get_line_count(); i++)
	{
		/* 行の終了位置を求める */
		pLineStop = wcswcs(pText + nLineStartCRLF, L"\r\n");
		nLineLen = pLineStop != NULL ?
			(int)(pLineStop - (pText + nLineStartCRLF)) :
			(int)wcslen(pText + nLineStartCRLF);

		/* 行の色付けを行う */
		ScriptView_SetTextColorForLine(pText, nLineStartCR, nLineStartCRLF, nLineLen);

		/* 次の行へ移動する */
		nLineStartCRLF += nLineLen + 2;	/* +2 for CRLF */
		nLineStartCR += nLineLen + 1;	/* +1 for CR */
	}
	free(pText);
}

/* 特定の行のテキスト色を設定する */
void ScriptView_SetTextColorForLine(const wchar_t *pText, int nLineStartCR, int nLineStartCRLF, int nLineLen)
{
	wchar_t wszCommandName[1024];
	const wchar_t *pCommandSpaceStop, *pCommandCRStop, *pParamStart, *pParamStop, *pParamSpace;
	int nParamLen, nCommandType;

	/* 行を選択して選択範囲のテキスト色をデフォルトに変更する */
	ScriptView_SetSelectedRange(nLineStartCR, nLineLen);
	ScriptView_SetTextColorForSelectedRange(dwColorFgDefault);

	/* コメントを処理する */
	if (pText[nLineStartCRLF] == L'#')
	{
		/* 行全体を選択して、選択範囲のテキスト色を変更する */
		ScriptView_SetSelectedRange(nLineStartCR, nLineLen);
		ScriptView_SetTextColorForSelectedRange(dwColorComment);
	}
	/* ラベルを処理する */
	else if (pText[nLineStartCRLF] == L':')
	{
		/* 行全体を選択して、選択範囲のテキスト色を変更する */
		ScriptView_SetSelectedRange(nLineStartCR, nLineLen);
		ScriptView_SetTextColorForSelectedRange(dwColorLabel);
	}
	/* エラー行を処理する */
	if (pText[nLineStartCRLF] == L'!')
	{
		/* 行全体を選択して、選択範囲のテキスト色を変更する */
		ScriptView_SetSelectedRange(nLineStartCR, nLineLen);
		ScriptView_SetTextColorForSelectedRange(dwColorError);
	}
	/* コマンド行を処理する */
	else if (pText[nLineStartCRLF] == L'@')
	{
		/* コマンド名部分を抽出する */
		pCommandSpaceStop = wcswcs(pText + nLineStartCRLF, L" ");
		pCommandCRStop = wcswcs(pText + nLineStartCRLF, L"\r\n");
		if (pCommandSpaceStop == NULL || pCommandCRStop == NULL)
			nParamLen = nLineLen; /* EOF */
		else if (pCommandSpaceStop < pCommandCRStop)
			nParamLen = (int)(pCommandSpaceStop - (pText + nLineStartCRLF));
		else
			nParamLen = (int)(pCommandCRStop - (pText + nLineStartCRLF));
		wcsncpy(wszCommandName, &pText[nLineStartCRLF],
				(size_t)nParamLen < sizeof(wszCommandName) / sizeof(wchar_t) ?
				(size_t)nParamLen :
				sizeof(wszCommandName) / sizeof(wchar_t));
		wszCommandName[nParamLen] = L'\0';

		nCommandType = get_command_type_from_name(conv_utf16_to_utf8(wszCommandName));
		if (nCommandType != -1)
		{
			/* コマンド名のテキストに色を付ける */
			ScriptView_SetSelectedRange(nLineStartCR, nParamLen);
			if (nCommandType != COMMAND_CIEL)
				ScriptView_SetTextColorForSelectedRange(dwColorCommandName);
			else
				ScriptView_SetTextColorForSelectedRange(dwColorCielCommand);

			if (nCommandType != COMMAND_SET &&
				nCommandType != COMMAND_IF &&
				nCommandType != COMMAND_UNLESS &&
				nCommandType != COMMAND_TEXT)
			{
				/* 引数名を灰色にする */
				pParamStart = pText + nLineStartCRLF + nParamLen;
				while ((pParamStart = wcswcs(pParamStart, L" ")) != NULL)
				{
					int nNameStart;
					int nNameLen;

					/* 次の行以降の' 'にヒットしている場合はループから抜ける */
					if (pParamStart >= pText + nLineStartCRLF + nLineLen)
						break;

					/* ' 'の次の文字を開始位置にする */
					pParamStart++;

					/* '='を探す。次の行以降にヒットした場合はループから抜ける */
					pParamStop = wcswcs(pParamStart, L"=");
					if (pParamStop == NULL || pParamStop >= pText + nLineStartCRLF + nLineLen)
						break;

					/* '='の手前に' 'があればスキップする */
					pParamSpace = wcswcs(pParamStart, L" ");
					if (pParamSpace != NULL && pParamSpace < pParamStop)
						continue;

					/* 引数名部分を選択してテキスト色を変更する */
					nNameStart = nLineStartCR + (int)(pParamStart - (pText + nLineStartCRLF));
					nNameLen = (int)(pParamStop - pParamStart) + 1;
					ScriptView_SetSelectedRange(nNameStart, nNameLen);
					ScriptView_SetTextColorForSelectedRange(dwColorParamName);
				}
			}
		}
	}
}

/* 次の実行行の背景色を設定する */
void ScriptView_SetBackgroundColorForNextExecuteLine(void)
{
	int nLine, nLineStart, nLineLen;

	/* すべてのテキストの背景色を白にする */
	ScriptView_ClearBackgroundColorAll();

	/* 実行行を取得する */
	nLine = get_expanded_line_num();

	/* 実行行の開始文字と終了文字を求める */
	ScriptView_GetLineStartAndLength(nLine, &nLineStart, &nLineLen);

	/* 実行行を選択する */
	ScriptView_SetSelectedRange(nLineStart, nLineLen);

	/* 選択範囲の背景色を変更する */
	ScriptView_SetBackgroundColorForSelectedRange(dwColorNextExec);

	/* カーソル位置を実行行の先頭に設定する */
	ScriptView_SetCursorPosition(nLineStart);
}

/* 現在実行中の行の背景色を設定する */
void ScriptView_SetBackgroundColorForCurrentExecuteLine(void)
{
	int nLine, nLineStart, nLineLen;

	/* すべてのテキストの背景色を白にする */
	ScriptView_ClearBackgroundColorAll();

	/* 実行行を取得する */
	nLine = get_expanded_line_num();

	/* 実行行の開始文字と終了文字を求める */
	ScriptView_GetLineStartAndLength(nLine, &nLineStart, &nLineLen);

	/* 実行行を選択する */
	ScriptView_SetSelectedRange(nLineStart, nLineLen);

	/* 選択範囲の背景色を変更する */
	ScriptView_SetBackgroundColorForSelectedRange(dwColorCurrentExec);

	/* カーソル位置を実行行の先頭に設定する */
	ScriptView_SetCursorPosition(nLineStart);
}

/* リッチエディットの書式をクリアする */
void ScriptView_ClearFormatAll(void)
{
	CHARFORMAT2W cf;

	bIgnoreChange = TRUE;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BACKCOLOR | CFM_COLOR;
	cf.crBackColor = dwColorBgDefault;
	cf.crTextColor = dwColorFgDefault;
	SendMessage(hWndScriptView, EM_SETCHARFORMAT, (WPARAM)SCF_ALL, (LPARAM)&cf);
}

/* リッチエディットのテキスト全体の背景色をクリアする */
void ScriptView_ClearBackgroundColorAll(void)
{
	CHARFORMAT2W cf;

	bIgnoreChange = TRUE;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BACKCOLOR;
	cf.crBackColor = dwColorBgDefault;
	SendMessage(hWndScriptView, EM_SETCHARFORMAT, (WPARAM)SCF_ALL, (LPARAM)&cf);
}

/* リッチエディットの選択範囲のテキスト色を変更する */
void ScriptView_SetTextColorForSelectedRange(COLORREF cl)
{
	CHARFORMAT2W cf;

	bIgnoreChange = TRUE;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = cl;
	bIgnoreChange = TRUE;
	SendMessage(hWndScriptView, EM_SETCHARFORMAT, (WPARAM)SCF_SELECTION, (LPARAM)&cf);
}

/* リッチエディットの選択範囲の背景色を変更する */
void ScriptView_SetBackgroundColorForSelectedRange(COLORREF cl)
{
	CHARFORMAT2W cf;

	bIgnoreChange = TRUE;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BACKCOLOR;
	cf.crBackColor = cl;
	SendMessage(hWndScriptView, EM_SETCHARFORMAT, (WPARAM)SCF_SELECTION, (LPARAM)&cf);
}

/* リッチエディットを自動スクロールする */
void ScriptView_AutoScroll(void)
{
	/* リッチエディットをフォーカスする */
	SetFocus(hWndScriptView);

	/* リッチエディットをスクロールする */
	SendMessage(hWndScriptView, EM_SETREADONLY, TRUE, 0);
	SendMessage(hWndScriptView, EM_SCROLLCARET, 0, 0);
	SendMessage(hWndScriptView, EM_SETREADONLY, FALSE, 0);

	/* リッチエディットを再描画する */
	InvalidateRect(hWndScriptView, NULL, TRUE);
}

/* 実行行の開始文字と終了文字を求める */
void ScriptView_GetLineStartAndLength(int nLine, int *nLineStart, int *nLineLen)
{
	wchar_t *pText, *pCRLF;
	int i, nLineStartCharCRLF, nLineStartCharCR;

	pText = ScriptView_GetText();
	nLineStartCharCRLF = 0;		/* WM_GETTEXTは改行をCRLFで返す */
	nLineStartCharCR = 0;		/* EM_EXSETSELでは改行はCRの1文字 */
	for (i = 0; i < nLine; i++)
	{
		int nLen;
		pCRLF = wcswcs(pText + nLineStartCharCRLF, L"\r\n");
		nLen = pCRLF != NULL ?
			(int)(pCRLF - (pText + nLineStartCharCRLF)) :
			(int)wcslen(pText + nLineStartCharCRLF);
		nLineStartCharCRLF += nLen + 2;		/* +2 for CRLF */
		nLineStartCharCR += nLen + 1;		/* +1 for CR */
	}
	pCRLF = wcswcs(pText + nLineStartCharCRLF, L"\r\n");
	*nLineStart = nLineStartCharCR;
	*nLineLen = pCRLF != NULL ?
		(int)(pCRLF - (pText + nLineStartCharCRLF)) :
		(int)wcslen(pText + nLineStartCharCRLF);
	free(pText);
}

/* リッチエディットで次のエラーを探す */
BOOL ScriptView_SearchNextError(int nStart, int nEnd)
{
	wchar_t *pWcs, *pCRLF, *pLine;
	int nTotal, nLineStartCharCR, nLineStartCharCRLF, nLen;
	BOOL bFound;

	/* リッチエディットのテキストの内容でスクリプトの各行をアップデートする */
	pWcs = ScriptView_GetText();
	nTotal = (int)wcslen(pWcs);
	nLineStartCharCR = nStart;
	nLineStartCharCRLF = 0;
	bFound = FALSE;
	while (nLineStartCharCRLF < nTotal)
	{
		if (nEnd != -1 && nLineStartCharCRLF >= nEnd)
			break;

		/* 行を切り出す */
		pLine = pWcs + nLineStartCharCRLF;
		pCRLF = wcswcs(pLine, L"\r\n");
		nLen = (pCRLF != NULL) ?
			(int)(pCRLF - (pWcs + nLineStartCharCRLF)) :
			(int)wcslen(pWcs + nLineStartCharCRLF);
		if (pCRLF != NULL)
			*pCRLF = L'\0';

		/* エラーを発見したらカーソルを移動する */
		if (pLine[0] == L'!')
		{
			bFound = TRUE;
			ScriptView_SetCursorPosition(nLineStartCharCR);
			break;
		}

		nLineStartCharCRLF += nLen + 2; /* +2 for CRLF */
		nLineStartCharCR += nLen + 1; /* +1 for CR */
	}
	free(pWcs);

	return bFound;
}

/* リッチエディットのテキストをスクリプトモデルを元に設定する */
void ScriptView_SetTextByScriptModel(void)
{
	wchar_t *pWcs;
	int nScriptSize;
	int i;

	/* スクリプトのサイズを計算する */
	nScriptSize = 0;
	for (i = 0; i < get_line_count(); i++)
	{
		const char *pUtf8Line = get_line_string_at_line_num(i);
//		nScriptSize += (int)strlen(pUtf8Line) + 1; /* +1 for CR */
		nScriptSize += (int)strlen(pUtf8Line) + 2; /* +2 for CRLF */
	}

	/* スクリプトを格納するメモリを確保する */
	pWcs = malloc((size_t)(nScriptSize + 1) * sizeof(wchar_t));
	if (pWcs == NULL)
	{
		log_memory();
		abort();
	}

	/* 行を連列してスクリプト文字列を作成する */
	pWcs[0] = L'\0';
	for (i = 0; i < get_line_count(); i++)
	{
		const char *pUtf8Line = get_line_string_at_line_num(i);
		wcscat(pWcs, conv_utf8_to_utf16(pUtf8Line));
//		wcscat(pWcs, L"\r");
		wcscat(pWcs, L"\r\n");
	}

	/* リッチエディットにテキストを設定する */
	bIgnoreChange = TRUE;
	SetWindowText(hWndScriptView, pWcs);

	/* メモリを解放する */
	free(pWcs);
}

/* リッチエディットの内容を元にスクリプトモデルを更新する */
void ScriptView_UpdateScriptModelFromText(void)
{
	char szLine[2048];
	wchar_t *pWcs, *pCRLF;
	int i, nTotal, nLine, nLineStartCharCRLF;

	/* パースエラーをリセットして、最初のパースエラーで通知を行う */
	dbg_reset_parse_error_count();

	/* リッチエディットのテキストの内容でスクリプトの各行をアップデートする */
	pWcs = ScriptView_GetText();
	nTotal = (int)wcslen(pWcs);
	nLine = 0;
	nLineStartCharCRLF = 0;
	while (nLineStartCharCRLF < nTotal)
	{
		wchar_t *pLine;
		int nLen;

		/* 行を切り出す */
		pLine = pWcs + nLineStartCharCRLF;
		pCRLF = wcswcs(pWcs + nLineStartCharCRLF, L"\r\n");
		nLen = (pCRLF != NULL) ?
			(int)(pCRLF - (pWcs + nLineStartCharCRLF)) :
			(int)wcslen(pWcs + nLineStartCharCRLF);
		if (pCRLF != NULL)
			*pCRLF = L'\0';

		/* 行を更新する */
		strncpy(szLine, conv_utf16_to_utf8(pLine), sizeof(szLine) - 1);
		szLine[sizeof(szLine) - 1] = '\0';
		if (nLine < get_line_count())
			update_script_line(nLine, szLine);
		else
			insert_script_line(nLine, szLine);

		nLine++;
		nLineStartCharCRLF += nLen + 2; /* +2 for CRLF */
	}
	free(pWcs);

	/* 削除された末尾の行を処理する */
	App_SetExecLineChanged(FALSE);
	for (i = get_line_count() - 1; i >= nLine; i--)
		if (delete_script_line(nLine))
			App_SetExecLineChanged(TRUE);

	/* 拡張構文がある場合に対応する */
	reparse_script_for_structured_syntax();

	/* コマンドのパースに失敗した場合 */
	if (dbg_get_parse_error_count() > 0)
	{
		/* 行頭の'!'を反映するためにテキストを再設定する */
		ScriptView_SetTextByScriptModel();
	}
}

/* テキストを挿入する */
void ScriptView_InsertText(const wchar_t *pFormat, ...)
{
	va_list ap;
	wchar_t buf[1024];
		
	int nLine, nLineStart, nLineLen;

	va_start(ap, pFormat);
	vswprintf(buf, sizeof(buf) / sizeof(wchar_t), pFormat, ap);
	va_end(ap);

	/* カーソル行を取得する */
	nLine = ScriptView_GetCursorLine();

	/* 行の先頭にカーソルを移す */
	ScriptView_GetLineStartAndLength(nLine, &nLineStart, &nLineLen);
	ScriptView_SetCursorPosition(nLineStart);

	/* スクリプトモデルに行を追加する */
	insert_script_line(nLine, conv_utf16_to_utf8(buf));

	/* リッチエディットに行を追加する */
	wcscat(buf, L"\r");
	ScriptView_SetTextColorForSelectedRange(dwColorFgDefault);
	SendMessage(hWndScriptView, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)buf);

	/* 行を選択する */
	ScriptView_SetCursorPosition(nLineStart);

	/* 次のフレームで実行位置を変更する */
	App_SetChangedLine(nLine);
	App_SetExecLineChanged(TRUE);
}

/* テキストを行末に挿入する */
void ScriptView_InsertTextAtEnd(const wchar_t *pszText)
{
	int nLine, nLineStart, nLineLen;

	/* カーソル行を取得する */
	nLine = ScriptView_GetCursorLine();

	/* 行の末尾にカーソルを移す */
	ScriptView_GetLineStartAndLength(nLine, &nLineStart, &nLineLen);
	ScriptView_SetCursorPosition(nLineStart + nLineLen);

	/* リッチエディットにテキストを追加する */
	SendMessage(hWndScriptView, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)pszText);
}

void ScriptView_UpdateTheme(void)
{
	int nCursor;

	if (hWndScriptView == NULL)
		return;

	SetFocus(NULL);

	nCursor = ScriptView_GetCursorPosition();
	SendMessage(hWndScriptView, EM_SETBKGNDCOLOR, (WPARAM)0, (LPARAM)dwColorBgDefault);
	ScriptView_ClearFormatAll();
	ScriptView_ClearBackgroundColorAll();
	if (bHighlightMode)
		ScriptView_SetTextColorForAllLines();
	if (!App_IsRunning())
		ScriptView_SetBackgroundColorForNextExecuteLine();
	else
		ScriptView_SetBackgroundColorForCurrentExecuteLine();
	ScriptView_SetCursorPosition(nCursor);
	ScriptView_AutoScroll();

	SetFocus(hWndScriptView);
}

void ScriptView_DelayedHighligth(void)
{
	SetTimer(hWndScriptView, ID_TIMER_FORMAT, 100, OnFormatTimer);
}

static void __stdcall OnFormatTimer(HWND hWnd, UINT nID, UINT_PTR uTime, DWORD dwParam)
{
	HIMC hImc;
	DWORD dwConversion, dwSentence;
	int nCursor;
	BOOL bRet;

	UNUSED_PARAMETER(hWnd);
	UNUSED_PARAMETER(nID);
	UNUSED_PARAMETER(uTime);
	UNUSED_PARAMETER(dwParam);

	/* ハイライトモードでない場合 */
	if (!bHighlightMode)
		return;

	/* 選択範囲がある場合は更新せず、1秒後に再び確認する */
	if (ScriptView_GetSelectedLen() > 0)
		return;

	/* IMEを使用中は更新せず、1秒後に再び確認する */
	hImc = ImmGetContext(hWndScriptView);
	if (hImc != NULL)
	{
		if (ImmGetOpenStatus(hImc))
		{
			bRet = ImmGetConversionStatus(hImc, &dwConversion, &dwSentence);
			ImmReleaseContext(hWndScriptView, hImc);
			if (bRet)
			{
				if ((dwConversion & IME_CMODE_CHARCODE) != 0 ||
					(dwConversion & IME_CMODE_EUDC) != 0 ||
					(dwConversion & IME_CMODE_FIXED) != 0 ||
					(dwConversion & IME_CMODE_HANJACONVERT) != 0 ||
					(dwConversion & IME_CMODE_KATAKANA) != 0 ||
					(dwConversion & IME_CMODE_NOCONVERSION) != 0 ||
					(dwConversion & IME_CMODE_ROMAN) != 0 ||
					(dwConversion & IME_CMODE_SOFTKBD) != 0 ||
					(dwConversion & IME_CMODE_SYMBOL) != 0)
				{
					/* 入力中 */
					return;
				}
			}
		}
	}

	/* タイマを止める */
	KillTimer(hWndScriptView, ID_TIMER_FORMAT);

	/* 現在のカーソル位置を取得する */
	nCursor = ScriptView_GetCursorPosition();

	/* スクロールを避けるためにフォーカスを外す */
	SetFocus(NULL);

	/* フォントを適用する */
	bIgnoreChange = TRUE;
	ScriptView_ApplyFont();

	/* ハイライトを適用する */
	if (bHighlightMode)
	{
		bIgnoreChange = TRUE;
		ScriptView_SetTextColorForAllLines();
	}

	/* カーソル位置を戻す */
	ScriptView_SetCursorPosition(nCursor);

	/* フォーカスを戻す */
	SetFocus(hWndScriptView);
}

#if 0
static void __stdcall OnReloadTimer(HWND hWnd, UINT nID, UINT_PTR uTime, DWORD dwParam)
{
	FILETIME ftCurrent;
	HANDLE hFile;
	uint64_t prev, cur;

	/* ファイルが外部エディタで更新されたかチェックする */
	hFile = CreateFile(conv_utf8_to_utf16(get_script_file_name()),
					   GENERIC_READ, FILE_SHARE_READ, NULL,
					   OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		GetFileTime(hFile, NULL, NULL, &ftCurrent);
		CloseHandle(hFile);
		prev = ((uint64_t)ftTimeStamp.dwLowDateTime) | (((uint64_t)ftTimeStamp.dwHighDateTime) << 32);
		cur = ((uint64_t)ftCurrent.dwLowDateTime) | (((uint64_t)ftCurrent.dwHighDateTime) << 32);
		if (cur > prev)
		{
			/* 外部のエディタで更新されているのでリロードする */
			App_SetScriptOpened(TRUE);
			App_SetExecLineChanged(TRUE);
			App_SetChangedLine(get_expanded_line_num());
			return;
		}
	}
}
#endif

BOOL ScriptView_IsDarkMode(void)
{
	return bDarkMode;
}

void ScriptView_SetDarkMode(BOOL bEnabled)
{
	if (!bEnabled)
	{
		bDarkMode = FALSE;
		dwColorBgDefault = LIGHT_BG_DEFAULT;
		dwColorFgDefault = LIGHT_FG_DEFAULT;
		dwColorComment = LIGHT_COMMENT;
		dwColorLabel = LIGHT_LABEL;
		dwColorError = LIGHT_ERROR;
		dwColorCommandName = LIGHT_COMMAND_NAME;
		dwColorCielCommand = LIGHT_CIEL_COMMAND;
		dwColorParamName = LIGHT_PARAM_NAME;
		dwColorNextExec = LIGHT_NEXT_EXEC;
		dwColorCurrentExec = LIGHT_CURRENT_EXEC;
	}
	else
	{
		bDarkMode = TRUE;
		dwColorBgDefault = DARK_BG_DEFAULT;
		dwColorFgDefault = DARK_FG_DEFAULT;
		dwColorComment = DARK_COMMENT;
		dwColorLabel = DARK_LABEL;
		dwColorError = DARK_ERROR;
		dwColorCommandName = DARK_COMMAND_NAME;
		dwColorCielCommand = DARK_CIEL_COMMAND;
		dwColorParamName = DARK_PARAM_NAME;
		dwColorNextExec = DARK_NEXT_EXEC;
		dwColorCurrentExec = DARK_CURRENT_EXEC;
	}

	/* Save the contents. */
	ScriptView_UpdateScriptModelFromText();

	/* Update the theme. */
	ScriptView_UpdateTheme();
}

BOOL ScriptView_IsHighlightMode(void)
{
	return bHighlightMode;
}

void ScriptView_SetHighlightMode(BOOL bEnabled)
{
	bHighlightMode = bEnabled;

	/* Save the contents. */
	ScriptView_UpdateScriptModelFromText();

	/* Update the theme. */
	ScriptView_UpdateTheme();
}

const wchar_t *ScriptView_GetFontName(void)
{
	return wszFontName;
}

void ScriptView_SetFontName(const wchar_t *pwszFontName)
{
	wcscpy(wszFontName, pwszFontName);
}

int ScriptView_GetFontSize(void)
{
	return nFontSize;
}

void ScriptView_SetFontSize(int nSize)
{
	nFontSize = nSize;
}

void ScriptView_UpdateTimeStamp(void)
{
	HANDLE hFile;

	hFile = CreateFile(conv_utf8_to_utf16(get_script_file_name()),
					   GENERIC_READ,
					   FILE_SHARE_READ,
					   NULL,
					   OPEN_EXISTING,
					   0,
					   NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		/* Save the file time stamp. */
		GetFileTime(hFile, NULL, NULL, &ftTimeStamp);
		return;
	}

	/* Clear the file time stamp. */
	ZeroMemory(&ftTimeStamp, sizeof(ftTimeStamp));
}
