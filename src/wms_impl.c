/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, The Authors. All rights reserved.
 */

#include "opennovel.h"
#include "wms.h"

#include <time.h>

/*
 * Stage save area
 *  - FIXME: memory leaks when the app successfully exits.
 */

static bool is_stage_pushed;
static char *saved_layer_file_name[LAYER_EFFECT4 + 1];
static int saved_layer_x[LAYER_EFFECT4 + 1];
static int saved_layer_y[LAYER_EFFECT4 + 1];
static int saved_layer_alpha[LAYER_EFFECT4 + 1];

/*
 * FFI function declaration
 */

static bool onvl_get_variable(struct wms_runtime *rt);
static bool onvl_set_variable(struct wms_runtime *rt);
static bool onvl_get_name_variable(struct wms_runtime *rt);
static bool onvl_get_year(struct wms_runtime *rt);
static bool onvl_get_month(struct wms_runtime *rt);
static bool onvl_get_day(struct wms_runtime *rt);
static bool onvl_get_hour(struct wms_runtime *rt);
static bool onvl_get_minute(struct wms_runtime *rt);
static bool onvl_get_second(struct wms_runtime *rt);
static bool onvl_get_wday(struct wms_runtime *rt);
static bool onvl_set_name_variable(struct wms_runtime *rt);
static bool onvl_random(struct wms_runtime *rt);
static bool onvl_round(struct wms_runtime *rt);
static bool onvl_ceil(struct wms_runtime *rt);
static bool onvl_floor(struct wms_runtime *rt);
static bool onvl_set_config(struct wms_runtime *rt);
static bool onvl_reflect_msgbox_and_namebox_config(struct wms_runtime *rt);
static bool onvl_reflect_font_config(struct wms_runtime *rt);
static bool onvl_clear_history(struct wms_runtime *rt);
static bool onvl_clear_msgbox(struct wms_runtime *rt);
static bool onvl_clear_namebox(struct wms_runtime *rt);
static bool onvl_hide_msgbox(struct wms_runtime *rt);
static bool onvl_hide_namebox(struct wms_runtime *rt);
static bool onvl_save_global(struct wms_runtime *rt);
bool onvl_push_stage(struct wms_runtime *rt);
bool onvl_pop_stage(struct wms_runtime *rt);
static bool onvl_remove_local_save(struct wms_runtime *rt);
static bool onvl_remove_global_save(struct wms_runtime *rt);
static bool onvl_reset_local_variables(struct wms_runtime *rt);
static bool onvl_reset_global_variables(struct wms_runtime *rt);

/*
 * FFI function table
 */

struct wms_ffi_func_tbl ffi_func_tbl[] = {
	{onvl_get_variable, "onvl_get_variable", {"index", NULL}},
	{onvl_set_variable, "onvl_set_variable", {"index", "value", NULL}},
	{onvl_get_name_variable, "onvl_get_name_variable", {"index", NULL}},
	{onvl_get_year, "onvl_get_year", {NULL}},
	{onvl_get_month, "onvl_get_month", {NULL}},
	{onvl_get_day, "onvl_get_day", {NULL}},
	{onvl_get_hour, "onvl_get_hour", {NULL}},
	{onvl_get_minute, "onvl_get_minute", {NULL}},
	{onvl_get_second, "onvl_get_second", {NULL}},
	{onvl_get_wday, "onvl_get_wday",{NULL}},
	{onvl_set_name_variable, "onvl_set_name_variable", {"index", "value", NULL}},
	{onvl_random, "onvl_random", {NULL}},
	{onvl_round,"onvl_round", {"value",NULL}},
	{onvl_ceil,"onvl_ceil", {"value",NULL}},
	{onvl_floor,"onvl_floor", {"value",NULL}},
	{onvl_set_config, "onvl_set_config", {"key", "value", NULL}},
	{onvl_reflect_msgbox_and_namebox_config, "onvl_reflect_msgbox_and_namebox_config", {NULL}},
	{onvl_reflect_font_config, "onvl_reflect_font_config", {NULL}},
	{onvl_clear_history, "onvl_clear_history", {NULL}},
	{onvl_clear_msgbox, "onvl_clear_msgbox", {NULL}},
	{onvl_clear_namebox, "onvl_clear_namebox", {NULL}},
	{onvl_hide_msgbox, "onvl_hide_msgbox", {NULL}},
	{onvl_hide_namebox, "onvl_hide_namebox", {NULL}},
	{onvl_save_global, "onvl_save_global", {NULL}},
	{onvl_push_stage, "onvl_push_stage", {NULL}},
	{onvl_pop_stage, "onvl_pop_stage", {NULL}},
	{onvl_remove_local_save, "onvl_remove_local_save", {"index", NULL}},
	{onvl_remove_global_save, "onvl_remove_global_save", {NULL}},
	{onvl_reset_local_variables, "onvl_reset_local_variables", {NULL}},
	{onvl_reset_global_variables, "onvl_reset_global_variables", {NULL}},
};

