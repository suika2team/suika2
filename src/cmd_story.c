/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

#include "opennovel.h"

/*
 * storyコマンド
 */
bool story_command(void)
{
	char *file, *label;

	/* パラメータからファイル名を取得する */
	file = strdup(get_string_param(STORY_PARAM_FILE));
	if (file == NULL) {
		log_memory();
		return false;
	}

	/* パラメータからラベル名を取得する */
	label = strdup(get_string_param(STORY_PARAM_LABEL));
	if (label == NULL) {
		log_memory();
		return false;
	}

	/* 既読フラグをセーブする */
	save_seen();

	/* スクリプトをロードする */
	if (!load_script(file)) {
		free(file);
		return false;
	}

	/* ラベルへジャンプする */
	if (strcmp(label, "") != 0) {
		if (!move_to_label(label)) {
			free(file);
			free(label);
			return false;
		}
	}

	free(file);
	free(label);

	return true;
}
