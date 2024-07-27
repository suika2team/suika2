#
# Source
#

HDRS = \
	../../src/opennovel.h \
	../../src/anime.h \
	../../src/conf.h \
	../../src/event.h \
	../../src/file.h \
	../../src/glyph.h \
	../../src/gui.h \
	../../src/history.h \
	../../src/image.h \
	../../src/log.h \
	../../src/main.h \
	../../src/mixer.h \
	../../src/motion.h \
	../../src/save.h \
	../../src/script.h \
	../../src/seen.h \
	../../src/stage.h \
	../../src/vars.h \
	../../src/wave.h \
	../../src/wms.h \
	../../src/khronos/glrender.h \
	../../src/linux/asound.h \
	../../src/linux/gstplay.h

SRCS_MAIN = \
	../../src/anime.c \
	../../src/conf.c \
	../../src/ciel.c \
	../../src/event.c \
	../../src/file.c \
	../../src/glyph.c \
	../../src/gui.c \
	../../src/history.c \
	../../src/image.c \
	../../src/log.c \
	../../src/main.c \
	../../src/mixer.c \
	../../src/readimage.c \
	../../src/readpng.c \
	../../src/readjpeg.c \
	../../src/readwebp.c \
	../../src/save.c \
	../../src/script.c \
	../../src/seen.c \
	../../src/stage.c \
	../../src/vars.c \
	../../src/wave.c \
	../../src/wms_core.c \
	../../src/wms_lexer.yy.c \
	../../src/wms_parser.tab.c \
	../../src/wms_impl.c \
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

SRCS_LINUX = \
	../../src/linux/asound.c \
	../../src/linux/gstplay.c \
	../../src/linux/x11main.c

SRCS_KHRONOS = \
	../../src/khronos/glrender.c

#
# CPPFLAGS
#

CPPFLAGS = \
	-I./libroot/include \
	-I/usr/include/freetype2 \
	-I/usr/include/png \
	-I/usr/local/include \
	-I/usr/pkg/include \
	-I/usr/X11R7/include

#
# CFLAGS
#

CFLAGS = \
	-O2 \
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
	-lpng \
	-ljpeg \
	-lwebp \
	-lfreetype \
	-lvorbisfile \
	-lvorbis \
	-logg \
	-lbrotlicommon \
	-lbrotlidec \
	-lbz2 \
	-lz \
	-lasound \
	-lX11 \
	-lXpm \
	-lGL \
	-lGLX \
	-lpthread \
	-lm

#
# .c.o compilation rules
#

OBJS = \
	$(SRCS_KHRONOS:../../src/khronos/%.c=%.o) \
	$(SRCS_LINUX:../../src/linux/%.c=%.o) \
	$(SRCS_MAIN:../../src/%.c=%.o)

%.o: ../../src/khronos/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

%.o: ../../src/linux/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) `pkg-config --cflags gstreamer-1.0 gstreamer-video-1.0` $<

%.o: ../../src/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

#
# Target
#

all: game-linux

game-linux: $(OBJS) $(HDRS)
	$(CC) -o game-linux $(OBJS) $(LDFLAGS) `pkg-config --libs gstreamer-1.0 gstreamer-video-1.0`

#
# Phony
#

setup:
	sudo apt-get install build-essential libasound2-dev libx11-dev libxpm-dev mesa-common-dev libfreetype-dev libpng-dev libjpeg-dev libwebp-dev libvorbis-dev libogg-dev libbrotli-dev libbz2-dev libz-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev

clean:
	@rm -rf *~ *.o game-linux log.txt sav tmp libroot
