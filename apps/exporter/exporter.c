/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, The Authors. All rights reserved.
 */

/*
 * The Exporter for Windows
 */

/* OpenNovel Base */
#include "../../src/opennovel.h"
#include "../../src/package.h"

/* Windows */
#include <windows.h>
#include <shlobj.h>

/* Title */
wchar_t szTitle[] = L"OpenNovel Exporter";

/*
 * Forward Declaration
 */

const wchar_t *conv_utf8_to_utf16(const char *utf8_message);
const char *conv_utf16_to_utf8(const wchar_t *utf16_message);
const char *conv_utf16_to_utf8(const wchar_t *s);

static BOOL ExportForIOS(VOID);
static BOOL ExportForAndroid(VOID);
static BOOL ExportForMacOS(VOID);
static BOOL ExportForUnity(VOID);
static BOOL ExportForWeb(VOID);
static BOOL SelectGameDirectory(wchar_t *szDir);
static VOID RecreateDirectory(const wchar_t *path);
static BOOL CopyLibraryFiles(const wchar_t *szSrcDir, const wchar_t *szDestDir);
static BOOL CopyGameFiles(const wchar_t *szSrcDir, const wchar_t *szDestDir);
static BOOL CopyVideoFiles(const wchar_t *szSrcDir, const wchar_t *szDestDir);
static BOOL MovePackageFile(const wchar_t *szPkgFile, wchar_t *szDestDir);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmd, int nCmdShow)
{
	UNUSED_PARAMETER(hInstance);
	UNUSED_PARAMETER(hPrevInstance);
	UNUSED_PARAMETER(lpszCmd);
	UNUSED_PARAMETER(nCmdShow);

	if (MessageBox(NULL,
				   L"Do you want to export for iOS?\n"
				   L"iOS向けに書き出しますか？",
				   szTitle,
				   MB_YESNO) == IDYES)
	{
		if (!ExportForIOS())
			return 1;
		return 0;
	}

	if (MessageBox(NULL,
				   L"Do you want to export for Android?\n"
				   L"Android向けに書き出しますか？",
				   szTitle,
				   MB_YESNO) == IDYES)
	{
		if (!ExportForAndroid())
			return 1;
		return 0;
	}

	if (MessageBox(NULL,
				   L"Do you want to export for macOS?\n"
				   L"macOS向けに書き出しますか？",
				   szTitle,
				   MB_YESNO) == IDYES)
	{
		if (!ExportForMacOS())
			return 1;
		return 0;
	}

	if (MessageBox(NULL,
				   L"Do you want to export for Unity?\n"
				   L"Unity向けに書き出しますか？",
				   szTitle,
				   MB_YESNO) == IDYES)
	{
		if (!ExportForUnity())
			return 1;
		return 0;
	}

	if (MessageBox(NULL,
				   L"Do you want to export for Web?\n"
				   L"Web向けに書き出しますか？；",
				   szTitle, MB_YESNO) == IDYES)
	{
		if (!ExportForWeb())
			return 1;
		return 0;
	}

	return 0;
}

/* iOS用に書き出す */
static BOOL ExportForIOS(VOID)
{
	wchar_t szPwd[PATH_MAX];
	wchar_t szGame[PATH_MAX];
	wchar_t szExport[PATH_MAX];
	wchar_t szPackageSrc[PATH_MAX];
	wchar_t szPackageDst[PATH_MAX];
	wchar_t szVideoSrc[PATH_MAX];
	wchar_t szVideoDst[PATH_MAX];

	if (!SelectGameDirectory(szGame))
		return FALSE;

	wcscpy(szExport, szGame);
	wcscat(szExport, L"\\");
	wcscat(szExport, L"export-ios");
	RecreateDirectory(szExport);

	GetCurrentDirectory(PATH_MAX, szPwd);
	SetCurrentDirectory(szGame);

	if (!create_package("")) {
		SetCurrentDirectory(szPwd);
		MessageBox(NULL, L"Failed to make package.", szTitle, MB_OK);
		return FALSE;
	}

	SetCurrentDirectory(szPwd);

	wcscpy(szPackageSrc, szGame);
	wcscat(szPackageSrc, L"\\");
	wcscat(szPackageSrc, L"package.pak");

	wcscpy(szPackageDst, szExport);
	wcscat(szPackageDst, L"\\");
	wcscat(szPackageDst, L"Resources");

	if (!CopyLibraryFiles(L"ios-src", szExport))
		return FALSE;
	if (!MovePackageFile(szPackageSrc, szPackageDst))
		return FALSE;

	wcscpy(szVideoSrc, szGame);
	wcscat(szVideoSrc, L"\\");
	wcscat(szVideoSrc, L"video");

	wcscpy(szVideoDst, szExport);
	wcscat(szVideoDst, L"\\");
	wcscat(szVideoDst, L"Resources\\mov");

	if (!CopyVideoFiles(szVideoSrc, szVideoDst))
		return FALSE;

	ShellExecuteW(NULL, L"explore", szExport, NULL, NULL, SW_SHOW);

	MessageBox(NULL,
			   L"Please build on Mac using Xcode.\n"
			   L"MacでXcodeを使ってビルドしてください。",
			   szTitle,
			   MB_OK);

	return TRUE;
}

