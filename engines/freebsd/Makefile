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
	-I/usr/local/include \
	-I/usr/local/include/freetype2 \
	-I/usr/local/include/gstreamer-1.0 \
	-I/usr/local/include/glib-2.0 \
	-I/usr/local/lib/glib-2.0/include
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
	-lpthread \
	-lm \
	-L/usr/local/lib \
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
	-lX11 \
	-lXpm \
	-lGL \
	-lGLX \
	-lasound \
	-L/usr/local/lib/gstreamer-1.0 \
	-lgstvideo-1.0 \
	-lgstbase-1.0 \
	-lgstreamer-1.0 \
	-lgobject-2.0 \
	-lglib-2.0

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
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

%.o: ../../src/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

#
# Target
#

all: game-freebsd

game-freebsd: $(OBJS) $(HDRS)
	$(CC) -o game-freebsd $(OBJS) $(LDFLAGS)

#
# Phony
#

setup:
	sudo pkg install git gmake libX11 mesa-devel alsa-lib alsa-plugins gstreamer1 gstreamer1-plugins

clean:
	@rm -rf *~ *.o game-freebsd errors.txt save tmp libroot
