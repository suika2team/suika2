/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, The Authors. All rights reserved.
 */

/*
 * 選択肢系コマンドの実装
 *  - @choose ... 通常の選択肢
 *  - @ichoose ... 全画面ノベル用のインライン選択肢
 *  - @mchoose ... 条件つき選択肢
 *  - @michoose ... 条件つきインライン選択肢
 */

#include "opennovel.h"

/* false assertion */
#define ASSERT_INVALID_BTN_INDEX (0)

/*
 * ボタンの最大数
 */
#define CHOOSE_COUNT		(10)

/*
 * 引数インデックスの計算
 *  - コマンドの種類によって変わる
 */

/* @chooseのラベルの引数インデックス */
#define CHOOSE_LABEL(n)			(CHOOSE_PARAM_LABEL1 + n * 2)

/* @choose/@ichooseのメッセージの引数インデックス */
#define CHOOSE_MESSAGE(n)		(CHOOSE_PARAM_LABEL1 + n * 2 + 1)

/* @mchoose/@michooseのラベルの引数インデックス */
#define MCHOOSE_LABEL(n)		(MCHOOSE_PARAM_LABEL1 + n * 3)

/* @mchoose/@michooseの変数の引数インデックス */
#define MCHOOSE_VAR(n)			(MCHOOSE_PARAM_LABEL1 + n * 3 + 1)

/* @mchoose/@michooseのメッセージの引数インデックス */
#define MCHOOSE_MESSAGE(n)		(MCHOOSE_PARAM_LABEL1 + n * 3 + 2)

/*
 * 選択肢の項目
 */

/* 選択肢のボタン */
static struct choose_button {
	const char *msg;
	const char *label;
	int x;
	int y;
	int w;
	int h;
	struct image *img_idle;
	struct image *img_hover;
} choose_button[CHOOSE_COUNT];

/*
 * 選択肢の状態
 */

/* ポイントされている項目のインデックス */
static int pointed_index;

/* キー操作によってポイントが変更されたか */
static bool is_selected_by_key;

/* キー操作によってポイントが変更されたときのマウス座標 */
static int save_mouse_pos_x, save_mouse_pos_y;

/* このコマンドを無視するか */
static bool ignore_as_no_options;

/* 最初のフレームでマウスオーバーのSEを回避するフラグ */
static bool is_first_frame;

/*
 * 描画の状態
 */

/* センタリングするか */
static bool is_centered;

/*
 * システム遷移フラグ
 */

/* システムメニューモードに遷移するか */
static bool need_sysmenu_mode;

/*
 * 時間制限
 */
static bool is_timed;
static bool is_bombed;
static uint64_t bomb_sw;

/*
 * 前方参照
 */

/* 主な処理 */
static void pre_process(void);
static bool blit_process(void);
static void render_process(void);
static bool post_process(void);

/* 初期化 */
static bool init(void);
static bool init_choose(void);
static bool init_ichoose(void);
static int init_mchoose(void);
static int init_michoose(void);
static void draw_text(struct image *target, const char *text, int w, int h, bool is_bg);

/* 入力処理 */
static void process_main_input(void);
static int get_pointed_index(void);

/* 描画 */
static void render_frame(void);

/* その他 */
static void play_se(const char *file);
static void run_anime(int unfocus_index, int focus_index);

/* クリーンアップ */
static bool cleanup(void);

/*
 * choose command
 */
bool choose_command(void)
{
	/* 初期化処理を行う */
	if (!is_in_command_repetition())
		if (!init())
			return false;

	/* 選択肢がなかった場合 */
	if (ignore_as_no_options)
		return move_to_next_command();

	pre_process();
	blit_process();
	render_process();
	if (!post_process())
		return false;

	/* 終了処理を行う */
	if (!is_in_command_repetition())
		if (!cleanup())
			return false;

	return true;
}

static void pre_process(void)
{
	if (is_timed) {
		if ((float)get_lap_timer_millisec(&bomb_sw) >= conf_choose_timed * 1000.0f) {
			is_bombed = true;
			return;
		}
	}

	process_main_input();
}