/* Android用に書き出す */
static BOOL ExportForAndroid(VOID)
{
	wchar_t szPwd[PATH_MAX];
	wchar_t szGame[PATH_MAX];
	wchar_t szExport[PATH_MAX];
	wchar_t szAssets[PATH_MAX];

	if (!SelectGameDirectory(szGame))
		return true;

	wcscpy(szExport, szGame);
	wcscat(szExport, L"\\");
	wcscat(szExport, L"export-android");
	RecreateDirectory(szExport);

	CopyLibraryFiles(L"android-src", szExport);

	GetCurrentDirectory(PATH_MAX, szPwd);
	SetCurrentDirectory(szGame);

	wcscpy(szAssets, szExport);
	wcscat(szAssets, L"\\app\\src\\main\\assets");
	CopyGameFiles(L"image", szAssets);
	CopyGameFiles(L"sound", szAssets);
	CopyGameFiles(L"story", szAssets);
	CopyGameFiles(L"script", szAssets);
	CopyGameFiles(L"font", szAssets);
	CopyGameFiles(L"video", szAssets);
	CopyGameFiles(L"project.txt", szAssets);

	SetCurrentDirectory(szPwd);

	ShellExecuteW(NULL, L"explore", szExport, NULL, NULL, SW_SHOW);

	if (MessageBox(NULL,
				   L"Do you want to build apk file automatically?\n"
				   L"apkファイルの自動ビルドを行いますか？",
				   szTitle,
				   MB_YESNO) == IDYES)
	{
		STARTUPINFOW si;
		PROCESS_INFORMATION pi;
		wchar_t szCmdLine[1024];

		/* コマンドライン文字列はCreateProcessW()に書き換えられるので、バッファにコピーしておく */
		wcscpy(szCmdLine, L"cmd.exe /k build.bat");

		/* プロセスを実行する */
		ZeroMemory(&si, sizeof(STARTUPINFOW));
		si.cb = sizeof(STARTUPINFOW);
		CreateProcessW(NULL,
					   szCmdLine,
					   NULL,
					   NULL,
					   FALSE,
					   NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
					   NULL,
					   szExport,
					   &si,
					   &pi);
		if (pi.hProcess != NULL)
			CloseHandle(pi.hThread);
		if (pi.hProcess != NULL)
			CloseHandle(pi.hProcess);
	}
	else
	{
		MessageBox(NULL,
				   L"Please build using Android Studio.\n"
				   L"Android Studioを使ってビルドしてください。",
				   szTitle,
				   MB_OK);
	}

	return TRUE;
}

