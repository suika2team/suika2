/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * Scenario Script Model
 */

#ifndef OPENNOVEL_SCRIPT_H
#define OPENNOVEL_SCRIPT_H

#include "types.h"

/* Maximum command number. */
#define SCRIPT_CMD_SIZE	(65536)

/* コマンド構造体 */
struct command;

/* コマンドの種類 */
enum command_type {
	COMMAND_INVALID = 0,	/* Invalid Value */

	COMMAND_LABEL,
	COMMAND_MESSAGE,
	COMMAND_SERIF,
	COMMAND_ANIME,
	COMMAND_BG,
	COMMAND_CH,
	COMMAND_CHAPTER,
	COMMAND_CHCH,
	COMMAND_CHOOSE,
	COMMAND_ICHOOSE,
	COMMAND_MCHOOSE,
	COMMAND_MICHOOSE,
	COMMAND_CLICK,
	COMMAND_CONFIG,
	COMMAND_GOSUB,
	COMMAND_GOTO,
	COMMAND_GOTOLABEL,
	COMMAND_IF,
	COMMAND_UNLESS,
	COMMAND_RETURN,
	COMMAND_STORY,
	COMMAND_SET,
	COMMAND_SHAKE,
	COMMAND_SKIP,
	COMMAND_SOUND,
	COMMAND_TEXT,
	COMMAND_TIME,
	COMMAND_LAYER,
	COMMAND_MENU,
	COMMAND_MOVE,
	COMMAND_MUSIC,
	COMMAND_NULL,
	COMMAND_PLUGIN,
	COMMAND_VIDEO,
	COMMAND_VOLUME,
	COMMAND_CIEL,

	COMMAND_MAX,	/* Invalid Value */
};

/* gosub/anime/wmsのコール引数の数 */
#define CALL_ARGS 9

/* labelコマンドのパラメータ */
enum label_command_param {
	LABEL_PARAM_LABEL = 0,
};

/* メッセージコマンドのパラメータ */
enum message_command_param {
	MESSAGE_PARAM_MESSAGE,
};

/* セリフコマンドのパラメータ */
enum serif_command_param {
	SERIF_PARAM_NAME = 1,
	SERIF_PARAM_VOICE,
	SERIF_PARAM_MESSAGE,
};

/* animeコマンドのパラメータ */
enum anime_command_param {
	ANIME_PARAM_FILE = 1,
	ANIME_PARAM_SPEC,
	ANIME_PARAM_ARG1,
	ANIME_PARAM_ARG2,
	ANIME_PARAM_ARG3,
	ANIME_PARAM_ARG4,
	ANIME_PARAM_ARG5,
	ANIME_PARAM_ARG6,
	ANIME_PARAM_ARG7,
	ANIME_PARAM_ARG8,
	ANIME_PARAM_ARG9,
};

/* bgコマンドのパラメータ */
enum bg_command_param {
	BG_PARAM_FILE = 1,
	BG_PARAM_SPAN,
	BG_PARAM_METHOD,
	BG_PARAM_X,
	BG_PARAM_Y,
};

/* chコマンドのパラメータ */
enum ch_command_param {
	CH_PARAM_POS = 1,
	CH_PARAM_FILE,
	CH_PARAM_SPAN,
	CH_PARAM_METHOD,
	CH_PARAM_OFFSET_X,
	CH_PARAM_OFFSET_Y,
	CH_PARAM_ALPHA,
};

/* chapterコマンドのパラメータ */
enum chapter_command_param {
	CHAPTER_PARAM_NAME = 1,
};

/* chchコマンドのパラメータ */
enum chch_command_param {
	CHCH_PARAM_C = 1,
	CHCH_PARAM_CX,
	CHCH_PARAM_CY,
	CHCH_PARAM_CA,
	CHCH_PARAM_CD,
	CHCH_PARAM_R,
	CHCH_PARAM_RX,
	CHCH_PARAM_RY,
	CHCH_PARAM_RA,
	CHCH_PARAM_RD,
	CHCH_PARAM_RC,
	CHCH_PARAM_RCX,
	CHCH_PARAM_RCY,
	CHCH_PARAM_RCA,
	CHCH_PARAM_RCD,
	CHCH_PARAM_L,
	CHCH_PARAM_LX,
	CHCH_PARAM_LY,
	CHCH_PARAM_LA,
	CHCH_PARAM_LD,
	CHCH_PARAM_LC,
	CHCH_PARAM_LCX,
	CHCH_PARAM_LCY,
	CHCH_PARAM_LCA,
	CHCH_PARAM_LCD,
	CHCH_PARAM_B,
	CHCH_PARAM_BX,
	CHCH_PARAM_BY,
	CHCH_PARAM_BA,
	CHCH_PARAM_BD,
	CHCH_PARAM_BG,
	CHCH_PARAM_BGX,
	CHCH_PARAM_BGY,
	CHCH_PARAM_BGA,
	CHCH_PARAM_SPAN,
	CHCH_PARAM_METHOD,
};