static bool blit_process(void)
{
	int i;

	if (is_bombed)
		return true;

	/*
	 * 必要な場合はステージのサムネイルを作成する
	 *  - クイックセーブされるとき
	 *  - システムGUIに遷移するとき
	 */
	if (need_sysmenu_mode) {
		draw_stage_to_thumb();
		for (i = 0; i < CHOOSE_COUNT; i++) {
			if (choose_button[i].img_idle == NULL)
				continue;
			draw_choose_to_thumb(choose_button[i].img_idle,
					     choose_button[i].x,
					     choose_button[i].y);
		}
	}

	if (need_sysmenu_mode) {
		if (!prepare_gui_mode(SYSMENU_GUI_FILE, true))
			return false;
		start_gui_mode();
	}

	return true;
}

static void render_process(void)
{
	/* GUIへ遷移する場合、メッセージコマンドのレンダリングではなくGUIのレンダリングを行う */
	if (need_sysmenu_mode) {
		run_gui_mode();
		return;
	}

	/* レンダリングを行う */
	render_frame();
}

static bool post_process(void)
{
	/*
	 * 必要な場合は繰り返し動作を停止する
	 *  - 時間制限に達したとき
	 *  - システムGUIに遷移するとき
	 */
	if (is_bombed || need_sysmenu_mode)
		stop_command_repetition();

	is_first_frame = false;

	return true;
}

/*
 * 初期化
 */

/* コマンドの初期化処理を行う */
bool init(void)
{
	int type;

	pointed_index = -1;

	ignore_as_no_options = false;

	is_centered = true;

	need_sysmenu_mode = false;

	is_first_frame = true;

	is_timed = false;
	is_bombed = false;
	reset_lap_timer(&bomb_sw);

#ifdef USE_DLL
	for (int i = 0; i < CHOOSE_COUNT; i++) {
		if (choose_button[i].img_idle != NULL)
			destroy_image(choose_button[i].img_idle);
		if (choose_button[i].img_hover != NULL)
			destroy_image(choose_button[i].img_hover);
		choose_button[i].img_idle = NULL;
		choose_button[i].img_hover = NULL;
	}
#endif

	/* コマンドの種類ごとに初期化を行う */
	type = get_command_type();
	switch (type) {
	case COMMAND_CHOOSE:
		if (!init_choose())
			return false;
		break;
	case COMMAND_ICHOOSE:
		if (!init_ichoose())
			return false;
		break;
	case COMMAND_MCHOOSE:
		switch (init_mchoose()) {
		case -1:
			/* エラー */
			return false;
		case 0:
			/* 表示する項目がない */
			ignore_as_no_options = true;
			return true;
		default:
			/* 表示を行う */
			break;
		}
		break;
	case COMMAND_MICHOOSE:
		switch (init_michoose()) {
		case -1:
			/* エラー */
			return false;
		case 0:
			/* 表示する項目がない */
			ignore_as_no_options = true;
			return true;
		default:
			/* 表示を行う */
			break;
		}
		break;
	default:
		assert(0);
		break;
	}

	start_command_repetition();

	/* 名前ボックス、メッセージボックスを非表示にする */
	if (!conf_msgbox_show_on_choose) {
		show_namebox(false);
		if (type == COMMAND_ICHOOSE || type == COMMAND_MICHOOSE)
			show_msgbox(true);
		else
			show_msgbox(false);
	}
	show_click(false);

	/* オートモードを終了する */
	if (is_auto_mode()) {
		stop_auto_mode();
		show_automode_banner(false);
	}

	/* スキップモードを終了する */
	if (is_skip_mode()) {
		stop_skip_mode();
		show_skipmode_banner(false);
	}

	/* 時間制限設定を行う */
	if (conf_choose_timed > 0)
		is_timed = true;
	else
		is_timed = false;

	/* 連続スワイプによるスキップ動作を無効にする */
	set_continuous_swipe_enabled(false);

	return true;
}

