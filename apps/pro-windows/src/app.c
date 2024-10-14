/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * OpenNovel (Editor) for Windows
 */

/* Base */
#include "opennovel.h"

/* Editor */
#include "pro.h"
#include "package.h"
#include "scriptview.h"
#include "dialog/dialog.h"

/* HAL Implementaions */
#include "microsoft/dx9render.h"	/* Graphics HAL */
#include "microsoft/dsound.h"		/* Sound HAL */
#include "microsoft/dsvideo.h"		/* Video HAL */

/* Windows */
#include <windows.h>
#include <shlobj.h>			/* SHGetFolderPath() */
#include <commctrl.h>		/* TOOLINFO */
#include <richedit.h>		/* RichEdit */
#include "resource.h"
#define WM_DPICHANGED       0x02E0 /* Vista */

/* Standard C */
#include <signal.h>

/* msvcrt  */
#include <io.h>				/* _access() */
#define wcsdup(s)	_wcsdup(s)

/* A macro to check whether a file exists. */
#define FILE_EXISTS(fname)	(_access(fname, 0) != -1)

/* A manifest for Windows XP control style */
#ifdef _MSC_VER
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

/*
 * Constants
 */

/* The window title of message boxes. */
#define TITLE				L"Suika2"

/* The font name for the controls. */
#define CONTROL_FONT		L"Yu Gothic UI"

/* The version string. */
#define VERSION_STRING 		\
	"Suika2 22.0\n" \
	"Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.\n"

/* The minimum window size. */
#define WINDOW_WIDTH_MIN	(800)
#define WINDOW_HEIGHT_MIN	(600)

/* The width of the editor panel. */
#define EDITOR_WIDTH		(440)

/* Framerate */
#define FPS					(30)

/* Time of a frame */
#define FRAME_MILLI			(33)

/* Time of a sleep of a frame */
#define SLEEP_MILLI			(5)

/* Size of the UTF-8/UTF-16 conversion buffer */
#define CONV_MESSAGE_SIZE	(65536)

/* Max. text size of the var box (Line: "$00001=12345678901\r\n") */
#define VAR_TEXTBOX_MAX		(11000 * (1 + 5 + 1 + 11 + 2))

/* i18n messages. */
enum {
	UIMSG_YES,
	UIMSG_NO,
	UIMSG_INFO,
	UIMSG_WARN,
	UIMSG_ERROR,
	UIMSG_CANNOT_OPEN_LOG,
	UIMSG_EXIT,
	UIMSG_TITLE,
	UIMSG_DELETE,
	UIMSG_OVERWRITE,
	UIMSG_DEFAULT,
	UIMSG_NO_SOUND_DEVICE,
	UIMSG_NO_GAME_FILES,
	UIMSG_WIN32_NO_DIRECT3D,
	UIMSG_WIN32_SMALL_DISPLAY,
};

/* Window class names */
static const wchar_t wszWindowClassMainWindow[] = L"OpenNovelMainWindow";
static const wchar_t wszWindowClassRenderingPanel[] = L"OpenNovelRenderingPanel";
static const wchar_t wszWindowClassEditorPanel[] = L"OpenNovelEditorPanel";

/*
 * Variables
 */

/* Windows objects */
HWND hWndMain;						/* Main window */
static HWND hWndRender;				/* Rendering panel */
static HWND hWndEditor;				/* Editor panel */
static HWND hWndBtnContinue;		/* Continue button */
static HWND hWndBtnNext;			/* Next button */
static HWND hWndBtnStop;			/* Stop button */
static HWND hWndBtnEdit;			/* Edit button */
static HWND hWndTextboxScript;		/* Textbox for file name */
static HWND hWndBtnSelectScript;	/* Button for file select */
static HWND hWndRichEdit;			/* RichEdit for script */
static HWND hWndTextboxVar;			/* Textbox for variables */
static HWND hWndBtnVar;				/* Button for update-variables */
static HWND hWndDlgProp;			/* Property dialog */
static HMENU hMenu;					/* Menu on the main window */
static HMENU hMenuPopup;			/* Popup menu */

/* Project root directory. */
static wchar_t wszProjectDir[1024];

/* Buffers to convert UTF-8 and UTF-16. */
static wchar_t wszMessage[CONV_MESSAGE_SIZE];
static char szMessage[CONV_MESSAGE_SIZE];

/* For WaitForNextFrame(). */
static DWORD dwStartTime;

/* A flag to show whether a project is loaded or not. */
static BOOL bProjectOpened;

/* A flag to show whether we are in the full screen mode or not. */
static BOOL bFullScreen;

/* A flag to show whether we have to enter the full screen mode. */
static BOOL bNeedFullScreen;

/* A flag to show whether we have to leave the full screen mode. */
static BOOL bNeedWindowed;

/* Styles in the windowed-mode. */
static DWORD dwStyle;
static DWORD dwExStyle;

/* Position in the windowed-mode. */
static RECT rcWindow;

/* Saved window size and DPI. */
static int nLastClientWidth, nLastClientHeight, nLastDpi;

/* A flag to show whether RunFrame() can render or not. */
static BOOL bRunFrameAllow;

/* Viewport. */
static int nViewportOffsetX;
static int nViewportOffsetY;
static int nViewportWidth;
static int nViewportHeight;

/* A scale for mouse position calculation. */
static float fMouseScale;

/* A flag to show whether we are playing a video by DirectShow or not. */
static BOOL bDShowMode;

/* A flag to show whether the current video is skippable by a click. */
static BOOL bDShowSkippable;

/* A flag to show whether we are in the English mode or not. */
BOOL bEnglish;

/* A flag to show whether the game is running or not. */
static BOOL bRunning;

/* Event status. */
static BOOL bContinuePressed;		/* "Continue" is pressed. */
static BOOL bNextPressed;			/* "Next" is pressed. */
static BOOL bStopPressed;			/* "Stop" is pressed. */
static BOOL bScriptOpened;			/* The script file is opened. */
static BOOL bExecLineChanged;		/* The running line is changed. */
static int nLineChanged;			/* The running line. */
static BOOL bNeedUpdateVars;		/* "Update Vars" is pressed. */

/* Property */
static int nLineProperty;			/* Line in edit by property dialog. */

/*
 * Completion
 */
struct completion_item {
	wchar_t *prefix;
	wchar_t *insert;
} completion_item[] = {
	{L"@anime", L" file="},
	{L"@bg", L" file= duration= effect="},
	{L"@ch", L" position=l/lc/c/rc/r file= effect=normal"},
	{L"@chapter", L" title=\"\""},
	{L"@choose", L" l1=\"label1\" t1=\"text1\" l2=\"label2\" t2=\"text2\""},
	{L"@move", L" position=l/lc/c/rc/r t= accel=move/accel/brake x= y= alpha="},
	{L"@music", L" file="},
	{L"@sound", L" file="},
	{L"@volume", L" track=bgm/voice/se volume="},
	{L"@wait", L" t="},
};

/*
 * Forward Declaration
 */

/*
 * Forward Declaration
 */

/* static */
static void SIGSEGV_Handler(int n);
static BOOL InitApp(HINSTANCE hInstance, int nCmdShow);
static void CleanupApp(void);
static BOOL InitWindow(HINSTANCE hInstance, int nCmdShow);
static BOOL InitRenderingPanel(HINSTANCE hInstance, int nWidth, int nHeight);
static BOOL InitMainWindow(HINSTANCE hInstance, int *pnRenderWidth, int *pnRenderHeight);
static BOOL InitEditorPanel(HINSTANCE hInstance);
static VOID InitMenu(HWND hWnd);
static HWND CreateTooltip(HWND hWndBtn, const wchar_t *pszTextEnglish, const wchar_t *pszTextJapanese);
static VOID StartGame(void);
static VOID GameLoop(void);
static BOOL RunFrame(void);
static BOOL SyncEvents(void);
static BOOL PretranslateMessage(MSG* pMsg);
static BOOL WaitForNextFrame(void);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int ConvertKeyCode(int nVK);
static void OnPaint(HWND hWnd);
static void OnCommand(WPARAM wParam, LPARAM lParam);
static void OnSizing(int edge, LPRECT lpRect);
static void OnSize(void);
static void OnDpiChanged(HWND hWnd, UINT nDpi, LPRECT lpRect);
static void Layout(int nClientWidth, int nClientHeight);

/* TextEdit (for Variables) */
static VOID Variable_UpdateText(void);

/* Project */
static BOOL CreateProjectFromTemplate(const wchar_t *pszTemplate);
static BOOL ChooseProject(void);
static BOOL OpenProjectAtPath(const wchar_t *pszPath);
static VOID ReadProjectFile(void);
static VOID WriteProjectFile(void);
static const wchar_t *GetLastProjectPath(void);
static VOID RecordLastProjectPath(void);

/* Command Handlers */
static VOID OnNewProject(const wchar_t *pszTemplate);
static VOID OnOpenProject(void);
static VOID OnOpenGameFolder(void);
static VOID OnOpenScript(void);
static VOID OnReloadScript(void);
const wchar_t *SelectFile(const char *pszDir);
static VOID OnContinue(void);
static VOID OnNext(void);
static VOID OnStop(void);
static VOID OnShiftEnter(void);
static VOID OnTab(void);
static VOID OnSave(void);
static VOID OnNextError(void);
static VOID OnPopup(void);
static VOID OnWriteVars(void);
static VOID OnExportPackage(void);
static VOID OnExportWin(void);
static VOID RunWindowsGame(void);
static VOID OnExportMac(void);
static VOID OnExportWeb(void);
static VOID RunWebTest(void);
static VOID OnExportAndroid(void);
static VOID RunAndroidBuild(void);
static VOID OnExportIOS(void);
static VOID OnExportUnity(void);
static VOID OnFont(void);
static VOID OnHighlightMode(void);
static VOID OnDarkMode(void);
static VOID OnVersion(void);
static VOID OnDocument(void);

/* Export Helpers */
static VOID RecreateDirectory(const wchar_t *path);
static BOOL CopyLibraryFiles(const wchar_t* lpszSrcDir, const wchar_t* lpszDestDir);
static BOOL CopyGameFiles(const wchar_t* lpszSrcDir, const wchar_t* lpszDestDir);
static BOOL CopyMovFiles(const wchar_t *lpszSrcDir, const wchar_t *lpszDestDir);
static BOOL MovePackageFile(const wchar_t *lpszPkgFile, wchar_t *lpszDestDir);

/* Command Insertion */
static VOID OnInsertMessage(void);
static VOID OnInsertSerif(void);
static VOID OnInsertBg(void);
static VOID OnInsertBgOnly(void);
static VOID OnInsertCh(void);
static VOID OnInsertChch(void);
static VOID OnInsertMusic(void);
static VOID OnInsertSound(void);
static VOID OnInsertVolume(void);
static VOID OnInsertVideo(void);
static VOID OnInsertShake(void);
static VOID OnInsertChoose(void);
static VOID OnInsertMenu(void);
static VOID OnInsertClick(void);
static VOID OnInsertTime(void);
static VOID OnInsertStory(void);

/* Property dialog */
static VOID OnProperty(void);
static VOID OnPropertyUpdate(void);

/* I18n translation */
static int get_locale_id(void);
static const char *get_ui_message(int id);

/*
 * Initialization
 */

/*
 * WinMain
 */
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpszCmd,
	int nCmdShow)
{
	int nRet;

	signal(SIGSEGV, SIGSEGV_Handler);

	nRet = 1;
	do {
		/* Decide Japanese or English. */
		bEnglish = (GetUserDefaultLCID() & 0x3ff) == LANG_JAPANESE ? FALSE : TRUE;

		/* Initialize the app. */
		if(!InitApp(hInstance, nCmdShow))
			break;

		/* Run the main loop. */
		GameLoop();

		/* Cleanup the app. */
		CleanupApp();

		nRet = 0;
	} while (0);

	UNUSED_PARAMETER(hPrevInstance);
	UNUSED_PARAMETER(lpszCmd);

	return nRet;
}

static void SIGSEGV_Handler(int n)
{
	BOOL bEnglish;

	UNUSED_PARAMETER(n);

	bEnglish = strcmp(get_system_locale(), "ja") != 0;

	log_error(bEnglish ?
			  "Sorry, Suika2 was crashed.\n"
			  "Please send a bug report to the project." :
			  "ご迷惑をかけ申し訳ございません。\n"
			  "Suika2がクラッシュしました。\n"
			  "バグ報告をいただけますと幸いです。\n");
	exit(1);
}

/* Initialize the app. */
static BOOL InitApp(HINSTANCE hInstance, int nCmdShow)
{
	conf_game_width = 1280;
	conf_game_height = 720;
	conf_game_title = strdup("Suika2");

	/* Initialize the window. */
	if (!InitWindow(hInstance, nCmdShow))
		return FALSE;

	/* Initialize the graphics HAL. */
	if (!D3DInitialize(hWndRender))
	{
		log_error(get_ui_message(UIMSG_WIN32_NO_DIRECT3D));
		return FALSE;
	}

	/* Initialize the sound HAL. */
	if (!DSInitialize(hWndMain))
	{
		log_error(get_ui_message(UIMSG_NO_SOUND_DEVICE));
	}

	/* Open a project file if specified in argv[1]. */
	if (__argc >= 2)
	{
		if (__argc >= 4)
		{
			if (!set_startup_file_and_line(conv_utf16_to_utf8(__wargv[2]), (int)wcstol(__wargv[3], NULL, 10)))
				return FALSE;
		}
		if (OpenProjectAtPath(__wargv[1]))
		{
			StartGame();
		}
	}

	return TRUE;
}

/* Cleanup the app. */
static void CleanupApp(void)
{
	if (bProjectOpened)
	{
		on_event_cleanup();
		cleanup_conf();
		cleanup_file();
	}

	/* Cleanup Direct3D. */
	D3DCleanup();

	/* Cleanup DirectSound. */
	DSCleanup();
}

/*
 * A wrapper for GetDpiForWindow().
 */
int App_GetDpiForWindow(HWND hWnd)
{
	static UINT (__stdcall *pGetDpiForWindow)(HWND) = NULL;
	UINT nDpi;

	if (pGetDpiForWindow == NULL)
	{
		HMODULE hModule = LoadLibrary(L"user32.dll");
		if (hModule == NULL)
			return 96;

		pGetDpiForWindow = (void *)GetProcAddress(hModule, "GetDpiForWindow");
		if (pGetDpiForWindow == NULL)
			return 96;
	}

	nDpi = pGetDpiForWindow(hWnd);
	if (nDpi == 0)
		return 96;

	return (int)nDpi;
}

/* Returns whether we are in the English mode or not. */
BOOL App_IsEnglishMode(void)
{
	return bEnglish;
}

/* Initialize the window. */
static BOOL InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	HRESULT hResult;
	int i, nRenderWidth, nRenderHeight;

	/* Initialize COM. */
	hResult = CoInitialize(0);
	if (FAILED(hResult))
	{
		log_api_error("CoInitialize");
		return FALSE;
	}

	/* Initialize the Common Controls. */
	InitCommonControls();

	/* Initialize the main window. */
	if (!InitMainWindow(hInstance, &nRenderWidth, &nRenderHeight))
		return FALSE;

	/* Initialize the rendering panel. */
	if (!InitRenderingPanel(hInstance, nRenderWidth, nRenderWidth))
		return FALSE;

	/* Initialize the editor panel. */
	if (!InitEditorPanel(hInstance))
		return FALSE;

	/* Initialize the menu. */
	InitMenu(hWndMain);

	/* Show the window. */
	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);

	/* Process events during a 0.1 second. */
	dwStartTime = GetTickCount();
	for(i = 0; i < FPS / 10; i++)
		WaitForNextFrame();

	return TRUE;
}

static BOOL InitMainWindow(HINSTANCE hInstance, int *pnRenderWidth, int *pnRenderHeight)
{
	wchar_t wszTitle[1024];
	WNDCLASSEX wcex;
	RECT rc, rcDesktop;
	HMONITOR monitor;
	MONITORINFOEX minfo;
	int nVirtualScreenWidth, nVirtualScreenHeight;
	int nFrameAddWidth, nFrameAddHeight;
	int nMonitors;
	int nRenderWidth, nRenderHeight;
	int nWinWidth, nWinHeight;
	int nPosX, nPosY;
	int nDpi, nMonitorWidth, nMonitorHeight;

	/* Register a window class. */
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.lpfnWndProc    = WndProc;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName  = wszWindowClassMainWindow;
	wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	if (!RegisterClassEx(&wcex))
		return FALSE;

	/* Decide a window style. */
	dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPED | WS_THICKFRAME;

	/* Get the frame margins. */
	nFrameAddWidth = GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	nFrameAddHeight = GetSystemMetrics(SM_CYCAPTION) +
					  GetSystemMetrics(SM_CYMENU) +
					  GetSystemMetrics(SM_CYFIXEDFRAME) * 2;

	/* Convert the window title from UTF-8 to UTF-16LE. */
	MultiByteToWideChar(CP_UTF8, 0, conf_game_title, -1, wszTitle, sizeof(wszTitle) / sizeof(wchar_t) - 1);

	/* Get the monitor count. */
	nMonitors = GetSystemMetrics(SM_CMONITORS);

	/* Get the window size from the config. */
	nRenderWidth = conf_game_width;
	nRenderHeight = conf_game_height;

	/* Get the display size. */
	nVirtualScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	nVirtualScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	/* Calc the window size. */
	nWinWidth = nRenderWidth + nFrameAddWidth + EDITOR_WIDTH;
	nWinHeight = nRenderHeight + nFrameAddHeight;

	/* If the display size is smaller than the window size: */
	if (nWinWidth > nVirtualScreenWidth ||
		nWinHeight > nVirtualScreenHeight)
	{
		nWinWidth = nVirtualScreenWidth;
		nWinHeight = nVirtualScreenHeight;
		nRenderWidth = nWinWidth - EDITOR_WIDTH;
		nRenderHeight = nWinHeight;
	}

	/* Center the window if not multi-display environment. */
	if (nMonitors == 1)
	{
		nPosX = (nVirtualScreenWidth - nWinWidth) / 2;
		nPosY = (nVirtualScreenHeight - nWinHeight) / 2;
	}
	else
	{
//		nPosX = CW_USEDEFAULT;
//		nPosY = CW_USEDEFAULT;
		nPosX = 0;
		nPosY = 0;
	}

	/* Create a window. */
	hWndMain = CreateWindowEx(0,
							  wszWindowClassMainWindow,
							  wszTitle,
							  dwStyle,
							  nPosX,
							  nPosY,
							  nWinWidth,
							  nWinHeight,
							  NULL,
							  NULL,
							  hInstance,
							  NULL);
	if (hWndMain == NULL)
	{
		log_api_error("CreateWindowEx");
		return FALSE;
	}

	/* Handle HiDPI. */
	nDpi = App_GetDpiForWindow(hWndMain);
	nRenderWidth = MulDiv(conf_game_width, nDpi, 96);
	nRenderHeight = MulDiv(conf_game_height, nDpi, 96);
	nWinWidth = nRenderWidth + nFrameAddWidth + EDITOR_WIDTH;
	nWinHeight = nRenderHeight + nFrameAddHeight;
	if (nMonitors != 1)
	{
		monitor = MonitorFromWindow(hWndMain, MONITOR_DEFAULTTONEAREST);
		minfo.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(monitor, (LPMONITORINFO)&minfo);
		rcDesktop = minfo.rcMonitor;
		nMonitorWidth = minfo.rcMonitor.right - minfo.rcMonitor.left;
		nMonitorHeight = minfo.rcMonitor.bottom - minfo.rcMonitor.top;
	}
	else
	{
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);
		nMonitorWidth = rcDesktop.right - rcDesktop.left;
		nMonitorHeight = rcDesktop.bottom - rcDesktop.top;
	}
	if (nWinWidth > nMonitorWidth || nWinHeight > nMonitorHeight)
	{
		nWinWidth = nMonitorWidth;
		nWinHeight = nMonitorHeight;
		nRenderWidth = nWinWidth - EDITOR_WIDTH;
		nRenderHeight = nWinHeight;
	}
	nPosX = (nMonitorWidth - nWinWidth) / 2 + rcDesktop.left / 2;
	nPosY = (nMonitorHeight - nWinHeight) / 2 + rcDesktop.top / 2;

	/* Adjust the window size. */
	AdjustWindowRectEx(&rc, dwStyle, TRUE, (DWORD)GetWindowLong(hWndMain, GWL_EXSTYLE));
	SetWindowPos(hWndMain, NULL, nPosX, nPosY, nWinWidth, nWinHeight, SWP_NOZORDER);
	GetWindowRect(hWndMain, &rcWindow);

	*pnRenderWidth = nRenderWidth;
	*pnRenderHeight = nRenderHeight;

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return TRUE;
}