/* choose/ichooseコマンドのパラメータ */
enum choose_command_param {
	CHOOSE_PARAM_LABEL1 = 1,
	CHOOSE_PARAM_TEXT1,
	CHOOSE_PARAM_LABEL2,
	CHOOSE_PARAM_TEXT2,
	CHOOSE_PARAM_LABEL3,
	CHOOSE_PARAM_TEXT3,
	CHOOSE_PARAM_LABEL4,
	CHOOSE_PARAM_TEXT4,
	CHOOSE_PARAM_LABEL5,
	CHOOSE_PARAM_TEXT5,
	CHOOSE_PARAM_LABEL6,
	CHOOSE_PARAM_TEXT6,
	CHOOSE_PARAM_LABEL7,
	CHOOSE_PARAM_TEXT7,
	CHOOSE_PARAM_LABEL8,
	CHOOSE_PARAM_TEXT8,
	CHOOSE_PARAM_LABEL9,
	CHOOSE_PARAM_TEXT9,
	CHOOSE_PARAM_LABEL10,
	CHOOSE_PARAM_TEXT10,
};

/* mchoose/michooseコマンドのパラメータ */
enum mchoose_command_param {
	MCHOOSE_PARAM_LABEL1 = 1,
	MCHOOSE_PARAM_COND1,
	MCHOOSE_PARAM_TEXT1,
	MCHOOSE_PARAM_LABEL2,
	MCHOOSE_PARAM_COND2,
	MCHOOSE_PARAM_TEXT2,
	MCHOOSE_PARAM_LABEL3,
	MCHOOSE_PARAM_COND3,
	MCHOOSE_PARAM_TEXT3,
	MCHOOSE_PARAM_LABEL4,
	MCHOOSE_PARAM_COND4,
	MCHOOSE_PARAM_TEXT4,
	MCHOOSE_PARAM_LABEL5,
	MCHOOSE_PARAM_COND5,
	MCHOOSE_PARAM_TEXT5,
	MCHOOSE_PARAM_LABEL6,
	MCHOOSE_PARAM_COND6,
	MCHOOSE_PARAM_TEXT6,
	MCHOOSE_PARAM_LABEL7,
	MCHOOSE_PARAM_COND7,
	MCHOOSE_PARAM_TEXT7,
	MCHOOSE_PARAM_LABEL8,
	MCHOOSE_PARAM_COND8,
	MCHOOSE_PARAM_TEXT8,
	MCHOOSE_PARAM_LABEL9,
	MCHOOSE_PARAM_COND9,
	MCHOOSE_PARAM_TEXT9,
	MCHOOSE_PARAM_LABEL10,
	MCHOOSE_PARAM_COND10,
	MCHOOSE_PARAM_TEXT10,
};

/* configコマンドのパラメータ */
enum config_command_param {
	CONFIG_PARAM_KEY = 1,
	CONFIG_PARAM_VALUE,
};

/* gosubコマンドのパラメータ */
enum gosub_command_param {
	GOSUB_PARAM_LABEL = 1,
	GOSUB_PARAM_ARG1,
	GOSUB_PARAM_ARG2,
	GOSUB_PARAM_ARG3,
	GOSUB_PARAM_ARG4,
	GOSUB_PARAM_ARG5,
	GOSUB_PARAM_ARG6,
	GOSUB_PARAM_ARG7,
	GOSUB_PARAM_ARG8,
	GOSUB_PARAM_ARG9,
};

/* gotoコマンドのパラメータ */
enum goto_command_param {
	GOTO_PARAM_LABEL = 1,
};