/* @chooseコマンドの初期化を行う */
static bool init_choose(void)
{
	const char *label, *msg;
	int i;

	memset(choose_button, 0, sizeof(choose_button));

	/* 選択肢の情報を取得する */
	for (i = 0; i < CHOOSE_COUNT; i++) {
		/* ラベルを取得する */
		label = get_string_param(CHOOSE_LABEL(i));
		if (strcmp(label, "") == 0)
			break;

		/* メッセージを取得する */
		msg = get_string_param(CHOOSE_MESSAGE(i));
		if (strcmp(msg, "") == 0) {
			log_script_choose_no_message();
			log_script_exec_footer();
			return false;
		}

		/* ボタンの情報を保存する */
		choose_button[i].msg = msg;
		choose_button[i].label = label;

		/* 座標を計算する */
		get_choose_rect(i,
				&choose_button[i].x,
				&choose_button[i].y,
				&choose_button[i].w,
				&choose_button[i].h);

		/* idle画像を作成する */
		choose_button[i].img_idle = create_image(choose_button[i].w, choose_button[i].h);
		if (choose_button[i].img_idle == NULL)
			return false;
		draw_choose_bg_image(choose_button[i].img_idle, i);

		/* hover画像を作成する */
		choose_button[i].img_hover = create_image(choose_button[i].w, choose_button[i].h);
		if (choose_button[i].img_hover == NULL)
			return false;
		draw_choose_fg_image(choose_button[i].img_hover, i);

		/* テキストを描画する */
		draw_text(choose_button[i].img_idle, choose_button[i].msg, choose_button[i].w, choose_button[i].h, true);
		draw_text(choose_button[i].img_hover, choose_button[i].msg, choose_button[i].w, choose_button[i].h, false);
	}

	/* テキスト読み上げする */
	if (conf_tts) {
		speak_text(NULL);
		if (strcmp(get_system_locale(), "ja") == 0)
			speak_text("選択肢が表示されています。左右のキーを押してください。");
		else
			speak_text("Options are displayed. Press the left or right arrow key.");
	}

	return true;
}

/* @ichooseコマンドの引数情報を取得する */
static bool init_ichoose(void)
{
	const char *label, *msg;
	int i, pen_x, pen_y;

	memset(choose_button, 0, sizeof(choose_button));

	is_centered = false;
	if (conf_msgbox_font_tategaki) {
		pen_x = get_pen_position_x() - conf_msgbox_margin_line;
		pen_y = conf_msgbox_y + conf_msgbox_margin_top;
	} else {
		pen_x = conf_msgbox_x + conf_msgbox_margin_left;
		pen_y = get_pen_position_y() + conf_msgbox_margin_line;
	}

	/* 選択肢の情報を取得する */
	for (i = 0; i < CHOOSE_COUNT; i++) {
		/* ラベルを取得する */
		label = get_string_param(CHOOSE_LABEL(i));
		if (strcmp(label, "") == 0)
			break;

		/* メッセージを取得する */
		msg = get_string_param(CHOOSE_MESSAGE(i));
		if (strcmp(msg, "") == 0) {
			log_script_choose_no_message();
			log_script_exec_footer();
			return false;
		}

		/* ボタンの情報を保存する */
		choose_button[i].msg = msg;
		choose_button[i].label = label;

		/* 座標を計算する */
		get_choose_rect(0,
				&choose_button[i].x,
				&choose_button[i].y,
				&choose_button[i].w,
				&choose_button[i].h);
		choose_button[i].x = pen_x;
		choose_button[i].y = pen_y;
		if (conf_msgbox_font_tategaki)
			pen_x -= conf_msgbox_margin_line;
		else
			pen_y += conf_msgbox_margin_line;

		/* idle画像を作成する */
		choose_button[i].img_idle = create_image(choose_button[i].w, choose_button[i].h);
		if (choose_button[i].img_idle == NULL)
			return false;
		draw_choose_bg_image(choose_button[i].img_idle, i);

		/* hover画像を作成する */
		choose_button[i].img_hover = create_image(choose_button[i].w, choose_button[i].h);
		if (choose_button[i].img_hover == NULL)
			return false;
		draw_choose_fg_image(choose_button[i].img_hover, i);

		/* テキストを描画する */
		draw_text(choose_button[i].img_idle, choose_button[i].msg, choose_button[i].w, choose_button[i].h, true);
		draw_text(choose_button[i].img_hover, choose_button[i].msg, choose_button[i].w, choose_button[i].h, false);
	}

	return true;
}

