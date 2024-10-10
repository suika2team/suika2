/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

#include "opennovel.h"

/*
 * chapter command
 */
bool chapter_command(void)
{
	const char *param;

	param = get_string_param(CHAPTER_PARAM_NAME);

	/* 章題を設定する */
	if (!set_chapter_name(param)) {
		log_script_exec_footer();
		return false;
	}

	return move_to_next_command();
}