/* macOS用に書き出す */
static BOOL ExportForMacOS(VOID)
{
	wchar_t szPwd[PATH_MAX];
	wchar_t szGame[PATH_MAX];
	wchar_t szExport[PATH_MAX];
	wchar_t szPackageSrc[PATH_MAX];
	wchar_t szPackageDst[PATH_MAX];
	wchar_t szVideoSrc[PATH_MAX];
	wchar_t szVideoDst[PATH_MAX];

	if (!SelectGameDirectory(szGame))
		return FALSE;

	wcscpy(szExport, szGame);
	wcscat(szExport, L"\\");
	wcscat(szExport, L"export-macos");
	RecreateDirectory(szExport);

	GetCurrentDirectory(PATH_MAX, szPwd);
	SetCurrentDirectory(szGame);

	if (!create_package("")) {
		SetCurrentDirectory(szPwd);
		MessageBox(NULL, L"Failed to make a package.", szTitle, MB_OK);
		return FALSE;
	}

	SetCurrentDirectory(szPwd);

	wcscpy(szPackageSrc, szGame);
	wcscat(szPackageSrc, L"\\");
	wcscat(szPackageSrc, L"package.pak");

	wcscpy(szPackageDst, szExport);
	wcscat(szPackageDst, L"\\");
	wcscat(szPackageDst, L"Resources");

	if (!CopyLibraryFiles(L"macos-src", szExport))
		return FALSE;
	if (!MovePackageFile(szPackageSrc, szPackageDst))
		return FALSE;

	wcscpy(szVideoSrc, szGame);
	wcscat(szVideoSrc, L"\\");
	wcscat(szVideoSrc, L"video");

	wcscpy(szVideoDst, szExport);
	wcscat(szVideoDst, L"\\");
	wcscat(szVideoDst, L"Resources\\video");

	if (!CopyVideoFiles(szVideoSrc, szVideoDst))
		return FALSE;

	ShellExecuteW(NULL, L"explore", szExport, NULL, NULL, SW_SHOW);

	MessageBox(NULL,
			   L"Please build on Mac using Xcode.\n"
			   L"MacでXcodeを使ってビルドしてください。",
			   szTitle,
			   MB_OK);

	return TRUE;
}

/* Unity用に書き出す */
static BOOL ExportForUnity(VOID)
{
	wchar_t szPwd[PATH_MAX];
	wchar_t szGame[PATH_MAX];
	wchar_t szExport[PATH_MAX];
	wchar_t szAssets[PATH_MAX];

	if (!SelectGameDirectory(szGame))
		return true;

	wcscpy(szExport, szGame);
	wcscat(szExport, L"\\");
	wcscat(szExport, L"export-unity");
	RecreateDirectory(szExport);

	wcscpy(szAssets, szExport);
	wcscat(szAssets, L"\\Assets\\StreamingAssets");

	CopyLibraryFiles(L"unity-src", szExport);

	GetCurrentDirectory(PATH_MAX, szPwd);
	SetCurrentDirectory(szGame);

	CopyGameFiles(L"image", szAssets);
	CopyGameFiles(L"sound", szAssets);
	CopyGameFiles(L"story", szAssets);
	CopyGameFiles(L"script", szAssets);
	CopyGameFiles(L"font", szAssets);
	CopyGameFiles(L"video", szAssets);
	CopyGameFiles(L"project.txt", szAssets);

	SetCurrentDirectory(szPwd);

	ShellExecuteW(NULL, L"explore", szExport, NULL, NULL, SW_SHOW);

	MessageBox(NULL,
			   L"Please build using Unity Editor.\n"
			   L"Unity Editorでビルドしてください。",
			   szTitle,
			   MB_OK);

	return true;
}

/* Web用に書き出す */
static BOOL ExportForWeb(VOID)
{
	wchar_t szPwd[PATH_MAX];
	wchar_t szGame[PATH_MAX];
	wchar_t szExport[PATH_MAX];
	wchar_t szPackageSrc[PATH_MAX];
	wchar_t szVideoSrc[PATH_MAX];

	if (!SelectGameDirectory(szGame))
		return FALSE;

	wcscpy(szExport, szGame);
	wcscat(szExport, L"\\");
	wcscat(szExport, L"export-web");
	RecreateDirectory(szExport);

	GetCurrentDirectory(PATH_MAX, szPwd);
	SetCurrentDirectory(szGame);

	if (!create_package("")) {
		SetCurrentDirectory(szPwd);
		MessageBox(NULL, L"Failed to make a package.", szTitle, MB_OK);
		return FALSE;
	}

	SetCurrentDirectory(szPwd);

	wcscpy(szPackageSrc, szGame);
	wcscat(szPackageSrc, L"\\");
	wcscat(szPackageSrc, L"package.pak");

	if (!CopyLibraryFiles(L"wasm-src", szExport))
		return FALSE;
	if (!MovePackageFile(szPackageSrc, szExport))
		return FALSE;

	wcscpy(szVideoSrc, szGame);
	wcscat(szVideoSrc, L"\\video\\*.mp4");

	if (!CopyVideoFiles(szVideoSrc, szExport))
		return FALSE;

	ShellExecuteW(NULL, L"explore", szExport, NULL, NULL, SW_SHOW);

	if (MessageBox(NULL,
				   L"Do you want to run on a broser?\n"
				   L"ブラウザで実行しますか？",
				   szTitle,
				   MB_YESNO) == IDYES)
	{
		wchar_t szExePath[MAX_PATH];
		STARTUPINFOW si;
		PROCESS_INFORMATION pi;
		wchar_t *pSep;

		GetModuleFileName(NULL, szExePath, MAX_PATH);
		pSep = wcsrchr(szExePath, L'\\');
		if (pSep != NULL)
			*(pSep + 1) = L'\0';
		wcscat(szExePath, L"web-test.exe");

		ZeroMemory(&si, sizeof(STARTUPINFOW));
		si.cb = sizeof(STARTUPINFOW);
		CreateProcessW(szExePath,
					   NULL,
					   NULL,
					   NULL,
					   FALSE,
					   NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
					   NULL,
					   szExport,
					   &si,
					   &pi);
		if (pi.hProcess != NULL)
			CloseHandle(pi.hThread);
		if (pi.hProcess != NULL)
			CloseHandle(pi.hProcess);
	}
	else
	{
		MessageBox(NULL,
				   L"Please note that you cannot play your game by opening index.html on your PC.\n"
				   L"PCに保存されたindex.htmlを開いてもゲームを開始できないことにご注意ください。",
				   szTitle,
				   MB_OK);
	}

	return TRUE;
}