/* @mchooseコマンドの初期化を行う */
static int init_mchoose(void)
{
	const char *var, *label, *msg;
	int i, pos, var_index, var_val;

	memset(choose_button, 0, sizeof(choose_button));

	/* 選択肢の情報を取得する */
	pos = 0;
	for (i = 0; i < CHOOSE_COUNT; i++) {
		/* 変数を取得する */
		var = get_string_param(MCHOOSE_VAR(i));
		if (strcmp(var, "") == 0)
			break;
		if (var[0] != '$' || strlen(var) == 1) {
			log_script_lhs_not_variable(var);
			log_script_exec_footer();
			return -1;
		}
		var_index = atoi(&var[1]);
		var_val = get_variable(var_index);
		if (var_val == 0)
			continue;

		/* ラベルを取得する */
		label = get_string_param(MCHOOSE_LABEL(i));
		if (strcmp(label, "") == 0)
			break;

		/* メッセージを取得する */
		msg = get_string_param(MCHOOSE_MESSAGE(i));
		if (strcmp(msg, "") == 0) {
			log_script_choose_no_message();
			log_script_exec_footer();
			return false;
		}

		/* ボタンの情報を保存する */
		choose_button[pos].msg = msg;
		choose_button[pos].label = label;

		/* 座標を計算する */
		get_choose_rect(pos,
				&choose_button[pos].x,
				&choose_button[pos].y,
				&choose_button[pos].w,
				&choose_button[pos].h);

		/* idle画像を作成する */
		choose_button[pos].img_idle = create_image(choose_button[pos].w, choose_button[pos].h);
		if (choose_button[pos].img_idle == NULL)
			return false;
		draw_choose_bg_image(choose_button[pos].img_idle, pos);

		/* hover画像を作成する */
		choose_button[pos].img_hover = create_image(choose_button[pos].w, choose_button[pos].h);
		if (choose_button[pos].img_hover == NULL)
			return false;
		draw_choose_fg_image(choose_button[pos].img_hover, pos);

		/* テキストを描画する */
		draw_text(choose_button[pos].img_idle, choose_button[pos].msg, choose_button[pos].w, choose_button[pos].h, true);
		draw_text(choose_button[pos].img_hover, choose_button[pos].msg, choose_button[pos].w, choose_button[pos].h, false);

		pos++;
	}

	return pos;
}

/* @michooseコマンドの初期化を行う */
static int init_michoose(void)
{
	const char *var, *label, *msg;
	int i, pen_x, pen_y, pos, var_index, var_val;

	memset(choose_button, 0, sizeof(choose_button));

	is_centered = false;
	if (conf_msgbox_font_tategaki) {
		pen_x = get_pen_position_x() - conf_msgbox_margin_line;
		pen_y = conf_msgbox_y + conf_msgbox_margin_top;
	} else {
		pen_x = conf_msgbox_x + conf_msgbox_margin_left;
		pen_y = get_pen_position_y() + conf_msgbox_margin_line;
	}

	/* 選択肢の情報を取得する */
	pos = 0;
	for (i = 0; i < CHOOSE_COUNT; i++) {
		/* 変数を取得する */
		var = get_string_param(MCHOOSE_VAR(i));
		if (strcmp(var, "") == 0)
			break;
		if (var[0] != '$' || strlen(var) == 1) {
			log_script_lhs_not_variable(var);
			log_script_exec_footer();
			return -1;
		}
		var_index = atoi(&var[1]);
		var_val = get_variable(var_index);
		if (var_val == 0)
			continue;

		/* ラベルを取得する */
		label = get_string_param(MCHOOSE_LABEL(i));
		if (strcmp(label, "") == 0)
			break;

		/* メッセージを取得する */
		msg = get_string_param(MCHOOSE_MESSAGE(i));
		if (strcmp(msg, "") == 0) {
			log_script_choose_no_message();
			log_script_exec_footer();
			return false;
		}

		/* ボタンの情報を保存する */
		choose_button[i].msg = msg;
		choose_button[i].label = label;

		/* 座標を計算する */
		get_choose_rect(0,
				&choose_button[i].x,
				&choose_button[i].y,
				&choose_button[i].w,
				&choose_button[i].h);
		choose_button[i].x = pen_x;
		choose_button[i].y = pen_y;
		if (conf_msgbox_font_tategaki)
			pen_x -= conf_msgbox_margin_line;
		else
			pen_y += conf_msgbox_margin_line;

		/* idle画像を作成する */
		choose_button[i].img_idle = create_image(choose_button[i].w, choose_button[i].h);
		if (choose_button[i].img_idle == NULL)
			return false;
		draw_choose_bg_image(choose_button[i].img_idle, i);

		/* hover画像を作成する */
		choose_button[i].img_hover = create_image(choose_button[i].w, choose_button[i].h);
		if (choose_button[i].img_hover == NULL)
			return false;
		draw_choose_fg_image(choose_button[i].img_hover, i);

		/* テキストを描画する */
		draw_text(choose_button[i].img_idle, choose_button[i].msg, choose_button[i].w, choose_button[i].h, true);
		draw_text(choose_button[i].img_hover, choose_button[i].msg, choose_button[i].w, choose_button[i].h, false);

		pos++;
	}

	return pos;
}