/* gotolabelコマンドのパラメータ */
enum gotolabel_command_param {
	GOTOLABEL_PARAM_LABEL = 1,
	GOTOLABEL_PARAM_GOTO,
};

/* ifコマンドのパラメータ */
enum if_command_param {
	IF_PARAM_LHS = 1,
	IF_PARAM_OP,
	IF_PARAM_RHS,
	IF_PARAM_LABEL,
};

/* unlessコマンドのパラメータ */
enum unless_command_param {
	UNLESS_PARAM_LHS = 1,
	UNLESS_PARAM_OP,
	UNLESS_PARAM_RHS,
	UNLESS_PARAM_LABEL,
	UNLESS_PARAM_FINALLY,
};

/* storyコマンドのパラメータ */
enum story_command_param {
	STORY_PARAM_FILE = 1,
	STORY_PARAM_LABEL,
};

/* setコマンドのパラメータ */
enum set_command_param {
	SET_PARAM_LHS = 1,
	SET_PARAM_OP,
	SET_PARAM_RHS,
};


/* shakeコマンドのパラメータ */
enum shake_command_param {
	SHAKE_PARAM_MOVE = 1,
	SHAKE_PARAM_SPAN,
	SHAKE_PARAM_TIMES,
	SHAKE_PARAM_AMOUNT,
};

/* skipコマンドのパラメータ */
enum skip_command_param {
	SKIP_PARAM_MODE = 1,
};

/* soundコマンドのパラメータ */
enum sound_command_param {
	SOUND_PARAM_FILE = 1,
	SOUND_PARAM_OPTION,
};

/* textコマンドのパラメータ */
enum text_command_param {
	TEXT_PARAM_LAYER = 1,
	TEXT_PARAM_TEXT,
};

/* timeコマンドのパラメータ */
enum time_command_param {
	TIME_PARAM_SPAN = 1,
	TIME_PARAM_OPT,
};

/* layerコマンドのパラメータ */
enum layer_command_param {
	LAYER_PARAM_NAME = 1,
	LAYER_PARAM_FILE,
	LAYER_PARAM_X,
	LAYER_PARAM_Y,
	LAYER_PARAM_A,
};

/* menuコマンドのパラメータ */
enum menu_command_param {
	MENU_PARAM_FILE = 1,
	MENU_PARAM_OPTIONS,
};

/* moveコマンドのパラメータ */
enum move_command_param {
	MOVE_PARAM_POS = 1,
	MOVE_PARAM_SPAN,
	MOVE_PARAM_ACCEL,
	MOVE_PARAM_OFFSET_X,
	MOVE_PARAM_OFFSET_Y,
	MOVE_PARAM_ALPHA,
};

/* musicコマンドのパラメータ */
enum music_command_param {
	MUSIC_PARAM_FILE = 1,
	MUSIC_PARAM_ONCE,
};

/* pluginコマンドのパラメータ */
enum plugin_command_param {
	PLUGIN_PARAM_FILE = 1,
	PLUGIN_PARAM_ARG1,
	PLUGIN_PARAM_ARG2,
	PLUGIN_PARAM_ARG3,
	PLUGIN_PARAM_ARG4,
	PLUGIN_PARAM_ARG5,
	PLUGIN_PARAM_ARG6,
	PLUGIN_PARAM_ARG7,
	PLUGIN_PARAM_ARG8,
	PLUGIN_PARAM_ARG9,
};

/* videoコマンドのパラメータ */
enum video_command_param {
	VIDEO_PARAM_FILE = 1,
	VIDEO_PARAM_OPTIONS,
};

/* volumeコマンドのパラメータ */
enum volume_command_param {
	VOLUME_PARAM_STREAM = 1,
	VOLUME_PARAM_VOL,
	VOLUME_PARAM_SPAN,
};

/* cielコマンドのパラメータ */
enum ciel_command_param {
	CIEL_PARAM_ACTION,	/* start from 0, e.g., "@cl.file" is stored at index 0 */
	CIEL_PARAM_NAME,
	CIEL_PARAM_FILE,
	CIEL_PARAM_ALIGN,
	CIEL_PARAM_VALIGN,
	CIEL_PARAM_XEQUAL,
	CIEL_PARAM_XPLUS,
	CIEL_PARAM_XMINUS,
	CIEL_PARAM_YEQUAL,
	CIEL_PARAM_YPLUS,
	CIEL_PARAM_YMINUS,
	CIEL_PARAM_ALPHA,
	CIEL_PARAM_TIME,
	CIEL_PARAM_EFFECT,
	CIEL_PARAM_DIM,
};