/* Initialize the rendering panel. */
static BOOL InitRenderingPanel(HINSTANCE hInstance, int nWidth, int nHeight)
{
	WNDCLASSEX wcex;

	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.lpfnWndProc    = WndProc;
	wcex.hInstance      = hInstance;
	wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName  = wszWindowClassRenderingPanel;
	if (!RegisterClassEx(&wcex))
		return FALSE;

	hWndRender = CreateWindowEx(0, wszWindowClassRenderingPanel, NULL,
							  WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
							  0, 0, nWidth, nHeight,
							  hWndMain, NULL, hInstance, NULL);
	if (hWndRender == NULL)
	{
		log_api_error("CreateWindowEx");
		return FALSE;
	}

	return TRUE;
}

/* Initialize the editor panel. */
static BOOL InitEditorPanel(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	RECT rcClient;
	HFONT hFont;
	int nDpi;

	/* Get the DPI. */
	nDpi = App_GetDpiForWindow(hWndMain);

	/* Get the client rect. */
	GetClientRect(hWndMain, &rcClient);

	/* Register a window class. */
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.hInstance      = hInstance;
	wcex.lpfnWndProc    = WndProc;
	wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszClassName  = wszWindowClassEditorPanel;
	if (!RegisterClassEx(&wcex))
		return FALSE;

	/* Create a window. */
	hWndEditor = CreateWindowEx(0,
								wszWindowClassEditorPanel,
								NULL,
								WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
								rcClient.right - EDITOR_WIDTH, 0,
								EDITOR_WIDTH,
								rcClient.bottom,
								hWndMain,
								NULL,
								GetModuleHandle(NULL),
								NULL);
	if(!hWndEditor)
		return FALSE;

	/* Create fonts. */
	hFont = CreateFont(MulDiv(18, nDpi, 96),
					   0,
					   0,
					   0,
					   FW_DONTCARE,
					   FALSE,
					   FALSE,
					   FALSE,
					   ANSI_CHARSET,
					   OUT_TT_PRECIS,
					   CLIP_DEFAULT_PRECIS,
					   DEFAULT_QUALITY,
					   DEFAULT_PITCH | FF_DONTCARE,
					   CONTROL_FONT);

	/* Create the continue button. */
	hWndBtnContinue = CreateWindow(
		L"BUTTON",
		bEnglish ? L"Continue" : L"続ける",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		MulDiv(10, nDpi, 96),
		MulDiv(10, nDpi, 96),
		MulDiv(100, nDpi, 96),
		MulDiv(40, nDpi, 96),
		hWndEditor,
		(HMENU)ID_CONTINUE,
		hInstance,
		NULL);
	SendMessage(hWndBtnContinue, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
	EnableWindow(hWndBtnContinue, FALSE);
	CreateTooltip(hWndBtnContinue,
				  L"Start executing script and run continuously.",
				  L"スクリプトの実行を開始し、継続して実行します。");

	/* Create the next button. */
	hWndBtnNext = CreateWindow(
		L"BUTTON",
		bEnglish ? L"Next" : L"次へ",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		MulDiv(120, nDpi, 96),
		MulDiv(10, nDpi, 96),
		MulDiv(100, nDpi, 96),
		MulDiv(40, nDpi, 96),
		hWndEditor,
		(HMENU)ID_NEXT,
		hInstance,
		NULL);
	SendMessage(hWndBtnNext, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
	EnableWindow(hWndBtnNext, FALSE);
	CreateTooltip(hWndBtnNext,
				  L"Run a single command and stop after it.",
				  L"コマンドを1つだけ実行し、停止します。");

	/* Create the stop button. */
	hWndBtnStop = CreateWindow(
		L"BUTTON",
		bEnglish ? L"Stopped" : L"停止",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		MulDiv(230, nDpi, 96),
		MulDiv(10, nDpi, 96),
		MulDiv(100, nDpi, 96),
		MulDiv(40, nDpi, 96),
		hWndEditor,
		(HMENU)ID_STOP,
		hInstance,
		NULL);
	EnableWindow(hWndBtnStop, FALSE);
	SendMessage(hWndBtnStop, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
	EnableWindow(hWndBtnStop, FALSE);
	CreateTooltip(hWndBtnStop,
				  L"Stop executing scripts.",
				  L"コマンドの実行を停止します。");

	/* Create the edit button. */
	hWndBtnEdit = CreateWindow(
		L"BUTTON",
		bEnglish ? L"Edit" : L"編集",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		MulDiv(350, nDpi, 96),
		MulDiv(10, nDpi, 96),
		MulDiv(80, nDpi, 96),
		MulDiv(40, nDpi, 96),
		hWndEditor,
		(HMENU)ID_PROPERTY,
		hInstance,
		NULL);
	EnableWindow(hWndBtnEdit, TRUE);
	SendMessage(hWndBtnEdit, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
	EnableWindow(hWndBtnEdit, FALSE);
	CreateTooltip(hWndBtnEdit,
				  L"Edit properties of a command.",
				  L"コマンドのプロパティを編集します。");

	/* Crreate a textbox for the script name. */
	hWndTextboxScript = CreateWindow(
		L"EDIT",
		NULL,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_AUTOHSCROLL,
		MulDiv(10, nDpi, 96),
		MulDiv(60, nDpi, 96),
		MulDiv(350, nDpi, 96),
		MulDiv(30, nDpi, 96),
		hWndEditor,
		0,
		hInstance,
		NULL);
	SendMessage(hWndTextboxScript, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
	CreateTooltip(hWndTextboxScript,
				  L"Write script file name to be jumped to.",
				  L"ジャンプしたいスクリプトファイル名を書きます。");

	/* Create a button for script selection. */
	hWndBtnSelectScript = CreateWindow(
		L"BUTTON", L"...",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		MulDiv(370, nDpi, 96),
		MulDiv(60, nDpi, 96),
		MulDiv(60, nDpi, 96),
		MulDiv(30, nDpi, 96),
		hWndEditor,
		(HMENU)ID_OPEN,
		hInstance,
		NULL);
	SendMessage(hWndBtnSelectScript, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
	CreateTooltip(hWndBtnSelectScript,
				  L"Select a script file and jump to it.",
				  L"スクリプトファイルを選択してジャンプします。");
	EnableWindow(hWndBtnSelectScript, FALSE);

	/* Create a RichEdit for the script. */
	hWndRichEdit = ScriptView_Create(hWndEditor);

	/* Create a textbox for variables. */
	hWndTextboxVar = CreateWindow(
		L"EDIT",
		NULL,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL |
		ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN,
		MulDiv(10, nDpi, 96),
		MulDiv(570, nDpi, 96),
		MulDiv(280, nDpi, 96),
		MulDiv(60, nDpi, 96),
		hWndEditor,
		0,
		hInstance,
		NULL);
	SendMessage(hWndTextboxVar, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
	EnableWindow(hWndTextboxVar, FALSE);
	CreateTooltip(hWndTextboxVar,
				  L"List of variables which have non-initial values.",
				  L"初期値から変更された変数の一覧です。");

	/* Create a button for write-variables. */
	hWndBtnVar = CreateWindow(
		L"BUTTON",
		bEnglish ? L"Write values" : L"値を書き込む",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		MulDiv(300, nDpi, 96),
		MulDiv(570, nDpi, 96),
		MulDiv(130, nDpi, 96),
		MulDiv(30, nDpi, 96),
		hWndEditor,
		(HMENU)ID_VARS,
		hInstance,
		NULL);
	SendMessage(hWndBtnVar, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
	EnableWindow(hWndBtnVar, FALSE);
	CreateTooltip(hWndBtnVar,
				  L"Write to the variables.",
				  L"変数の内容を書き込みます。");

	return TRUE;
}

/* Initialize the menu. */
static VOID InitMenu(HWND hWnd)
{
	HMENU hMenuFile = CreatePopupMenu();
	HMENU hMenuRun = CreatePopupMenu();
	HMENU hMenuDirection = CreatePopupMenu();
	HMENU hMenuExport = CreatePopupMenu();
	HMENU hMenuPref = CreatePopupMenu();
	HMENU hMenuHelp = CreatePopupMenu();
	HMENU hMenuProject = CreatePopupMenu();
    MENUITEMINFO mi;
	UINT nOrder;

	/* We reuse the direction menu for popups. */
	hMenuPopup = hMenuDirection;

	/* Create a main menu. */
	hMenu = CreateMenu();

	/* Prepare for the first layer. */
	ZeroMemory(&mi, sizeof(MENUITEMINFO));
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_TYPE | MIIM_SUBMENU;
	mi.fType = MFT_STRING;
	mi.fState = MFS_ENABLED;

	/* Create "File" */
	nOrder = 0;
	mi.hSubMenu = hMenuFile;
	mi.dwTypeData = bEnglish ? L"File": L"ファイル";
	InsertMenuItem(hMenu, nOrder++, TRUE, &mi);

	/* Create "Run" */
	mi.hSubMenu = hMenuRun;
	mi.dwTypeData = bEnglish ? L"Run": L"実行";
	InsertMenuItem(hMenu, nOrder++, TRUE, &mi);

	/* Create "Novel Option" */
	mi.hSubMenu = hMenuDirection;
	mi.dwTypeData = bEnglish ? L"Novel Option": L"演出";
	InsertMenuItem(hMenu, nOrder++, TRUE, &mi);

	/* Create "Export" */
	mi.hSubMenu = hMenuExport;
	mi.dwTypeData = bEnglish ? L"Export": L"エクスポート";
	InsertMenuItem(hMenu, nOrder++, TRUE, &mi);

	/* Create "Preference" */
	mi.hSubMenu = hMenuPref;
	mi.dwTypeData = bEnglish ? L"Preference": L"設定";
	InsertMenuItem(hMenu, nOrder++, TRUE, &mi);

	/* Create "Help" */
	mi.hSubMenu = hMenuHelp;
	mi.dwTypeData = bEnglish ? L"Help": L"ヘルプ";
	InsertMenuItem(hMenu, nOrder++, TRUE, &mi);

	/*
	 * New Game
	 */

	/* Create a second layer for "Create a new project" */
	mi.hSubMenu = hMenuProject;
	mi.dwTypeData = bEnglish ? L"Create a new project" : L"新規プロジェクトを作成";
	InsertMenuItem(hMenuFile, 0, TRUE, &mi);

	/* Create a menu item for "Japanese Adventure" */
	nOrder = 0;
	mi.fMask = MIIM_TYPE | MIIM_ID;
	mi.wID = ID_NEW_PROJECT_JAPANESE_ADV;
	mi.dwTypeData = bEnglish ? L"Japanese Adventure" : L"日本語アドベンチャー形式";
	InsertMenuItem(hMenuProject, nOrder++, TRUE, &mi);

	/* Create a menu item for "English Adventure" */
	mi.fMask = MIIM_TYPE | MIIM_ID;
	mi.wID = ID_NEW_PROJECT_ENGLISH_ADV;
	mi.dwTypeData = bEnglish ? L"English Adventure" : L"英語アドベンチャー形式";
	InsertMenuItem(hMenuProject, nOrder++, TRUE, &mi);

	/*
	 * File
	 */

	mi.fMask = MIIM_TYPE | MIIM_ID;

	/* Create a menu item for "Open project" */
	mi.wID = ID_OPEN_PROJECT;
	mi.dwTypeData = bEnglish ?
		L"Open project\tCtrl+G" :
		L"プロジェクトを開く\tCtrl+G";
	InsertMenuItem(hMenuFile, nOrder++, TRUE, &mi);

	/* Create a menu item for "Open project folder" */
	mi.wID = ID_OPEN_GAME_FOLDER;
	mi.dwTypeData = bEnglish ?
		L"Open project folder" :
		L"プロジェクトフォルダを表示";
	InsertMenuItem(hMenuFile, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_OPEN_GAME_FOLDER, MF_GRAYED);

	/* Create a menu item for "Open script" */
	mi.wID = ID_OPEN;
	mi.dwTypeData = bEnglish ?
		L"Open script\tCtrl+O" :
		L"スクリプトを開く\tCtrl+O";
	InsertMenuItem(hMenuFile, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_OPEN, MF_GRAYED);

	/* Create a menu item for "Reload script" */
	mi.wID = ID_RELOAD;
	mi.dwTypeData = bEnglish ?
		L"Reload script\tCtrl+L" :
		L"スクリプトをリロードする\tCtrl+L";
	InsertMenuItem(hMenuFile, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_RELOAD, MF_GRAYED);

	/* Create a menu item for "Overwrite script" */
	mi.wID = ID_SAVE;
	mi.dwTypeData = bEnglish ?
		L"Overwrite script\tCtrl+S" :
		L"スクリプトを上書き保存する\tCtrl+S";
	InsertMenuItem(hMenuFile, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_SAVE, MF_GRAYED);

	/* Create a menu item for "Write layer images for debugging" */
	mi.wID = ID_DEBUG_LAYERS;
	mi.dwTypeData = bEnglish ?
		L"Write layer images for debugging" :
		L"レイヤー画像のデバッグ出力";
	InsertMenuItem(hMenuFile, nOrder++, TRUE, &mi);

	/* Create a menu item for "Quit" */
	mi.wID = ID_QUIT;
	mi.dwTypeData = bEnglish ?
		L"Quit\tCtrl+Q" :
		L"終了\tCtrl+Q";
	InsertMenuItem(hMenuFile, nOrder++, TRUE, &mi);

	/*
	 * Run
	 */

	/* Create a menu item for "Continue" */
	nOrder = 0;
	mi.wID = ID_CONTINUE;
	mi.dwTypeData = bEnglish ?
		L"Continue\tCtrl+R" :
		L"続ける\tCtrl+R";
	InsertMenuItem(hMenuRun, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CONTINUE, MF_GRAYED);

	/* Create a menu item for "Next" */
	mi.wID = ID_NEXT;
	mi.dwTypeData = bEnglish ?
		L"Next\tCtrl+N" :
		L"次へ\tCtrl+N";
	InsertMenuItem(hMenuRun, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_NEXT, MF_GRAYED);

	/* Create a menu item for "Stop" */
	mi.wID = ID_STOP;
	mi.dwTypeData = bEnglish ?
		L"Stop\tCtrl+P" :
		L"停止\tCtrl+P";
	InsertMenuItem(hMenuRun, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_STOP, MF_GRAYED);

	/* Create a menu item for "Go to next error" */
	mi.wID = ID_ERROR;
	mi.dwTypeData = bEnglish ?
		L"Go to next error\tCtrl+E" :
		L"次のエラー箇所へ移動\tCtrl+E";
	InsertMenuItem(hMenuRun, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_ERROR, MF_GRAYED);

	/*
	 * Export
	 */

	/* Create a menu item for "Export iOS project" */
	nOrder = 0;
	mi.wID = ID_EXPORT_IOS;
	mi.dwTypeData = bEnglish ?
		L"Export iOS project" :
		L"iOSプロジェクトをエクスポートする";
	InsertMenuItem(hMenuExport, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_EXPORT_IOS, MF_GRAYED);

	/* Create a menu item for "Export Android project" */
	mi.wID = ID_EXPORT_ANDROID;
	mi.dwTypeData = bEnglish ?
		L"Export Android project" :
		L"Androidプロジェクトをエクスポートする";
	InsertMenuItem(hMenuExport, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_EXPORT_ANDROID, MF_GRAYED);

	/* Create a menu item for "Export macOS project" */
	mi.wID = ID_EXPORT_MAC;
	mi.dwTypeData = bEnglish ?
		L"Export macOS project" :
		L"macOSプロジェクトをエクスポートする";
	InsertMenuItem(hMenuExport, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_EXPORT_MAC, MF_GRAYED);

	/* Create a menu item for "Export for Web" */
	mi.wID = ID_EXPORT_WEB;
	mi.dwTypeData = bEnglish ?
		L"Export for Web" :
		L"Webゲームをエクスポートする";
	InsertMenuItem(hMenuExport, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_EXPORT_WEB, MF_GRAYED);

	/* Create a menu item for "Export a Windows game" */
	mi.wID = ID_EXPORT_WIN;
	mi.dwTypeData = bEnglish ?
		L"Export a Windows game" :
		L"Windowsゲームをエクスポートする";
	InsertMenuItem(hMenuExport, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_EXPORT_WIN, MF_GRAYED);

	/* Create a menu item for "Export Unity project" */
	mi.wID = ID_EXPORT_UNITY;
	mi.dwTypeData = bEnglish ?
		L"Export Unity project" :
		L"Unityプロジェクトをエクスポートする";
	InsertMenuItem(hMenuExport, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_EXPORT_UNITY, MF_GRAYED);

	/* Create a menu item for "Export package only" */
	mi.wID = ID_EXPORT_PACKAGE;
	mi.dwTypeData = bEnglish ?
		L"Export package only" :
		L"パッケージのみをエクスポートする";
	InsertMenuItem(hMenuExport, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_EXPORT_PACKAGE, MF_GRAYED);

	/*
	 * Direction
	 */

	/* Create a menu item for "Message" */
	nOrder = 0;
	mi.wID = ID_CMD_MESSAGE;
	mi.dwTypeData = bEnglish ?
		L"Message" :
		L"地の文を入力";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_MESSAGE, MF_GRAYED);

	/* Create a menu item for "Line" */
	mi.wID = ID_CMD_SERIF;
	mi.dwTypeData = bEnglish ? L"Line" : L"セリフを入力";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_SERIF, MF_GRAYED);

	/* Create a menu item for "Background" */
	mi.wID = ID_CMD_BG;
	mi.dwTypeData = bEnglish ?
		L"Background" :
		L"背景";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_BG, MF_GRAYED);

	/* Create a menu item for "Background only" */
	mi.wID = ID_CMD_BG_ONLY;
	mi.dwTypeData = bEnglish ?
		L"Change Background Only" :
		L"背景だけ変更";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_BG_ONLY, MF_GRAYED);

	/* Create a menu item for "Character" */
	mi.wID = ID_CMD_CH;
	mi.dwTypeData = bEnglish ?
		L"Character" :
		L"キャラクタ";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_CH, MF_GRAYED);

	/* Create a menu item for "Change Multiple Characters" */
	mi.wID = ID_CMD_CHCH;
	mi.dwTypeData = bEnglish ?
		L"Change Multiple Characters" :
		L"キャラクタを複数同時に変更";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_CHCH, MF_GRAYED);

	/* Create a menu item for "Play Music" */
	mi.wID = ID_CMD_MUSIC;
	mi.dwTypeData = bEnglish ?
		L"Play Background Music" :
		L"音楽(BGM)を再生";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_MUSIC, MF_GRAYED);

	/* Create a menu item for "Play Sound Effect" */
	mi.wID = ID_CMD_SOUND;
	mi.dwTypeData = bEnglish ?
		L"Play Sound Effect" :
		L"効果音(SE)を再生する";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_SOUND, MF_GRAYED);

	/* Create a menu item for "Sound Effect Volume" */
	mi.wID = ID_CMD_VOLUME;
	mi.dwTypeData = bEnglish ?
		L"Sound Volume" :
		L"ボリュームを変更する";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_VOLUME, MF_GRAYED);

	/* Create a menu item for "Play Video" */
	mi.wID = ID_CMD_VIDEO;
	mi.dwTypeData = bEnglish ?
		L"Play Video" :
		L"動画を再生する";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_VIDEO, MF_GRAYED);

	/* Create a menu item for "Shake Screen Horizontally" */
	mi.wID = ID_CMD_SHAKE;
	mi.dwTypeData = bEnglish ?
		L"Shake Screen" :
		L"画面を揺らす";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_SHAKE, MF_GRAYED);

	/* Create a menu item for "Show Options" */
	mi.wID = ID_CMD_CHOOSE;
	mi.dwTypeData = bEnglish ?
		L"Show Options" :
		L"選択肢を表示する";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_CHOOSE, MF_GRAYED);

	/* Create a menu item for "Menu Screen" */
	mi.wID = ID_CMD_MENU;
	mi.dwTypeData = bEnglish ?
		L"Menu Screen" :
		L"メニュー画面を表示する";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_MENU, MF_GRAYED);

	/* Create a menu item for "Click Wait" */
	mi.wID = ID_CMD_CLICK;
	mi.dwTypeData = bEnglish ?
		L"Click Wait" :
		L"クリックを待つ";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_CLICK, MF_GRAYED);

	/* Create a menu item for "Timed Wait" */
	mi.wID = ID_CMD_TIME;
	mi.dwTypeData = bEnglish ?
		L"Timed Wait" :
		L"指定時間待つ";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_TIME, MF_GRAYED);

	/* Create a menu item for "Load Other Story" */
	mi.wID = ID_CMD_STORY;
	mi.dwTypeData = bEnglish ?
		L"Load Other Story" :
		L"他のストーリーへ移動する";
	InsertMenuItem(hMenuDirection, nOrder++, TRUE, &mi);
	EnableMenuItem(hMenu, ID_CMD_STORY, MF_GRAYED);

	/*
	 * Preference
	 */

	/* Create a menu item for "Font settings" */
	nOrder = 0;
	mi.wID = ID_FONT;
	mi.dwTypeData = bEnglish ?
		L"Font settings" :
		L"フォント設定";
	InsertMenuItem(hMenuPref, nOrder++, TRUE, &mi);

	/* Create a menu item for "Highlight mode" */
	mi.wID = ID_HIGHLIGHTMODE;
	mi.dwTypeData = bEnglish ?
		L"Highlight mode" :
		L"ハイライトモード";
	InsertMenuItem(hMenuPref, nOrder++, TRUE, &mi);

	/* Create a menu item for "Dark mode" */
	mi.wID = ID_DARKMODE;
	mi.dwTypeData = bEnglish ?
		L"Dark mode" :
		L"ダークモード";
	InsertMenuItem(hMenuPref, nOrder++, TRUE, &mi);

	/*
	 * Help
	 */

	/* Create a menu item for "Version" */
	nOrder = 0;
	mi.wID = ID_VERSION;
	mi.dwTypeData = bEnglish ?
		L"Version" :
		L"バージョン";
	InsertMenuItem(hMenuHelp, nOrder++, TRUE, &mi);

	/* Create a menu item for "Document" */
	mi.wID = ID_DOCUMENT;
	mi.dwTypeData = bEnglish ?
		L"Document" :
		L"ドキュメント";
	InsertMenuItem(hMenuHelp, nOrder++, TRUE, &mi);

	/* Set the menu to the main window. */
	SetMenu(hWnd, hMenu);
}

