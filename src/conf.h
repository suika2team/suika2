/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, The Authors. All rights reserved.
 */

#ifndef OPENNOVEL_CONF_H
#define OPENNOVEL_CONF_H

#include "types.h"
#include "vars.h"
#include "mixer.h"

/*
 * Game Settings
 */

/* Title (for window title and save data name) */
extern char *conf_game_title;

/* Screen width */
extern int conf_game_width;

/* Screen height */
extern int conf_game_height;

/* Novel mode */
extern bool conf_game_novel;

/* Locale */
extern char *conf_game_locale;

/*
 * Font Settings
 */

/* TTF file name */
extern char *conf_font_ttf[4];

/*
 * Message Box
 */

/* Image file name */
extern char *conf_msgbox_image;

/* Position */
extern int conf_msgbox_x;
extern int conf_msgbox_y;

/* Margins */
extern int conf_msgbox_margin_left;
extern int conf_msgbox_margin_top;
extern int conf_msgbox_margin_right;
extern int conf_msgbox_margin_bottom;
extern int conf_msgbox_margin_line;
extern int conf_msgbox_margin_char;

/* Font */
extern int conf_msgbox_font;
extern int conf_msgbox_font_size;
extern int conf_msgbox_font_r;
extern int conf_msgbox_font_g;
extern int conf_msgbox_font_b;
extern int conf_msgbox_font_outline;
extern int conf_msgbox_font_outline_r;
extern int conf_msgbox_font_outline_g;
extern int conf_msgbox_font_outline_b;
extern int conf_msgbox_font_ruby;
extern bool conf_msgbox_font_tategaki;

/* Dimming */
extern bool conf_msgbox_dim;
extern int conf_msgbox_dim_r;
extern int conf_msgbox_dim_g;
extern int conf_msgbox_dim_b;
extern int conf_msgbox_dim_outline_r;
extern int conf_msgbox_dim_outline_g;
extern int conf_msgbox_dim_outline_b;

/* Seen Coloring */
extern bool conf_msgbox_seen;
extern int conf_msgbox_seen_r;
extern int conf_msgbox_seen_g;
extern int conf_msgbox_seen_b;
extern int conf_msgbox_seen_outline;
extern int conf_msgbox_seen_outline_r;
extern int conf_msgbox_seen_outline_g;
extern int conf_msgbox_seen_outline_b;

/* Misc. */
extern bool conf_msgbox_nowait;
extern char *conf_msgbox_history_control;
extern bool conf_msgbox_show_on_ch;
extern bool conf_msgbox_show_on_bg;
extern bool conf_msgbox_show_on_choose;
extern bool conf_msgbox_skip_unseen;

/*
 * Name Box Settings
 */

/* File name */
extern char *conf_namebox_image;

/* Position */
extern int conf_namebox_x;
extern int conf_namebox_y;

/* Margins */
extern int conf_namebox_margin_top;
extern bool conf_namebox_leftify;
extern int conf_namebox_margin_left;

/* Font */
extern int conf_namebox_font;
extern int conf_namebox_font_size;
extern int conf_namebox_font_r;
extern int conf_namebox_font_g;
extern int conf_namebox_font_b;
extern int conf_namebox_font_outline;
extern int conf_namebox_font_outline_r;
extern int conf_namebox_font_outline_g;
extern int conf_namebox_font_outline_b;
extern int conf_namebox_font_ruby;
extern bool conf_namebox_font_tategaki;

/* Misc. */
extern bool conf_namebox_hide;

/*
 * Click Animation
 */

/* Position */
extern int conf_click_x;
extern int conf_click_y;

/* Interval */
extern float conf_click_interval;

/* File names */
extern char *conf_click_image[16];

/* Misc. */
extern bool conf_click_move;

/* Hidden */
extern int click_frames;

/*
 * Choose Box Settings
 */

/* File names */
extern char *conf_choose_bg_image[10];
extern char *conf_choose_fg_image[10];

/* Positions */
extern int conf_choose_x[10];
extern int conf_choose_y[10];

/* Margins */
extern int conf_choose_margin_y;
extern int conf_choose_text_margin_y;

/* Font */
extern int conf_choose_font;
extern int conf_choose_font_size;
extern int conf_choose_font_r;
extern int conf_choose_font_g;
extern int conf_choose_font_b;
extern int conf_choose_font_outline;
extern int conf_choose_font_outline_r;
extern int conf_choose_font_outline_g;
extern int conf_choose_font_outline_b;
extern int conf_choose_font_ruby;
extern bool conf_choose_font_tategaki;

/* Active Color */
extern bool conf_choose_active;
extern int conf_choose_active_r;
extern int conf_choose_active_g;
extern int conf_choose_active_b;
extern int conf_choose_active_outline_r;
extern int conf_choose_active_outline_g;
extern int conf_choose_active_outline_b;

/* Inactive Color */
extern bool conf_choose_inactive;
extern int conf_choose_inactive_r;
extern int conf_choose_inactive_g;
extern int conf_choose_inactive_b;
extern int conf_choose_inactive_outline_r;
extern int conf_choose_inactive_outline_g;
extern int conf_choose_inactive_outline_b;

/* SE */
extern char *conf_choose_change_se;
extern char *conf_choose_click_se;

/* Timed Bomb */
extern float conf_choose_timed;

/* Anime */
extern char *conf_choose_anime_focus[10];
extern char *conf_choose_anime_unfocus[10];

/*
 * Save Data Settings
 */

/* Thumbnail size */
extern int conf_save_data_thumb_width;
extern int conf_save_data_thumb_height;