/* 選択肢のテキストを描画する */
static void draw_text(struct image *target, const char *text, int w, int h, bool is_bg)
{
	struct draw_msg_context context;
	pixel_t color, outline_color;
	int char_count;
	int x, y;

	x = 0;
	y = 0;

	/* 色を決める */
	if (is_bg) {
		if (!conf_choose_inactive) {
			color = make_pixel(0xff,
					   (pixel_t)conf_choose_font_r,
					   (pixel_t)conf_choose_font_g,
					   (pixel_t)conf_choose_font_b);
			outline_color = make_pixel(0xff,
						   (pixel_t)conf_choose_font_outline_r,
						   (pixel_t)conf_choose_font_outline_g,
						   (pixel_t)conf_choose_font_outline_b);
		} else {
			color = make_pixel(0xff,
					   (pixel_t)conf_choose_inactive_r,
					   (pixel_t)conf_choose_inactive_g,
					   (pixel_t)conf_choose_inactive_b);
			outline_color = make_pixel(0xff,
						   (pixel_t)conf_choose_inactive_outline_r,
						   (pixel_t)conf_choose_inactive_outline_g,
						   (pixel_t)conf_choose_inactive_outline_b);
		}
	} else {
		if (!conf_choose_active) {
			color = make_pixel(0xff,
					   (pixel_t)conf_choose_font_r,
					   (pixel_t)conf_choose_font_g,
					   (pixel_t)conf_choose_font_b);
			outline_color = make_pixel(0xff,
						   (pixel_t)conf_choose_font_outline_r,
						   (pixel_t)conf_choose_font_outline_g,
						   (pixel_t)conf_choose_font_outline_b);
		} else {
			color = make_pixel(0xff,
					   (pixel_t)conf_choose_active_r,
					   (pixel_t)conf_choose_active_g,
					   (pixel_t)conf_choose_active_b);
			outline_color = make_pixel(0xff,
						   (pixel_t)conf_choose_active_outline_r,
						   (pixel_t)conf_choose_active_outline_g,
						   (pixel_t)conf_choose_active_outline_b);
		}
	}

	/* 描画位置を決める */
	if (is_centered) {
		if (!conf_choose_font_tategaki) {
			x = x + (w - get_string_width(conf_choose_font, conf_choose_font_size, text)) / 2;
			y += conf_choose_text_margin_y;
		} else {
			x = x + (w - conf_choose_font_size) / 2;
			y = y + (h - get_string_height(conf_choose_font, conf_choose_font_size, text)) / 2;
		}
	} else {
		y += conf_choose_text_margin_y;
	}

	/* 文字を描画する */
	construct_draw_msg_context(
		&context,
		-1,
		text,
		conf_choose_font,
		conf_choose_font_size,
		conf_choose_font_size,	/* base_font_size */
		conf_choose_font_ruby,	/* FIXME: namebox.ruby.sizeの導入 */
		conf_choose_font_outline,
		x,
		y,
		conf_game_width,
		conf_game_height,
		x,			/* left_margin */
		0,			/* right_margin */
		conf_choose_text_margin_y,
		0,			/* bottom_margin */
		0,			/* line_margin */
		conf_msgbox_margin_char,
		color,
		outline_color,
		false,			/* is_dimming */
		true,			/* ignore_linefeed */
		false,			/* ignore_font */
		false,			/* ignore_size */
		false,			/* ignore_color */
		false,			/* ignore_size */
		false,			/* ignore_position */
		false,			/* ignore_ruby */
		true,			/* ignore_wait */
		NULL,			/* inline_wait_hook */
		conf_choose_font_tategaki);
	set_alternative_target_image(&context, target);
	char_count = count_chars_common(&context, NULL);
	draw_msg_common(&context, char_count);
}