/* ツールチップを作成する */
static HWND CreateTooltip(HWND hWndBtn, const wchar_t *pszTextEnglish,
						  const wchar_t *pszTextJapanese)
{
	TOOLINFO ti;

	/* ツールチップを作成する */
	HWND hWndTip = CreateWindowEx(0, TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP,
								  CW_USEDEFAULT, CW_USEDEFAULT,
								  CW_USEDEFAULT, CW_USEDEFAULT,
								  hWndEditor, NULL, GetModuleHandle(NULL),
								  NULL);

	/* ツールチップをボタンに紐付ける */
	ZeroMemory(&ti, sizeof(ti));
	ti.cbSize = sizeof(ti);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = hWndBtn;
	ti.lpszText = (wchar_t *)(bEnglish ? pszTextEnglish : pszTextJapanese);
	GetClientRect(hWndBtn, &ti.rect);
	SendMessage(hWndTip, TTM_ADDTOOL, 0, (LPARAM)&ti);

	return hWndTip;
}

/*
 * Execution Control
 */

BOOL App_IsRunning(void)
{
	return bRunning;
}

void App_SetExecLineChanged(BOOL bVal)
{
	bExecLineChanged = bVal;
}

void App_SetChangedLine(int nLine)
{
	nLineChanged = nLine;
}

void App_SetScriptOpened(BOOL bVal)
{
	bScriptOpened = bVal;
}

/*******************************************/

static VOID StartGame(void)
{
	RECT rcClient;
	UINT nID;

	do {
		assert(FILE_EXISTS("project.txt"));
		assert(FILE_EXISTS("story\\story.txt"));

		/* Initialize the file subsyetem. */
		if (!init_file())
			break;

		/* Initialize the config subsyetem. */
		if (!init_conf())
			break;

		/* Move the game panel and notify its position to the rendering subsyetem. */
		GetClientRect(hWndMain, &rcClient);
		nLastClientWidth = 0;
		nLastClientHeight = 0;
		Layout(rcClient.right, rcClient.bottom);

		/* Do the upper layer initialization. */
		if (!on_event_init())
			break;

		/* Mark as opened. */
		bProjectOpened = TRUE;

		/* Make controls enabled/disabled. */
		EnableWindow(hWndBtnContinue, TRUE);
		EnableWindow(hWndBtnNext, TRUE);
		EnableWindow(hWndBtnEdit, TRUE);
		EnableWindow(hWndBtnStop, FALSE);
		EnableWindow(hWndBtnSelectScript, TRUE);
		EnableWindow(hWndRichEdit, TRUE);
		EnableWindow(hWndTextboxVar, TRUE);
		EnableWindow(hWndBtnVar, TRUE);

		/* Make menu items enabled/disabled. */
		EnableMenuItem(hMenu, ID_NEW_PROJECT_JAPANESE_ADV, MF_GRAYED);
		EnableMenuItem(hMenu, ID_OPEN_PROJECT, MF_GRAYED);
		EnableMenuItem(hMenu, ID_OPEN_GAME_FOLDER, MF_ENABLED);
		EnableMenuItem(hMenu, ID_OPEN, MF_ENABLED);
		EnableMenuItem(hMenu, ID_RELOAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_SAVE, MF_ENABLED);
		EnableMenuItem(hMenu, ID_CONTINUE, MF_ENABLED);
		EnableMenuItem(hMenu, ID_NEXT, MF_ENABLED);
		EnableMenuItem(hMenu, ID_STOP, MF_ENABLED);
		EnableMenuItem(hMenu, ID_ERROR, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_WIN, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_MAC, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_WEB, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_ANDROID, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_IOS, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_UNITY, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_PACKAGE, MF_ENABLED);
		for (nID = ID_CMD_MESSAGE; nID <= ID_CMD_STORY; nID++)
			EnableMenuItem(hMenu, nID, MF_ENABLED);
	} while (0);
}

/* ゲームループを実行する */
static void GameLoop(void)
{
	BOOL bBreak;

	/* WM_PAINTでの描画を許可する */
	bRunFrameAllow = TRUE;

	/* ゲームループ */
	bBreak = FALSE;
	while (!bBreak)
	{
		/* イベントを処理する */
		if(!SyncEvents())
			break;	/* 閉じるボタンが押された */

		/* 次の描画までスリープする */
		if(!WaitForNextFrame())
			break;	/* 閉じるボタンが押された */

		/* フレームの開始時刻を取得する */
		dwStartTime = GetTickCount();

		/* フレームを実行する */
		/* TODO: bRunning */
		if (!RunFrame())
			bBreak = TRUE;
	}
}

/* フレームを実行する */
static BOOL RunFrame(void)
{
	BOOL bRet;

	/* プロジェクトが開かれていない場合 */
	if (!bProjectOpened)
		return TRUE;

	/* 実行許可前の場合 */
	if (!bRunFrameAllow)
		return TRUE;

	/* DirectShowで動画を再生中の場合は特別に処理する */
	if(bDShowMode)
	{
		/* ウィンドウイベントを処理する */
		if(!SyncEvents())
			return FALSE;

		/* @videoコマンドを実行する */
		if(!on_event_frame())
			return FALSE;

		return TRUE;
	}

	/* フレームの描画を開始する */
	D3DStartFrame();

	/* フレームの実行と描画を行う */
	bRet = TRUE;
	if(!on_event_frame())
	{
		/* スクリプトの終端に達した */
		bRet = FALSE;
		bRunFrameAllow = FALSE;
	}

	/* フレームの描画を終了する */
	D3DEndFrame();

	return bRet;
}

/* キューにあるイベントを処理する */
static BOOL SyncEvents(void)
{
	/* DWORD dwStopWatchPauseStart; */
	MSG msg;

	/* イベント処理の開始時刻を求める */
	/* dwStopWatchPauseStart = GetTickCount(); */

	/* イベント処理を行う */
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return FALSE;
		if (PretranslateMessage(&msg))
			continue;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return TRUE;
}

/*
 * メッセージのトランスレート前処理を行う
 *  - return: メッセージが消費されたか
 *    - TRUEなら呼出元はメッセージをウィンドウプロシージャに送ってはならない
 *    - FALSEなら呼出元はメッセージをウィンドウプロシージャに送らなくてはいけない
 */
static BOOL PretranslateMessage(MSG* pMsg)
{
	static BOOL bShiftDown;
	static BOOL bControlDown;

	/* Alt+Enterを処理する */
	if (pMsg->hwnd == hWndRichEdit &&
		pMsg->message == WM_SYSKEYDOWN &&
		pMsg->wParam == VK_RETURN &&
		(HIWORD(pMsg->lParam) & KF_ALTDOWN))
	{
		if (!bFullScreen)
			bNeedFullScreen = TRUE;
		else
			bNeedWindowed = TRUE;
		SendMessage(hWndMain, WM_SIZE, 0, 0);

		/* このメッセージをリッチエディットにディスパッチしない */
		return TRUE;
	}

	/* シフト押下状態を保存する */
	if (pMsg->hwnd == hWndRichEdit && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SHIFT)
	{
		bShiftDown = TRUE;
		return FALSE;
	}
	if (pMsg->hwnd == hWndRichEdit && pMsg->message == WM_KEYUP && pMsg->wParam == VK_SHIFT)
	{
		bShiftDown = FALSE;
		return FALSE;
	}

	/* コントロール押下状態を保存する */
	if (pMsg->hwnd == hWndRichEdit && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_CONTROL)
	{
		bControlDown = TRUE;
		return FALSE;
	}
	if (pMsg->hwnd == hWndRichEdit && pMsg->message == WM_KEYUP && pMsg->wParam == VK_CONTROL)
	{
		bControlDown = FALSE;
		return FALSE;
	}

	/* フォーカスを失うときにシフトとコントロールの押下状態をクリアする */
	if (pMsg->hwnd == hWndRichEdit && pMsg->message == WM_KILLFOCUS)
	{
		bShiftDown = FALSE;
		bControlDown = FALSE;
		return FALSE;
	}

	/* 右クリック押下を処理する */
	if (pMsg->hwnd == hWndRichEdit &&
		pMsg->message == WM_RBUTTONDOWN)
	{
		/* ポップアップを開くためのWM_COMMANDをポストする */
		PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_POPUP, 0);
		return FALSE;
	}
	
	/* Ctrl-Gを処理する */
	if (pMsg->message == WM_KEYDOWN &&
		pMsg->message == WM_KEYDOWN &&
		pMsg->wParam == 'G')
	{
		/* ゲームを開くためのWM_COMMANDをポストする */
		PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_OPEN_PROJECT, 0);
	}

	/* リッチエディットでのキー押下を処理する */
	if (pMsg->hwnd == hWndRichEdit && pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		/*
		 * リッチエディットの編集
		 */
		case VK_RETURN:
			if (bShiftDown)
			{
				OnShiftEnter();
				bShiftDown = FALSE;

				/* このメッセージはリッチエディットに送らない(改行しない) */
				return TRUE;
			}
			break;
		case VK_TAB:
			if (!bShiftDown && !bControlDown)
			{
				OnTab();

				/* このメッセージはリッチエディットに送らない */
				return TRUE;
			}
			break;
		/*
		 * メニュー
		 */
		case 'O':
			/* Ctrl+Oを処理する */
			if (bControlDown)
			{
				bControlDown = FALSE;
				PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_OPEN, 0);

				/* このメッセージはリッチエディットに送らない */
				return TRUE;
			}
			break;
		case 'L':
			/* Ctrl+Lを処理する */
			if (bControlDown)
			{
				bControlDown = FALSE;
				PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_RELOAD, 0);

				/* このメッセージはリッチエディットに送らない */
				return TRUE;
			}
			break;
		case 'S':
			/* Ctrl+Sを処理する */
			if (bControlDown)
			{
				bControlDown = FALSE;
				PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_SAVE, 0);

				/* このメッセージはリッチエディットに送らない */
				return TRUE;
			}
			break;
		case 'Q':
			/* Ctrl+Qを処理する */
			if (bControlDown)
			{
				bControlDown = FALSE;
				PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_QUIT, 0);

				/* このメッセージはリッチエディットに送らない */
				return TRUE;
			}
			break;
		case 'R':
			/* Ctrl+Rを処理する */
			if (bControlDown)
			{
				bControlDown = FALSE;
				PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_CONTINUE, 0);

				/* このメッセージはリッチエディットに送らない */
				return TRUE;
			}
			break;
		case 'N':
			/* Ctrl+Nを処理する */
			if (bControlDown)
			{
				bControlDown = FALSE;
				PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_NEXT, 0);

				/* このメッセージはリッチエディットに送らない */
				return TRUE;
			}
			break;
		case 'P':
			/* Ctrl+Pを処理する */
			if (bControlDown)
			{
				bControlDown = FALSE;
				PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_STOP, 0);

				/* このメッセージはリッチエディットに送らない */
				return TRUE;
			}
			break;
		case 'E':
			/* Ctrl+Eを処理する */
			if (bControlDown)
			{
				bControlDown = FALSE;
				PostMessage(hWndMain, WM_COMMAND, (WPARAM)ID_ERROR, 0);

				/* このメッセージはリッチエディットに送らない */
				return TRUE;
			}
			break;
		case 'Z':
		case 'Y':
			/* Ctrl+Z,Ctrl+Yを処理する */
			if (bControlDown)
			{
				/* ハイライト操作もUNDOされるので、現状非サポート */
				/* このメッセージはリッチエディットに送らない */
				return TRUE;
			}
			break;
		default:
			break;
		}
	}

	/* このメッセージは引き続きリッチエディットで処理する */
	return FALSE;
}

/* 次のフレームの開始時刻までイベント処理とスリープを行う */
static BOOL WaitForNextFrame(void)
{
	DWORD end, lap, wait, span;

	/* 30FPSを目指す */
	span = FRAME_MILLI;
	if (D3DIsSoftRendering())
	{
		/* ソフトレンダリングのときは15fpsとする */
		span *= 2;
	}

	/* 次のフレームの開始時刻になるまでイベント処理とスリープを行う */
	do {
		/* イベントがある場合は処理する */
		if(!SyncEvents())
			return FALSE;

		/* 経過時刻を取得する */
		end = GetTickCount();
		lap = end - dwStartTime;

		/* 次のフレームの開始時刻になった場合はスリープを終了する */
		if(lap >= span) {
			dwStartTime = end;
			break;
		}

		/* スリープする時間を求める */
		wait = (span - lap > SLEEP_MILLI) ? SLEEP_MILLI : span - lap;

		/* スリープする */
		Sleep(wait);
	} while(wait > 0);

	return TRUE;
}

/* ウィンドウプロシージャ */
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int kc;

	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SYSKEYDOWN:
		/* Alt + Enter */
		if(wParam == VK_RETURN && (HIWORD(lParam) & KF_ALTDOWN))
		{
			if (!bFullScreen)
			{
				bNeedFullScreen = TRUE;
				bNeedWindowed = FALSE;
			}
			else
			{
				bNeedWindowed = TRUE;
				bNeedFullScreen = FALSE;
			}
			SendMessage(hWndMain, WM_SIZE, 0, 0);
			return 0;
		}

		/* Alt + F4 */
		if(wParam == VK_F4)
		{
			DestroyWindow(hWnd);
			return 0;
		}
		break;
	case WM_CLOSE:
		if (hWnd != NULL && hWnd == hWndMain)
		{
			DestroyWindow(hWnd);
			return 0;
		}
		break;
	case WM_LBUTTONDOWN:
		if (hWnd != NULL && hWnd == hWndRender)
		{
			on_event_mouse_press(MOUSE_LEFT,
								 (int)((float)(LOWORD(lParam) - nViewportOffsetX) / fMouseScale),
								 (int)((float)(HIWORD(lParam) - nViewportOffsetY) / fMouseScale));
			return 0;
		}
		break;
	case WM_LBUTTONUP:
		if (hWnd != NULL && hWnd == hWndRender)
		{
			on_event_mouse_release(MOUSE_LEFT,
								   (int)((float)(LOWORD(lParam) - nViewportOffsetX) / fMouseScale),
								   (int)((float)(HIWORD(lParam) - nViewportOffsetY) / fMouseScale));
			return 0;
		}
		break;
	case WM_RBUTTONDOWN:
		if (hWnd != NULL && hWnd == hWndRender)
		{
			on_event_mouse_press(MOUSE_RIGHT,
								 (int)((float)(LOWORD(lParam) - nViewportOffsetX) / fMouseScale),
								 (int)((float)(HIWORD(lParam) - nViewportOffsetY) / fMouseScale));
			return 0;
		}
		break;
	case WM_RBUTTONUP:
		if (hWnd != NULL && hWnd == hWndRender)
		{
			on_event_mouse_release(MOUSE_RIGHT,
								   (int)((float)(LOWORD(lParam) - nViewportOffsetX) / fMouseScale),
								   (int)((float)(HIWORD(lParam) - nViewportOffsetY) / fMouseScale));
			return 0;
		}
		break;
	case WM_KEYDOWN:
		if (hWnd != NULL && hWnd == hWndRender)
		{
			/* オートリピートの場合を除外する */
			if((HIWORD(lParam) & 0x4000) != 0)
				return 0;

			/* フルスクリーン中のエスケープキーの場合 */
			if((int)wParam == VK_ESCAPE && bFullScreen)
			{
				bNeedWindowed = TRUE;
				SendMessage(hWndMain, WM_SIZE, 0, 0);
				return 0;
			}

			/* その他のキーの場合 */
			kc = ConvertKeyCode((int)wParam);
			if(kc != -1)
				on_event_key_press(kc);
			return 0;
		}
		break;
	case WM_KEYUP:
		if (hWnd != NULL && (hWnd == hWndRender || hWnd == hWndMain))
		{
			kc = ConvertKeyCode((int)wParam);
			if(kc != -1)
				on_event_key_release(kc);
			return 0;
		}
		break;
	case WM_MOUSEMOVE:
		if (hWnd != NULL && hWnd == hWndRender)
		{
			on_event_mouse_move((int)((float)(LOWORD(lParam) - nViewportOffsetX) / fMouseScale),
								(int)((float)(HIWORD(lParam) - nViewportOffsetY) / fMouseScale));
			return 0;
		}
		break;
	case WM_MOUSEWHEEL:
		if (hWnd != NULL && hWnd == hWndRender)
		{
			if((int)(short)HIWORD(wParam) > 0)
			{
				on_event_key_press(KEY_UP);
				on_event_key_release(KEY_UP);
			}
			else if((int)(short)HIWORD(wParam) < 0)
			{
				on_event_key_press(KEY_DOWN);
				on_event_key_release(KEY_DOWN);
			}
			return 0;
		}
		break;
	case WM_KILLFOCUS:
		if (hWnd != NULL && (hWnd == hWndRender || hWnd == hWndMain))
		{
			on_event_key_release(KEY_CONTROL);
			return 0;
		}
		break;
	case WM_SYSCHAR:
		if (hWnd != NULL && (hWnd == hWndRender || hWnd == hWndMain))
		{
			return 0;
		}
		break;
	case WM_PAINT:
		if (hWnd != NULL && (hWnd == hWndRender || hWnd == hWndMain))
		{
			OnPaint(hWnd);
			return 0;
		}
		break;
	case WM_COMMAND:
		OnCommand(wParam, lParam);
		return 0;
	case WM_GRAPHNOTIFY:
		if(!DShowProcessEvent())
			bDShowMode = FALSE;
		break;
	case WM_SIZING:
		if (hWnd != NULL && hWnd == hWndMain)
		{
			OnSizing((int)wParam, (LPRECT)lParam);
			return TRUE;
		}
		break;
	case WM_SIZE:
		if (hWnd != NULL && hWnd == hWndMain)
		{
			OnSize();
			return 0;
		}
		break;
	case WM_DPICHANGED:
		OnDpiChanged(hWnd, HIWORD(wParam), (LPRECT)lParam);
		return 0;
	default:
		break;
	}

	/* システムのウィンドウプロシージャにチェインする */
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/* キーコードの変換を行う */
static int ConvertKeyCode(int nVK)
{
	switch(nVK)
	{
	case VK_CONTROL:
		return KEY_CONTROL;
	case VK_SPACE:
		return KEY_SPACE;
	case VK_RETURN:
		return KEY_RETURN;
	case VK_UP:
		return KEY_UP;
	case VK_DOWN:
		return KEY_DOWN;
	case VK_LEFT:
		return KEY_LEFT;
	case VK_RIGHT:
		return KEY_RIGHT;
	case VK_ESCAPE:
		return KEY_ESCAPE;
	case 'S':
		return KEY_S;
	case 'L':
		return KEY_L;
	case 'H':
		return KEY_H;
	default:
		break;
	}
	return -1;
}