#define FFI_FUNC_TBL_SIZE (sizeof(ffi_func_tbl) / sizeof(ffi_func_tbl[0]))

/*
 * Forward declaration
 */

/* TODO: Declare static functions here. */

/*
 * FFI function definition
 */

/* Get the value of the specified variable. */
static bool onvl_get_variable(struct wms_runtime *rt)
{
	struct wms_value *index;
	int index_i;
	int value;

	assert(rt != NULL);

	/* Get the argument pointer. */
	if (!wms_get_var_value(rt, "index", &index))
		return false;

	/* Get the argument value. */
	if (!wms_get_int_value(rt, index, &index_i))
		return false;

	/* Get the value of the OpenNovel variable. */
	value = get_variable(index_i);

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", value, NULL))
		return false;

	return true;
}

/* Set the value of the specified variable. */
static bool onvl_set_variable(struct wms_runtime *rt)
{
	struct wms_value *index, *value;
	int index_i, value_i;

	assert(rt != NULL);

	/* Get the argument pointers. */
	if (!wms_get_var_value(rt, "index", &index))
		return false;
	if (!wms_get_var_value(rt, "value", &value))
		return false;

	/* Get the argument values. */
	if (!wms_get_int_value(rt, index, &index_i))
		return false;
	if (!wms_get_int_value(rt, value, &value_i))
		return false;

	/* Set the value of the OpenNovel variable. */
	set_variable(index_i, value_i);

	return true;
}

/* Get the value of the specified name variable. */
static bool onvl_get_name_variable(struct wms_runtime *rt)
{
	struct wms_value *index;
	const char *value;
	int index_i;

	assert(rt != NULL);

	/* Get the argument pointer. */
	if (!wms_get_var_value(rt, "index", &index))
		return false;

	/* Get the argument value. */
	if (!wms_get_int_value(rt, index, &index_i))
		return false;

	/* Get the value of the OpenNovel name variable. */
	value = get_name_variable(index_i);

	/* Set the return value. */
	if (!wms_make_str_var(rt, "__return", value, NULL))
		return false;

	return true;
}

/* Set the value of the specified name variable. */
static bool onvl_set_name_variable(struct wms_runtime *rt)
{
	struct wms_value *index, *value;
	const char *value_s;
	int index_i;

	assert(rt != NULL);

	/* Get the argument pointers. */
	if (!wms_get_var_value(rt, "index", &index))
		return false;
	if (!wms_get_var_value(rt, "value", &value))
		return false;

	/* Get the argument values. */
	if (!wms_get_int_value(rt, index, &index_i))
		return false;
	if (!wms_get_str_value(rt, value, &value_s))
		return false;

	/* Set the value of the OpenNovel name variable. */
	if (!set_name_variable(index_i, value_s))
		return false;

	return true;
}

static bool onvl_get_year(struct wms_runtime *rt)
{
	int now_year;
	time_t now_unixtime;
	struct tm *local;

	assert(rt != NULL);

	now_unixtime = time(NULL);

	local = localtime(&now_unixtime);

	now_year = local->tm_year + 1900;

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return" , now_year, NULL))
		return false;

	return true;
}

/* Returns now month*/
static bool onvl_get_month(struct wms_runtime *rt)
{
	int now_month;
	time_t now_unixtime;
	struct tm *local;

	assert(rt != NULL);

	now_unixtime = time(NULL);

	local = localtime(&now_unixtime);

	now_month = local->tm_mon + 1;

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", now_month, NULL))
		return false;

	return true;
}

