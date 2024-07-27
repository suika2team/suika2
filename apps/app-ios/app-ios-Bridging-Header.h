//
//  Use this file to import your target's public headers that you would like to expose to Swift.
//

#include "opennovel.h"
#include "aunit.h"
#include "GameShaderTypes.h"

extern void (*wrap_log_info)(const char * s);
extern void (*wrap_log_warn)(const char * s);
extern void (*wrap_log_error)(const char * s);
extern void (*wrap_make_sav_dir)(void);
extern void (*wrap_make_valid_path)(const char * dir, const char * fname, const char * dst, int len);
extern void (*wrap_notify_image_update)(int id, int width, int height, const uint32_t *pixels);
extern void (*wrap_notify_image_free)(int id);
extern void (*wrap_render_image_normal)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_add)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_dim)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_rule)(int src_img, int rule_img, int threshold);
extern void (*wrap_render_image_melt)(int src_img, int rule_img, int progress);
extern void (*wrap_render_image_3d_normal)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_3d_add)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_reset_lap_timer)(uint64_t *origin);
extern uint64_t (*wrap_get_lap_timer_millisec)(uint64_t *origin);
extern void (*wrap_play_sound)(int stream, void *wave);
extern void (*wrap_stop_sound)(int stream);
extern void (*wrap_set_sound_volume)(int stream, float vol);
extern int (*wrap_is_sound_finished)(int stream);
extern bool (*wrap_play_video)(const char *fname, bool is_skippable);
extern void (*wrap_stop_video)(void);
extern bool (*wrap_is_video_playing)(void);
extern void (*wrap_update_window_title)(void);
extern bool (*wrap_is_full_screen_supported)(void);
extern bool (*wrap_is_full_screen_mode)(void);
extern void (*wrap_enter_full_screen_mode)(void);
extern void (*wrap_leave_full_screen_mode)(void);
extern void (*wrap_get_system_locale)(char *dst, int len);
extern void (*wrap_speak_text)(const char *text);
extern void (*wrap_set_continuous_swipe_enabled)(bool is_enabled);
extern void (*wrap_free_shared)(void *p);
extern bool (*wrap_check_file_exist)(const char *file_name);
extern void *(*wrap_get_file_contents)(const char *file_name, int *len);
extern void (*wrap_open_save_file)(const char *file_name);
extern void (*wrap_write_save_file)(int b);
extern void (*wrap_close_save_file)(void);
extern void (*wrap_set_continuous_swipe_enabled)(bool is_enabled);
extern bool (*wrap_is_continue_pushed)(void);
extern bool (*wrap_is_next_pushed)(void);
extern bool (*wrap_is_stop_pushed)(void);
extern bool (*wrap_is_script_opened)(void);
extern const char *(*wrap_get_opened_script)(void);
extern bool (*wrap_is_exec_line_changed)(void);
extern bool (*wrap_get_changed_exec_line)(void);
extern void (*wrap_on_change_running_state)(int running, int request_stop);
extern void (*wrap_on_load_script)(void);
extern void (*wrap_on_change_position)(void);
extern void (*wrap_on_update_variable)(void);

extern void cleanup_image(void);
