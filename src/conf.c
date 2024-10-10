/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

#include "opennovel.h"
#include <math.h>	/* NAN */

/* False assertion */
#define CONFIG_KEY_NOT_FOUND	(0)
#define INVALID_CONFIG_TYPE	(0)

/*
 * Game Settings
 */

/* Title (for window title and save data name) */
char *conf_game_title;

/* Screen width */
int conf_game_width;

/* Screen height */
int conf_game_height;

/* Novel mode */
bool conf_game_novel;

/* Locale */
char *conf_game_locale;

/*
 * Font Settings
 */

/* TTF file name */
char *conf_font_ttf[4];

/*
 * Message Box
 */

/* Image file name */
char *conf_msgbox_image;

/* Position */
int conf_msgbox_x;
int conf_msgbox_y;

/* Margins */
int conf_msgbox_margin_left;
int conf_msgbox_margin_top;
int conf_msgbox_margin_right;
int conf_msgbox_margin_bottom;
int conf_msgbox_margin_line;
int conf_msgbox_margin_char;

/* Font */
int conf_msgbox_font;
int conf_msgbox_font_size;
int conf_msgbox_font_r;
int conf_msgbox_font_g;
int conf_msgbox_font_b;
int conf_msgbox_font_outline;
int conf_msgbox_font_outline_r;
int conf_msgbox_font_outline_g;
int conf_msgbox_font_outline_b;
int conf_msgbox_font_ruby;
bool conf_msgbox_font_tategaki;

/* Dimming */
bool conf_msgbox_dim;
int conf_msgbox_dim_r;
int conf_msgbox_dim_g;
int conf_msgbox_dim_b;
int conf_msgbox_dim_outline_r;
int conf_msgbox_dim_outline_g;
int conf_msgbox_dim_outline_b;

/* Seen Coloring */
bool conf_msgbox_seen;
int conf_msgbox_seen_r;
int conf_msgbox_seen_g;
int conf_msgbox_seen_b;
int conf_msgbox_seen_outline_r;
int conf_msgbox_seen_outline_g;
int conf_msgbox_seen_outline_b;

/* Misc. */
bool conf_msgbox_nowait;
char *conf_msgbox_history_control;
bool conf_msgbox_show_on_ch;
bool conf_msgbox_show_on_bg;
bool conf_msgbox_show_on_choose;
bool conf_msgbox_skip_unseen;

/*
 * Name Box Settings
 */

/* File name */
char *conf_namebox_image;

/* Position */
int conf_namebox_x;
int conf_namebox_y;

/* Margins */
int conf_namebox_margin_top;
bool conf_namebox_leftify;
int conf_namebox_margin_left;

/* Font */
int conf_namebox_font;
int conf_namebox_font_size;
int conf_namebox_font_r;
int conf_namebox_font_g;
int conf_namebox_font_b;
int conf_namebox_font_outline;
int conf_namebox_font_outline_r;
int conf_namebox_font_outline_g;
int conf_namebox_font_outline_b;
int conf_namebox_font_ruby;
bool conf_namebox_font_tategaki;

/* Misc. */
bool conf_namebox_hide;

/*
 * Click Animation
 */

/* Position */
int conf_click_x;
int conf_click_y;

/* Interval */
float conf_click_interval;

/* File names */
char *conf_click_image[16];

/* Misc. */
bool conf_click_move;

/* Hidden */
int click_frames;

/*
 * Choose Box Settings
 */

/* File names */
char *conf_choose_bg_image[10];
char *conf_choose_fg_image[10];

/* Positions */
int conf_choose_x[10];
int conf_choose_y[10];

/* Margins */
int conf_choose_margin_y;
int conf_choose_text_margin_y;

/* Font */
int conf_choose_font;
int conf_choose_font_size;
int conf_choose_font_r;
int conf_choose_font_g;
int conf_choose_font_b;
int conf_choose_font_outline;
int conf_choose_font_outline_r;
int conf_choose_font_outline_g;
int conf_choose_font_outline_b;
int conf_choose_font_ruby;
bool conf_choose_font_tategaki;

/* Active Color */
bool conf_choose_active;
int conf_choose_active_r;
int conf_choose_active_g;
int conf_choose_active_b;
int conf_choose_active_outline_r;
int conf_choose_active_outline_g;
int conf_choose_active_outline_b;

/* Inactive Color */
bool conf_choose_inactive;
int conf_choose_inactive_r;
int conf_choose_inactive_g;
int conf_choose_inactive_b;
int conf_choose_inactive_outline_r;
int conf_choose_inactive_outline_g;
int conf_choose_inactive_outline_b;

/* SE */
char *conf_choose_change_se;
char *conf_choose_click_se;

/* Timed Bomb */
float conf_choose_timed;

/* Anime */
char *conf_choose_anime_focus[10];
char *conf_choose_anime_unfocus[10];

/*
 * Save Data Settings
 */

/* Thumbnail size */
int conf_save_data_thumb_width;
int conf_save_data_thumb_height;

/* NEW image file name */
char *conf_save_data_new_image;

/* 
 * System Button Settings
 */

/* File names */
char *conf_sysbtn_idle_image;
char *conf_sysbtn_hover_image;

/* Position */
int conf_sysbtn_x;
int conf_sysbtn_y;

/* SE */
char *conf_sysbtn_se;

/* Misc. */
int conf_sysbtn_hide;
int conf_sysbtn_transition;

/*
 * Auto Mode Settings
 */

/* File name */
char *conf_automode_banner_image;

/* Position */
int conf_automode_banner_x;
int conf_automode_banner_y;

/* SE */
char *conf_automode_enter_se;
char *conf_automode_leave_se;

/*
 * Skip Mode Settings
 */

/* File name */
char *conf_skipmode_banner_image;

/* Position */
int conf_skipmode_banner_x;
int conf_skipmode_banner_y;

/* SE */
char *conf_skipmode_enter_se;
char *conf_skipmode_leave_se;

/*
 * Menu Settings
 */

/* Save Font */
int conf_menu_save_font;
int conf_menu_save_font_size;
int conf_menu_save_font_r;
int conf_menu_save_font_g;
int conf_menu_save_font_b;
int conf_menu_save_font_outline;
int conf_menu_save_font_outline_r;
int conf_menu_save_font_outline_g;
int conf_menu_save_font_outline_b;
int conf_menu_save_font_ruby;
bool conf_menu_save_font_tategaki;

/* History Font */
int conf_menu_history_font;
int conf_menu_history_font_size;
int conf_menu_history_font_r;
int conf_menu_history_font_g;
int conf_menu_history_font_b;
int conf_menu_history_font_outline;
int conf_menu_history_font_outline_r;
int conf_menu_history_font_outline_g;
int conf_menu_history_font_outline_b;
int conf_menu_history_font_ruby;
bool conf_menu_history_font_tategaki;

/* History Margins */
int conf_menu_history_margin_line;

/* Misc. */
char *conf_menu_history_quote_prefix;
char *conf_menu_history_quote_start;
char *conf_menu_history_quote_end;
bool conf_menu_history_nolast;
bool conf_menu_preview_tategaki;

/*
 * Initial Volumes
 */

/* Default */
float conf_sound_vol_bgm;
float conf_sound_vol_voice;
float conf_sound_vol_se;
float conf_sound_vol_character;

/*
 * Character Volume Mapping
 */

/* [0] is unused */
char *conf_sound_character_name[CH_VOL_SLOTS];

/*
 * Character
 */

/* Auto Focus */
bool conf_character_focus;

/* Eye-blink */
float conf_character_eyeblink_interval;
float conf_character_eyeblink_frame;

/* Lip-sync */
float conf_character_lipsync_frame;
int conf_character_lipsync_chars;