/*
 * クリック処理
 */

/* システムメニュー非表示中の入力を処理する */
static void process_main_input(void)
{
	int old_pointed_index, new_pointed_index;
	bool enter_sysmenu;

	/* 選択項目を取得する */
	old_pointed_index = pointed_index;
	new_pointed_index = get_pointed_index();

	/* 選択項目が変更され、項目が選択され、TTSが有効で、キー入力で変更された場合 */
	if (new_pointed_index != old_pointed_index &&
	    new_pointed_index != -1 &&
	    conf_tts &&
	    is_selected_by_key &&
	    choose_button[new_pointed_index].msg != NULL) {
		speak_text(NULL);
		speak_text(choose_button[pointed_index].msg);
	}

	/* 選択項目が変更され、項目が選択された場合 */
	if (new_pointed_index != -1 && new_pointed_index != old_pointed_index) {
		/* 最初のフレームは避ける */
		if (!is_first_frame) {
			play_se(is_left_clicked ? conf_choose_click_se : conf_choose_change_se);
			run_anime(old_pointed_index, new_pointed_index);
		}
	}

	/* 選択項目がなくなった場合 */
	if (old_pointed_index != -1 && new_pointed_index == -1)
		run_anime(old_pointed_index, -1);

	/* 選択項目の変化を変数pointed_indexにコミットする */
	pointed_index = new_pointed_index;

	/* マウスの左ボタンでクリックされた場合 */
	if (pointed_index != -1 && (is_left_clicked || is_return_pressed)) {
		play_se(conf_choose_click_se);
		stop_command_repetition();
		run_anime(pointed_index, -1);
	}

	/* システムボタンを使用しない場合 */
	if (conf_sysbtn_hide)
		return;

	/* システムメニューへの遷移を確認していく */
	enter_sysmenu = false;

	/* 右クリックされたとき */
	if (is_right_clicked)
		enter_sysmenu = true;

	/* エスケープキーが押下されたとき */
	if (is_escape_pressed)
		enter_sysmenu = true;

	/* 折りたたみシステムメニューがクリックされたとき */
	if (is_left_clicked && is_sysbtn_pointed())
		enter_sysmenu = true;

	/* システムメニューを開始するとき */
	if (enter_sysmenu) {
		run_anime(pointed_index, -1);
		need_sysmenu_mode = true;
	}
}