/* Returns now day*/
static bool onvl_get_day(struct wms_runtime *rt)
{
	int now_day;
	time_t now_unixtime;
	struct tm *local;

	assert(rt != NULL);

	now_unixtime = time(NULL);

	local = localtime(&now_unixtime);

	now_day = local->tm_mday;

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", now_day, NULL))
		return false;

	return true;
}

/* Returns now hour*/
static bool onvl_get_hour(struct wms_runtime *rt)
{
	int now_hour;
	time_t now_unixtime;
	struct tm *local;

	assert(rt != NULL);

	now_unixtime = time(NULL);

	local = localtime(&now_unixtime);

	now_hour = local->tm_hour;

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", now_hour, NULL))
		return false;

	return true;
}

/* Returns now minute*/
static bool onvl_get_minute(struct wms_runtime *rt)
{
	int now_minute;
	time_t now_unixtime;
	struct tm *local;

	assert(rt != NULL);

	now_unixtime = time(NULL);

	local = localtime(&now_unixtime);

	now_minute = local->tm_min;

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", now_minute, NULL))
		return false;

	return true;
}

/* Returns now second*/
static bool onvl_get_second(struct wms_runtime *rt)
{
	int now_second;
	time_t now_unixtime;
	struct tm *local;

	assert(rt != NULL);

	now_unixtime = time(NULL);

	local = localtime(&now_unixtime);

	now_second = local->tm_sec;

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", now_second, NULL))
		return false;

	return true;
}

/* Returns day of week (Sunday=0) */
static bool onvl_get_wday(struct wms_runtime *rt)
{
	int now_wday;
	time_t now_unixtime;
	struct tm *local;

	assert(rt != NULL);

	now_unixtime = time(NULL);

	local = localtime(&now_unixtime);

	now_wday = local->tm_wday;

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", now_wday, NULL))
		return false;

	return true;
}

/* Returns a random number that ranges from 0 to 99999. */
static bool onvl_random(struct wms_runtime *rt)
{
	int rand_value;

	assert(rt != NULL);

	rand_value = (int)((float)rand() / (float)RAND_MAX * 99999);

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", rand_value, NULL))
		return false;

	return true;
}

/* Returns a rounded value. */
static bool onvl_round(struct wms_runtime *rt)
{
	struct wms_value *value;
	double value_i;
	double value_round;

	assert(rt != NULL);

	if (!wms_get_var_value(rt, "value", &value))
		return false;

	/* Get the argument value. */
	if (!wms_get_float_value(rt, value , &value_i))
		return false;

	/* Round the value. */
	value_round = round(value_i);

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", (int)value_round, NULL))
		return false;

	return true;
}

/* Returns a value rounded up. */
static bool onvl_ceil(struct wms_runtime *rt)
{
	struct wms_value *value;
	double value_i;
	double value_round_up;

	assert(rt != NULL);

	if (!wms_get_var_value(rt, "value", &value))
		return false;

	/* Get the argument value. */
	if (!wms_get_float_value(rt, value , &value_i))
		return false;

	/* Round the value up. */
	value_round_up = ceil(value_i);

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", (int)value_round_up, NULL))
		return false;

	return true;
}

/* Returns a value rounded down. */
static bool onvl_floor(struct wms_runtime *rt)
{
	struct wms_value *value;
	double value_i;
	double value_round_down;

	assert(rt != NULL);

	if (!wms_get_var_value(rt, "value", &value))
		return false;

	/* Get the argument value. */
	if (!wms_get_float_value(rt, value , &value_i))
		return false;

	/* Round the value down. */
	value_round_down = floor(value_i);

	/* Set the return value. */
	if (!wms_make_int_var(rt, "__return", (int)value_round_down, NULL))
		return false;

	return true;
}

/* Set the value of the config. */
static bool onvl_set_config(struct wms_runtime *rt)
{
	struct wms_value *key, *value;
	const char *key_s, *value_s;

	assert(rt != NULL);

	/* Get the argument pointers. */
	if (!wms_get_var_value(rt, "key", &key))
		return false;
	if (!wms_get_var_value(rt, "value", &value))
		return false;

	/* Get the argument values. */
	if (!wms_get_str_value(rt, key, &key_s))
		return false;
	if (!wms_get_str_value(rt, value, &value_s))
		return false;

	/* Set the value of the OpenNovel variable. */
	if (!overwrite_config(key_s, value_s))
		return false;

	return true;
}

