CC=gcc
AR=ar
STRIP=strip

#
# CPPFLAGS
#

CPPFLAGS = \
	-I./libroot/include \
	-I./libroot/include/freetype2 \
	-I./libroot/include/png

#
# CFLAGS
#

CFLAGS = \
	-O0 \
	-g3 \
	-ffast-math \
	-ftree-vectorize \
	-std=gnu11 \
	-Wall \
	-Werror \
	-Wextra \
	-Wundef \
	-Wconversion \
	-Wno-multichar

#
# LDFLAGS
#

LDFLAGS = \
	./libroot/lib/libpng.a \
	./libroot/lib/libjpeg.a \
	./libroot/lib/libwebp.a \
	./libroot/lib/libfreetype.a \
	./libroot/lib/libvorbis.a \
	./libroot/lib/libogg.a \
	./libroot/lib/libbrotlicommon.a \
	./libroot/lib/libbrotlidec.a \
	./libroot/lib/libbz2.a \
	./libroot/lib/libz.a \
	-lasound \
	-lX11 \
	-lXpm \
	-lGL \
	-lGLX \
	-lpthread \
	-lm

#
# Source
#

include ../common.mk

SRCS = $(SRCS_MAIN) $(SRCS_LINUX) $(SRCS_OPENGL)

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
	objs/x11main.o \
	objs/asound.o \
	objs/gstplay.o \
	objs/glrender.o

#
# Compilation Rules
#

all: engine-linux

engine-linux: libroot $(OBJS) $(SRCS)
	@$(CC) -o engine-linux $(CPPFLAGS) $(CFLAGS) $(OBJS) $(LDFLAGS) `pkg-config --libs gstreamer-1.0 gstreamer-video-1.0`
	#@$(STRIP) engine-linux

libroot:
	@../../external/build-libs.sh "" $(CC) "-O2 -g0" $(AR)

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

objs/x11main.o: ../../src/linux/x11main.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/asound.o: ../../src/linux/asound.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

objs/gstplay.o: ../../src/linux/gstplay.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) `pkg-config --cflags gstreamer-1.0 gstreamer-video-1.0` $<

objs/glrender.o: ../../src/khronos/glrender.c
	mkdir -p objs
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

#
# Phony
#

setup:
	sudo apt-get install build-essential libasound2-dev libx11-dev libxpm-dev mesa-common-dev libfreetype-dev libpng-dev libjpeg-dev libwebp-dev libvorbis-dev libogg-dev libbrotli-dev libbz2-dev libz-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev

clean:
	@rm -rf engine-linux objs libroot errors.txt save tmp *~