/* ポイントされている選択肢を取得する */
static int get_pointed_index(void)
{
	int i;

	/* 右キーを処理する */
	if (is_right_arrow_pressed) {
		is_selected_by_key = true;
		save_mouse_pos_x = mouse_pos_x;
		save_mouse_pos_y = mouse_pos_y;
		if (pointed_index == -1)
			return 0;
		if (pointed_index == CHOOSE_COUNT - 1)
			return 0;
		if (choose_button[pointed_index + 1].msg != NULL)
			return pointed_index + 1;
		else
			return 0;
	}

	/* 左キーを処理する */
	if (is_left_arrow_pressed) {
		is_selected_by_key = true;
		save_mouse_pos_x = mouse_pos_x;
		save_mouse_pos_y = mouse_pos_y;
		if (pointed_index == -1 ||
		    pointed_index == 0) {
			for (i = CHOOSE_COUNT - 1; i >= 0; i--)
				if (choose_button[i].msg != NULL)
					return i;
		}
		return pointed_index - 1;
	}

	/* マウスポイントを処理する */
	for (i = 0; i < CHOOSE_COUNT; i++) {
		if (choose_button[i].msg == NULL)
			continue;

		if (mouse_pos_x >= choose_button[i].x &&
		    mouse_pos_x < choose_button[i].x + choose_button[i].w &&
		    mouse_pos_y >= choose_button[i].y &&
		    mouse_pos_y < choose_button[i].y + choose_button[i].h) {
			/* キーで選択済みの項目があり、マウスが移動していない場合 */
			if (is_selected_by_key &&
			    mouse_pos_x == save_mouse_pos_x &&
			    mouse_pos_y == save_mouse_pos_y)
				continue;
			is_selected_by_key = false;
			return i;
		}
	}

	/* キーによる選択が行われている場合は維持する */
	if (is_selected_by_key)
		return pointed_index;

	/* その他の場合、何も選択しない */
	return -1;
}

/*
 * 描画
 */

/* フレームを描画する */
static void render_frame(void)
{
	int i;

	/* セーブ画面かヒストリ画面から復帰した場合のフラグをクリアする */
	check_gui_flag();

	/* ステージを描画する */
	render_stage();

	/* 選択肢を描画する */
	for (i = 0; i < CHOOSE_COUNT; i++) {
		struct image *img;

		if (i != pointed_index)
			img = choose_button[i].img_idle;
		else
			img = choose_button[i].img_hover;
	
		if (img == NULL)
			continue;

		render_image_normal(choose_button[i].x,
				    choose_button[i].y,
				    img->width,
				    img->height,
				    img,
				    0,
				    0,
				    img->width,
				    img->height,
				    255);
	}

	/* 折りたたみシステムメニューを描画する */
	if (!is_non_interruptible())
		render_sysbtn(false);

	/* システムメニューを表示する */
	if (!is_non_interruptible() && !conf_sysbtn_hide)
		render_sysbtn(is_sysbtn_pointed());
}


/*
 * その他
 */

/* SEを再生する */
static void play_se(const char *file)
{
	struct wave *w;

	if (file == NULL || strcmp(file, "") == 0)
		return;

	w = create_wave_from_file(SE_DIR, file, false);
	if (w == NULL)
		return;

	set_mixer_input(SYS_STREAM, w);
}

/* アニメを実行する */
static void run_anime(int unfocus_index, int focus_index)
{
	/* フォーカスされなくなる項目のアニメ */
	if (unfocus_index != -1 && conf_choose_anime_unfocus[unfocus_index] != NULL)
		load_anime_from_file(conf_choose_anime_unfocus[unfocus_index], -1, NULL);

	/* フォーカスされる項目のアニメ */
	if (focus_index != -1 && conf_choose_anime_focus[focus_index] != NULL)
		load_anime_from_file(conf_choose_anime_focus[focus_index], -1, NULL);
}

/*
 * クリーンアップ
 */

/* コマンドを終了する */
static bool cleanup(void)
{
	int i;

	/* アニメを停止する */
	for (i = 0; i < CHOOSE_COUNT; i++)
		run_anime(i, -1);

	/* 画像を破棄する */
	for (i = 0; i < CHOOSE_COUNT; i++) {
		if (choose_button[i].img_idle != NULL)
			destroy_image(choose_button[i].img_idle);
		if (choose_button[i].img_hover != NULL)
			destroy_image(choose_button[i].img_hover);
		choose_button[i].img_idle = NULL;
		choose_button[i].img_hover = NULL;
	}

	/* システムGUIへの遷移を行う場合 */
	if (need_sysmenu_mode) {
		/* コマンドの移動を行わない */
		return true;
	}

	/* 時間制限に達したとき */
	if (is_bombed) {
		if (!move_to_next_command())
			return false;
		return true;
	}

	/*
	 * 選択肢が選択された場合
	 */
	return move_to_label(choose_button[pointed_index].label);
}