/* Character Name to File Mapping */
char *conf_character_name[CHARACTER_MAP_COUNT];
char *conf_character_image[CHARACTER_MAP_COUNT];

/*
 * Stage Margins (adjustments to character layers)
 */

int conf_stage_ch_margin_bottom;
int conf_stage_ch_margin_left;
int conf_stage_ch_margin_right;

/*
 * Variable Aliases
 */

char *conf_local_var_name[NAMED_LOCAL_VAR_COUNT];
char *conf_global_var_name[NAMED_GLOBAL_VAR_COUNT];

/*
 * Kirakira Effect
 */

bool conf_kirakira;
bool conf_kirakira_add;
float conf_kirakira_frame;
char *conf_kirakira_image[KIRAKIRA_FRAME_COUNT];

/*
 * Emoticon
 */

#define EMOTICON_COUNT		(16)

char *conf_emoticon_name[EMOTICON_COUNT];
char *conf_emoticon_image[EMOTICON_COUNT];

/*
 * Text-to-speech
 */

bool conf_tts;

/*
 * Misc.
 */

/* Is release app? */
bool conf_release;

/*
 * Private Usage
 */

/* Last saved/loaded page */
int conf_menu_save_last_page;

/*
 * Line Size
 */
#define BUF_SIZE	(1024)

/*
 * Config Table
 */

#define MUST		true
#define OPTIONAL	false
#define SAVE		true
#define NOSAVE		false

static struct rule {
	/* Type: 's' for string, 'b' for boolean, 'i' for integer, 'f' for float */
	char type;

	/* Key */
	const char *key;

	/* Value */
	void *var_ptr;

	/* Is always-required? */
	bool must;