/* ゲームディレクトリを選択する */
static BOOL SelectGameDirectory(wchar_t *szDir)
{
	BROWSEINFO bInfo;
	LPITEMIDLIST lpItem;

	bInfo.hwndOwner = NULL;
	bInfo.pidlRoot = NULL;
	bInfo.pszDisplayName = szDir;
	bInfo.lpszTitle = L"Select a folder";
	bInfo.ulFlags = 0 ;
	bInfo.lpfn = NULL;
	bInfo.lParam = 0;
	bInfo.iImage = -1;

	lpItem = SHBrowseForFolder(&bInfo);
	if (lpItem != NULL)
	{
		SHGetPathFromIDList(lpItem, szDir);
		return TRUE;
	}

	return FALSE;
}
	
/* フォルダを再作成する */
static VOID RecreateDirectory(const wchar_t *path)
{
	wchar_t newpath[MAX_PATH];
	SHFILEOPSTRUCT fos;

	/* 二重のNUL終端を行う */
	wcscpy(newpath, path);
	newpath[wcslen(path) + 1] = L'\0';

	/* コピーする */
	ZeroMemory(&fos, sizeof(SHFILEOPSTRUCT));
	fos.wFunc = FO_DELETE;
	fos.pFrom = newpath;
	fos.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
	SHFileOperationW(&fos);
}

/* ライブラリファイルをコピーする (インストール先 to エクスポート先) */
static BOOL CopyLibraryFiles(const wchar_t *lpszSrcDir, const wchar_t *lpszDestDir)
{
	wchar_t from[MAX_PATH];
	wchar_t to[MAX_PATH];
	SHFILEOPSTRUCTW fos;
	wchar_t *pSep;
	int ret;

	/* コピー元を求める */
	GetModuleFileName(NULL, from, MAX_PATH);
	pSep = wcsrchr(from, L'\\');
	if (pSep != NULL)
		*(pSep + 1) = L'\0';
	wcscat(from, lpszSrcDir);
	from[wcslen(from) + 1] = L'\0';	/* 二重のNUL終端を行う */

	/* コピー先を求める */
	wcscpy(to, lpszDestDir);
	to[wcslen(lpszDestDir) + 1] = L'\0';	/* 二重のNUL終端を行う */

	/* コピーする */
	ZeroMemory(&fos, sizeof(SHFILEOPSTRUCT));
	fos.wFunc = FO_COPY;
	fos.pFrom = from;
	fos.pTo = to;
	fos.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	ret = SHFileOperationW(&fos);
	if (ret != 0)
	{
		MessageBox(NULL, L"Failed to copy a template folder.", szTitle, MB_OK);
		return FALSE;
	}

	return TRUE;
}

/* ゲームファイルをコピーする (ゲーム内 to エクスポート先) */
static BOOL CopyGameFiles(const wchar_t *lpszSrcDir, const wchar_t *lpszDestDir)
{
	wchar_t from[MAX_PATH];
	wchar_t to[MAX_PATH];
	SHFILEOPSTRUCTW fos;
	int ret;

	/* 二重のNUL終端を行う */
	wcscpy(from, lpszSrcDir);
	from[wcslen(lpszSrcDir) + 1] = L'\0';
	wcscpy(to, lpszDestDir);
	to[wcslen(lpszDestDir) + 1] = L'\0';

	/* コピーする */
	ZeroMemory(&fos, sizeof(SHFILEOPSTRUCT));
	fos.wFunc = FO_COPY;
	fos.pFrom = from;
	fos.pTo = to;
	fos.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	ret = SHFileOperationW(&fos);
	if (ret != 0)
	{
		MessageBox(NULL, L"Failed to copy game files.", szTitle, MB_OK);
		return FALSE;
	}

	return TRUE;
}