/* NEW image file name */
extern char *conf_save_data_new_image;

/* 
 * System Button Settings
 */

/* File names */
extern char *conf_sysbtn_idle_image;
extern char *conf_sysbtn_hover_image;

/* Position */
extern int conf_sysbtn_x;
extern int conf_sysbtn_y;

/* SE */
extern char *conf_sysbtn_se;

/* Misc. */
extern int conf_sysbtn_hide;
extern int conf_sysbtn_transition;

/*
 * Auto Mode Settings
 */

/* File name */
extern char *conf_automode_banner_image;

/* Position */
extern int conf_automode_banner_x;
extern int conf_automode_banner_y;

/* SE */
extern char *conf_automode_enter_se;
extern char *conf_automode_leave_se;

/*
 * Skip Mode Settings
 */

/* File name */
extern char *conf_skipmode_banner_image;

/* Position */
extern int conf_skipmode_banner_x;
extern int conf_skipmode_banner_y;

/* SE */
extern char *conf_skipmode_enter_se;
extern char *conf_skipmode_leave_se;

/*
 * Menu Settings
 */

/* Save Font */
extern int conf_menu_save_font;
extern int conf_menu_save_font_size;
extern int conf_menu_save_font_r;
extern int conf_menu_save_font_g;
extern int conf_menu_save_font_b;
extern int conf_menu_save_font_outline;
extern int conf_menu_save_font_outline_r;
extern int conf_menu_save_font_outline_g;
extern int conf_menu_save_font_outline_b;
extern int conf_menu_save_font_ruby;
extern bool conf_menu_save_font_tategaki;

/* History Font */
extern int conf_menu_history_font;
extern int conf_menu_history_font_size;
extern int conf_menu_history_font_r;
extern int conf_menu_history_font_g;
extern int conf_menu_history_font_b;
extern int conf_menu_history_font_outline;
extern int conf_menu_history_font_outline_r;
extern int conf_menu_history_font_outline_g;
extern int conf_menu_history_font_outline_b;
extern int conf_menu_history_font_ruby;
extern bool conf_menu_history_font_tategaki;

/* History Margins */
extern int conf_menu_history_margin_line;

/* Misc. */
extern char *conf_menu_history_quote_prefix;
extern char *conf_menu_history_quote_start;
extern char *conf_menu_history_quote_end;
extern bool conf_menu_history_nolast;
extern bool conf_menu_preview_tategaki;

/*
 * Initial Volumes
 */

/* Default */
extern float conf_sound_vol_bgm;
extern float conf_sound_vol_voice;
extern float conf_sound_vol_se;
extern float conf_sound_vol_character;

/*
 * Character Volume Mapping
 */

/* [0] is unused */
extern char *conf_sound_character_name[CH_VOL_SLOTS];

/*
 * Character
 */

/* Auto Focus */
extern bool conf_character_focus;

/* Eye-blink */
extern float conf_character_eyeblink_interval;
extern float conf_character_eyeblink_frame;

/* Lip-sync */
extern float conf_character_lipsync_frame;
extern int conf_character_lipsync_chars;

/* Character Name to File Mapping */

#define CHARACTER_MAP_COUNT	32

extern char *conf_character_name[CHARACTER_MAP_COUNT];
extern char *conf_character_image[CHARACTER_MAP_COUNT];

/*
 * Stage Margins (adjustments to character layers)
 */

extern int conf_stage_ch_margin_bottom;
extern int conf_stage_ch_margin_left;
extern int conf_stage_ch_margin_right;

/*
 * Variable Aliases
 */

#define NAMED_LOCAL_VAR_COUNT	32
#define NAMED_GLOBAL_VAR_COUNT	32

extern char *conf_local_var_name[NAMED_LOCAL_VAR_COUNT];
extern char *conf_global_var_name[NAMED_GLOBAL_VAR_COUNT];

/*
 * Kirakira Effect
 */

#define KIRAKIRA_FRAME_COUNT	16

extern bool conf_kirakira;
extern bool conf_kirakira_add;
extern float conf_kirakira_frame;
extern char *conf_kirakira_image[KIRAKIRA_FRAME_COUNT];

/*
 * Emoticon
 */

#define EMOTICON_COUNT		(16)

extern char *conf_emoticon_name[EMOTICON_COUNT];
extern char *conf_emoticon_image[EMOTICON_COUNT];

/*
 * Text-to-speech
 */

extern bool conf_tts;

/*
 * Misc.
 */

/* Is release app? */
extern bool conf_release;

/*
 * Private Usage
 */

/* Last saved/loaded page */
extern int conf_menu_save_last_page;

/*
 * Functions
 */

/* Initialize the config subsystem. */
bool init_conf(void);

/* Cleanup the config subsystem. */
void cleanup_conf(void);

/* Initialize various settings by config values. */
bool apply_initial_values(void);

/* Overwrite a config. */
bool overwrite_config(const char *key, const char *val);

/* Get a config key for index. */
const char *get_config_key_for_save_data(int index);

/* Check if config key is stored to global save data. */
bool is_config_key_global(const char *key);

/* Get a config value type. ('s', 'b', 'i', 'f') */
char get_config_type_for_key(const char *key);

/* Get a string config value. */
const char *get_string_config_value_for_key(const char *key);

/* Get a boolean config value. */
bool get_bool_config_value_for_key(const char *key);

/* Get an integer config value. */
int get_int_config_value_for_key(const char *key);

/* Get an integer config value. */
float get_float_config_value_for_key(const char *key);

/* Check the locale. */
bool compare_locale(const char *s);

#endif