/*
 * 選択肢の個数
 */

#define SWITCH_MAX 10

/*
 * 初期化
 */

/* 初期スクリプトを読み込む */
bool init_script(void);

/* コマンドを破棄する */
void cleanup_script(void);

/*
 * スクリプトとコマンドへの公開アクセス
 */

/* スクリプトをロードする */
bool load_script(const char *fname);

/* スクリプトファイル名を取得する */
const char *get_script_file_name(void);

/* 実行中のコマンドのインデックスを取得する(セーブ用) */
int get_command_index(void);

/* 実行中のコマンドのインデックスを設定する(gosub,ロード用) */
bool move_to_command_index(int index);

/* 次のコマンドに移動する */
bool move_to_next_command(void);

/* ラベルへ移動する */
bool move_to_label(const char *label);

/* ラベルへ移動する(なければfinallyラベルにジャンプする) */
bool move_to_label_finally(const char *label, const char *finally_label);

/* gosubによるリターンポイントを記録する(gosub用) */
void push_return_point(void);

/* gosubによるリターンポイントを記録する(カスタムSYSMENUのgosub用) */
void push_return_point_minus_one(void);

/* gosubによるリターンポイントがGUIであることを記録する(カスタムGUIのgosub用) */
void push_return_gui(const char *gui_file);

/* gosubによるリターンポイントを記録する(SysMenuからのGUIによるgosub-back用) */
void set_deep_return_point(int deep_rp);

/* SysMenu経由のGUIからgosubされてreturnしたか */
const char *get_return_gui(void);

/* gosubによるリターンポイントを取得する(return用) */
int pop_return_point(void);

/* gosubによるリターンポイントの行番号を設定する(ロード用) */
bool set_return_point(int index);

/* gosubによるリターンポイントの行番号を取得する(セーブ用) */
int get_return_point(void);

/* SysMenu経由のGUIからgosubされてreturnしたか */
bool is_return_from_sysmenu_gosub(void);

/* 最後のコマンドであるかを取得する(@goto $SAVE用) */
bool is_final_command(void);

/* コマンドの行番号を取得する(ログ用) */
int get_line_num(void);

/* コマンドの行全体を取得する(ログ用) */
const char *get_line_string(void);

/* コマンドのタイプを取得する */
int get_command_type(void);

/* コマンドのロケール指定を取得する */
const char *get_command_locale(void);

/* 文字列のコマンドパラメータを取得する */
const char *get_string_param(int index);

/* 整数のコマンドパラメータを取得する */
int get_int_param(int index);

/* 浮動小数点数のコマンドパラメータを取得する */
float get_float_param(int index);

/* 行の数を取得する */
int get_line_count(void);

/* コマンドの数を取得する */
int get_command_count(void);

/*
 * For the Editor
 */
#if defined(USE_EDITOR)

/* スタートアップ位置を指定する */
bool set_startup_file_and_line(const char *file, int line);

/* using展開後のコマンドの行番号を取得する(ログ用) */
int get_expanded_line_num(void);

/* 指定した行番号以降の最初のコマンドインデックスを取得する */
int get_command_index_from_line_num(int line);

/* 指定した行番号の行全体を取得する */
const char *get_line_string_at_line_num(int line);

/* エラー時に@コマンドを'!'で始まるメッセージに変換する */
void translate_command_to_message_for_runtime_error(int index);

/* デバッグ用の仮のスクリプトをロードする */
bool load_debug_script(void);

/* スクリプトの行を挿入する */
bool insert_script_line(int line, const char *text);

/* スクリプトの行を更新する */
bool update_script_line(int line, const char *text);

/* スクリプトの行を削除する */
bool delete_script_line(int line);

/* スクリプトモデルを元にスクリプトファイルを保存する */
bool save_script(void);

/* スクリプトの拡張構文を再度パースする */
bool reparse_script_for_structured_syntax(void);

/* コマンド名からコマンドタイプを返す */
int get_command_type_from_name(const char *name);

#endif /* defined(USE_EDITOR) */

#endif