/* Reflect the changed configs for the message box and the name box. */
static bool onvl_reflect_msgbox_and_namebox_config(struct wms_runtime *rt)
{
	UNUSED_PARAMETER(rt);

	fill_namebox();
	fill_msgbox();

	return true;
}

/* Reflect the changed font configs. */
static bool onvl_reflect_font_config(struct wms_runtime *rt)
{
	UNUSED_PARAMETER(rt);

	/*
	 * This function does nothing and exists for the compatibility.
	 * Now we change font in overwrite_config_font_file().
	 */

	return true;
}

/* Clear the message history. */
static bool onvl_clear_history(struct wms_runtime *rt)
{
	UNUSED_PARAMETER(rt);

	/* Clear the message history. */
	clear_history();

	return true;
}

/* Clear the message box. */
static bool onvl_clear_msgbox(struct wms_runtime *rt)
{
	UNUSED_PARAMETER(rt);

	/* Clear the message box. */
	fill_msgbox();

	return true;
}

/* Clear the name box. */
static bool onvl_clear_namebox(struct wms_runtime *rt)
{
	UNUSED_PARAMETER(rt);

	/* Clear the name box. */
	fill_namebox();

	return true;
}

/* Hide the message box. */
static bool onvl_hide_msgbox(struct wms_runtime *rt)
{
	UNUSED_PARAMETER(rt);

	/* Hide the message box. */
	show_msgbox(false);

	return true;
}

/* Hide the name box. */
static bool onvl_hide_namebox(struct wms_runtime *rt)
{
	UNUSED_PARAMETER(rt);

	/* Hide the name box. */
	show_namebox(false);

	return true;
}

/* Save the global data. */
static bool onvl_save_global(struct wms_runtime *rt)
{
	UNUSED_PARAMETER(rt);

	save_global_data();

	return true;
}

/* Push the stage. */
bool onvl_push_stage(struct wms_runtime *rt)
{
	const char *s;
	int i;

	UNUSED_PARAMETER(rt);

	is_stage_pushed = true;

	for (i = LAYER_BG; i <= LAYER_EFFECT4; i++) {
		/* Exclude the following layers. */
		switch (i) {
		case LAYER_MSG:		/* fall-thru */
		case LAYER_NAME:	/* fall-thru */
		case LAYER_CLICK:	/* fall-thru */
		case LAYER_AUTO:	/* fall-thru */
		case LAYER_SKIP:
			continue;
		default:
			break;
		}

		saved_layer_x[i] = get_layer_x(i);
		saved_layer_y[i] = get_layer_y(i);
		saved_layer_alpha[i] = get_layer_alpha(i);

		if (saved_layer_file_name[i] != NULL) {
			free(saved_layer_file_name[i]);
			saved_layer_file_name[i] = NULL;
		}

		s = get_layer_file_name(i);
		if (s != NULL) {
			saved_layer_file_name[i] = strdup(s);
			if (saved_layer_file_name[i] == NULL) {
				log_memory();
				return false;
			}
		} else {
			saved_layer_file_name[i] = NULL;
		}
	}

	return true;
}