/* ウィンドウの内容を更新する */
static void OnPaint(HWND hWnd)
{
	HDC hDC;
	PAINTSTRUCT ps;

	hDC = BeginPaint(hWnd, &ps);
	if (hWnd == hWndRender)
		RunFrame();
	EndPaint(hWnd, &ps);

	UNUSED_PARAMETER(hDC);
}

/* WM_COMMANDを処理する */
static void OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nID;
	UINT nNotify;

	UNUSED_PARAMETER(lParam);

	nID = LOWORD(wParam);
	nNotify = (WORD)(wParam >> 16) & 0xFFFF;

	/* リッチエディットのEN_CHANGEを確認する */
	if (nID == ID_RICHEDIT && nNotify == EN_CHANGE)
	{
		ScriptView_OnChange();
		return;
	}

	/* IDごとに処理する */
	switch(nID)
	{
	/* File */
	case ID_NEW_PROJECT_JAPANESE_ADV:
		OnNewProject(L"games\\jp-adv\\*");
		break;
	case ID_NEW_PROJECT_ENGLISH_ADV:
		OnNewProject(L"games\\en-adv\\*");
		break;
	case ID_OPEN_PROJECT:
		OnOpenProject();
		break;
	case ID_OPEN_GAME_FOLDER:
		OnOpenGameFolder();
		break;
	case ID_OPEN:
		OnOpenScript();
		break;
	case ID_RELOAD:
		OnReloadScript();
		break;
	case ID_SAVE:
		OnSave();
		break;
	case ID_DEBUG_LAYERS:
		write_layers_to_files();
		break;
	case ID_QUIT:
		DestroyWindow(hWndMain);
		break;
	/* Execution */
	case ID_CONTINUE:
		OnContinue();
		break;
	case ID_NEXT:
		OnNext();
		break;
	case ID_STOP:
		OnStop();
		break;
	case ID_ERROR:
		OnNextError();
		break;
	/* Popup */
	case ID_POPUP:
		OnPopup();
		break;
	/* Direction */
	case ID_PROPERTY:
		OnProperty();
		break;
	case ID_PROPERTY_UPDATE:
		OnPropertyUpdate();
		break;
	case ID_CMD_MESSAGE:
		OnInsertMessage();
		break;
	case ID_CMD_SERIF:
		OnInsertSerif();
		break;
	case ID_CMD_BG:
		OnInsertBg();
		break;
	case ID_CMD_BG_ONLY:
		OnInsertBgOnly();
		break;
	case ID_CMD_CH:
		OnInsertCh();
		break;
	case ID_CMD_CHCH:
		OnInsertChch();
		break;
	case ID_CMD_MUSIC:
		OnInsertMusic();
		break;
	case ID_CMD_SOUND:
		OnInsertSound();
		break;
	case ID_CMD_VOLUME:
		OnInsertVolume();
		break;
	case ID_CMD_VIDEO:
		OnInsertVideo();
		break;
	case ID_CMD_SHAKE:
		OnInsertShake();
		break;
	case ID_CMD_CHOOSE:
		OnInsertChoose();
		break;
	case ID_CMD_MENU:
		OnInsertMenu();
		break;
	case ID_CMD_CLICK:
		OnInsertClick();
		break;
	case ID_CMD_TIME:
		OnInsertTime();
		break;
	case ID_CMD_STORY:
		OnInsertStory();
		break;
	/* Export */
	case ID_EXPORT_WIN:
		OnExportWin();
		break;
	case ID_EXPORT_MAC:
		OnExportMac();
		break;
	case ID_EXPORT_WEB:
		OnExportWeb();
		break;
	case ID_EXPORT_ANDROID:
		OnExportAndroid();
		break;
	case ID_EXPORT_IOS:
		OnExportIOS();
		break;
	case ID_EXPORT_UNITY:
		OnExportUnity();
		break;
	case ID_EXPORT_PACKAGE:
		OnExportPackage();
		break;
	/* View */
	case ID_FONT:
		OnFont();
		break;
	case ID_HIGHLIGHTMODE:
		OnHighlightMode();
		break;
	case ID_DARKMODE:
		OnDarkMode();
		break;
	/* Help */
	case ID_VERSION:
		OnVersion();
		break;
	case ID_DOCUMENT:
		OnDocument();
		break;
	/* Button */
	case ID_VARS:
		OnWriteVars();
		break;
	default:
		break;
	}
}

/* WM_SIZING */
static void OnSizing(int edge, LPRECT lpRect)
{
	UNUSED_PARAMETER(edge);
	UNUSED_PARAMETER(lpRect);
}

/* WM_SIZE */
static void OnSize(void)
{
	RECT rc;

	if(bNeedFullScreen)
	{
		HMONITOR monitor;
		MONITORINFOEX minfo;

		bNeedFullScreen = FALSE;
		bNeedWindowed = FALSE;
		bFullScreen = TRUE;

		monitor = MonitorFromWindow(hWndMain, MONITOR_DEFAULTTONEAREST);
		minfo.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(monitor, (LPMONITORINFO)&minfo);
		rc = minfo.rcMonitor;

		dwStyle = (DWORD)GetWindowLong(hWndMain, GWL_STYLE);
		dwExStyle = (DWORD)GetWindowLong(hWndMain, GWL_EXSTYLE);
		GetWindowRect(hWndMain, &rcWindow);

		SetWindowLong(hWndMain, GWL_STYLE, (LONG)(WS_POPUP | WS_VISIBLE));
		SetWindowLong(hWndMain, GWL_EXSTYLE, WS_EX_TOPMOST);
		SetWindowPos(hWndMain, NULL, 0, 0, 0, 0,
					 SWP_NOMOVE | SWP_NOSIZE |
					 SWP_NOZORDER | SWP_FRAMECHANGED);
		MoveWindow(hWndMain,
				   rc.left,
				   rc.top,
				   rc.right - rc.left,
				   rc.bottom - rc.top,
				   TRUE);
		InvalidateRect(hWndMain, NULL, TRUE);

		/* Update the screen offset and scale. */
		Layout(rc.right - rc.left, rc.bottom - rc.top - GetSystemMetrics(SM_CYMENU));
	}
	else if (bNeedWindowed)
	{
		bNeedWindowed = FALSE;
		bNeedFullScreen = FALSE;
		bFullScreen = FALSE;

		if (hMenu != NULL)
			SetMenu(hWndMain, hMenu);

		SetWindowLong(hWndMain, GWL_STYLE, (LONG)dwStyle);
		SetWindowLong(hWndMain, GWL_EXSTYLE, (LONG)dwExStyle);
		MoveWindow(hWndMain, rcWindow.left, rcWindow.top, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, TRUE);
		InvalidateRect(hWndMain, NULL, TRUE);
		GetClientRect(hWndMain, &rc);

		/* Update the screen offset and scale. */
		Layout(rc.right - rc.left, rc.bottom - rc.top);
	}
	else
	{
		GetClientRect(hWndMain, &rc);

		/* Update the screen offset and scale. */
		Layout(rc.right - rc.left, rc.bottom - rc.top);
	}

	/* FIXME */
	ScriptView_ApplyFont();
}

/* スクリーンのオフセットとスケールを計算する */
static void Layout(int nClientWidth, int nClientHeight)
{
	float fAspect, fRenderWidth, fRenderHeight;
	int nDpi, nRenderWidth, nEditorWidth, y;

	nDpi = App_GetDpiForWindow(hWndMain);

	/* If size and dpi are not changed, just return. */
	if (nClientWidth == nLastClientWidth && nClientHeight == nLastClientHeight && nLastDpi != nDpi)
		return;

	/* Save the last client size and the dpi. */
	nLastClientWidth = nClientWidth;
	nLastClientHeight = nClientHeight;
	nLastDpi = nDpi;

	/* Calc the editor width and render width. */
	nEditorWidth = MulDiv(EDITOR_WIDTH, nDpi, 96);
	nRenderWidth = nClientWidth - nEditorWidth;

	/* Calc the rendering area. */
	fAspect = (float)conf_game_height / (float)conf_game_width;
	if ((float)nRenderWidth * fAspect <= (float)nClientHeight)
	{
		/* Width-first way. */
		fRenderWidth = (float)nRenderWidth;
		fRenderHeight = fRenderWidth * fAspect;
		fMouseScale = (float)nRenderWidth / (float)conf_game_width;
	}
	else
	{
		/* Height-first way. */
        fRenderHeight = (float)nClientHeight;
        fRenderWidth = (float)nClientHeight / fAspect;
        fMouseScale = (float)nClientHeight / (float)conf_game_height;
    }

	/* Calc the viewport origin. */
	nViewportOffsetX = (int)((((float)nRenderWidth - fRenderWidth) / 2.0f) + 0.5);
	nViewportOffsetY = (int)((((float)nClientHeight - fRenderHeight) / 2.0f) + 0.5);

	/* Save the viewport size. */
	nViewportWidth = nRenderWidth;
	nViewportHeight = (int)fRenderHeight;

	/* Move the rendering panel. */
	MoveWindow(hWndRender, 0, 0, nRenderWidth, nClientHeight, TRUE);
	
	/* Update the screen offset and scale for drawing subsystem. */
	D3DResizeWindow(nViewportOffsetX, nViewportOffsetY, fMouseScale);

	/* Change the control sizes. */
	MoveWindow(hWndBtnContinue, MulDiv(10, nDpi, 96), MulDiv(10, nDpi, 96), MulDiv(100, nDpi, 96), MulDiv(40, nDpi, 96), TRUE);
	MoveWindow(hWndBtnNext, MulDiv(120, nDpi, 96), MulDiv(10, nDpi, 96), MulDiv(100, nDpi, 96), MulDiv(40, nDpi, 96), TRUE);
	MoveWindow(hWndBtnStop, MulDiv(230, nDpi, 96), MulDiv(10, nDpi, 96), MulDiv(100, nDpi, 96), MulDiv(40, nDpi, 96), TRUE);
	MoveWindow(hWndBtnEdit, MulDiv(350, nDpi, 96), MulDiv(10, nDpi, 96), MulDiv(80, nDpi, 96), MulDiv(40, nDpi, 96), TRUE);
	MoveWindow(hWndTextboxScript, MulDiv(10, nDpi, 96), MulDiv(60, nDpi, 96), MulDiv(350, nDpi, 96), MulDiv(30, nDpi, 96), TRUE);
	MoveWindow(hWndBtnSelectScript, MulDiv(370, nDpi, 96), MulDiv(60, nDpi, 96), MulDiv(60, nDpi, 96), MulDiv(30, nDpi, 96), TRUE);
	MoveWindow(hWndRichEdit,MulDiv(10, nDpi, 96), MulDiv(100, nDpi, 96), MulDiv(420, nDpi, 96), nClientHeight - MulDiv(180, nDpi, 96), TRUE);
	y = nClientHeight - MulDiv(130, nDpi, 96);
	MoveWindow(hWndTextboxVar, MulDiv(10, nDpi, 96), y + MulDiv(60, nDpi, 96), MulDiv(280, nDpi, 96), MulDiv(60, nDpi, 96), TRUE);
	MoveWindow(hWndBtnVar, MulDiv(300, nDpi, 96), y + MulDiv(70, nDpi, 96), MulDiv(130, nDpi, 96), MulDiv(30, nDpi, 96), TRUE);

	/* Move the editor panel. */
	MoveWindow(hWndEditor, nRenderWidth, 0, nEditorWidth, nClientHeight, TRUE);

	if (D3DIsSoftRendering())
	{
		nViewportWidth = conf_game_width;
		nViewportHeight = conf_game_height;
		nViewportOffsetX = 0;
		nViewportOffsetY = 0;
		fMouseScale = 1.0f;
	}
}

/* WM_DPICHANGED */
VOID OnDpiChanged(HWND hWnd, UINT nDpi, LPRECT lpRect)
{
	UNUSED_PARAMETER(nDpi);
	UNUSED_PARAMETER(lpRect);

	if (hWnd == hWndMain)
		SendMessage(hWndMain, WM_SIZE, 0, 0);
}

/*
 * HAL (main)
 */

/*
 * INFOログを出力する
 */
bool log_info(const char *s, ...)
{
	char buf[4096];
	va_list ap;

	/* メッセージボックスを表示する */
	va_start(ap, s);
	vsnprintf(buf, sizeof(buf), s, ap);
	va_end(ap);
	MessageBox(hWndMain, conv_utf8_to_utf16(buf), TITLE, MB_OK | MB_ICONINFORMATION);

	return true;
}

/*
 * WARNログを出力する
 */
bool log_warn(const char *s, ...)
{
	char buf[4096];
	va_list ap;

	/* メッセージボックスを表示する */
	va_start(ap, s);
	vsnprintf(buf, sizeof(buf), s, ap);
	va_end(ap);
	MessageBox(hWndMain, conv_utf8_to_utf16(buf), TITLE, MB_OK | MB_ICONWARNING);

	return true;
}

/*
 * ERRORログを出力する
 */
bool log_error(const char *s, ...)
{
	char buf[4096];
	va_list ap;

	/* メッセージボックスを表示する */
	va_start(ap, s);
	vsnprintf(buf, sizeof(buf), s, ap);
	va_end(ap);
	MessageBox(hWndMain, conv_utf8_to_utf16(buf), TITLE, MB_OK | MB_ICONERROR);

	return true;
}

/*
 * UTF-8のメッセージをUTF-16に変換する
 */
const wchar_t *conv_utf8_to_utf16(const char *utf8_message)
{
	assert(utf8_message != NULL);

	/* UTF8からUTF16に変換する */
	MultiByteToWideChar(CP_UTF8, 0, utf8_message, -1, wszMessage,
						CONV_MESSAGE_SIZE - 1);

	return wszMessage;
}

/*
 * UTF-16のメッセージをUTF-8に変換する
 */
const char *conv_utf16_to_utf8(const wchar_t *utf16_message)
{
	assert(utf16_message != NULL);

	/* ワイド文字からUTF-8に変換する */
	WideCharToMultiByte(CP_UTF8, 0, utf16_message, -1, szMessage,
						CONV_MESSAGE_SIZE - 1, NULL, NULL);

	return szMessage;
}

/*
 * セーブディレクトリを作成する
 */
bool make_sav_dir(void)
{
	wchar_t path[MAX_PATH] = {0};

	if (conf_release) {
		/* AppDataに作成する */
		SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
		wcsncat(path, L"\\", MAX_PATH - 1);
		wcsncat(path, conv_utf8_to_utf16(conf_game_title), MAX_PATH - 1);
		CreateDirectory(path, NULL);
	} else {
		/* ゲームディレクトリに作成する */
		CreateDirectory(conv_utf8_to_utf16(SAVE_DIR), NULL);
	}

	return true;
}

/*
 * データのディレクトリ名とファイル名を指定して有効なパスを取得する
 */
char *make_valid_path(const char *dir, const char *fname)
{
	wchar_t *buf;
	const char *result;
	size_t len;

	if (dir == NULL)
		dir = "";

	if (conf_release && strcmp(dir, SAVE_DIR) == 0) {
		/* AppDataを参照する場合 */
		wchar_t path[MAX_PATH] = {0};
		SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
		wcsncat(path, L"\\", MAX_PATH - 1);
		wcsncat(path, conv_utf8_to_utf16(conf_game_title), MAX_PATH - 1);
		wcsncat(path, L"\\", MAX_PATH - 1);
		wcsncat(path, conv_utf8_to_utf16(fname), MAX_PATH - 1);
		return strdup(conv_utf16_to_utf8(path));
	}

	/* パスのメモリを確保する */
	len = strlen(dir) + 1 + strlen(fname) + 1;
	buf = malloc(sizeof(wchar_t) * len);
	if (buf == NULL)
		return NULL;

	/* パスを生成する */
	wcscpy(buf, conv_utf8_to_utf16(dir));
	if (strlen(dir) != 0)
		wcscat(buf, L"\\");
	wcscat(buf, conv_utf8_to_utf16(fname));

	result = conv_utf16_to_utf8(buf);
	free(buf);
	return strdup(result);
}

/*
 * タイマをリセットする
 */
void reset_lap_timer(uint64_t *origin)
{
	*origin = GetTickCount();
}

/*
 * タイマのラップを秒単位で取得する
 */
uint64_t get_lap_timer_millisec(uint64_t *origin)
{
	DWORD dwCur = GetTickCount();
	return (uint64_t)(dwCur - *origin);
}

/*
 * ビデオを再生する
 */
bool play_video(const char *fname, bool is_skippable)
{
	char *path;

	path = make_valid_path(MOV_DIR, fname);

	/* イベントループをDirectShow再生モードに設定する */
	bDShowMode = TRUE;

	/* クリックでスキップするかを設定する */
	bDShowSkippable = is_skippable;

	/* ビデオの再生を開始する */
	BOOL ret = DShowPlayVideo(hWndRender, path, nViewportOffsetX, nViewportOffsetY, nViewportWidth, nViewportHeight);
	if(!ret)
		bDShowMode = FALSE;

	free(path);
	return ret;
}

/*
 * ビデオを停止する
 */
void stop_video(void)
{
	DShowStopVideo();
	bDShowMode = FALSE;
}

/*
 * ビデオが再生中か調べる
 */
bool is_video_playing(void)
{
	return bDShowMode;
}

/*
 * フルスクリーンモードがサポートされるか調べる
 */
bool is_full_screen_supported()
{
	return true;
}

/*
 * フルスクリーンモードであるか調べる
 */
bool is_full_screen_mode(void)
{
	return bFullScreen ? true : false;
}

/*
 * フルスクリーンモードを開始する
 */
void enter_full_screen_mode(void)
{
	if (!bFullScreen)
	{
		bNeedFullScreen = TRUE;
		bNeedWindowed = FALSE;
		SendMessage(hWndMain, WM_SIZE, 0, 0);
	}
}

/*
 * フルスクリーンモードを終了する
 */
void leave_full_screen_mode(void)
{
	if (bFullScreen)
	{
		bNeedWindowed = TRUE;
		bNeedFullScreen = FALSE;
		SendMessage(hWndMain, WM_SIZE, 0, 0);
	}
}

