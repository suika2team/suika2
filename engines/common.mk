SRCS_MAIN_MINUS_FILE = \
	../../src/anime.c \
	../../src/ciel.c \
	../../src/conf.c \
	../../src/event.c \
	../../src/glyph.c \
	../../src/gui.c \
	../../src/history.c \
	../../src/image.c \
	../../src/log.c \
	../../src/main.c \
	../../src/mixer.c \
	../../src/readimage.c \
	../../src/readjpeg.c \
	../../src/readpng.c \
	../../src/readwebp.c \
	../../src/save.c \
	../../src/script.c \
	../../src/seen.c \
	../../src/stage.c \
	../../src/vars.c \
	../../src/wave.c \
	../../src/wms_core.c \
	../../src/wms_impl.c \
	../../src/wms_lexer.yy.c \
	../../src/wms_parser.tab.c \
	../../src/cmd_anime.c \
	../../src/cmd_bg.c \
	../../src/cmd_ch.c \
	../../src/cmd_chapter.c \
	../../src/cmd_chch.c \
	../../src/cmd_choose.c \
	../../src/cmd_click.c \
	../../src/cmd_config.c \
	../../src/cmd_gosub.c \
	../../src/cmd_goto.c \
	../../src/cmd_if.c \
	../../src/cmd_layer.c \
	../../src/cmd_menu.c \
	../../src/cmd_message.c \
	../../src/cmd_move.c \
	../../src/cmd_music.c \
	../../src/cmd_plugin.c \
	../../src/cmd_return.c \
	../../src/cmd_set.c \
	../../src/cmd_shake.c \
	../../src/cmd_skip.c \
	../../src/cmd_sound.c \
	../../src/cmd_story.c \
	../../src/cmd_text.c \
	../../src/cmd_time.c \
	../../src/cmd_video.c \
	../../src/cmd_volume.c

SRCS_MAIN = \
	$(SRCS_MAIN_MINUS_FILE) \
	../../src/file.c

SRC_APPLE = \
	../../src/apple/GameRenderer.m \
	../../src/apple/GameShaders.metal \
	../../src/apple/aunit.c

SRCS_WIN_C = \
	../../src/microsoft/winmain.c \
	../../src/microsoft/dsound.c

SRCS_WIN_CC = \
	../../src/microsoft/dx9render.cc \
	../../src/microsoft/dsvideo.cc \
	../../src/microsoft/tts_sapi.cc

SRCS_LINUX = \
	../../src/linux/asound.c \
	../../src/linux/gstplay.c \
	../../src/linux/x11main.c

SRCS_ANDROID = \
	../../src/google/ndkmain.c \
	../../src/google/ndkfile.c

SRCS_OPENGL = \
	../../src/khronos/glrender.c

SRCS_OPENSL = \
	../../src/khronos/slsound.c

SRCS_SWIFT = \
	../../src/halwrap.c \

SRCS_CSHARP = \
	../../src/halwrap.c \

SRCS_WASM = \
	../../src/wasm/emmain.c \
	../../src/wasm/alsound.c

SRCS_PACKAGE = \
	../../src/package.c
