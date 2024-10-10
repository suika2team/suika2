/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

#include "opennovel.h"

static bool init(void);
static bool cleanup(void);

/*
 * menuコマンド
 */
bool menu_command(void)
{
	/* 最初のフレームの場合 */
	if (!is_in_command_repetition()) {
		/* 初期化を行う */
		if (!init())
			return false;

		/* 最初のフレームの描画を行う */
		if (!run_gui_mode())
			return false;

		/* 次のフレーム以降はmain.cでrun_gui_mode()が呼ばれる */
		return true;
	}

	/*
	 * GUIモードが終了した際にここに到達する
	 *  - タイトルへ戻るが選択された場合、ここに到達しない
	 */
	if (!cleanup())
		return false;

	return true;
}

/* 初期化を行う */
static bool init(void)
{
	const char *file;

	/* GUIファイル名を取得する */
	file = get_string_param(MENU_PARAM_FILE);

	/* セーブに備えてサムネイルを作成する */
	draw_stage_to_thumb();

	/* GUIファイルと指定された画像の読み込みを行う */
	if (!prepare_gui_mode(file, false)) {
		log_script_exec_footer();
		return false;
	}

	if (is_auto_mode()) {
		stop_auto_mode();
		show_automode_banner(false);
	}
	if (is_skip_mode()) {
		stop_skip_mode();
		show_skipmode_banner(false);
	}

	/* 繰り返し処理を開始する */
	start_command_repetition();

	/* 繰り返し処理中のままGUIモードに移行する */
	start_gui_mode();

	return true;
}

/* 終了処理を行う */
static bool cleanup(void)
{
	const char *label;
	bool ret;

	ret = true;

	/* 繰り返し処理を終了する */
	stop_command_repetition();

	/* ラベルジャンプボタンが押下された場合 */
	label = get_gui_result_label();
	if (label != NULL) {
		ret = move_to_label(label);
		cleanup_gui();
		return ret;
	}

	/* 終了ボタンが押下された場合 */
	if (is_gui_result_exit()) {
		cleanup_gui();
		return false;
	}

	/*
	 * キャンセルボタンが押下された場合と、
	 * 右クリックでキャンセルされた場合で、
	 * セーブされていなければ、次のコマンドへ移動する
	 */
	ret = move_to_next_command();

	cleanup_gui();

	return ret;
}