/*
 * システムのロケールを取得する
 */
const char *get_system_locale(void)
{
	DWORD dwLang = GetUserDefaultLCID() & 0x3ff;
	switch (dwLang) {
	case LANG_ENGLISH:
		return "en";
	case LANG_FRENCH:
		return "fr";
	case LANG_GERMAN:
		return "de";
	case LANG_SPANISH:
		return "es";
	case LANG_ITALIAN:
		return "it";
	case LANG_GREEK:
		return "el";
	case LANG_RUSSIAN:
		return "ru";
	case LANG_CHINESE_SIMPLIFIED:
		return "zh";
	case LANG_CHINESE_TRADITIONAL:
		return "tw";
	case LANG_JAPANESE:
		return "ja";
	default:
		break;
	}
	return "other";
}

void set_continuous_swipe_enabled(bool is_enabled)
{
	UNUSED_PARAMETER(is_enabled);
}

/*
 * TTSによる読み上げを行う
 */
void speak_text(const char *text)
{
	UNUSED_PARAMETER(text);
}

/*
 * HAL (pro)
 */

/*
 * 続けるボタンが押されたか調べる
 */
bool is_continue_pushed(void)
{
	bool ret = bContinuePressed;
	bContinuePressed = FALSE;
	return ret;
}

/*
 * 次へボタンが押されたか調べる
 */
bool is_next_pushed(void)
{
	bool ret = bNextPressed;
	bNextPressed = FALSE;
	return ret;
}

/*
 * 停止ボタンが押されたか調べる
 */
bool is_stop_pushed(void)
{
	bool ret = bStopPressed;
	bStopPressed = FALSE;
	return ret;
}

/*
 * 実行するスクリプトファイルが変更されたか調べる
 */
bool is_script_opened(void)
{
	bool ret = bScriptOpened;
	bScriptOpened = FALSE;
	return ret;
}

/*
 * 変更された実行するスクリプトファイル名を取得する
 */
const char *get_opened_script(void)
{
	static wchar_t script[256];

	GetWindowText(hWndTextboxScript,
				  script,
				  sizeof(script) /sizeof(wchar_t) - 1);
	script[255] = L'\0';
	return conv_utf16_to_utf8(script);
}

/*
 * 実行する行番号が変更されたか調べる
 */
bool is_exec_line_changed(void)
{
	bool ret = bExecLineChanged;
	bExecLineChanged = FALSE;
	return ret;
}

/*
 * 変更された実行する行番号を取得する
 */
int get_changed_exec_line(void)
{
	return nLineChanged;
}

/*
 * コマンドの実行中状態を設定する
 */
void on_change_running_state(bool running, bool request_stop)
{
	UINT i;

	bRunning = running;

	if(request_stop)
	{
		/*
		 * 実行中だが停止要求によりコマンドの完了を待機中のとき
		 *  - コントロールとメニューアイテムを無効にする
		 */
		EnableWindow(hWndBtnContinue, FALSE);
		EnableWindow(hWndBtnNext, FALSE);
		EnableWindow(hWndBtnStop, FALSE);
		EnableWindow(hWndBtnEdit, FALSE);
		EnableWindow(hWndTextboxScript, FALSE);
		EnableWindow(hWndBtnSelectScript, FALSE);
		SendMessage(hWndRichEdit, EM_SETREADONLY, TRUE, 0);
		SendMessage(hWndTextboxVar, EM_SETREADONLY, TRUE, 0);
		EnableWindow(hWndBtnVar, FALSE);
		EnableMenuItem(hMenu, ID_OPEN, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SAVE, MF_GRAYED);
		EnableMenuItem(hMenu, ID_RELOAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_CONTINUE, MF_GRAYED);
		EnableMenuItem(hMenu, ID_NEXT, MF_GRAYED);
		EnableMenuItem(hMenu, ID_STOP, MF_GRAYED);
		EnableMenuItem(hMenu, ID_ERROR, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_WIN, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_MAC, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_WEB, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_ANDROID, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_IOS, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_PACKAGE, MF_GRAYED);
		for (i = ID_CMD_MESSAGE; i <= ID_CMD_STORY; i++)
			EnableMenuItem(hMenu, i, MF_GRAYED);

		/* 実行中の背景色を設定する */
		SetFocus(NULL);
		ScriptView_SetBackgroundColorForCurrentExecuteLine();
		SetFocus(hWndRichEdit);
	}
	else if(running)
	{
		/*
		 * 実行中のとき
		 *  - 「停止」だけ有効、他は無効にする
		 */
		EnableWindow(hWndBtnContinue, FALSE);
		EnableWindow(hWndBtnNext, FALSE);
		EnableWindow(hWndBtnStop, TRUE);
		EnableWindow(hWndBtnEdit, FALSE);
		EnableWindow(hWndTextboxScript, FALSE);
		EnableWindow(hWndBtnSelectScript, FALSE);
		SendMessage(hWndRichEdit, EM_SETREADONLY, TRUE, 0);
		SendMessage(hWndTextboxVar, EM_SETREADONLY, TRUE, 0);
		EnableWindow(hWndBtnVar, FALSE);
		EnableMenuItem(hMenu, ID_OPEN, MF_GRAYED);
		EnableMenuItem(hMenu, ID_SAVE, MF_GRAYED);
		EnableMenuItem(hMenu, ID_RELOAD, MF_GRAYED);
		EnableMenuItem(hMenu, ID_CONTINUE, MF_GRAYED);
		EnableMenuItem(hMenu, ID_NEXT, MF_GRAYED);
		EnableMenuItem(hMenu, ID_STOP, MF_ENABLED);
		EnableMenuItem(hMenu, ID_ERROR, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_WIN, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_MAC, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_WEB, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_ANDROID, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_IOS, MF_GRAYED);
		EnableMenuItem(hMenu, ID_EXPORT_PACKAGE, MF_GRAYED);
		for (i = ID_CMD_MESSAGE; i <= ID_CMD_STORY; i++)
			EnableMenuItem(hMenu, i, MF_GRAYED);

		/* 実行中の背景色を設定する */
		SetFocus(NULL);
		ScriptView_SetBackgroundColorForCurrentExecuteLine();
		SetFocus(hWndRichEdit);
	}
	else
	{
		/*
		 * 完全に停止中のとき
		 *  - 「停止」だけ無効、他は有効にする
		 */
		EnableWindow(hWndBtnContinue, TRUE);
		EnableWindow(hWndBtnNext, TRUE);
		EnableWindow(hWndBtnStop, FALSE);
		EnableWindow(hWndBtnEdit, TRUE);
		EnableWindow(hWndTextboxScript, TRUE);
		EnableWindow(hWndBtnSelectScript, TRUE);
		SendMessage(hWndRichEdit, EM_SETREADONLY, FALSE, 0);
		SendMessage(hWndTextboxVar, EM_SETREADONLY, FALSE, 0);
		EnableWindow(hWndBtnVar, TRUE);
		EnableMenuItem(hMenu, ID_OPEN, MF_ENABLED);
		EnableMenuItem(hMenu, ID_SAVE, MF_ENABLED);
		EnableMenuItem(hMenu, ID_RELOAD, MF_ENABLED);
		EnableMenuItem(hMenu, ID_CONTINUE, MF_ENABLED);
		EnableMenuItem(hMenu, ID_NEXT, MF_ENABLED);
		EnableMenuItem(hMenu, ID_STOP, MF_GRAYED);
		EnableMenuItem(hMenu, ID_ERROR, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_WIN, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_MAC, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_WEB, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_ANDROID, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_IOS, MF_ENABLED);
		EnableMenuItem(hMenu, ID_EXPORT_PACKAGE, MF_ENABLED);
		for (i = ID_CMD_MESSAGE; i <= ID_CMD_STORY; i++)
			EnableMenuItem(hMenu, i, MF_ENABLED);

		/* 次の実行される行の背景色を設定する */
		SetFocus(NULL);
		ScriptView_SetBackgroundColorForNextExecuteLine();
		SetFocus(hWndRichEdit);
	}
}

/*
 * スクリプトがロードされたときのコールバック
 */
void on_load_script(void)
{
	const char *script_file;

	/* スクリプトファイル名を設定する */
	script_file = get_script_file_name();
	SetWindowText(hWndTextboxScript, conv_utf8_to_utf16(script_file));

	/* スクリプトの最終更新時刻を取得する */
	ScriptView_UpdateTimeStamp();

	/* 実行中のスクリプトファイルが変更されたとき、リッチエディットにテキストを設定する */
	SetFocus(NULL);
	ScriptView_SetTextByScriptModel();
	SetFocus(hWndRichEdit);

	/* 全体のテキスト色を変更する(遅延) */
	ScriptView_DelayedHighligth();
}

/*
 * 実行位置が変更されたときのコールバック
 */
void on_change_position(void)
{
	SetFocus(NULL);

	/* 実行行のハイライトを行う */
	if (!bRunning)
		ScriptView_SetBackgroundColorForNextExecuteLine();
	else
		ScriptView_SetBackgroundColorForCurrentExecuteLine();

	/* 変数の情報を更新する */
	if (bNeedUpdateVars)
	{
		Variable_UpdateText();
		bNeedUpdateVars = FALSE;
	}

	/* スクロールする */
	ScriptView_AutoScroll();

	SetFocus(hWndRichEdit);
}

/*
 * 変数が変更されたときのコールバック
 */
void on_update_variable(void)
{
	bNeedUpdateVars = TRUE;
}

/*
 * 変数テキストボックス
 */

/* 変数の情報を更新する */
static VOID Variable_UpdateText(void)
{
	static wchar_t szTextboxVar[VAR_TEXTBOX_MAX];
	wchar_t line[1024];
	int index;
	int val;

	szTextboxVar[0] = L'\0';

	for(index = 0; index < LOCAL_VAR_SIZE + GLOBAL_VAR_SIZE; index++)
	{
		/* 変数が初期値の場合 */
		val = get_variable(index);
		if(val == 0 && !is_variable_changed(index))
			continue;

		/* 行を追加する */
		_snwprintf(line,
				   sizeof(line) / sizeof(wchar_t),
				   L"$%d=%d\r\n",
				   index,
				   val);
		line[1023] = L'\0';
		wcscat(szTextboxVar, line);
	}

	/* テキストボックスにセットする */
	SetWindowText(hWndTextboxVar, szTextboxVar);
}

/*
 * Project
 */

/* Create a new project from a template. */
static BOOL CreateProjectFromTemplate(const wchar_t *pszTemplate)
{
	static wchar_t wszPath[1024];
	OPENFILENAMEW ofn;
    WIN32_FIND_DATAW wfd;
    HANDLE hFind;
	HANDLE hFile;
	wchar_t *pFile;
	BOOL bNonEmpty;

	while (1)
	{
		/* Show a save dialog. */
		ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
		wcscpy(&wszPath[0], L"game.suika2");
		ofn.lStructSize = sizeof(OPENFILENAMEW);
		ofn.nFilterIndex  = 1;
		ofn.lpstrFile = wszPath;
		ofn.nMaxFile = sizeof(wszPath) / sizeof(wchar_t);
		ofn.Flags = OFN_OVERWRITEPROMPT;
		ofn.lpstrFilter = bEnglish ?
			L"Suika2 Project Files\0*.suika2\0\0" :
			L"Suika2 プロジェクトファイル\0*.suika2\0\0";
		ofn.lpstrDefExt = L".suika2";
		if (!GetSaveFileNameW(&ofn))
			return FALSE;
		if (ofn.lpstrFile[0] == L'\0')
			return FALSE;

		/* Get the base file name. */
		pFile = wcsrchr(ofn.lpstrFile, L'\\');
		if (pFile == NULL)
			return FALSE;
		pFile++;

		/* Check if the directory is empty. */
		hFind = FindFirstFileW(L".\\*.*", &wfd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			MessageBox(NULL, bEnglish ?
					   L"Invalid folder. Please choose one again." :
					   L"フォルダが存在しません。選択しなおしてください。",
					   TITLE,
					   MB_OK | MB_ICONERROR);
			continue;
		}
		bNonEmpty = FALSE;
		do
		{
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && wfd.cFileName[0] == L'.')
				continue;
			bNonEmpty = TRUE;
			break;
		} while(FindNextFileW(hFind, &wfd));
		FindClose(hFind);
		if (bNonEmpty)
		{
			MessageBox(NULL, bEnglish ?
					   L"Folder is not empty. Please create an empty folder." :
					   L"フォルダが空ではありません。空のフォルダを作成してください。",
					   TITLE,
					   MB_OK | MB_ICONERROR);
			continue;
		}

		GetCurrentDirectory(sizeof(wszProjectDir) / sizeof(wchar_t), wszProjectDir);

		/* Finish choosing a directory. */
		break;
	}

	/* プロジェクトファイルを作成する */
	hFile = CreateFileW(pFile, GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
	CloseHandle(hFile);

	/* コピーを行う */
	if (!CopyLibraryFiles(pszTemplate, L".\\"))
		return FALSE;

	return TRUE;
}

/* Choose a project and open it. */
static BOOL ChooseProject(void)
{
	static wchar_t wszPath[1024];
	OPENFILENAMEW ofn;
	BOOL bRet;

	/* Open a file dialog. */
	ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
	wcscpy(&wszPath[0], L"game.suika2");
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = wszPath;
	ofn.lpstrInitialDir = GetLastProjectPath();
	ofn.nMaxFile = sizeof(wszPath) / sizeof(wchar_t);
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrFilter = bEnglish ?
		L"Suika2 Project Files\0*.suika2\0\0" :
		L"Suika2 プロジェクトファイル\0*.suika2\0\0";
	ofn.lpstrDefExt = L".suika2";

	/* This will set the working directory to the game directory. */
	bRet = GetOpenFileNameW(&ofn);
	if (!bRet)
		return FALSE;

	/* If no file was selected. */
	if(ofn. lpstrFile[0] == L'\0')
		return FALSE;

	GetCurrentDirectory(sizeof(wszProjectDir) / sizeof(wchar_t), wszProjectDir);

	/* Read a project file. */
	ReadProjectFile();

	return TRUE;
}

/* Open a project by a path. */
static BOOL OpenProjectAtPath(const wchar_t *pszPath)
{
	wchar_t path[1024];
	wchar_t *pLastSeparator;

	/* Get the folder name. */
	wcsncpy(path, pszPath, sizeof(path) / sizeof(wchar_t) - 1);
	path[sizeof(path) / sizeof(wchar_t) - 1] = L'\0';
	pLastSeparator = wcsrchr(path, L'\\');
	if (pLastSeparator == NULL)
	{
		MessageBox(NULL, bEnglish ?
				   L"Invalid file name." :
				   L"ファイル名が正しくありません。",
				   TITLE,
				   MB_OK | MB_ICONERROR);
		return FALSE;
	}
	*pLastSeparator = L'\0';

	/* Set the working directory. */
	if (!SetCurrentDirectory(path))
	{
		MessageBox(NULL, bEnglish ?
				   L"Invalid game folder." :
				   L"ゲームフォルダが正しくありません。",
				   TITLE,
				   MB_OK | MB_ICONERROR);
		return FALSE;
	}
	GetCurrentDirectory(sizeof(wszProjectDir) / sizeof(wchar_t), wszProjectDir);

	/* Read a project file. */
	ReadProjectFile();

	RecordLastProjectPath();

	return TRUE;
}

static void ReadProjectFile(void)
{
	char buf[1024];
	FILE *fp;

	/* Defaults */
	CheckMenuItem(hMenu, ID_HIGHLIGHTMODE, MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_DARKMODE, MF_UNCHECKED);

	/* Read the preference. */
	fp = fopen("game.suika2", "r");
	if (fp == NULL)
		log_info("failed to read the project file.");
	while (1)
	{
		char *stop;

		if (fgets(buf, sizeof(buf) - 1, fp) == NULL)
			break;
		stop = strstr(buf, "\n");
		if (stop != NULL)
			*stop = '\0';

		if (strncmp(buf, "font-name:", 10) == 0)
		{
			ScriptView_SetFontName(conv_utf8_to_utf16(buf + 10));
		}
		else if (strncmp(buf, "font-size:", 10) == 0)
		{
			ScriptView_SetFontSize(abs(atoi(buf + 10)));
		}
		else if (strcmp(buf, "highlightmode") == 0)
		{
			ScriptView_SetHighlightMode(TRUE);
			CheckMenuItem(hMenu, ID_HIGHLIGHTMODE, MF_CHECKED);
		}
		else if (strcmp(buf, "darkmode") == 0)
		{
			ScriptView_SetDarkMode(TRUE);
			CheckMenuItem(hMenu, ID_DARKMODE, MF_CHECKED);
		}
		else
		{
			log_info("unknown project setting: %s", buf);
		}
	}
	fclose(fp);
}

static void WriteProjectFile(void)
{
	FILE *fp;

	fp = fopen("game.suika2", "w");
	if (fp == NULL)
		return;

	fprintf(fp, "font-name:%s\n", conv_utf16_to_utf8(ScriptView_GetFontName()));
	fprintf(fp, "font-size:%d\n", ScriptView_GetFontSize());
	if (ScriptView_IsHighlightMode())
		fprintf(fp, "highlightmode\n");
	if (ScriptView_IsDarkMode())
		fprintf(fp, "darkmode\n");

	fclose(fp);
}

static VOID RecordLastProjectPath(void)
{
	wchar_t path[MAX_PATH];
	wchar_t *pSep;
	FILE *fp;

	GetModuleFileName(NULL, path, MAX_PATH);
	pSep = wcsrchr(path, L'\\');
	if (pSep != NULL)
		*(pSep + 1) = L'\0';
	wcscat(path, L"settings.txt");

	fp = _wfopen(path, L"wb");
	if (fp != NULL)
	{
		fprintf(fp, "%s", conv_utf16_to_utf8(wszProjectDir));
		fclose(fp);
	}
}

static const wchar_t *GetLastProjectPath(void)
{
	wchar_t path[MAX_PATH];
	char buf[1024];
	wchar_t *pSep;
	FILE *fp;

	GetModuleFileName(NULL, path, MAX_PATH);
	pSep = wcsrchr(path, L'\\');
	if (pSep != NULL)
		*(pSep + 1) = L'\0';
	wcscat(path, L"settings.txt");

	fp = _wfopen(path, L"rb");
	if (fp != NULL)
	{
		fgets(buf, sizeof(buf), fp);
		fclose(fp);
		return conv_utf8_to_utf16(buf);
	}

	return NULL;
}

/*
 * コマンド処理
 */

/* 新規ゲームプロジェクト作成 */
static VOID OnNewProject(const wchar_t *pszTemplate)
{
	if (!CreateProjectFromTemplate(pszTemplate))
		return;

	RecordLastProjectPath();

	StartGame();
}

/* ゲームプロジェクトを開く */
static VOID OnOpenProject(void)
{
	if (!ChooseProject())
		return;

	RecordLastProjectPath();

	StartGame();
}

/* ゲームフォルダオープン */
static VOID OnOpenGameFolder(void)
{
	/* Explorerを開く */
	ShellExecuteW(NULL, L"explore", L".\\", NULL, NULL, SW_SHOW);
}

/* スクリプトオープン */
static VOID OnOpenScript(void)
{
	const wchar_t *pFile;

	pFile = SelectFile(SCENARIO_DIR);
	if (pFile == NULL)
		return;

	SetWindowText(hWndTextboxScript, pFile);
	bScriptOpened = TRUE;
	bExecLineChanged = FALSE;
	nLineChanged = 0;
}

/* スクリプトリロード */
static VOID OnReloadScript(void)
{
	bScriptOpened = TRUE;
	bExecLineChanged = TRUE;
	nLineChanged = get_expanded_line_num();
}

