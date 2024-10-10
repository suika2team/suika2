/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

#include "opennovel.h"

/* ロード画面を呼び出す特殊なラベル */
#define LOAD_LABEL	"$LOAD"

/* セーブ画面を呼び出す特殊なラベル */
#define SAVE_LABEL	"$SAVE"

/*
 * gotoコマンド
 */
bool goto_command(bool *cont)
{
	const char *label;

	/* パラメータからラベルを取得する */
	if (get_command_type() == COMMAND_GOTO)
		label = get_string_param(GOTO_PARAM_LABEL);
	else
		label = get_string_param(GOTOLABEL_PARAM_GOTO);

	/* コマンドを連続実行する */
	*cont = true;

	/* ラベルの次のコマンドへ移動する */
	if (!move_to_label(label))
		return false;

	return true;
}