/* Pop the stage. */
bool onvl_pop_stage(struct wms_runtime *rt)
{
	struct image *img;
	int i;

	UNUSED_PARAMETER(rt);

	if (!is_stage_pushed)
		return true;
	is_stage_pushed = false;

	for (i = LAYER_BG; i <= LAYER_EFFECT4; i++) {
		/* Exclude the following layers. */
		switch (i) {
		case LAYER_MSG:		/* fall-thru */
		case LAYER_NAME:	/* fall-thru */
		case LAYER_CLICK:	/* fall-thru */
		case LAYER_AUTO:	/* fall-thru */
		case LAYER_SKIP:
			continue;
		default:
			break;
		}

		if (i == LAYER_BG && saved_layer_file_name[i] == NULL) {
			/* Restore an empty background. */
			img = create_initial_bg();
			if (img == NULL)
				return false;
			set_layer_file_name(i, NULL);
			set_layer_image(i, img);
		} else if (i == LAYER_BG &&
			   saved_layer_file_name[i][0] == '#') {
			/* Restore a color background. */
			img = create_image_from_color_string(
				conf_game_width,
				conf_game_height,
				&saved_layer_file_name[i][1]);
			if (img == NULL)
				return false;
			if (!set_layer_file_name(i, saved_layer_file_name[i]))
				return false;
			set_layer_image(i, img);
		} else if (i == LAYER_BG) {
			/* Restore an image background. */
			if (strncmp(saved_layer_file_name[i], "cg/", 3) == 0) {
				img = create_image_from_file(
					CG_DIR,
					&saved_layer_file_name[i][3]);
			} else {
				img = create_image_from_file(
					BG_DIR,
					saved_layer_file_name[i]);
			}
			if (img == NULL)
				return false;
			if (!set_layer_file_name(i, saved_layer_file_name[i]))
				return false;
			set_layer_image(i, img);
		} else if ((i >= LAYER_CHB && i <= LAYER_CHC_EYE) ||
			   i == LAYER_CHF) {
			/* Restore an character. */
			if (saved_layer_file_name[i] != NULL) {
				img = create_image_from_file(CH_DIR,
							     saved_layer_file_name[i]);
				if (img == NULL)
					return false;
			} else {
				img = NULL;
			}
			if (!set_layer_file_name(i, saved_layer_file_name[i]))
				return false;
			set_layer_image(i, img);
		} else {
			/* Restore an image. */
			if (saved_layer_file_name[i] != NULL) {
				img = create_image_from_file(CG_DIR,
							     saved_layer_file_name[i]);
				if (img == NULL)
					return false;
			} else {
				img = NULL;
			}
			if (!set_layer_file_name(i, saved_layer_file_name[i]))
				return false;
			set_layer_image(i, img);
		}

		set_layer_position(i, saved_layer_x[i], saved_layer_y[i]);
		set_layer_alpha(i, saved_layer_alpha[i]);

		if (saved_layer_file_name[i] != NULL) {
			free(saved_layer_file_name[i]);
			saved_layer_file_name[i] = NULL;
		}
	}

	return true;
}

/* Removes all local save data. */
static bool onvl_remove_local_save(struct wms_runtime *rt)
{
	struct wms_value *index;
	int index_i;

	assert(rt != NULL);

	/* Get the argument pointer. */
	if (!wms_get_var_value(rt, "index", &index))
		return false;

	/* Get the argument value. */
	if (!wms_get_int_value(rt, index, &index_i))
		return false;

	/* Delete a save file. */
	delete_local_save(index_i);

	return true;
}

/* Removes a global save data. */
static bool onvl_remove_global_save(struct wms_runtime *rt)
{
	UNUSED_PARAMETER(rt);

	delete_global_save();

	return true;
}

/* Resets the local variables to zero. */
static bool onvl_reset_local_variables(struct wms_runtime *rt)
{
	int i;

	UNUSED_PARAMETER(rt);

	for (i = 0; i < LOCAL_VAR_SIZE; i++)
		set_variable(i, 0);

	return true;
}

/* Resets the global variables to zero. */
static bool onvl_reset_global_variables(struct wms_runtime *rt)
{
	int i;

	UNUSED_PARAMETER(rt);

	for (i = GLOBAL_VAR_OFFSET; i < GLOBAL_VAR_SIZE; i++)
		set_variable(i, 0);

	return true;
}

/*
 * FFI function registration
 */

bool register_onvl_functions(struct wms_runtime *rt)
{
	if (!wms_register_ffi_func_tbl(rt, ffi_func_tbl, FFI_FUNC_TBL_SIZE)) {
		log_wms_runtime_error("", 0, wms_get_runtime_error_message(rt));
		return false;
	}
	return true;
}

/*
 * For intrinsic
 */

#include <stdio.h>

/*
 * Printer. (for print() intrinsic)
 */
int wms_printf(const char *s, ...)
{
	char buf[1024];
	va_list ap;
	int ret;

	va_start(ap, s);
	ret = vsnprintf(buf, sizeof(buf), s, ap);
	va_end(ap);

	if (strcmp(buf, "\n") == 0)
		return 0;

	log_warn(buf);
	return ret;
}
