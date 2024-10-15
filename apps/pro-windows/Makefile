CC = i686-w64-mingw32-gcc
CXX = i686-w64-mingw32-g++
AR = i686-w64-mingw32-ar
STRIP = i686-w64-mingw32-strip
WINDRES = i686-w64-mingw32-windres

CPPFLAGS = \
	-DUSE_EDITOR \
	-I../../src \
	-I./res \
	-I./libroot/include \
	-I./libroot/include/freetype2

CFLAGS = \
	-O3 \
	-g0 \
	-s \
	-fno-exceptions \
	-fno-stack-protector \
	-fomit-frame-pointer \
	-municode \
	-msse3 \
	-ffast-math \
	-ftree-vectorize \
	-finput-charset=utf-8 \
	-fexec-charset=utf-8 \
	-Wall \
	-Werror \
	-Wextra \
	-Wundef \
	-Wconversion

CXXFLAGS = \
	-fno-rtti

LDFLAGS = \
	-Wl,--gc-sections \
	-mwindows \
	-lgdi32 \
	-lole32 \
	-ldsound \
	-ldxguid \
	-ld3d9 \
	-lstrmiids \
	-lcomctl32 \
	-limm32 \
	-lsapi \
	./libroot/lib/libpng.a \
	./libroot/lib/libjpeg.a \
	./libroot/lib/libwebp.a \
	./libroot/lib/libfreetype.a \
	./libroot/lib/libbz2.a \
	./libroot/lib/libz.a \
	./libroot/lib/libbrotlidec.a \
	./libroot/lib/libbrotlicommon.a \
	./libroot/lib/libvorbis.a \
	./libroot/lib/libogg.a

#
# Main Target
#

all: libroot editor.exe

#
# Sources
#

SRCS_MAIN = \
	../../src/anime.c \
	../../src/ciel.c \
	../../src/conf.c \
	../../src/event.c \
	../../src/file.c \
	../../src/glyph.c \
	../../src/gui.c \
	../../src/history.c \
	../../src/image.c \
	../../src/log.c \
	../../src/main.c \
	../../src/mixer.c \
	../../src/package.c \
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

SRCS_WIN_C = \
	../../src/microsoft/app.c \
	../../src/microsoft/scriptview.c \
	../../src/microsoft/dsound.c \
	../../src/microsoft/dialog_message.c \
	../../src/microsoft/dialog_bg.c \
	../../src/microsoft/dialog_ch.c \
	../../src/microsoft/dialog_chch.c \
	../../src/microsoft/dialog_music.c \
	../../src/microsoft/dialog_sound.c \
	../../src/microsoft/dialog_volume.c \
	../../src/microsoft/dialog_video.c \
	../../src/microsoft/dialog_shake.c \
	../../src/microsoft/dialog_choose.c \
	../../src/microsoft/dialog_menu.c \
	../../src/microsoft/dialog_time.c \
	../../src/microsoft/dialog_story.c

SRCS_WIN_CC = \
	../../src/microsoft/dx9render.cc \
	../../src/microsoft/dsvideo.cc \
	../../src/microsoft/tts_sapi.cc

SRCS_R = res/resource.rc

#
# Objects
#

OBJS = \
	objs/anime.o \
	objs/ciel.o \
	objs/conf.o \
	objs/event.o \
	objs/file.o \
	objs/glyph.o \
	objs/gui.o \
	objs/history.o \
	objs/image.o \
	objs/log.o \
	objs/main.o \
	objs/mixer.o \
	objs/package.o \
	objs/readimage.o \
	objs/readjpeg.o \
	objs/readpng.o \
	objs/readwebp.o \
	objs/save.o \
	objs/script.o \
	objs/seen.o \
	objs/stage.o \
	objs/vars.o \
	objs/wave.o \
	objs/wms_core.o \
	objs/wms_impl.o \
	objs/wms_lexer.yy.o \
	objs/wms_parser.tab.o \
	objs/cmd_anime.o \
	objs/cmd_bg.o \
	objs/cmd_ch.o \
	objs/cmd_chapter.o \
	objs/cmd_chch.o \
	objs/cmd_choose.o \
	objs/cmd_click.o \
	objs/cmd_config.o \
	objs/cmd_gosub.o \
	objs/cmd_goto.o \
	objs/cmd_if.o \
	objs/cmd_layer.o \
	objs/cmd_menu.o \
	objs/cmd_message.o \
	objs/cmd_move.o \
	objs/cmd_music.o \
	objs/cmd_plugin.o \
	objs/cmd_return.o \
	objs/cmd_set.o \
	objs/cmd_shake.o \
	objs/cmd_skip.o \
	objs/cmd_sound.o \
	objs/cmd_story.o \
	objs/cmd_text.o \
	objs/cmd_time.o \
	objs/cmd_video.o \
	objs/cmd_volume.o \
	objs/dsound.o \
	objs/dx9render.o \
	objs/dsvideo.o \
	objs/tts_sapi.o \
	objs/winpro.o \
	objs/dialog_message.o \
	objs/dialog_bg.o \
	objs/dialog_ch.o \
	objs/dialog_chch.o \
	objs/dialog_music.o \
	objs/dialog_sound.o \
	objs/dialog_volume.o \
	objs/dialog_video.o \
	objs/dialog_shake.o \
	objs/dialog_choose.o \
	objs/dialog_menu.o \
	objs/dialog_time.o \
	objs/dialog_story.o \
	objs/resource.o