/* ファイルを開くダイアログを表示して素材ファイルを選択する */
const wchar_t *SelectFile(const char *pszDir)
{
	static wchar_t wszPath[1024];
	wchar_t wszBase[1024];
	OPENFILENAMEW ofn;
	wchar_t *pSlash;
	BOOL bRet;

	ZeroMemory(&wszPath[0], sizeof(wszPath));
	ZeroMemory(&ofn, sizeof(OPENFILENAMEW));

	SetCurrentDirectory(wszProjectDir);
	wcscpy(wszBase, wszProjectDir);
	wcscat(wszBase, L"\\");
	wcscat(wszBase, conv_utf8_to_utf16(pszDir));
	pSlash = &wszBase[0];
	while ((pSlash = wcsstr(pSlash, L"/")))
		*pSlash++ = L'\\';

	/* ファイルダイアログの準備を行う */
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = hWndMain;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = wszPath;
	ofn.nMaxFile = sizeof(wszPath);
	ofn.lpstrInitialDir = wszBase;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (strcmp(pszDir, BG_DIR) == 0 ||
		strcmp(pszDir, CH_DIR) == 0 ||
		strcmp(pszDir, RULE_DIR) == 0)
	{
		ofn.lpstrFilter = bEnglish ?
			L"Image Files\0*.png;*.jpg;*.webp;\0All Files(*.*)\0*.*\0\0" : 
			L"画像ファイル\0*.png;*.jpg;*.webp;\0すべてのファイル(*.*)\0*.*\0\0";
		ofn.lpstrDefExt = L"png";
	}
	else if (strcmp(pszDir, BGM_DIR) == 0 ||
			 strcmp(pszDir, SE_DIR) == 0 ||
			 strcmp(pszDir, CV_DIR) == 0)
	{
		ofn.lpstrFilter = bEnglish ?
			L"Sound Files\0*.ogg;\0All Files(*.*)\0*.*\0\0" : 
			L"音声ファイル\0*.ogg;\0すべてのファイル(*.*)\0*.*\0\0";
		ofn.lpstrDefExt = L"ogg";
	}
	else if (strcmp(pszDir, MOV_DIR) == 0)
	{
		ofn.lpstrFilter = bEnglish ?
			L"Video Files\0*.mp4;*.wmv;\0All Files(*.*)\0*.*\0\0" : 
			L"動画ファイル\0*.mp4;*.wmv;\0すべてのファイル(*.*)\0*.*\0\0";
		ofn.lpstrDefExt = L"ogg";
	}
	else if (strcmp(pszDir, SCENARIO_DIR) == 0 ||
			 strcmp(pszDir, GUI_DIR) == 0)
	{
		ofn.lpstrFilter = bEnglish ?
			L"Scenario Files\0*.txt;\0All Files(*.*)\0*.*\0\0" : 
			L"シナリオファイル\0*.txt;\0すべてのファイル(*.*)\0*.*\0\0";
		ofn.lpstrDefExt = L"txt";
	}

	/* ファイルダイアログを開く */
	bRet = GetOpenFileNameW(&ofn);
	SetCurrentDirectory(wszProjectDir);
	if (!bRet)
		return NULL;
	if(ofn.lpstrFile[0] == L'\0')
		return NULL;
	if (wcswcs(wszPath, wszBase) != wszPath)
	{
		MessageBox(hWndMain, bEnglish ?
				   L"Invalid folder." :
				   L"フォルダが違います。",
				   TITLE,
				   MB_ICONEXCLAMATION);
		return NULL;
	}
	if (wcslen(wszPath) <= wcslen(wszBase) + 1)
	{
		MessageBox(hWndMain, bEnglish ?
				   L"No file chosen." :
				   L"ファイルが選択されませんでした。",
				   TITLE,
				   MB_ICONEXCLAMATION);
		return NULL;
	}

	pSlash = wszPath;
	while ((pSlash = wcsstr(pSlash, L"\\")))
		*pSlash++ = L'/';

	/* 素材ディレクトリ内の相対パスを返す */
	return wszPath + wcslen(wszBase) + 1 ;
}

/* 上書き保存 */
static VOID OnSave(void)
{
	ScriptView_UpdateScriptModelFromText();
	if (!save_script())
	{
		MessageBox(hWndMain, bEnglish ?
				   L"Cannot write to file." :
				   L"ファイルに書き込めません。",
				   TITLE,
				   MB_OK | MB_ICONERROR);
	}

	/* スクリプトの最終更新時刻を取得する */
	ScriptView_UpdateTimeStamp();
}

/* 続ける */
static VOID OnContinue(void)
{
	ScriptView_UpdateScriptModelFromText();
	nLineChanged = ScriptView_GetCursorLine();
	bExecLineChanged = TRUE;
	bContinuePressed = TRUE;
}

/* 次へ */
static VOID OnNext(void)
{
	ScriptView_UpdateScriptModelFromText();
	nLineChanged = ScriptView_GetCursorLine();
	bExecLineChanged = TRUE;
	bNextPressed = TRUE;
}

/* 停止 */
static VOID OnStop(void)
{
	bStopPressed = TRUE;
}

/* 次のエラー箇所へ移動ボタンが押下されたとき */
static VOID OnNextError(void)
{
	int nStart;

	nStart = ScriptView_GetCursorPosition();
	if (ScriptView_SearchNextError(nStart, -1))
		return;

	if (ScriptView_SearchNextError(0, nStart - 1))
		return;

	MessageBox(hWndMain, bEnglish ?
			   L"No error.\n" :
			   L"エラーはありません。\n",
			   TITLE,
			   MB_ICONINFORMATION | MB_OK);
}

/* リッチエディットでのShift+Enterを処理する */
static VOID OnShiftEnter(void)
{
	int nCursorLine;

	/* スクリプトモデルを更新する */
	ScriptView_UpdateScriptModelFromText();

	/* パースエラーがないとき */
	if (dbg_get_parse_error_count() == 0)
	{
		/* 次フレームでの一行実行する */
		nCursorLine = ScriptView_GetCursorLine();
		if (nCursorLine != -1)
		{
			bNextPressed = TRUE;
			nLineChanged = nCursorLine;
			bExecLineChanged = TRUE;
		}
	}

	/* 全体のテキスト色を変更する(遅延) */
	ScriptView_DelayedHighligth();
}

/* リッチエディットでのTabを処理する */
static VOID OnTab(void)
{
	wchar_t szCmd[1024];
	wchar_t *pWcs, *pCRLF;
	int i, nLine, nTotal, nLineStartCharCR, nLineStartCharCRLF;

	memset(szCmd, 0, sizeof(szCmd));

	nLine = ScriptView_GetCursorLine();
	pWcs = ScriptView_GetText();
	nTotal = (int)wcslen(pWcs);
	nLineStartCharCR = 0;
	nLineStartCharCRLF = 0;
	for (i = 0; i <= nLine; i++)
	{
		if (nLineStartCharCRLF >= nTotal)
			break;

		pCRLF = wcswcs(pWcs + nLineStartCharCRLF, L"\r\n");
		if (pCRLF != NULL)
		{
			int nLen = (int)(pCRLF - (pWcs + nLineStartCharCRLF));
			if (i == nLine)
			{
				wcsncpy(szCmd, pWcs + nLineStartCharCRLF, (size_t)nLen);
				break;
			}
			nLineStartCharCRLF += nLen + 2; /* +2 for CRLF */
			nLineStartCharCR += nLen + 1; /* +1 for CR */
		}
		else
		{
			int nLen = (int)wcslen(pWcs + nLineStartCharCRLF);
			if (i == nLine)
			{
				wcscpy(szCmd, pWcs + nLineStartCharCRLF);
				break;
			}
			nLineStartCharCRLF += nLen + 2; /* +2 for CRLF */
			nLineStartCharCR += nLen + 1; /* +1 for CR */
		}
	}
	free(pWcs);

    for (i = 0; i < (int)(sizeof(completion_item) / sizeof(struct completion_item)); i++)
	{
		if (wcscmp(completion_item[i].prefix, szCmd) == 0)
		{
			ScriptView_InsertTextAtEnd(completion_item[i].insert);
			return;
		}
	}

	ScriptView_InsertTextAtEnd(L"    ");
}

/* ポップアップを表示する */
static VOID OnPopup(void)
{
	POINT point;

	GetCursorPos(&point);
	TrackPopupMenu(hMenuPopup, 0, point.x, point.y, 0, hWndMain, NULL);
}

/* 変数の書き込みボタンが押下された場合を処理する */
static VOID OnWriteVars(void)
{
	static wchar_t szTextboxVar[VAR_TEXTBOX_MAX];
	wchar_t *p, *next_line;
	int index, val;

	/* テキストボックスの内容を取得する */
	GetWindowText(hWndTextboxVar, szTextboxVar, sizeof(szTextboxVar) / sizeof(wchar_t) - 1);

	/* パースする */
	p = szTextboxVar;
	while(*p)
	{
		/* 空行を読み飛ばす */
		if(*p == '\n')
		{
			p++;
			continue;
		}

		/* 次の行の開始文字を探す */
		next_line = p;
		while(*next_line)
		{
			if(*next_line == '\r')
			{
				*next_line = '\0';
				next_line++;
				break;
			}
			next_line++;
		}

		/* パースする */
		if(swscanf(p, L"$%d=%d", &index, &val) != 2)
			index = -1, val = -1;
		if(index >= LOCAL_VAR_SIZE + GLOBAL_VAR_SIZE)
			index = -1;

		/* 変数を設定する */
		if(index != -1)
			set_variable(index, val);

		/* 次の行へポインタを進める */
		p = next_line;
	}

	Variable_UpdateText();
}

/* パッケージを作成メニューが押下されたときの処理を行う */
static VOID OnExportPackage(void)
{
	if (MessageBox(hWndMain, bEnglish ?
				   L"Are you sure you want to export the package file?\n"
				   L"This may take a while." :
				   L"パッケージをエクスポートします。\n"
				   L"この処理には時間がかかります。\n"
				   L"よろしいですか？",
				   TITLE,
				   MB_ICONWARNING | MB_OKCANCEL) != IDOK)
		return;

	/* パッケージを作成する */
	if (create_package("")) {
		log_info(bEnglish ?
				 "Successfully exported data01.arc" :
				 "package.pakのエクスポートに成功しました。");
	}
}

/* Windows向けにエクスポートのメニューが押下されたときの処理を行う */
static VOID OnExportWin(void)
{
	if (MessageBox(hWndMain, bEnglish ?
				   L"The process may take some time. Do you wish to proceed?\n" :
				   L"エクスポートには時間がかかります。よろしいですか？",
				   TITLE,
				   MB_ICONWARNING | MB_OKCANCEL) != IDOK)
		return;

	/* パッケージを作成する */
	if (!create_package(""))
	{
		log_info(bEnglish ?
				 "Failed to export package.pack" :
				 "package.pakのエクスポートに失敗しました。");
		return;
	}

	/* フォルダを再作成する */
	RecreateDirectory(L".\\windows-export");

	/* ファイルをコピーする */
	if (!CopyLibraryFiles(L"engine.exe", L".\\windows-export\\engine.exe"))
	{
		log_info(bEnglish ?
				 "Failed to copy exe file." :
				 "実行ファイルのコピーに失敗しました。");
		return;
	}

	/* movをコピーする */
	CopyMovFiles(L".\\video\\*.wmv", L".\\windows-export\\video");

	/* パッケージを移動する */
	if (!MovePackageFile(L".\\package.pak", L".\\windows-export\\package.pak"))
	{
		log_info(bEnglish ?
				 "Failed to move package.pak" :
				 "package.pakの移動に失敗しました。");
		return;
	}

	if (MessageBox(hWndMain,
				   bEnglish ?
				   L"Export succeeded. Will open the folder." :
				   L"エクスポートに成功しました。ゲームを実行しますか？",
				   TITLE,
				   MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		/* engine.exeを実行する */
		RunWindowsGame();
	}
	else
	{
		/* Explorerを開く */
		ShellExecuteW(NULL, L"explore", L".\\windows-export", NULL, NULL, SW_SHOW);
	}
}

/* エクスポートされたWindowsゲームを実行する */
static VOID RunWindowsGame(void)
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	/* プロセスを実行する */
	ZeroMemory(&si, sizeof(STARTUPINFOW));
	si.cb = sizeof(STARTUPINFOW);
	CreateProcessW(L".\\windows-export\\engine.exe",	/* lpApplication */
				   NULL,	/* lpCommandLine */
				   NULL,	/* lpProcessAttribute */
				   NULL,	/* lpThreadAttributes */
				   FALSE,	/* bInheritHandles */
				   NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
				   NULL,	/* lpEnvironment */
				   L".\\windows-export",
				   &si,
				   &pi);
	if (pi.hProcess != NULL)
		CloseHandle(pi.hThread);
	if (pi.hProcess != NULL)
		CloseHandle(pi.hProcess);
}

/* Mac向けにエクスポートのメニューが押下されたときの処理を行う */
static VOID OnExportMac(void)
{
	if (MessageBox(hWndMain, bEnglish ?
				   L"The process may take some time. Do you wish to proceed?\n" :
				   L"エクスポートには時間がかかります。よろしいですか？",
				   TITLE,
				   MB_ICONWARNING | MB_OKCANCEL) != IDOK)
		return;

	/* パッケージを作成する */
	if (!create_package(""))
	{
		log_info(bEnglish ?
				 "Failed to export package.pak" :
				 "package.pakのエクスポートに失敗しました。");
		return;
	}

	/* フォルダを再作成する */
	RecreateDirectory(L".\\macos-export");

#if 0
	if (MessageBox(hWndMain,
				   bEnglish ?
				   L"Do you want to export an prebuilt app?." :
				   L"ビルド済みアプリを出力しますか？",
				   TITLE,
				   MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		/* アプリをコピーする */
		if (!CopyLibraryFiles(L"tools\\game-mac.dmg", L".\\macos-export\\game-mac.dmg"))
		{
			log_info(bEnglish ?
					 "Failed to copy source files for Android." :
					 "アプリのコピーに失敗しました。"
					 "最新のtoolsフォルダが存在するか確認してください。");
			return;
		}

		/* パッケージを移動する */
		if (!MovePackageFile(L".\\package.pak", L".\\macos-export\\package.pak"))
		{
			log_info(bEnglish ?
					 "Failed to move package.pak" :
					 "package.pakの移動に失敗しました。");
			return;
		}

		MessageBox(hWndMain, bEnglish ?
				   L"Will open the exported app folder.\n" :
				   L"エクスポートしたアプリフォルダを開きます。\n",
				   TITLE,
				   MB_ICONINFORMATION | MB_OK);

		/* Explorerを開く */
		ShellExecuteW(NULL, L"explore", L".\\macos-export", NULL, NULL, SW_SHOW);
		return;
	}
#endif

	/* ソースツリーをコピーする */
	if (!CopyLibraryFiles(L"tools\\macos-src", L".\\macos-export"))
	{
		log_info(bEnglish ?
				 "Failed to copy source files for Android." :
				 "ソースコードのコピーに失敗しました。"
				 "最新のtools/macos-srcフォルダが存在するか確認してください。");
		return;
	}

	/* パッケージを移動する */
	if (!MovePackageFile(L".\\package.pak", L".\\macos-export\\Resources\\package.pak"))
	{
		log_info(bEnglish ?
				 "Failed to move package.pak" :
				 "package.pakの移動に失敗しました。");
		return;
	}

	MessageBox(hWndMain, bEnglish ?
			   L"Will open the exported source code folder.\n"
			   L"Build with Xcode." :
			   L"エクスポートしたソースコードフォルダを開きます。\n"
			   L"Xcodeでそのままビルドできます。\n",
			   TITLE,
			   MB_ICONINFORMATION | MB_OK);

	/* Explorerを開く */
	ShellExecuteW(NULL, L"explore", L".\\macos-export", NULL, NULL, SW_SHOW);
}

/* Web向けにエクスポートのメニューが押下されたときの処理を行う */
static VOID OnExportWeb(void)
{
	if (MessageBox(hWndMain, bEnglish ?
				   L"The process may take some time. Do you wish to proceed?\n" :
				   L"エクスポートには時間がかかります。よろしいですか？",
				   TITLE,
				   MB_ICONWARNING | MB_OKCANCEL) != IDOK)
		return;

	/* パッケージを作成する */
	if (!create_package(""))
	{
		log_info(bEnglish ?
				 "Failed to export package.pak" :
				 "package.pakのエクスポートに失敗しました。");
		return;
	}

	/* フォルダを再作成する */
	RecreateDirectory(L".\\web-export");

	/* ソースをコピーする */
	if (!CopyLibraryFiles(L"tools\\wasm-src\\*", L".\\web-export"))
	{
		log_info(bEnglish ?
				 "Failed to copy source files for Web." :
				 "ソースコードのコピーに失敗しました。"
				 "最新のtools/webフォルダが存在するか確認してください。");
		return;
	}

	/* movをコピーする */
	CopyMovFiles(L".\\video\\*.mp4", L".\\web-export\\video");

	/* パッケージを移動する */
	if (!MovePackageFile(L".\\package.pak", L".\\web-export\\package.pak"))
	{
		log_info(bEnglish ?
				 "Failed to move package.pak" :
				 "package.pakの移動に失敗しました。");
		return;
	}

	if (MessageBox(hWndMain,
				   bEnglish ?
				   L"Export succeeded. Do you want to run the game on a browser?." :
				   L"エクスポートに成功しました。ブラウザで開きますか？",
				   TITLE,
				   MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		/* web-test.exeを実行する */
		RunWebTest();
	}
	else
	{
		/* Explorerを開く */
		ShellExecuteW(NULL, L"explore", L".\\web-export", NULL, NULL, SW_SHOW);
	}
}

/* web-test.exeを実行する */
static VOID RunWebTest(void)
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	wchar_t szPath[MAX_PATH];
	wchar_t *pSep;

	/* web-test.exeのパスを求める */
	GetModuleFileName(NULL, szPath, MAX_PATH);
	pSep = wcsrchr(szPath, L'\\');
	if (pSep != NULL)
		*(pSep + 1) = L'\0';
	wcscat(szPath, L"tools\\web-test.exe");

	/* プロセスを実行する */
	ZeroMemory(&si, sizeof(STARTUPINFOW));
	si.cb = sizeof(STARTUPINFOW);
	CreateProcessW(szPath,	/* lpApplication */
				   NULL,	/* lpCommandLine */
				   NULL,	/* lpProcessAttribute */
				   NULL,	/* lpThreadAttributes */
				   FALSE,	/* bInheritHandles */
				   NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
				   NULL,	/* lpEnvironment */
				   L".\\web-export",
				   &si,
				   &pi);
	if (pi.hProcess != NULL)
		CloseHandle(pi.hThread);
	if (pi.hProcess != NULL)
		CloseHandle(pi.hProcess);
}

/* Androidプロジェクトをエクスポートのメニューが押下されたときの処理を行う */
static VOID OnExportAndroid(void)
{
	if (MessageBox(hWndMain, bEnglish ?
				   L"The process may take some time. Do you wish to proceed?\n" :
				   L"エクスポートには時間がかかります。よろしいですか？",
				   TITLE,
				   MB_ICONWARNING | MB_OKCANCEL) != IDOK)
		return;

	/* フォルダを再作成する */
	RecreateDirectory(L".\\android-export");

	/*
	 * ソースをコピーする
	 *  - JVMのプロセスが生きているとandroid-exportが削除されないので、末尾の\\*が必要
	 */
	if (!CopyLibraryFiles(L"tools\\android-src\\*", L".\\android-export"))
	{
		log_info(bEnglish ?
				 "Failed to copy source files for Android." :
				 "ソースコードのコピーに失敗しました。"
				 "最新のtools/android-srcフォルダが存在するか確認してください。");
		return;
	}

	/* アセットをコピーする */
	CopyGameFiles(L".\\image", L".\\android-export\\app\\src\\main\\assets\\image");
	CopyGameFiles(L".\\sound", L".\\android-export\\app\\src\\main\\assets\\sound");
	CopyGameFiles(L".\\story", L".\\android-export\\app\\src\\main\\assets\\story");
	CopyGameFiles(L".\\script", L".\\android-export\\app\\src\\main\\assets\\script");
	CopyGameFiles(L".\\font", L".\\android-export\\app\\src\\main\\assets\\font");
	CopyGameFiles(L".\\video", L".\\android-export\\app\\src\\main\\assets\\video");
	CopyGameFiles(L".\\project.txt", L".\\android-export\\app\\src\\main\\assets\\project.txt");

	if (MessageBox(hWndMain, bEnglish ?
				   L"Would you like to build APK file?" :
				   L"APKファイルをビルドしますか？",
				   TITLE,
				   MB_ICONINFORMATION | MB_YESNO) == IDYES)
	{
		/* Explorerを開く */
		ShellExecuteW(NULL, L"explore", L".\\android-export", NULL, NULL, SW_SHOW);

		MessageBox(hWndMain, bEnglish ?
				   L"If the build fails, please move the game folder under C:\\." :
				   L"ビルドがうまくいかない場合、ゲームフォルダをCドライブ直下に移動して試してください。",
				   TITLE,
				   MB_ICONINFORMATION | MB_OK);

		/* バッチファイルを呼び出す */
		RunAndroidBuild();
		return;
	}

	MessageBox(hWndMain, bEnglish ?
			   L"Will open the exported source code folder.\n"
			   L"Build with Android Studio." :
			   L"エクスポートしたソースコードフォルダを開きます。\n"
			   L"Android Studioでそのままビルドできます。",
			   TITLE,
			   MB_ICONINFORMATION | MB_OK);

	/* Explorerを開く */
	ShellExecuteW(NULL, L"explore", L".\\android-export", NULL, NULL, SW_SHOW);
}

/* build.batを実行する */
static VOID RunAndroidBuild(void)
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	wchar_t cmdline[1024];

	/* コマンドライン文字列はCreateProcessW()に書き換えられるので、バッファにコピーしておく */
	wcscpy(cmdline, L"cmd.exe /k build.bat");

	/* プロセスを実行する */
	ZeroMemory(&si, sizeof(STARTUPINFOW));
	si.cb = sizeof(STARTUPINFOW);
	CreateProcessW(NULL,	/* lpApplication */
				   cmdline,	/* lpCommandLine */
				   NULL,	/* lpProcessAttribute */
				   NULL,	/* lpThreadAttributes */
				   FALSE,	/* bInheritHandles */
				   NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
				   NULL,	/* lpEnvironment */
				   L".\\android-export",
				   &si,
				   &pi);
	if (pi.hProcess != NULL)
		CloseHandle(pi.hThread);
	if (pi.hProcess != NULL)
		CloseHandle(pi.hProcess);
}