/* 動画をコピーする */
static BOOL CopyVideoFiles(const wchar_t *lpszSrcDir, const wchar_t *lpszDestDir)
{
	wchar_t from[MAX_PATH];
	wchar_t to[MAX_PATH];
	SHFILEOPSTRUCTW fos;
	int ret;

	/* 二重のNUL終端を行う */
	wcscpy(from, lpszSrcDir);
	from[wcslen(lpszSrcDir) + 1] = L'\0';
	wcscpy(to, lpszDestDir);
	to[wcslen(lpszDestDir) + 1] = L'\0';

	/* コピーする */
	ZeroMemory(&fos, sizeof(SHFILEOPSTRUCT));
	fos.wFunc = FO_COPY;
	fos.pFrom = from;
	fos.pTo = to;
	fos.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI |
		FOF_SILENT;
	ret = SHFileOperationW(&fos);
	if (ret != 0)
	{
		MessageBox(NULL, L"Failed to copy video files.", szTitle, MB_OK);
		return FALSE;
	}

	return TRUE;
}

/* パッケージファイルを移動する */
static BOOL MovePackageFile(const wchar_t *lpszPkgFile, wchar_t *lpszDestDir)
{
	wchar_t from[MAX_PATH];
	wchar_t to[MAX_PATH];
	SHFILEOPSTRUCTW fos;
	int ret;

	/* 二重のNUL終端を行う */
	wcscpy(from, lpszPkgFile);
	from[wcslen(lpszPkgFile) + 1] = L'\0';
	wcscpy(to, lpszDestDir);
	to[wcslen(lpszDestDir) + 1] = L'\0';

	/* 移動する */
	ZeroMemory(&fos, sizeof(SHFILEOPSTRUCT));
	fos.hwnd = NULL;
	fos.wFunc = FO_MOVE;
	fos.pFrom = from;
	fos.pTo = to;
	fos.fFlags = FOF_NOCONFIRMATION;
	ret = SHFileOperationW(&fos);
	if (ret != 0)
	{
		MessageBox(NULL, L"Failed to move a package file.", szTitle, MB_OK);
		return FALSE;
	}

	return TRUE;
}

/*
 * Utf-8 <--> Utf-16 conversion
 */

#define CONV_MESSAGE_SIZE 65536

static wchar_t wszMessage[CONV_MESSAGE_SIZE];
static char szMessage[CONV_MESSAGE_SIZE];

const wchar_t *conv_utf8_to_utf16(const char *utf8_message)
{
	MultiByteToWideChar(CP_UTF8, 0, utf8_message, -1, wszMessage, CONV_MESSAGE_SIZE - 1);
	return wszMessage;
}

const char *conv_utf16_to_utf8(const wchar_t *utf16_message)
{
	WideCharToMultiByte(CP_UTF8, 0, utf16_message, -1, szMessage, CONV_MESSAGE_SIZE - 1, NULL, NULL);
	return szMessage;
}

/*
 * Stubs
 */

/* Stub for platform.c */
bool log_error(const char *s, ...) { UNUSED_PARAMETER(s); return true; }
bool log_warn(const char *s, ...) { UNUSED_PARAMETER(s); return true; }
bool log_info(const char *s, ...) { UNUSED_PARAMETER(s); return true; }
void log_dir_many_files(void) { log_error("Too many files.\n"); }
const char *get_system_locale(void) { return "other"; }
char *make_valid_path(const char *dir, const char *fname) { UNUSED_PARAMETER(dir); UNUSED_PARAMETER(fname); return strdup(""); }

/* Stub for script.c */
const char *get_script_file_name(void) { return ""; }
int get_line_num(void) { return 0; }
const char *get_line_string(void) { return ""; }
int get_command_index(void) { return 0; }
void translate_failed_command_to_message(int index) { UNUSED_PARAMETER(index); }

/* Stub for main.c */
void dbg_set_error_state(void) { }
int dbg_get_parse_error_count(void) { return 0; }
void translate_command_to_message_for_runtime_error(int index) { UNUSED_PARAMETER(index); }