	/* Is stored to save file? */
	bool save;
} rule_tbl[] = {
	/* Game Info */
	{'s',	"game.title",			&conf_game_title,			MUST,		NOSAVE},
	{'i',	"game.width",			&conf_game_width,			MUST,		NOSAVE},
	{'i',	"game.height",			&conf_game_height,			MUST,		NOSAVE},
	{'b',	"game.novel",			&conf_game_novel,			OPTIONAL,	SAVE},
	{'s',	"game.locale",			&conf_game_locale,			OPTIONAL,	SAVE},

	/* Font */
	{'s',	"font.ttf1",			&conf_font_ttf[0],			MUST,		SAVE},
	{'s',	"font.ttf2",			&conf_font_ttf[1],			OPTIONAL,	NOSAVE},
	{'s',	"font.ttf3",			&conf_font_ttf[2],			OPTIONAL,	NOSAVE},
	{'s',	"font.ttf4",			&conf_font_ttf[3],			OPTIONAL,	NOSAVE},

	/* Message Box */
	{'s',	"msgbox.image",			&conf_msgbox_image,			MUST,		SAVE},
	{'i',	"msgbox.x",			&conf_msgbox_x,				MUST,		SAVE},
	{'i',	"msgbox.y",			&conf_msgbox_y,				MUST,		SAVE},
	{'i',	"msgbox.margin.left",		&conf_msgbox_margin_left,		MUST,		SAVE},
	{'i',	"msgbox.margin.top",		&conf_msgbox_margin_top,		MUST,		SAVE},
	{'i',	"msgbox.margin.right",		&conf_msgbox_margin_right,		MUST,		SAVE},
	{'i',	"msgbox.margin.bottom",		&conf_msgbox_margin_bottom,		MUST,		SAVE},
	{'i',	"msgbox.margin.line",		&conf_msgbox_margin_line,		MUST,		SAVE},
	{'i',	"msgbox.margin.char",		&conf_msgbox_margin_char,		OPTIONAL,	SAVE},
	{'i',	"msgbox.font",			&conf_msgbox_font,			OPTIONAL,	SAVE},
	{'i',	"msgbox.font.size",		&conf_msgbox_font_size,			OPTIONAL,	SAVE},
	{'i',	"msgbox.font.r",		&conf_msgbox_font_r,			OPTIONAL,	SAVE},
	{'i',	"msgbox.font.g",		&conf_msgbox_font_g,			OPTIONAL,	SAVE},
	{'i',	"msgbox.font.b",		&conf_msgbox_font_b,			OPTIONAL,	SAVE},
	{'i',	"msgbox.font.outline",		&conf_msgbox_font_outline,		OPTIONAL,	SAVE},
	{'i',	"msgbox.font.outline.r",	&conf_msgbox_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"msgbox.font.outline.g",	&conf_msgbox_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"msgbox.font.outline.b",	&conf_msgbox_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"msgbox.font.ruby",		&conf_msgbox_font_ruby,			OPTIONAL,	SAVE},
	{'b',	"msgbox.font.tategaki",		&conf_msgbox_font_tategaki,		OPTIONAL,	SAVE},
	{'i',	"msgbox.dim",			&conf_msgbox_dim,			OPTIONAL,	SAVE},
	{'i',	"msgbox.dim.r",			&conf_msgbox_dim_r,			OPTIONAL,	SAVE},
	{'i',	"msgbox.dim.g",			&conf_msgbox_dim_g,			OPTIONAL,	SAVE},
	{'i',	"msgbox.dim.b",			&conf_msgbox_dim_b,			OPTIONAL,	SAVE},
	{'i',	"msgbox.dim.outline.r",		&conf_msgbox_dim_outline_r,		OPTIONAL,	SAVE},
	{'i',	"msgbox.dim.outline.g",		&conf_msgbox_dim_outline_g,		OPTIONAL,	SAVE},
	{'i',	"msgbox.dim.outline.b",		&conf_msgbox_dim_outline_b,		OPTIONAL,	SAVE},
	{'b',	"msgbox.seen",			&conf_msgbox_seen,			OPTIONAL,	SAVE},
	{'i',	"msgbox.seen.r",		&conf_msgbox_seen_r,			OPTIONAL,	SAVE},
	{'i',	"msgbox.seen.g",		&conf_msgbox_seen_g,			OPTIONAL,	SAVE},
	{'i',	"msgbox.seen.b",		&conf_msgbox_seen_b,			OPTIONAL,	SAVE},
	{'i',	"msgbox.seen.outline.r",	&conf_msgbox_seen_outline_r,		OPTIONAL,	SAVE},
	{'i',	"msgbox.seen.outline.g",	&conf_msgbox_seen_outline_g,		OPTIONAL,	SAVE},
	{'i',	"msgbox.seen.outline.b",	&conf_msgbox_seen_outline_b,		OPTIONAL,	SAVE},
	{'b',	"msgbox.nowait",		&conf_msgbox_nowait,			OPTIONAL,	SAVE},
	{'s',	"msgbox.history.control",	&conf_msgbox_history_control,		OPTIONAL,	SAVE},
	{'b',	"msgbox.show.on.ch",		&conf_msgbox_show_on_ch,		OPTIONAL,	SAVE},
	{'b',	"msgbox.show.on.bg",		&conf_msgbox_show_on_bg,		OPTIONAL,	SAVE},
	{'b',	"msgbox.show.on.choose",	&conf_msgbox_show_on_choose,		OPTIONAL,	SAVE},
	{'b',	"msgbox.skip.unseen",		&conf_msgbox_skip_unseen,		OPTIONAL,	SAVE},

	/* Name Box */
	{'s',	"namebox.image",		&conf_namebox_image,			MUST,		SAVE},
	{'i',	"namebox.x",			&conf_namebox_x,			MUST,		SAVE},
	{'i',	"namebox.y", 			&conf_namebox_y,			MUST,		SAVE},
	{'i',	"namebox.margin.top",		&conf_namebox_margin_top,		MUST,		SAVE},
	{'b',	"namebox.leftify",		&conf_namebox_leftify,			OPTIONAL,	SAVE},
	{'i',	"namebox.margin.left",		&conf_namebox_margin_left,		OPTIONAL,	SAVE},
	{'i',	"namebox.font",			&conf_namebox_font,			MUST,		SAVE},
	{'i',	"namebox.font.size",		&conf_namebox_font_size,		MUST,		SAVE},
	{'i',	"namebox.font.r",		&conf_namebox_font_r,			MUST,		SAVE},
	{'i',	"namebox.font.g",		&conf_namebox_font_g,			MUST,		SAVE},
	{'i',	"namebox.font.b",		&conf_namebox_font_b,			MUST,		SAVE},
	{'i',	"namebox.font.outline",		&conf_namebox_font_outline,		OPTIONAL,	SAVE},
	{'i',	"namebox.font.outline.r",	&conf_namebox_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"namebox.font.outline.g",	&conf_namebox_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"namebox.font.outline.b",	&conf_namebox_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"namebox.font.ruby",		&conf_namebox_font_ruby,		OPTIONAL,	SAVE},
	{'b',	"namebox.font.tategaki",	&conf_namebox_font_tategaki,		OPTIONAL,	SAVE},
	{'b',	"namebox.hide",			&conf_namebox_hide,			MUST,		SAVE},

	/* Click Animation */
	{'i',	"click.x",			&conf_click_x,				OPTIONAL,	SAVE},
	{'i',	"click.y",			&conf_click_y,				OPTIONAL,	SAVE},
	{'f',	"click.interval",		&conf_click_interval,			MUST,		SAVE},
	{'s',	"click.image1",			&conf_click_image[0],			MUST,		SAVE},
	{'s',	"click.image2",			&conf_click_image[1],			OPTIONAL,	SAVE},
	{'s',	"click.image3",			&conf_click_image[2],			OPTIONAL,	SAVE},
	{'s',	"click.image4",			&conf_click_image[3],			OPTIONAL,	SAVE},
	{'s',	"click.image5",			&conf_click_image[4],			OPTIONAL,	SAVE},
	{'s',	"click.image6",			&conf_click_image[5],			OPTIONAL,	SAVE},
	{'s',	"click.image7",			&conf_click_image[6],			OPTIONAL,	SAVE},
	{'s',	"click.image8",			&conf_click_image[7],			OPTIONAL,	SAVE},
	{'s',	"click.image9",			&conf_click_image[8],			OPTIONAL,	SAVE},
	{'s',	"click.image10",		&conf_click_image[9],			OPTIONAL,	SAVE},
	{'s',	"click.image11",		&conf_click_image[10],			OPTIONAL,	SAVE},
	{'s',	"click.image12",		&conf_click_image[11],			OPTIONAL,	SAVE},
	{'s',	"click.image13",		&conf_click_image[12],			OPTIONAL,	SAVE},
	{'s',	"click.image14",		&conf_click_image[13],			OPTIONAL,	SAVE},
	{'s',	"click.image15",		&conf_click_image[14],			OPTIONAL,	SAVE},
	{'s',	"click.image16",		&conf_click_image[15],			OPTIONAL,	SAVE},
	{'b',	"click.move",			&conf_click_move,			OPTIONAL,	SAVE},

	/* Choose */
	{'s',	"choose.bg.image1",		&conf_choose_bg_image[0],		MUST,		SAVE},
	{'s',	"choose.fg.image1",		&conf_choose_fg_image[0],		MUST,		SAVE},
	{'i',	"choose.x1",			&conf_choose_x[0],			MUST,		SAVE},
	{'i',	"choose.y1",			&conf_choose_y[0],			MUST,		SAVE},
	{'s',	"choose.bg.image2",		&conf_choose_bg_image[1],		OPTIONAL,	SAVE},
	{'s',	"choose.fg.image2",		&conf_choose_fg_image[1],		OPTIONAL,	SAVE},
	{'i',	"choose.x2",			&conf_choose_x[1],			OPTIONAL,	SAVE},
	{'i',	"choose.y2", 			&conf_choose_y[1],			OPTIONAL,	SAVE},
	{'s',	"choose.bg.image3",		&conf_choose_bg_image[2],		OPTIONAL,	SAVE},
	{'s',	"choose.fg.image3",		&conf_choose_fg_image[2],		OPTIONAL,	SAVE},
	{'i',	"choose.x3",			&conf_choose_x[2],			OPTIONAL,	SAVE},
	{'i',	"choose.y3", 			&conf_choose_y[2],			OPTIONAL,	SAVE},
	{'s',	"choose.bg.image4",		&conf_choose_bg_image[3],		OPTIONAL,	SAVE},
	{'s',	"choose.fg.image4",		&conf_choose_fg_image[3],		OPTIONAL,	SAVE},
	{'i',	"choose.x4",			&conf_choose_x[3],			OPTIONAL,	SAVE},
	{'i',	"choose.y4", 			&conf_choose_y[3],			OPTIONAL,	SAVE},
	{'s',	"choose.bg.image5",		&conf_choose_bg_image[4],		OPTIONAL,	SAVE},
	{'s',	"choose.fg.image5",		&conf_choose_fg_image[4],		OPTIONAL,	SAVE},
	{'i',	"choose.x5",			&conf_choose_x[4],			OPTIONAL,	SAVE},
	{'i',	"choose.y5", 			&conf_choose_y[4],			OPTIONAL,	SAVE},
	{'s',	"choose.bg.image6",		&conf_choose_bg_image[5],		OPTIONAL,	SAVE},
	{'s',	"choose.fg.image6",		&conf_choose_fg_image[5],		OPTIONAL,	SAVE},
	{'i',	"choose.x6",			&conf_choose_x[5],			OPTIONAL,	SAVE},
	{'i',	"choose.y6", 			&conf_choose_y[5],			OPTIONAL,	SAVE},
	{'s',	"choose.bg.image7",		&conf_choose_bg_image[6],		OPTIONAL,	SAVE},
	{'s',	"choose.fg.image7",		&conf_choose_fg_image[6],		OPTIONAL,	SAVE},
	{'i',	"choose.x7",			&conf_choose_x[6],			OPTIONAL,	SAVE},
	{'i',	"choose.y7", 			&conf_choose_y[6],			OPTIONAL,	SAVE},
	{'s',	"choose.bg.image8",		&conf_choose_bg_image[7],		OPTIONAL,	SAVE},
	{'s',	"choose.fg.image8",		&conf_choose_fg_image[7],		OPTIONAL,	SAVE},
	{'i',	"choose.x8",			&conf_choose_x[7],			OPTIONAL,	SAVE},
	{'i',	"choose.y8", 			&conf_choose_y[7],			OPTIONAL,	SAVE},
	{'s',	"choose.bg.image9",		&conf_choose_bg_image[8],		OPTIONAL,	SAVE},
	{'s',	"choose.fg.image9",		&conf_choose_fg_image[8],		OPTIONAL,	SAVE},
	{'i',	"choose.x9",			&conf_choose_x[8],			OPTIONAL,	SAVE},
	{'i',	"choose.y9", 			&conf_choose_y[8],			OPTIONAL,	SAVE},
	{'s',	"choose.bg.image10",		&conf_choose_bg_image[9],		OPTIONAL,	SAVE},
	{'s',	"choose.fg.image10",		&conf_choose_fg_image[9],		OPTIONAL,	SAVE},
	{'i',	"choose.x10",			&conf_choose_x[9],			OPTIONAL,	SAVE},
	{'i',	"choose.y10", 			&conf_choose_y[9],			OPTIONAL,	SAVE},
	{'i',	"choose.margin.y",		&conf_choose_margin_y,			MUST,		SAVE},
	{'i',	"choose.text.margin.y",		&conf_choose_text_margin_y,		MUST,		SAVE},
	{'i',	"choose.font",			&conf_choose_font,			OPTIONAL,	SAVE},
	{'i',	"choose.font.size",		&conf_choose_font_size,			OPTIONAL,	SAVE},
	{'i',	"choose.font.r",		&conf_choose_font_r,			OPTIONAL,	SAVE},
	{'i',	"choose.font.g",		&conf_choose_font_g,			OPTIONAL,	SAVE},
	{'i',	"choose.font.b",		&conf_choose_font_b,			OPTIONAL,	SAVE},
	{'i',	"choose.font.outline",		&conf_choose_font_outline,		OPTIONAL,	SAVE},
	{'i',	"choose.font.outline.r",	&conf_choose_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"choose.font.outline.g",	&conf_choose_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"choose.font.outline.b",	&conf_choose_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"choose.font.ruby",		&conf_choose_font_ruby,			OPTIONAL,	SAVE},
	{'b',	"choose.font.tategaki",		&conf_choose_font_tategaki,		OPTIONAL,	SAVE},
	{'b',	"choose.active",		&conf_choose_active,			OPTIONAL,	SAVE},
	{'i',	"choose.active.r",		&conf_choose_active_r,			OPTIONAL,	SAVE},
	{'i',	"choose.active.g",		&conf_choose_active_g,			OPTIONAL,	SAVE},
	{'i',	"choose.active.b",		&conf_choose_active_b,			OPTIONAL,	SAVE},
	{'i',	"choose.active.outline.r",	&conf_choose_active_outline_r,		OPTIONAL,	SAVE},
	{'i',	"choose.active.outline.g",	&conf_choose_active_outline_g,		OPTIONAL,	SAVE},
	{'i',	"choose.active.outline.b",	&conf_choose_active_outline_b,		OPTIONAL,	SAVE},
	{'b',	"choose.inactive",		&conf_choose_inactive,			OPTIONAL,	SAVE},
	{'i',	"choose.inactive.r",		&conf_choose_inactive_r,		OPTIONAL,	SAVE},
	{'i',	"choose.inactive.g",		&conf_choose_inactive_g,		OPTIONAL,	SAVE},
	{'i',	"choose.inactive.b",		&conf_choose_inactive_b,		OPTIONAL,	SAVE},
	{'i',	"choose.inactive.outline.r",	&conf_choose_inactive_outline_r,	OPTIONAL,	SAVE},
	{'i',	"choose.inactive.outline.g",	&conf_choose_inactive_outline_g,	OPTIONAL,	SAVE},
	{'i',	"choose.inactive.outline.b",	&conf_choose_inactive_outline_b,	OPTIONAL,	SAVE},
	{'s',	"choose.change.se",		&conf_choose_change_se,			OPTIONAL,	SAVE},
	{'s',	"choose.click.se",		&conf_choose_click_se,			OPTIONAL,	SAVE},
	{'f',	"choose.timed",			&conf_choose_timed,			OPTIONAL,	SAVE},
	{'s',	"choose.anime.focus1",		&conf_choose_anime_focus[0],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.unfocus1",	&conf_choose_anime_unfocus[0],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.focus2",		&conf_choose_anime_focus[1],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.unfocus2",	&conf_choose_anime_unfocus[1],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.focus3",		&conf_choose_anime_focus[2],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.unfocus3",	&conf_choose_anime_unfocus[2],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.focus4",		&conf_choose_anime_focus[3],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.unfocus4",	&conf_choose_anime_unfocus[3],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.focus5",		&conf_choose_anime_focus[4],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.unfocus5",	&conf_choose_anime_unfocus[4],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.focus6",		&conf_choose_anime_focus[5],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.unfocus6",	&conf_choose_anime_unfocus[5],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.focus7",		&conf_choose_anime_focus[6],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.unfocus7",	&conf_choose_anime_unfocus[6],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.focus8",		&conf_choose_anime_focus[7],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.unfocus8",	&conf_choose_anime_unfocus[7],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.focus9",		&conf_choose_anime_focus[8],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.unfocus9",	&conf_choose_anime_unfocus[8],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.focus10",		&conf_choose_anime_focus[9],		OPTIONAL,	SAVE},
	{'s',	"choose.anime.unfocus10",	&conf_choose_anime_unfocus[9],		OPTIONAL,	SAVE},

	/* Save Data */
	{'i',	"save.data.thumb.width",	&conf_save_data_thumb_width,		MUST,		NOSAVE},
	{'i',	"save.data.thumb.height",	&conf_save_data_thumb_height,		MUST,		NOSAVE},
	{'s',	"save.data.new.image",		&conf_save_data_new_image,		OPTIONAL,	NOSAVE},

	/* System Button */
	{'s',	"sysbtn.idle.image",		&conf_sysbtn_idle_image,		MUST,		SAVE},
	{'s',	"sysbtn.hover.image",		&conf_sysbtn_hover_image,		MUST,		SAVE},
	{'i',	"sysbtn.x",			&conf_sysbtn_x,				MUST,		SAVE},
	{'i',	"sysbtn.y",			&conf_sysbtn_y,				MUST,		SAVE},
	{'s',	"sysbtn.se",			&conf_sysbtn_se,			OPTIONAL,	SAVE},
	{'b',	"sysbtn.hide",			&conf_sysbtn_hide,			OPTIONAL,	SAVE},
	{'b',	"sysbtn.transition",		&conf_sysbtn_transition,		OPTIONAL,	SAVE},

	/* Auto Mode */
	{'s',	"automode.banner.image",	&conf_automode_banner_image,		MUST,		SAVE},
	{'i',	"automode.banner.x",		&conf_automode_banner_x,		MUST,		SAVE},
	{'i',	"automode.banner.y",		&conf_automode_banner_y,		MUST,		SAVE},
	{'s',	"automode.enter.se",		&conf_automode_enter_se,		OPTIONAL,	SAVE},
	{'s',	"automode.leave.se",		&conf_automode_leave_se,		OPTIONAL,	SAVE},

	/* Skip Mode */
	{'s',	"skipmode.banner.image",	&conf_skipmode_banner_image,		MUST,		SAVE},
	{'i',	"skipmode.banner.x",		&conf_skipmode_banner_x,		MUST,		SAVE},
	{'i',	"skipmode.banner.y",		&conf_skipmode_banner_y,		MUST,		SAVE},
	{'s',	"skipmode.enter.se",		&conf_skipmode_enter_se,		OPTIONAL,	SAVE},
	{'s',	"skipmode.leave.se",		&conf_skipmode_leave_se,		OPTIONAL,	SAVE},

	/* Menu */
	{'i',	"menu.save.font",		&conf_menu_save_font,			MUST,		SAVE},
	{'i',	"menu.save.font.size",		&conf_menu_save_font_size,		MUST,		SAVE},
	{'i',	"menu.save.font.r",		&conf_menu_save_font_r,			MUST,		SAVE},
	{'i',	"menu.save.font.g",		&conf_menu_save_font_g,			MUST,		SAVE},
	{'i',	"menu.save.font.b",		&conf_menu_save_font_b,			MUST,		SAVE},
	{'i',	"menu.save.font.outline",	&conf_menu_save_font_outline,		MUST,		SAVE},
	{'i',	"menu.save.font.outline.r",	&conf_menu_save_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"menu.save.font.outline.g",	&conf_menu_save_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"menu.save.font.outline.b",	&conf_menu_save_font_outline_r,		OPTIONAL,	SAVE},
	{'i',	"menu.save.font.ruby",		&conf_menu_save_font_ruby,		OPTIONAL,	SAVE},
	{'b',	"menu.save.font.tategaki",	&conf_menu_save_font_tategaki,		OPTIONAL,	SAVE},
	{'i',	"menu.history.margin.line",	&conf_menu_history_margin_line,		MUST,		SAVE},
	{'i',	"menu.history.font",		&conf_menu_history_font,		MUST,		SAVE},
	{'i',	"menu.history.font.size",	&conf_menu_history_font_size,		MUST,		SAVE},
	{'i',	"menu.history.font.r",		&conf_menu_history_font_r,		MUST,		SAVE},
	{'i',	"menu.history.font.g",		&conf_menu_history_font_g,		MUST,		SAVE},
	{'i',	"menu.history.font.b",		&conf_menu_history_font_b,		MUST,		SAVE},
	{'i',	"menu.history.font.outline",	&conf_menu_history_font_outline,	MUST,		SAVE},
	{'i',	"menu.history.font.outline.r",	&conf_menu_history_font_outline_r,	OPTIONAL,	SAVE},
	{'i',	"menu.history.font.outline.g",	&conf_menu_history_font_outline_r,	OPTIONAL,	SAVE},
	{'i',	"menu.history.font.outline.b",	&conf_menu_history_font_outline_r,	OPTIONAL,	SAVE},
	{'i',	"menu.history.font.ruby",	&conf_menu_history_font_ruby,		OPTIONAL,	SAVE},
	{'b',	"menu.history.font.tategaki",	&conf_menu_history_font_tategaki,	OPTIONAL,	SAVE},
	{'s',	"menu.history.quote.prefix",	&conf_menu_history_quote_prefix,	OPTIONAL,	SAVE},
	{'s',	"menu.history.quote.start",	&conf_menu_history_quote_start, 	OPTIONAL,	SAVE},
	{'s',	"menu.history.quote.end",	&conf_menu_history_quote_end, 		OPTIONAL,	SAVE},
	{'b',	"menu.history.nolast",		&conf_menu_history_nolast,	 	OPTIONAL,	SAVE},
	{'b',	"menu.preview.tategaki",	&conf_menu_preview_tategaki,		OPTIONAL,	SAVE},

	/* Initial Volumes (no need to save) */
	{'f',	"sound.vol.bgm",		&conf_sound_vol_bgm,			MUST,		NOSAVE},
	{'f',	"sound.vol.voice",		&conf_sound_vol_voice,			MUST,		NOSAVE},
	{'f',	"sound.vol.se",			&conf_sound_vol_se,			MUST,		NOSAVE},
	{'f',	"sound.vol.character",		&conf_sound_vol_character,		MUST,		NOSAVE},

	/* Character Volume Mapping (no need to save) */
	{'s',	"sound.character.name1",	&conf_sound_character_name[1],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name2",	&conf_sound_character_name[2],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name3",	&conf_sound_character_name[3],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name4",	&conf_sound_character_name[4],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name5",	&conf_sound_character_name[5],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name6",	&conf_sound_character_name[6],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name7",	&conf_sound_character_name[7],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name8",	&conf_sound_character_name[8],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name9",	&conf_sound_character_name[9],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name10",	&conf_sound_character_name[10],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name11",	&conf_sound_character_name[11],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name12",	&conf_sound_character_name[12],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name13",	&conf_sound_character_name[13],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name14",	&conf_sound_character_name[14],		OPTIONAL,	NOSAVE},
	{'s',	"sound.character.name15",	&conf_sound_character_name[15],		OPTIONAL,	NOSAVE},

	/* Character */
	{'b',	"character.focus",		&conf_character_focus,			OPTIONAL,	NOSAVE},
	{'f',	"character.eyeblink.interval",	&conf_character_eyeblink_interval,	OPTIONAL,	NOSAVE},
	{'f',	"character.eyeblink.frame",	&conf_character_eyeblink_frame,		OPTIONAL,	NOSAVE},
	{'f',	"character.lipsync.frame",	&conf_character_lipsync_frame,		OPTIONAL,	NOSAVE},
	{'i',	"character.lipsync.chars",	&conf_character_lipsync_chars,		OPTIONAL,	NOSAVE},

	/* Character Name to File Mapping */
	{'s',	"character.name1",		&conf_character_name[0],		OPTIONAL,	NOSAVE},
	{'s',	"character.image1",		&conf_character_image[0],		OPTIONAL,	NOSAVE},
	{'s',	"character.name2",		&conf_character_name[1],		OPTIONAL,	NOSAVE},
	{'s',	"character.image2",		&conf_character_image[1],		OPTIONAL,	NOSAVE},
	{'s',	"character.name3",		&conf_character_name[2],		OPTIONAL,	NOSAVE},
	{'s',	"character.image3",		&conf_character_image[2],		OPTIONAL,	NOSAVE},
	{'s',	"character.name4",		&conf_character_name[3],		OPTIONAL,	NOSAVE},
	{'s',	"character.image4",		&conf_character_image[3],		OPTIONAL,	NOSAVE},
	{'s',	"character.name5",		&conf_character_name[4],		OPTIONAL,	NOSAVE},
	{'s',	"character.image5",		&conf_character_image[4],		OPTIONAL,	NOSAVE},
	{'s',	"character.name6",		&conf_character_name[5],		OPTIONAL,	NOSAVE},
	{'s',	"character.image6",		&conf_character_image[5],		OPTIONAL,	NOSAVE},
	{'s',	"character.name7",		&conf_character_name[6],		OPTIONAL,	NOSAVE},
	{'s',	"character.image7",		&conf_character_image[6],		OPTIONAL,	NOSAVE},
	{'s',	"character.name8",		&conf_character_name[7],		OPTIONAL,	NOSAVE},
	{'s',	"character.image8",		&conf_character_image[7],		OPTIONAL,	NOSAVE},
	{'s',	"character.name9",		&conf_character_name[8],		OPTIONAL,	NOSAVE},
	{'s',	"character.image9",		&conf_character_image[8],		OPTIONAL,	NOSAVE},
	{'s',	"character.name10",		&conf_character_name[9],		OPTIONAL,	NOSAVE},
	{'s',	"character.image10",		&conf_character_image[9],		OPTIONAL,	NOSAVE},
	{'s',	"character.name11",		&conf_character_name[10],		OPTIONAL,	NOSAVE},
	{'s',	"character.image11",		&conf_character_image[10],		OPTIONAL,	NOSAVE},
	{'s',	"character.name12",		&conf_character_name[11],		OPTIONAL,	NOSAVE},
	{'s',	"character.image12",		&conf_character_image[11],		OPTIONAL,	NOSAVE},
	{'s',	"character.name13",		&conf_character_name[12],		OPTIONAL,	NOSAVE},
	{'s',	"character.image13",		&conf_character_image[12],		OPTIONAL,	NOSAVE},
	{'s',	"character.name14",		&conf_character_name[13],		OPTIONAL,	NOSAVE},
	{'s',	"character.image14",		&conf_character_image[13],		OPTIONAL,	NOSAVE},
	{'s',	"character.name15",		&conf_character_name[14],		OPTIONAL,	NOSAVE},
	{'s',	"character.image15",		&conf_character_image[14],		OPTIONAL,	NOSAVE},
	{'s',	"character.name16",		&conf_character_name[15],		OPTIONAL,	NOSAVE},
	{'s',	"character.image16",		&conf_character_image[15],		OPTIONAL,	NOSAVE},
	{'s',	"character.name17",		&conf_character_name[16],		OPTIONAL,	NOSAVE},
	{'s',	"character.image17",		&conf_character_image[16],		OPTIONAL,	NOSAVE},
	{'s',	"character.name18",		&conf_character_name[17],		OPTIONAL,	NOSAVE},
	{'s',	"character.image18",		&conf_character_image[17],		OPTIONAL,	NOSAVE},
	{'s',	"character.name19",		&conf_character_name[18],		OPTIONAL,	NOSAVE},
	{'s',	"character.image19",		&conf_character_image[18],		OPTIONAL,	NOSAVE},
	{'s',	"character.name20",		&conf_character_name[19],		OPTIONAL,	NOSAVE},
	{'s',	"character.image20",		&conf_character_image[19],		OPTIONAL,	NOSAVE},
	{'s',	"character.name21",		&conf_character_name[20],		OPTIONAL,	NOSAVE},
	{'s',	"character.image21",		&conf_character_image[20],		OPTIONAL,	NOSAVE},
	{'s',	"character.name22",		&conf_character_name[21],		OPTIONAL,	NOSAVE},
	{'s',	"character.image22",		&conf_character_image[21],		OPTIONAL,	NOSAVE},
	{'s',	"character.name23",		&conf_character_name[22],		OPTIONAL,	NOSAVE},
	{'s',	"character.image23",		&conf_character_image[22],		OPTIONAL,	NOSAVE},
	{'s',	"character.name24",		&conf_character_name[23],		OPTIONAL,	NOSAVE},
	{'s',	"character.image24",		&conf_character_image[23],		OPTIONAL,	NOSAVE},
	{'s',	"character.name25",		&conf_character_name[24],		OPTIONAL,	NOSAVE},
	{'s',	"character.image25",		&conf_character_image[24],		OPTIONAL,	NOSAVE},
	{'s',	"character.name26",		&conf_character_name[25],		OPTIONAL,	NOSAVE},
	{'s',	"character.image26",		&conf_character_image[25],		OPTIONAL,	NOSAVE},
	{'s',	"character.name27",		&conf_character_name[26],		OPTIONAL,	NOSAVE},
	{'s',	"character.image27",		&conf_character_image[26],		OPTIONAL,	NOSAVE},
	{'s',	"character.name28",		&conf_character_name[27],		OPTIONAL,	NOSAVE},
	{'s',	"character.image28",		&conf_character_image[27],		OPTIONAL,	NOSAVE},
	{'s',	"character.name29",		&conf_character_name[28],		OPTIONAL,	NOSAVE},
	{'s',	"character.image29",		&conf_character_image[28],		OPTIONAL,	NOSAVE},
	{'s',	"character.name30",		&conf_character_name[29],		OPTIONAL,	NOSAVE},
	{'s',	"character.image30",		&conf_character_image[29],		OPTIONAL,	NOSAVE},
	{'s',	"character.name31",		&conf_character_name[30],		OPTIONAL,	NOSAVE},
	{'s',	"character.image31",		&conf_character_image[30],		OPTIONAL,	NOSAVE},
	{'s',	"character.name32",		&conf_character_name[31],		OPTIONAL,	NOSAVE},
	{'s',	"character.image32",		&conf_character_image[31],		OPTIONAL,	NOSAVE},

	/* Stage Margins */
	{'i',	"stage.ch.margin.bottom",	&conf_stage_ch_margin_bottom,		OPTIONAL,	SAVE},
	{'i',	"stage.ch.margin.left",		&conf_stage_ch_margin_left,		OPTIONAL,	SAVE},
	{'i',	"stage.ch.margin.right",	&conf_stage_ch_margin_right,		OPTIONAL,	SAVE},

	/* Local Variable Aliases */
	{'s',	"var.local.name0",		&conf_local_var_name[0],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name1",		&conf_local_var_name[1],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name2",		&conf_local_var_name[2],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name3",		&conf_local_var_name[3],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name4",		&conf_local_var_name[4],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name5",		&conf_local_var_name[5],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name6",		&conf_local_var_name[6],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name7",		&conf_local_var_name[7],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name8",		&conf_local_var_name[8],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name9",		&conf_local_var_name[9],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name10",		&conf_local_var_name[10],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name11",		&conf_local_var_name[11],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name12",		&conf_local_var_name[12],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name13",		&conf_local_var_name[13],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name14",		&conf_local_var_name[14],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name15",		&conf_local_var_name[15],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name16",		&conf_local_var_name[16],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name17",		&conf_local_var_name[17],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name18",		&conf_local_var_name[18],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name19",		&conf_local_var_name[19],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name20",		&conf_local_var_name[20],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name21",		&conf_local_var_name[21],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name22",		&conf_local_var_name[22],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name23",		&conf_local_var_name[23],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name24",		&conf_local_var_name[24],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name25",		&conf_local_var_name[25],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name26",		&conf_local_var_name[26],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name27",		&conf_local_var_name[27],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name28",		&conf_local_var_name[28],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name29",		&conf_local_var_name[29],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name30",		&conf_local_var_name[30],		OPTIONAL,	NOSAVE},
	{'s',	"var.local.name31",		&conf_local_var_name[31],		OPTIONAL,	NOSAVE},

	/* Global Variable Aliases */
	{'s',	"var.global.name0",		&conf_global_var_name[0],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name1",		&conf_global_var_name[1],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name2",		&conf_global_var_name[2],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name3",		&conf_global_var_name[3],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name4",		&conf_global_var_name[4],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name5",		&conf_global_var_name[5],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name6",		&conf_global_var_name[6],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name7",		&conf_global_var_name[7],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name8",		&conf_global_var_name[8],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name9",		&conf_global_var_name[9],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name10",		&conf_global_var_name[10],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name11",		&conf_global_var_name[11],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name12",		&conf_global_var_name[12],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name13",		&conf_global_var_name[13],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name14",		&conf_global_var_name[14],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name15",		&conf_global_var_name[15],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name16",		&conf_global_var_name[16],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name17",		&conf_global_var_name[17],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name18",		&conf_global_var_name[18],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name19",		&conf_global_var_name[19],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name20",		&conf_global_var_name[20],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name21",		&conf_global_var_name[21],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name22",		&conf_global_var_name[22],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name23",		&conf_global_var_name[23],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name24",		&conf_global_var_name[24],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name25",		&conf_global_var_name[25],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name26",		&conf_global_var_name[26],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name27",		&conf_global_var_name[27],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name28",		&conf_global_var_name[28],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name29",		&conf_global_var_name[29],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name30",		&conf_global_var_name[30],		OPTIONAL,	NOSAVE},
	{'s',	"var.global.name31",		&conf_global_var_name[31],		OPTIONAL,	NOSAVE},

	/* Kirakira Effect */
	{'b',	"kirakira",			&conf_kirakira,				OPTIONAL,	NOSAVE},
	{'b',	"kirakira.add",			&conf_kirakira_add,			OPTIONAL,	NOSAVE},
	{'f',	"kirakira.frame",		&conf_kirakira_frame,			OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image1",		&conf_kirakira_image[0],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image2",		&conf_kirakira_image[1],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image3",		&conf_kirakira_image[2],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image4",		&conf_kirakira_image[3],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image5",		&conf_kirakira_image[4],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image6",		&conf_kirakira_image[5],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image7",		&conf_kirakira_image[6],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image8",		&conf_kirakira_image[7],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image9",		&conf_kirakira_image[8],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image10",		&conf_kirakira_image[9],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image11",		&conf_kirakira_image[10],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image12",		&conf_kirakira_image[11],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image13",		&conf_kirakira_image[12],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image14",		&conf_kirakira_image[13],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image15",		&conf_kirakira_image[14],		OPTIONAL,	NOSAVE},
	{'s',	"kirakira.image16",		&conf_kirakira_image[15],		OPTIONAL,	NOSAVE},

	/* Emoticon */
	{'s',	"emoticon.name1",		&conf_emoticon_name[0],			OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image1",		&conf_emoticon_image[0],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name2",		&conf_emoticon_name[1],			OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image2",		&conf_emoticon_image[1],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name3",		&conf_emoticon_name[2],			OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image3",		&conf_emoticon_image[2],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name4",		&conf_emoticon_name[3],			OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image4",		&conf_emoticon_image[3],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name5",		&conf_emoticon_name[4],			OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image5",		&conf_emoticon_image[4],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name6",		&conf_emoticon_name[5],			OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image6",		&conf_emoticon_image[5],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name7",		&conf_emoticon_name[6],			OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image7",		&conf_emoticon_image[6],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name8",		&conf_emoticon_name[7],			OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image8",		&conf_emoticon_image[7],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name9",		&conf_emoticon_name[8],			OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image9",		&conf_emoticon_image[8],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name10",		&conf_emoticon_name[9],			OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image10",		&conf_emoticon_image[9],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name11",		&conf_emoticon_name[10],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image11",		&conf_emoticon_image[10],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name12",		&conf_emoticon_name[11],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image12",		&conf_emoticon_image[11],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name13",		&conf_emoticon_name[12],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image13",		&conf_emoticon_image[12],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name14",		&conf_emoticon_name[13],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image14",		&conf_emoticon_image[13],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name15",		&conf_emoticon_name[14],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image15",		&conf_emoticon_image[14],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.name16",		&conf_emoticon_name[15],		OPTIONAL,	NOSAVE},
	{'s',	"emoticon.image16",		&conf_emoticon_image[15],		OPTIONAL,	NOSAVE},

	/* Text-to-speech */
	{'b',	"tts",				&conf_tts,				OPTIONAL,	SAVE},

	/* Release Mode */
	{'b',	"release",			&conf_release,				OPTIONAL,	NOSAVE},
};

#define RULE_TBL_SIZE	((int)(sizeof(rule_tbl) / sizeof(struct rule)))

/* Keys to write to global save data file. */
const char *global_tbl[] = {
	"font0.ttf",
	"game.locale",
	"menu.save.last.page",
};

#define GLOBAL_TBL_SIZE	((int)(sizeof(global_tbl) / sizeof(const char*)))

/* Flags to indicate whether each config is loaded or not. */
static bool loaded_tbl[RULE_TBL_SIZE];

/*
 * Forward Declarations
 */
static bool read_conf(void);
static bool save_value(const char *k, const char *v);
static bool check_conf(void);
static bool overwrite_config_game_locale(const char *val);
static bool overwrite_config_font_ttf1(const char *val);

/*
 * Initialize the config subsystem.
 */
bool init_conf(void)
{
#ifdef USE_DLL
	/* Cleanup for when DLL is recycled. */
	cleanup_conf();
#endif

	/* Read "config.txt" file. */
	if (!read_conf())
		return false;

	/* Check for non-existent config keys that has a MUST flag. */
	if (!check_conf())
		return false;

	/* Initialize various variables by config values. */
	if (!apply_initial_values())
		return false;

	return true;
}

/* Read "config.txt" file. */
static bool read_conf(void)
{
	char buf[BUF_SIZE];
	struct rfile *rf;
	char *k, *v;

	rf = open_rfile(CONF_DIR, CONFIG_FILE, false);
	if (rf == NULL)
		return false;

	while (gets_rfile(rf, buf, sizeof(buf)) != NULL) {
		if (buf[0] == '#' || buf[0] == '\0')
			continue;

		/* Get a key name. */
		k = strtok(buf, "=");
		if (k == NULL || k[0] == '\0')
			continue;

		/* Get a value string. */
		v = strtok(NULL, "=");
		if (v == NULL || v[0] == '\0') {
			log_empty_conf_string(k);
			return false;
		}

		/* Store the value. */
		if (!save_value(k, v)) {
			close_rfile(rf);
			return false;
		}
	}
	close_rfile(rf);
	return true;
}

/* Store a key-value pair. */
static bool save_value(const char *k, const char *v)
{
	char *dup;
	int i;

	/* Search the rule table and store a value. */
	for (i = 0; i < RULE_TBL_SIZE; i++) {
		/* Go to next if the key doesn't match. */
		if (strcmp(rule_tbl[i].key, k) != 0)
			continue;

		/* If the key already has a value. */
		if (loaded_tbl[i]) {
			log_duplicated_conf(k);
			return false;
		}

		/* If an ignorable key. */
		if (rule_tbl[i].var_ptr == NULL)
			return true;

		/* Store by a type. */
		if (rule_tbl[i].type == 'i') {
			*(int *)rule_tbl[i].var_ptr = atoi(v);
		} else if (rule_tbl[i].type == 'b') {
			*(bool *)rule_tbl[i].var_ptr = strcmp(v, "yes") == 0 ? true : false;
		} else if (rule_tbl[i].type == 'f') {
			*(float *)rule_tbl[i].var_ptr = (float)atof(v);
		} else if (rule_tbl[i].type == 's') {
			/* Duplicate a string. */
			dup = strdup(v);
			if (dup == NULL) {
				log_memory();
				return false;
			}
			*(char **)rule_tbl[i].var_ptr = dup;
		} else {
			assert(0);
		}

		loaded_tbl[i] = true;

		return true;
	}

	log_unknown_conf(k);

	return false;
}

/* Check for non-existent config keys that has a MUST flag. */
static bool check_conf(void)
{
	int i;

	for (i = 0; i < RULE_TBL_SIZE; i++) {
		if (rule_tbl[i].must && !loaded_tbl[i]) {
			log_undefined_conf(rule_tbl[i].key);
			return false;
		}
	}
	return true;
}

/*
 * Cleanup the config subsystem.
 */
void cleanup_conf(void)
{
	int i;

	/* Free string values. */
	for (i = 0; i < RULE_TBL_SIZE; i++) {
		loaded_tbl[i] = false;
		if (rule_tbl[i].type == 's' && rule_tbl[i].var_ptr != NULL) {
			free(*(char **)rule_tbl[i].var_ptr);
			*(char **)rule_tbl[i].var_ptr = NULL;
		}
	}
}

/*
 * Initialize various variables by config values.
 */
bool apply_initial_values(void)
{
	int i;

	/*
	 * Set global volumes.
	 *  - These values will changed in load_global_data() if global save data exist.
	 */
	set_mixer_global_volume(BGM_STREAM, conf_sound_vol_bgm);
	set_mixer_global_volume(VOICE_STREAM, conf_sound_vol_voice);
	set_mixer_global_volume(SE_STREAM, conf_sound_vol_se);

	/* Set character volumes. */
	for (i = 0; i < CH_VOL_SLOTS; i++)
		set_character_volume(i, conf_sound_vol_character);

	/* Count click anime frames. */
	for (click_frames = 16; click_frames > 1; click_frames--)
		if (conf_click_image[click_frames - 1] != NULL)
			break;

	return true;
}

/*
 * Overwrite a config.
 */
bool overwrite_config(const char *key, const char *val)
{
	int i;
	char *s;

	assert(key != NULL);
	assert(val != NULL);

	/* Treat special keys. */
	if (strcmp(key, "game.locale") == 0)
		return overwrite_config_game_locale(val);
	else if (strcmp(key, "font.ttf1") == 0)
		return overwrite_config_font_ttf1(val);

	/* Search for a key index. */
	for (i = 0; i < RULE_TBL_SIZE; i++) {
		if (strcmp(rule_tbl[i].key, key) == 0)
			break;	/* Found. */
	}
	if (i == RULE_TBL_SIZE)
		return false;

	/* Ignore a NULL-valued key. */
	if (rule_tbl[i].var_ptr == NULL)
		return true;

	/* Assign by the type. */
	switch (rule_tbl[i].type) {
	case 'b':
		/* var_ptr is a pointer to bool var. */
		*(int *)rule_tbl[i].var_ptr = strcmp(val, "yes") == 0 ? true : false;
		break;
	case 'i':
		/* var_ptr is a pointer to int var. */
		*(int *)rule_tbl[i].var_ptr = atoi(val);
		break;
	case 'f':
		/* var_ptr is a pointer to float var. */
		*(float *)rule_tbl[i].var_ptr = (float)atof(val);
		break;
	case 's':
		/* Free the previous string. */
		if (*(char **)rule_tbl[i].var_ptr != NULL) {
			free(*(char **)rule_tbl[i].var_ptr);
			*(char **)rule_tbl[i].var_ptr = NULL;
		}

		/* Duplicate the new string. */
		if (strcmp(val, "") != 0) {
			s = strdup(val);
			if (s == NULL) {
				log_memory();
				return false;
			}

			/* var_ptr is a pointer to char* var. */
			*(char **)rule_tbl[i].var_ptr = s;
		}
		break;
	default:
		assert(INVALID_CONFIG_TYPE);
		break;
	}

	/* Update the layer (x, y) positions by config keys. */
	update_layer_position_by_config();

	/* Postprocess for image load. */
	if (strcmp(key, "msgbox.image") == 0)
		update_msgbox();
	else if (strcmp(key, "namebox.image") == 0)
		update_namebox();
	else if (strcmp(key, "choose.bg.image") == 0)
		update_choosebox(false, 0);
	else if (strcmp(key, "choose.fg.image") == 0)
		update_choosebox(true, 0);
	else if (strcmp(key, "choose.bg.image2") == 0)
		update_choosebox(false, 1);
	else if (strcmp(key, "choose.fg.image2") == 0)
		update_choosebox(true, 1);
	else if (strcmp(key, "choose.bg.image3") == 0)
		update_choosebox(false, 2);
	else if (strcmp(key, "choose.fg.image3") == 0)
		update_choosebox(true, 2);
	else if (strcmp(key, "choose.bg.image4") == 0)
		update_choosebox(false, 3);
	else if (strcmp(key, "choose.fg.image4") == 0)
		update_choosebox(true, 3);
	else if (strcmp(key, "choose.bg.image5") == 0)
		update_choosebox(false, 4);
	else if (strcmp(key, "choose.fg.image5") == 0)
		update_choosebox(true, 4);
	else if (strcmp(key, "choose.bg.image6") == 0)
		update_choosebox(false, 5);
	else if (strcmp(key, "choose.fg.image6") == 0)
		update_choosebox(true, 5);
	else if (strcmp(key, "choose.bg.image7") == 0)
		update_choosebox(false, 6);
	else if (strcmp(key, "choose.fg.image7") == 0)
		update_choosebox(true, 6);
	else if (strcmp(key, "choose.bg.image8") == 0)
		update_choosebox(false, 7);
	else if (strcmp(key, "choose.fg.image8") == 0)
		update_choosebox(true, 7);
	else if (strcmp(key, "choose.bg.image9") == 0)
		update_choosebox(false, 8);
	else if (strcmp(key, "choose.fg.image9") == 0)
		update_choosebox(true, 8);
	else if (strcmp(key, "choose.bg.image10") == 0)
		update_choosebox(false, 9);
	else if (strcmp(key, "choose.fg.image10") == 0)
		update_choosebox(true, 9);

	return true;
}

/* Overwrite game.locale config. */
static bool overwrite_config_game_locale(const char *val)
{
	assert(val != NULL);

	if (conf_game_locale != NULL) {
		free(conf_game_locale);
		conf_game_locale = NULL;
	}

	if (strcmp(val, "") != 0) {
		conf_game_locale = strdup(val);
		if (conf_game_locale == NULL) {
			log_memory();
			return false;
		}
	}

	return true;
}

/* Overwrite font.ttf1 config. */
static bool overwrite_config_font_ttf1(const char *val)
{
	assert(val != NULL);

	if (strcmp(val, "") != 0) {
		assert(conf_font_ttf[0] != NULL);
		free(conf_font_ttf[0]);
		conf_font_ttf[0] = strdup(val);
		if (conf_font_ttf[0] == NULL) {
			log_memory();
			return false;
		}
	}

	if (!update_global_font())
		return false;

	return true;
}

/*
 * Save-data related
 */

/*
 * Get a config key for index.
 */
const char *get_config_key_for_save_data(int index)
{
	int i, save_key_count;

	save_key_count = 0;
	for (i = 0; i < RULE_TBL_SIZE; i++) {
		if (!rule_tbl[i].save)
			continue;
		if (save_key_count == index)
			return rule_tbl[i].key;
		save_key_count++;
	}
	return NULL;
}

/*
 * Check if config key is stored to global save data.
 */
bool is_config_key_global(const char *key)
{
	int i;

	for (i = 0; i < GLOBAL_TBL_SIZE; i++)
		if (strcmp(global_tbl[i], key) == 0)
			return true;
	return false;
}

/*
 * Get a config value type. ('s', 'b', 'i', 'f')
 */
char get_config_type_for_key(const char *key)
{
	int i;

	assert(key != NULL);

	for (i = 0; i < RULE_TBL_SIZE; i++)
		if (strcmp(rule_tbl[i].key, key) == 0)
			return rule_tbl[i].type;

	assert(CONFIG_KEY_NOT_FOUND);
	return '?';
}

/*
 * Get a string config value.
 */
const char *get_string_config_value_for_key(const char *key)
{
	int i;

	assert(key != NULL);
	for (i = 0; i < RULE_TBL_SIZE; i++) {
		if (strcmp(rule_tbl[i].key, key) == 0) {
			assert(rule_tbl[i].type == 's');
			if (rule_tbl[i].var_ptr == NULL)
				return "";
			return *(char **)rule_tbl[i].var_ptr;
		}
	}
	assert(CONFIG_KEY_NOT_FOUND);
	return NULL;
}

/*
 * Get a boolean config value.
 */
bool get_bool_config_value_for_key(const char *key)
{
	int i;

	assert(key != NULL);
	for (i = 0; i < RULE_TBL_SIZE; i++) {
		if (strcmp(rule_tbl[i].key, key) == 0) {
			assert(rule_tbl[i].type == 'b');
			assert(rule_tbl[i].var_ptr != NULL);
			return *(bool *)rule_tbl[i].var_ptr;
		}
	}
	assert(CONFIG_KEY_NOT_FOUND);
	return -1;
}

/*
 * Get an integer config value.
 */
int get_int_config_value_for_key(const char *key)
{
	int i;

	assert(key != NULL);
	for (i = 0; i < RULE_TBL_SIZE; i++) {
		if (strcmp(rule_tbl[i].key, key) == 0) {
			assert(rule_tbl[i].type == 'i');
			assert(rule_tbl[i].var_ptr != NULL);
			return *(int *)rule_tbl[i].var_ptr;
		}
	}
	assert(CONFIG_KEY_NOT_FOUND);
	return -1;
}

/*
 * Get a float config value.
 */
float get_float_config_value_for_key(const char *key)
{
	int i;

	assert(key != NULL);
	for (i = 0; i < RULE_TBL_SIZE; i++) {
		if (strcmp(rule_tbl[i].key, key) == 0) {
			assert(rule_tbl[i].type == 'f');
			assert(rule_tbl[i].var_ptr != NULL);
			return *(float *)rule_tbl[i].var_ptr;
		}
	}
	assert(CONFIG_KEY_NOT_FOUND);
	return 0;
}

/*
 * Check the locale.
 */
bool compare_locale(const char *s)
{
	const char *loc;

	assert(s != NULL);

	if (conf_game_locale != NULL)
		loc = conf_game_locale;
	else
		loc = get_system_locale();

	if (strcmp(loc, s) == 0)
		return true;

	return false;
}