/* iOSプロジェクトをエクスポートのメニューが押下されたときの処理を行う */
static VOID OnExportIOS(void)
{
	if (MessageBox(hWndMain, bEnglish ?
				   L"The process may take some time. Do you wish to proceed?\n" :
				   L"エクスポートには時間がかかります。よろしいですか？",
				   TITLE,
				   MB_ICONWARNING | MB_OKCANCEL) != IDOK)
		return;

	/* パッケージを作成する */
	if (!create_package(""))
	{
		log_info(bEnglish ?
				 "Failed to export package.pak" :
				 "package.pakのエクスポートに失敗しました。");
		return;
	}

	/* フォルダを再作成する */
	RecreateDirectory(L".\\ios-export");

	/* ソースをコピーする */
	if (!CopyLibraryFiles(L"tools\\ios-src", L".\\ios-export"))
	{
		log_info(bEnglish ?
				 "Failed to copy source files for Android." :
				 "ソースコードのコピーに失敗しました。"
				 "最新のtools/ios-srcフォルダが存在するか確認してください。");
		return;
	}

	/* パッケージを移動する */
	if (!MovePackageFile(L".\\package.pak", L".\\ios-export\\Resources\\package.pak"))
	{
		log_info(bEnglish ?
				 "Failed to move package.pak" :
				 "package.pakの移動に失敗しました。");
		return;
	}

	/* movをコピーする */
	CopyMovFiles(L".\\video\\*.mp4", L".\\ios-export\\Resources\\mov\\");

	MessageBox(hWndMain, bEnglish ?
			   L"Will open the exported source code folder.\n"
			   L"Build with Xcode." :
			   L"エクスポートしたソースコードフォルダを開きます。\n"
			   L"Xcodeでそのままビルドできます。\n",
			   TITLE,
			   MB_ICONINFORMATION | MB_OK);

	/* Explorerを開く */
	ShellExecuteW(NULL, L"explore", L".\\ios-export", NULL, NULL, SW_SHOW);
}

/* Unityプロジェクトをエクスポートのメニューが押下されたときの処理を行う */
static VOID OnExportUnity(void)
{
	if (MessageBox(hWndMain, bEnglish ?
				   L"The process may take some time. Do you wish to proceed?\n" :
				   L"エクスポートには時間がかかります。よろしいですか？",
				   TITLE,
				   MB_ICONWARNING | MB_OKCANCEL) != IDOK)
		return;

	/* フォルダを再作成する */
	RecreateDirectory(L".\\unity-export");

	/* ソースをコピーする */
	if (!CopyLibraryFiles(L"tools\\unity-src", L".\\unity-export"))
	{
		log_info(bEnglish ?
				 "Failed to copy source files for Unity." :
				 "ソースコードのコピーに失敗しました。"
				 "最新のtools/unity-srcフォルダが存在するか確認してください。");
		return;
	}

	/* アセットをコピーする */
	CopyGameFiles(L".\\image", L".\\unity-export\\Assets\\StreamingAssets\\image");
	CopyGameFiles(L".\\sound", L".\\unity-export\\Assets\\StreamingAssets\\sound");
	CopyGameFiles(L".\\story", L".\\unity-export\\Assets\\StreamingAssets\\story");
	CopyGameFiles(L".\\script", L".\\unity-export\\Assets\\StreamingAssets\\script");
	CopyGameFiles(L".\\font", L".\\unity-export\\Assets\\StreamingAssets\\font");
	CopyGameFiles(L".\\video", L".\\unity-export\\Assets\\StreamingAssets\\video");
	CopyGameFiles(L".\\project.txt", L".\\unity-export\\Assets\\StreamingAssets\\project.txt");

	MessageBox(hWndMain, bEnglish ?
			   L"Will open the exported source code folder.\n"
			   L"Build with Unity." :
			   L"エクスポートしたソースコードフォルダを開きます。\n"
			   L"Unityでビルドしてください。",
			   TITLE,
			   MB_ICONINFORMATION | MB_OK);

	/* Explorerを開く */
	ShellExecuteW(NULL, L"explore", L".\\unity-export", NULL, NULL, SW_SHOW);
}

/* フォルダを再作成する */
static VOID RecreateDirectory(const wchar_t *path)
{
	wchar_t newpath[MAX_PATH];
	SHFILEOPSTRUCT fos;
	const int WINDOWS_XP = 5;

	/* 二重のNUL終端を行う */
	wcscpy(newpath, path);
	newpath[wcslen(path) + 1] = L'\0';

	/*
	 * On Windows XP, SHFileOperationW() with a relative path makes 8.3 format copy.
	 * We can avoid it by an absolute path. However, on Windows 10/11,
	 * an absolute path raises a security dialog, and we have to avoid it.
	 */
	if ((DWORD)(LOBYTE(LOWORD(GetVersion()))) == WINDOWS_XP)
	{
		GetCurrentDirectory(sizeof(newpath) / sizeof(wchar_t), newpath);
		wcscat(newpath, L"\\");
		wcscat(newpath, path);
		newpath[wcslen(newpath) + 1] = L'\0';	/* Double-NUL termination needed. */
	}

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
	const int WINDOWS_XP = 5;

	/* コピー元を求める */
	GetModuleFileName(NULL, from, MAX_PATH);
	pSep = wcsrchr(from, L'\\');
	if (pSep != NULL)
		*(pSep + 1) = L'\0';
	wcscat(from, lpszSrcDir);
	from[wcslen(from) + 1] = L'\0';	/* 二重のNUL終端を行う */

	/* コピー先を求める */
	wcscpy(to, lpszDestDir);
	to[wcslen(lpszDestDir) + 1] = L'\0';	/* Double-NUL termination needed. */

	/*
	 * On Windows XP, SHFileOperationW() with a relative path makes 8.3 format copy.
	 * We can avoid it by an absolute path. However, on Windows 10/11,
	 * an absolute path raises a security dialog, and we have to avoid it.
	 */
	if ((DWORD)(LOBYTE(LOWORD(GetVersion()))) == WINDOWS_XP)
	{
		GetCurrentDirectory(sizeof(to) / sizeof(wchar_t), to);
		wcscat(to, L"\\");
		wcscat(to, lpszDestDir);
		to[wcslen(to) + 1] = L'\0';	/* Double-NUL termination needed. */
	}

	/* コピーする */
	ZeroMemory(&fos, sizeof(SHFILEOPSTRUCT));
	fos.wFunc = FO_COPY;
	fos.pFrom = from;
	fos.pTo = to;
	fos.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	ret = SHFileOperationW(&fos);
	if (ret != 0)
	{
		log_info("%s: error code = %d", conv_utf16_to_utf8(lpszSrcDir), ret);
		return FALSE;
	}

	return TRUE;
}

/* ゲームファイルをコピーする (ゲーム内 to エクスポート先) */
static BOOL CopyGameFiles(const wchar_t* lpszSrcDir, const wchar_t* lpszDestDir)
{
	wchar_t from[MAX_PATH];
	wchar_t to[MAX_PATH];
	SHFILEOPSTRUCTW fos;
	int ret;
	const int WINDOWS_XP = 5;

	/* 二重のNUL終端を行う */
	wcscpy(from, lpszSrcDir);
	from[wcslen(lpszSrcDir) + 1] = L'\0';
	wcscpy(to, lpszDestDir);
	to[wcslen(lpszDestDir) + 1] = L'\0';

	/*
	 * On Windows XP, SHFileOperationW() with a relative path makes 8.3 format copy.
	 * We can avoid it by an absolute path. However, on Windows 10/11,
	 * an absolute path raises a security dialog, and we have to avoid it.
	 */
	if ((DWORD)(LOBYTE(LOWORD(GetVersion()))) == WINDOWS_XP)
	{
		GetCurrentDirectory(sizeof(to) / sizeof(wchar_t), to);
		wcscat(to, L"\\");
		wcscat(to, lpszDestDir);
		to[wcslen(to) + 1] = L'\0';	/* Double-NUL termination needed. */
	}

	/* コピーする */
	ZeroMemory(&fos, sizeof(SHFILEOPSTRUCT));
	fos.wFunc = FO_COPY;
	fos.pFrom = from;
	fos.pTo = to;
	fos.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	ret = SHFileOperationW(&fos);
	if (ret != 0)
	{
		log_info("%s: error code = %d", conv_utf16_to_utf8(lpszSrcDir), ret);
		return FALSE;
	}

	return TRUE;
}

/* movをコピーする */
static BOOL CopyMovFiles(const wchar_t *lpszSrcDir, const wchar_t *lpszDestDir)
{
	wchar_t from[MAX_PATH];
	wchar_t to[MAX_PATH];
	SHFILEOPSTRUCTW fos;
	const int WINDOWS_XP = 5;

	/* 二重のNUL終端を行う */
	wcscpy(from, lpszSrcDir);
	from[wcslen(lpszSrcDir) + 1] = L'\0';
	wcscpy(to, lpszDestDir);
	to[wcslen(lpszDestDir) + 1] = L'\0';

	/* On Windows XP, a relative path makes 8.3 format copy. Avoid it by a absolute path. */
	if ((DWORD)(LOBYTE(LOWORD(GetVersion()))) == WINDOWS_XP)
	{
		GetCurrentDirectory(sizeof(to) / sizeof(wchar_t), to);
		wcscat(to, L"\\");
		wcscat(to, lpszDestDir);
		to[wcslen(to) + 1] = L'\0';	/* Double-NUL termination needed. */
	}

	/* コピーする */
	ZeroMemory(&fos, sizeof(SHFILEOPSTRUCT));
	fos.wFunc = FO_COPY;
	fos.pFrom = from;
	fos.pTo = to;
	fos.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI |
		FOF_SILENT;
	SHFileOperationW(&fos);

	return TRUE;
}

/* パッケージファイルを移動する */
static BOOL MovePackageFile(const wchar_t *lpszPkgFile, wchar_t *lpszDestDir)
{
	wchar_t from[MAX_PATH];
	wchar_t to[MAX_PATH];
	SHFILEOPSTRUCTW fos;
	int ret;
	const int WINDOWS_XP = 5;

	/* 二重のNUL終端を行う */
	wcscpy(from, lpszPkgFile);
	from[wcslen(lpszPkgFile) + 1] = L'\0';
	wcscpy(to, lpszDestDir);
	to[wcslen(lpszDestDir) + 1] = L'\0';

	/* On Windows XP, a relative path makes 8.3 format copy. Avoid it by a absolute path. */
	if ((DWORD)(LOBYTE(LOWORD(GetVersion()))) == WINDOWS_XP)
	{
		GetCurrentDirectory(sizeof(to) / sizeof(wchar_t), to);
		wcscat(to, L"\\");
		wcscat(to, lpszDestDir);
		to[wcslen(to) + 1] = L'\0';	/* Double-NUL termination needed. */
	}

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
		log_info("error code = %d", ret);
		return FALSE;
	}

	return TRUE;
}

/*
 * View
 */

static VOID OnFont(void)
{
	CHOOSEFONT cf;
	LOGFONT lf;
	HDC hDC;

	ZeroMemory(&cf, sizeof(cf));
	cf.lStructSize = sizeof(cf);
	cf.hwndOwner = hWndMain;
	cf.lpLogFont = &lf;
	cf.Flags = CF_NOVERTFONTS;
	if (!ChooseFont(&cf))
		return;

	hDC = GetDC(NULL);
	ScriptView_SetFontSize(-MulDiv(lf.lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY)));
	ReleaseDC(NULL, hDC);

	ScriptView_SetFontName(lf.lfFaceName);

	if (bProjectOpened)
		WriteProjectFile();

	ScriptView_UpdateScriptModelFromText();
	ScriptView_ApplyFont();
}

static VOID OnHighlightMode(void)
{
	if (!ScriptView_IsHighlightMode())
	{
		CheckMenuItem(hMenu, ID_HIGHLIGHTMODE, MF_CHECKED);
		ScriptView_SetHighlightMode(TRUE);
	}
	else
	{
		CheckMenuItem(hMenu, ID_HIGHLIGHTMODE, MF_UNCHECKED);
		ScriptView_SetHighlightMode(FALSE);
	}

	if (bProjectOpened)
		WriteProjectFile();
}

static VOID OnDarkMode(void)
{
	if (!ScriptView_IsDarkMode())
	{
		CheckMenuItem(hMenu, ID_DARKMODE, MF_CHECKED);
		ScriptView_SetDarkMode(TRUE);
	}
	else
	{
		CheckMenuItem(hMenu, ID_DARKMODE, MF_UNCHECKED);
		ScriptView_SetDarkMode(FALSE);
	}

	if (bProjectOpened)
		WriteProjectFile();
}

static VOID OnVersion(void)
{
	MessageBox(hWndMain, conv_utf8_to_utf16(VERSION_STRING), TITLE, MB_OK | MB_ICONINFORMATION);
}

static VOID OnDocument(void)
{
	wchar_t wszURL[1024];
	wchar_t wszPath[1024];
	wchar_t *pSep;

	/* Get the installed directory. */
	GetModuleFileName(NULL, wszPath, MAX_PATH);
	pSep = wcsrchr(wszPath, L'\\');
	if (pSep != NULL)
		*pSep = L'\0';
	while ((pSep = wcschr(wszPath, L'\\')))
		   *pSep = L'/';

	/* Open */
	wcscpy(wszURL, L"file:///");
	wcscat(wszURL, wszPath);
	wcscat(wszURL, bEnglish ?
		   L"/manual/html_en/index.html" :
		   L"/manual/html_ja/index.html");
	ShellExecuteW(NULL, L"open", wszURL, NULL, NULL, SW_SHOWNORMAL);
}

/*
 * コマンドの挿入
 */