#
# Compilation Rules
#

editor.exe: libroot $(OBJS) $(HDRS_MAIN) $(HDRS)
	@$(CC) -o editor.exe $(OBJS) $(CFLAGS) $(LDFLAGS)
	@$(STRIP) --strip-all editor.exe

libroot:
	@../../external/build-libs.sh "" $(CC) "-O3 -g0 -s -fno-stack-protector -fomit-frame-pointer -msse3 -ffast-math -ftree-vectorize -ffunction-sections -fdata-sections" $(AR)

objs/anime.o: ../../src/anime.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/ciel.o: ../../src/ciel.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/conf.o: ../../src/conf.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/event.o: ../../src/event.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/file.o: ../../src/file.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/glyph.o: ../../src/glyph.c libroot
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/gui.o: ../../src/gui.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/history.o: ../../src/history.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/image.o: ../../src/image.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/log.o: ../../src/log.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/main.o: ../../src/main.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/mixer.o: ../../src/mixer.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/package.o: ../../src/package.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/readimage.o: ../../src/readimage.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/readjpeg.o: ../../src/readjpeg.c libroot
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/readpng.o: ../../src/readpng.c libroot
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/readwebp.o: ../../src/readwebp.c libroot
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/save.o: ../../src/save.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/script.o: ../../src/script.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/seen.o: ../../src/seen.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/stage.o: ../../src/stage.c libroot
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/vars.o: ../../src/vars.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/wave.o: ../../src/wave.c libroot
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/wms_core.o: ../../src/wms_core.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/wms_impl.o: ../../src/wms_impl.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/wms_lexer.yy.o: ../../src/wms_lexer.yy.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/wms_parser.tab.o: ../../src/wms_parser.tab.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_anime.o: ../../src/cmd_anime.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_bg.o: ../../src/cmd_bg.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_ch.o: ../../src/cmd_ch.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_chapter.o: ../../src/cmd_chapter.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_chch.o: ../../src/cmd_chch.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_choose.o: ../../src/cmd_choose.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_click.o: ../../src/cmd_click.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_config.o: ../../src/cmd_config.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_gosub.o: ../../src/cmd_gosub.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_goto.o: ../../src/cmd_goto.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_if.o: ../../src/cmd_if.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_layer.o: ../../src/cmd_layer.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_menu.o: ../../src/cmd_menu.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_message.o: ../../src/cmd_message.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_move.o: ../../src/cmd_move.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_music.o: ../../src/cmd_music.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_plugin.o: ../../src/cmd_plugin.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_return.o: ../../src/cmd_return.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_set.o: ../../src/cmd_set.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_shake.o: ../../src/cmd_shake.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_skip.o: ../../src/cmd_skip.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_sound.o: ../../src/cmd_sound.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_story.o: ../../src/cmd_story.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_text.o: ../../src/cmd_text.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_time.o: ../../src/cmd_time.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_video.o: ../../src/cmd_video.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/cmd_volume.o: ../../src/cmd_volume.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dsound.o: ../../src/microsoft/dsound.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dx9render.o: ../../src/microsoft/dx9render.cc
	mkdir -p objs
	$(CXX) -o $@ -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $<

objs/dsvideo.o: ../../src/microsoft/dsvideo.cc
	mkdir -p objs
	$(CXX) -o $@ -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $<

objs/tts_sapi.o: ../../src/microsoft/tts_sapi.cc
	mkdir -p objs
	$(CXX) -o $@ -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $<

objs/winpro.o: src/winpro.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/scriptview.o: src/scriptview.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_message.o: src/dialog/dialog_message.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_bg.o: src/dialog/dialog_bg.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_ch.o: src/dialog/dialog_ch.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_chch.o: src/dialog/dialog_chch.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_music.o: src/dialog/dialog_music.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_sound.o: src/dialog/dialog_sound.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_volume.o: src/dialog/dialog_volume.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_video.o: src/dialog/dialog_video.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_shake.o: src/dialog/dialog_shake.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_choose.o: src/dialog/dialog_choose.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_menu.o: src/dialog/dialog_menu.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_time.o: src/dialog/dialog_time.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/dialog_story.o: src/dialog/dialog_story.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/resource.o: res/resource.rc
	mkdir -p objs
	$(WINDRES) $< $@

#
# Phony
#

clean:
	@rm -rf editor.exe objs libroot errors.txt save tmp *~
