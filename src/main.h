/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * The game loop handler
 */

#ifndef OPENNOVEL_MAIN_H
#define OPENNOVEL_MAIN_H

#include "types.h"

/*
 * 入力の状態
 */
extern bool is_left_button_pressed;
extern bool is_right_button_pressed;
extern bool is_left_clicked;
extern bool is_right_clicked;
extern bool is_return_pressed;
extern bool is_space_pressed;
extern bool is_escape_pressed;
extern bool is_up_pressed;
extern bool is_down_pressed;
extern bool is_left_arrow_pressed;
extern bool is_right_arrow_pressed;
extern bool is_page_up_pressed;
extern bool is_page_down_pressed;
extern bool is_control_pressed;
extern bool is_s_pressed;
extern bool is_l_pressed;
extern bool is_h_pressed;
extern int mouse_pos_x;
extern int mouse_pos_y;
extern bool is_mouse_dragging;
extern bool is_touch_canceled;
extern bool is_swiped;

void clear_input_state(void);

/*
 * ゲームループの中身
 */

void init_game_loop(void);
bool game_loop_iter(void);
void cleanup_game_loop(void);

/*
 * コマンドの実装
 */

bool message_command(bool *cont);
bool anime_command(void);
bool bg_command(void);
bool ch_command(void);
bool chch_command(void);
bool chapter_command(void);
bool choose_command(void);
bool click_command(void);
bool config_command(void);
bool gosub_command(void);
bool goto_command(bool *cont);
bool if_command(void);
bool return_command(bool *cont);
bool story_command(void);
bool set_command(void);
bool shake_command(void);
bool skip_command(void);
bool sound_command(void);
bool text_command(void);
bool time_command(void);
bool layer_command(void);
bool menu_command(void);
bool move_command(void);
bool music_command(void);
bool plugin_command(void);
bool video_command(void);
bool volume_command(void);
bool ciel_command(bool *cont);
bool addmsg_command(void);

/*
 * 複数のイテレーションに渡るコマンドの実行中であるかの設定
 */

void start_command_repetition(void);
void stop_command_repetition(void);
bool is_in_command_repetition(void);

/*
 * メッセージの表示中状態の設定
 *  - メッセージ表示中にシステムGUIに移動した場合、「表示中」状態が保持される
 *  - メッセージコマンドから次のコマンドに進むときにクリアされる
 *  - ロードされてもクリアされる
 */

void set_message_active(void);
void clear_message_active(void);
bool is_message_active(void);

/*
 * オートモードの設定
 */

void start_auto_mode(void);
void stop_auto_mode(void);
bool is_auto_mode(void);

/*
 * スキップモードの設定
 */

void start_skip_mode(void);
void stop_skip_mode(void);
bool is_skip_mode(void);

/*
 * セーブ・ロード画面の許可の設定
 */

void set_save_load(bool enable);
bool is_save_load_enabled(void);

/*
 * 割り込み不可モードの設定
 */

void set_non_interruptible(bool mode);
bool is_non_interruptible(void);

/*
 * ペンの位置(@ichoose用)
 */

void set_pen_position(int x, int y);
int get_pen_position_x(void);
int get_pen_position_y(void);

/*
 * マクロ/WMS/アニメ引数
 */

bool set_call_argument(int index, const char *val);
const char *get_call_argument(int index);

/*
 * スクリプトのページモード
 */

bool is_page_mode(void);
bool append_buffered_message(const char *msg);
const char *get_buffered_message(void);
void clear_buffered_message(void);
void reset_page_line(void);
void inc_page_line(void);
bool is_page_top(void);

/*
 * Editing/Debugging Support
 */
#ifdef USE_EDITOR

/*
 * Stop execution and update debugger UI elements.
 */
void dbg_stop(void);

/*
 * Return whether a stop request is pending.
 *  - This is for:
 *    - Stopping execution on command moves and script loads in script.c
 *    - Specia message displaying behaviour in cmd_message.c
 */
bool dbg_is_stop_requested(void);

/*
 * Set the runtime error state.
 */
void dbg_raise_runtime_error(void);

/*
 * Reset the error count.
 */
void dbg_reset_parse_error_count(void);

/*
 * Increment the error count.
 */
void dbg_increment_parse_error_count(void);

/*
 * Get the error count.
 */
int dbg_get_parse_error_count(void);

#endif	/* USE_EDITOR */

#endif	/* OPENNOVEL_MAIN_H */