static VOID OnInsertMessage(void)
{
	if (bEnglish)
		ScriptView_InsertText(L"Edit this message.");
	else
		ScriptView_InsertText(L"この行のメッセージを編集してください。");

	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertSerif(void)
{
	if (bEnglish)
		ScriptView_InsertText(L"*Name*Edit this line and press return.");
	else
		ScriptView_InsertText(L"名前「このセリフを編集してください。」");

	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertBg(void)
{
	const wchar_t *pFile;

	pFile = SelectFile(BG_DIR);
	if (pFile == NULL)
		return;

	ScriptView_InsertText(L"@bg file=%ls t=1.0", pFile);
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertBgOnly(void)
{
	const wchar_t *pFile;

	pFile = SelectFile(BG_DIR);
	if (pFile == NULL)
		return;

	ScriptView_InsertText(L"@chch bg=%ls t=1.0", pFile);
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertCh(void)
{
	const wchar_t *pFile;

	pFile = SelectFile(CH_DIR);
	if (pFile == NULL)
		return;

	ScriptView_InsertText(L"@ch pos=center file=%ls t=1.0", pFile);
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertChch(void)
{
	ScriptView_InsertText(L"@chch t=1.0");
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertMusic(void)
{
	const wchar_t *pFile;

	pFile = SelectFile(BGM_DIR);
	if (pFile == NULL)
		return;

	ScriptView_InsertText(L"@music file=%ls", pFile);
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertSound(void)
{
	const wchar_t *pFile;

	pFile = SelectFile(SE_DIR);
	if (pFile == NULL)
		return;

	ScriptView_InsertText(L"@sound file=%ls", pFile);
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertVolume(void)
{
	ScriptView_InsertText(L"@volume track=bgm vol=1.0 t=1.0");
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertVideo(void)
{
	wchar_t buf[1024], *pExt;
	const wchar_t *pFile;

	pFile = SelectFile(MOV_DIR);
	if (pFile == NULL)
		return;

	/* Remove the file extension. */
	wcscpy(buf, pFile);
	pExt = wcswcs(buf, L".mp4");
	if (pExt == NULL)
		pExt = wcswcs(buf, L".wmv");
	if (pExt != NULL)
		*pExt = L'\0';

	ScriptView_InsertText(L"@video file=%ls", buf);
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertShake(void)
{
	ScriptView_InsertText(L"@shake dir=horizontal t=1.0 count=3 amp=100");
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertChoose(void)
{
	if (bEnglish)
		ScriptView_InsertText(L"@choose L1 \"Option1\" L2 \"Option2\" L3 \"Option3\"");
	else
		ScriptView_InsertText(L"@choose L1 \"選択肢1\" L2 \"選択肢2\" L3 \"選択肢3\"");

	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertMenu(void)
{
	const wchar_t *pFile;

	pFile = SelectFile(GUI_DIR);
	if (pFile == NULL)
		return;

	ScriptView_InsertText(L"@menu file=%ls", pFile);
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertClick(void)
{
	ScriptView_InsertText(L"@click");
	ScriptView_UpdateScriptModelFromText();
}

static VOID OnInsertTime(void)
{
	ScriptView_InsertText(L"@time t=1.0");
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

static VOID OnInsertStory(void)
{
	const wchar_t *pFile;

	pFile = SelectFile(SCENARIO_DIR);
	if (pFile == NULL)
		return;

	ScriptView_InsertText(L"@story file=%ls", pFile);
	ScriptView_UpdateScriptModelFromText();
	OnProperty();
}

/*
 * Property
 */

/*
 * Called when the Edit button pressed.
 */
VOID OnProperty(void)
{
	RECT rcRichEdit, rcPanel;
	int nLine, nCmdIndex, nCmdType, nDialogID;
	UINT nID;
	DLGPROC pDlgProc;

	if (hWndDlgProp != NULL)
		return;

	/* Apply the current RichEdit content to the script model. */
	ScriptView_UpdateScriptModelFromText();

	/* Get the current cursor line number. */
	nLine = ScriptView_GetCursorLine();

	/* Get the command. */
	nCmdIndex = get_command_index_from_line_num(nLine);
	move_to_command_index(nCmdIndex);
	nCmdType = get_command_type();

	/* Convert a command type to a dialog id. */
	switch (nCmdType)
	{
	case COMMAND_MESSAGE:
	case COMMAND_SERIF:
		nDialogID = bEnglish ? IDD_MESSAGE_EN : IDD_MESSAGE;
		pDlgProc = DlgMessageWndProc;
		break;
	case COMMAND_BG:
		nDialogID = bEnglish ? IDD_BG_EN : IDD_BG;
		pDlgProc = DlgBgWndProc;
		break;
	case COMMAND_CH:
		nDialogID = bEnglish ? IDD_CH_EN : IDD_CH;
		pDlgProc = DlgChWndProc;
		break;
	case COMMAND_CHCH:
		nDialogID = bEnglish ? IDD_CHCH_EN : IDD_CHCH;
		pDlgProc = DlgChchWndProc;
		break;
	case COMMAND_MUSIC:
		nDialogID = bEnglish ? IDD_MUSIC_EN : IDD_MUSIC;
		pDlgProc = DlgMusicWndProc;
		break;
	case COMMAND_SOUND:
		nDialogID = bEnglish ? IDD_SOUND_EN : IDD_SOUND;
		pDlgProc = DlgSoundWndProc;
		break;
	case COMMAND_VOLUME:
		nDialogID = bEnglish ? IDD_VOLUME_EN : IDD_VOLUME;
		pDlgProc = DlgVolumeWndProc;
		break;
	case COMMAND_VIDEO:
		nDialogID = bEnglish ? IDD_VIDEO_EN : IDD_VIDEO;
		pDlgProc = DlgVideoWndProc;
		break;
	case COMMAND_SHAKE:
		nDialogID = bEnglish ? IDD_SHAKE_EN : IDD_SHAKE;
		pDlgProc = DlgShakeWndProc;
		break;
	case COMMAND_CHOOSE:
		nDialogID = bEnglish ? IDD_CHOOSE_EN : IDD_CHOOSE;
		pDlgProc = DlgChooseWndProc;
		break;
	case COMMAND_MENU:
		nDialogID = bEnglish ? IDD_MENU_EN : IDD_MENU;
		pDlgProc = DlgMenuWndProc;
		break;
	case COMMAND_TIME:
		nDialogID = bEnglish ? IDD_TIME_EN : IDD_TIME;
		pDlgProc = DlgTimeWndProc;
		break;
	case COMMAND_STORY:
		nDialogID = bEnglish ? IDD_STORY_EN : IDD_STORY;
		pDlgProc = DlgStoryWndProc;
		break;
	default:
		/* Not implemented yet. */
		MessageBox(hWndMain,
				   bEnglish ?
				   L"Not implemented for this command." :
				   L"このコマンドの編集はまだ実装されていません。",
				   TITLE,
				   MB_OK | MB_ICONINFORMATION);
		return;
	}

	/* Create a dialog as a child window. */
	assert(hWndDlgProp == NULL);
	hWndDlgProp = CreateDialog(NULL,
							   MAKEINTRESOURCE(nDialogID),
							   hWndEditor,
							   (DLGPROC)pDlgProc);

	/* Resize the dialog to fit the editor panel. */
	GetWindowRect(hWndRichEdit, &rcRichEdit);
	GetWindowRect(hWndEditor, &rcPanel);
	MoveWindow(hWndDlgProp,
			   rcRichEdit.left - rcPanel.left ,
			   rcRichEdit.top - rcPanel.top,
			   rcRichEdit.right - rcPanel.left,
			   rcRichEdit.bottom - rcPanel.top,
			   TRUE);

	/* Disable buttons. */
	EnableWindow(hWndBtnContinue, FALSE);
	EnableWindow(hWndBtnNext, FALSE);
	EnableWindow(hWndBtnStop, FALSE);
	EnableWindow(hWndBtnEdit, FALSE);

	/* Disable menu items. */
	EnableMenuItem(hMenu, ID_CONTINUE, MF_GRAYED);
	EnableMenuItem(hMenu, ID_NEXT, MF_GRAYED);
	EnableMenuItem(hMenu, ID_STOP, MF_GRAYED);
	EnableMenuItem(hMenu, ID_PROPERTY, MF_GRAYED);
	for (nID = ID_CMD_MESSAGE; nID <= ID_CMD_STORY; nID++)
		EnableMenuItem(hMenu, nID, MF_GRAYED);

	/* Hide the RichEdit and the variable editor, then show the dialog. */
	ShowWindow(hWndRichEdit, SW_HIDE);
	ShowWindow(hWndTextboxVar, SW_HIDE);
	ShowWindow(hWndBtnVar, SW_HIDE);
	ShowWindow(hWndDlgProp, SW_SHOW);

	/* Save the line in edit. */
	nLineProperty = nLine;
}

/*
 * Called when the property panel is finished.
 */
void OnPropertyUpdate(void)
{
	UINT nID;

	/* Close the dialog. */
	EndDialog(hWndDlgProp, 0);
	hWndDlgProp = NULL;

	/* Enable buttons. */
	EnableWindow(hWndBtnContinue, TRUE);
	EnableWindow(hWndBtnNext, TRUE);
	EnableWindow(hWndBtnStop, FALSE);
	EnableWindow(hWndBtnEdit, TRUE);

	/* Enable menu items. */
	EnableMenuItem(hMenu, ID_CONTINUE, MF_ENABLED);
	EnableMenuItem(hMenu, ID_NEXT, MF_ENABLED);
	EnableMenuItem(hMenu, ID_STOP, MF_ENABLED);
	EnableMenuItem(hMenu, ID_PROPERTY, MF_ENABLED);
	for (nID = ID_CMD_MESSAGE; nID <= ID_CMD_STORY; nID++)
		EnableMenuItem(hMenu, nID, MF_ENABLED);

	/* Show controls again. */
	ShowWindow(hWndRichEdit, SW_SHOW);
	ShowWindow(hWndTextboxVar, SW_SHOW);
	ShowWindow(hWndBtnVar, SW_SHOW);

	/* Update RichEdit by the script model. */
	ScriptView_SetTextByScriptModel();
	ScriptView_DelayedHighligth();

	/* Change the execution line. */
	bExecLineChanged = TRUE;
	nLineChanged = nLineProperty;
}

/*
 * Internationalized UI messages.
 */

/* False assertion */
#define INVALID_UI_MSG_ID	(0)

/* Integer locale ID */
enum language_code {
	LOCALE_EN,
	LOCALE_FR,
	LOCALE_DE,
	LOCALE_ES,
	LOCALE_IT,
	LOCALE_EL,
	LOCALE_RU,
	LOCALE_ZH,
	LOCALE_TW,
	LOCALE_JA,
	LOCALE_OTHER,
};

/* Get a UI message. */
static const char *get_ui_message(int id)
{
	int locale_id;

	locale_id = get_locale_id();

	switch (id) {
	case UIMSG_YES:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("YES");
		case LOCALE_JA:
			return U8("はい");
		case LOCALE_ZH:
			return U8("是");
		case LOCALE_TW:
			return U8("是");
		case LOCALE_FR:
			return U8("Oui");
		case LOCALE_RU:
			return U8("Да");
		case LOCALE_DE:
			return U8("Ja");
		case LOCALE_IT:
			return U8("Sì");
		case LOCALE_ES:
			return U8("Sí");
		case LOCALE_EL:
			return U8("Ναί");
		default:
			return U8("Yes");
		}
		break;
	case UIMSG_NO:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("No");
		case LOCALE_JA:
			return U8("いいえ");
		case LOCALE_ZH:
			return U8("不是");
		case LOCALE_TW:
			return U8("不是");
		case LOCALE_FR:
			return U8("Non");
		case LOCALE_RU:
			return U8("Нет");
		case LOCALE_DE:
			return U8("Nein");
		case LOCALE_IT:
			return U8("No");
		case LOCALE_ES:
			return U8("No");
		case LOCALE_EL:
			return U8("Οχι");
		default:
			return U8("No");
		}
		break;
	case UIMSG_INFO:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Information");
		case LOCALE_JA:
			return U8("情報");
		case LOCALE_ZH:
			return U8("信息");
		case LOCALE_TW:
			return U8("信息");
		case LOCALE_FR:
			return U8("Informations");
		case LOCALE_RU:
			return U8("Информация");
		case LOCALE_DE:
			return U8("Information");
		case LOCALE_IT:
			return U8("Informazione");
		case LOCALE_ES:
			return U8("Información");
		case LOCALE_EL:
			return U8("Πληροφορίες");
		default:
			return U8("Information");
		}
		break;
	case UIMSG_WARN:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Warning");
		case LOCALE_JA:
			return U8("警告");
		case LOCALE_ZH:
			return U8("警告");
		case LOCALE_TW:
			return U8("警告");
		case LOCALE_FR:
			return U8("Attention");
		case LOCALE_RU:
			return U8("Предупреждение");
		case LOCALE_DE:
			return U8("Warnung");
		case LOCALE_IT:
			return U8("Avvertimento");
		case LOCALE_ES:
			return U8("Advertencia");
		case LOCALE_EL:
			return U8("Προειδοποίηση");
		default:
			return U8("Warning");
		}
		break;
	case UIMSG_ERROR:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Error");
		case LOCALE_JA:
			return U8("エラー");
		case LOCALE_ZH:
			return U8("错误");
		case LOCALE_TW:
			return U8("錯誤");
		case LOCALE_FR:
			return U8("Erreur");
		case LOCALE_RU:
			return U8("Ошибка");
		case LOCALE_DE:
			return U8("Fehler");
		case LOCALE_IT:
			return U8("Errore");
		case LOCALE_ES:
			return U8("Error");
		case LOCALE_EL:
			return U8("Λάθος");
		default:
			return U8("Error");
		}
		break;
	case UIMSG_CANNOT_OPEN_LOG:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Cannot open log file.");
		case LOCALE_JA:
			return U8("ログファイルをオープンできません。");
		case LOCALE_ZH:
			return U8("无法打开日志文件。");
		case LOCALE_TW:
			return U8("無法打開日誌文件。");
		case LOCALE_FR:
			return U8("Impossible d'ouvrir le fichier journal.");
		case LOCALE_RU:
			return U8("Не удаётся открыть файл журнала.");
		case LOCALE_DE:
			return U8("Protokolldatei kann nicht geöffnet werden.");
		case LOCALE_IT:
			return U8("Impossibile aprire il file di registro.");
		case LOCALE_ES:
			return U8("No se puede abrir el archivo de registro.");
		case LOCALE_EL:
			return U8("Δεν είναι δυνατό το άνοιγμα του αρχείου καταγραφής.");
		default:
			return U8("Cannot open log file.");
		}
		break;
	case UIMSG_EXIT:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Are you sure you want to quit?");
		case LOCALE_JA:
			return U8("ゲームを終了しますか？");
		case LOCALE_ZH:
			return U8("游戏结束了吗？");
		case LOCALE_TW:
			return U8("遊戲結束了嗎？");
		case LOCALE_FR:
			return U8("Quitter le jeu?");
		case LOCALE_RU:
			return U8("Вы уверены, что хотите выйти?");
		case LOCALE_DE:
			return U8("Sind Sie sicher, dass Sie aufhören wollen?");
		case LOCALE_IT:
			return U8("Sei sicuro di voler uscire?");
		case LOCALE_ES:
			return U8("¿Seguro que quieres salir?");
		case LOCALE_EL:
			return U8("Είσαι σίγουρος ότι θέλεις να παραιτηθείς?");
		default:
			return U8("Are you sure you want to quit?");
		}
		break;
	case UIMSG_TITLE:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Are you sure you want to go to title?");
		case LOCALE_JA:
			return U8("タイトルに戻りますか？");
		case LOCALE_ZH:
			return U8("回到标题？");
		case LOCALE_TW:
			return U8("回到標題？");
		case LOCALE_FR:
			return U8("Retour au titre?");
		case LOCALE_RU:
			return U8("Вы уверены, что хотите вернуться к началу игры?");
		case LOCALE_DE:
			return U8("Sind Sie sicher, dass Sie zum Titel wechseln möchten?");
		case LOCALE_IT:
			return U8("Sei sicuro di voler andare al titolo?");
		case LOCALE_ES:
			return U8("¿Seguro que quieres ir al título?");
		case LOCALE_EL:
			return U8("Είστε σίγουροι ότι θέλετε να πάτε στον τίτλο;");
		default:
			return U8("Are you sure you want to go to title?");
		}
		break;
	case UIMSG_DELETE:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Are you sure you want to delete the save data?");
		case LOCALE_JA:
			return U8("削除してもよろしいですか？");
		case LOCALE_ZH:
			return U8("删除确定要删除吗？");
		case LOCALE_TW:
			return U8("刪除確定要刪除嗎？");
		case LOCALE_FR:
			return U8("Supprimer Voulez-vous vraiment?");
		case LOCALE_RU:
			return U8("Вы уверены, что хотите удалить сохранённые данные?");
		case LOCALE_DE:
			return U8("Möchten Sie die Speicherdaten wirklich löschen?");
		case LOCALE_IT:
			return U8("Sei sicuro di voler eliminare i dati di salvataggio?");
		case LOCALE_ES:
			return U8("¿Está seguro de que desea eliminar los datos guardados?");
		case LOCALE_EL:
			return U8("Είστε βέβαιοι ότι θέλετε να διαγράψετε τα δεδομένα αποθήκευσης;");
		default:
			return U8("Are you sure you want to delete the save data?");
		}
		break;
	case UIMSG_OVERWRITE:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Are you sure you want to overwrite the save data?");
		case LOCALE_JA:
			return U8("上書きしてもよろしいですか？");
		case LOCALE_ZH:
			return U8("您确定要覆盖吗？");
		case LOCALE_TW:
			return U8("您確定要覆蓋嗎？");
		case LOCALE_FR:
			return U8("Voulez-vous vraiment écraser?");
		case LOCALE_RU:
			return U8("Вы уверены, что хотите перезаписать сохранённые данные?");
		case LOCALE_DE:
			return U8("Möchten Sie die Speicherdaten wirklich überschreiben?");
		case LOCALE_IT:
			return U8("Sei sicuro di voler sovrascrivere i dati di salvataggio?");
		case LOCALE_ES:
			return U8("¿Está seguro de que desea sobrescribir los datos guardados?");
		case LOCALE_EL:
			return U8("Είστε βέβαιοι ότι θέλετε να αντικαταστήσετε τα δεδομένα αποθήκευσης;");
		default:
			return U8("Are you sure you want to overwrite the save data?");
		}
		break;
	case UIMSG_DEFAULT:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Are you sure you want to reset the settings?");
		case LOCALE_JA:
			return U8("設定をリセットしてもよろしいですか？");
		case LOCALE_ZH:
			return U8("您确定要重置设置吗？");
		case LOCALE_TW:
			return U8("您確定要重置設置嗎？");
		case LOCALE_FR:
			return U8("Voulez-vous vraiment réinitialiser les paramètres?");
		case LOCALE_RU:
			return U8("Вы уверены, что хотите сбросить настройки?");
		case LOCALE_DE:
			return U8("Möchten Sie die Einstellungen wirklich zurücksetzen?");
		case LOCALE_IT:
			return U8("Sei sicuro di voler ripristinare le impostazioni?");
		case LOCALE_ES:
			return U8("¿Está seguro de que desea restablecer la configuración?");
		case LOCALE_EL:
			return U8("Είστε βέβαιοι ότι θέλετε να επαναφέρετε τις ρυθμίσεις;");
		default:
			return U8("Are you sure you want to reset the settings?");
		}
		break;
	case UIMSG_NO_SOUND_DEVICE:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("No sound output device.");
		case LOCALE_JA:
			return U8("サウンド出力デバイスがありません。");
		case LOCALE_ZH:
			return U8("No sound output device.");
		case LOCALE_TW:
			return U8("No sound output device.");
		case LOCALE_FR:
			return U8("No sound output device.");
		case LOCALE_RU:
			return U8("No sound output device.");
		case LOCALE_DE:
			return U8("No sound output device.");
		case LOCALE_IT:
			return U8("No sound output device.");
		case LOCALE_ES:
			return U8("No sound output device.");
		case LOCALE_EL:
			return U8("No sound output device.");
		default:
			return U8("No sound output device.");
		}
		break;
	case UIMSG_NO_GAME_FILES:
		switch (locale_id) {
		case LOCALE_JA:
			return U8("ゲームデータがありません。");
		default:
			return U8("No game data.");
		}
		break;
#ifdef OPENNOVEL_TARGET_WIN32
	case UIMSG_WIN32_NO_DIRECT3D:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Direct3D is not supported.");
		case LOCALE_JA:
			return U8("Direct3Dはサポートされません。");
		case LOCALE_ZH:
			return U8("不支持Direct3D。");
		case LOCALE_TW:
			return U8("不支持Direct3D。");
		case LOCALE_FR:
			return U8("Direct3D n'est pas pris en charge.");
		case LOCALE_RU:
			return U8("Direct3D не поддерживается.");
		case LOCALE_DE:
			return U8("Direct3D wird nicht unterstützt.");
		case LOCALE_IT:
			return U8("Direct3D non è supportato.");
		case LOCALE_ES:
			return U8("Direct3D no es compatible.");
		case LOCALE_EL:
			return U8("Το Direct3D δεν υποστηρίζεται.");
		default:
			return U8("Direct3D is not supported.");
		}
		break;
	case UIMSG_WIN32_SMALL_DISPLAY:
		switch (locale_id) {
		case LOCALE_EN:
			return U8("Display size too small (%d x %d).");
		case LOCALE_JA:
			return U8("ディスプレイのサイズが足りません。(%d x %d)");
		case LOCALE_ZH:
			return U8("显示尺寸不足。(%d x %d)");
		case LOCALE_TW:
			return U8("顯示尺寸不足。(%d x %d)");
		case LOCALE_FR:
			return U8("Taille d'affichage insuffisante. (%d x %d)");
		case LOCALE_RU:
			return U8("Размер экрана слишком маленький. (%d x %d)");
		case LOCALE_DE:
			return U8("Anzeigegrö ÿe zu klein. (%d x %d)");
		case LOCALE_IT:
			return U8("Dimensioni del display troppo piccole. (%d x %d)");
		case LOCALE_ES:
			return U8("Tamaño de la pantalla demasiado pequeño. (%d x %d)");
		case LOCALE_EL:
			return U8("Το μέγεθος της οθόνης είναι πολύ μικρό. (%d x %d)");
		default:
			return U8("Display size too small. (%d x %d)");
		}
		break;
#endif
	}

	/* Never come here. */
	assert(INVALID_UI_MSG_ID);
	return NULL;
}

/* Convert a string locale to an integer locale. */
static int get_locale_id(void)
{
	const char *locale;

	if (conf_game_locale == NULL)
		locale = get_system_locale();
	else
		locale = conf_game_locale;

	if (strcmp(locale, "en") == 0)
		return LOCALE_EN;
	else if (strcmp(locale, "fr") == 0)
		return LOCALE_FR;
	else if (strcmp(locale, "de") == 0)
		return LOCALE_DE;
	else if (strcmp(locale, "es") == 0)
		return LOCALE_ES;
	else if (strcmp(locale, "it") == 0)
		return LOCALE_IT;
	else if (strcmp(locale, "el") == 0)
		return LOCALE_EL;
	else if (strcmp(locale, "ru") == 0)
		return LOCALE_RU;
	else if (strcmp(locale, "zh") == 0)
		return LOCALE_ZH;
	else if (strcmp(locale, "tw") == 0)
		return LOCALE_TW;
	else if (strcmp(locale, "ja") == 0)
		return LOCALE_JA;

	return LOCALE_OTHER;
}
