CC = i686-w64-mingw32-gcc
CXX = i686-w64-mingw32-g++
AR = i686-w64-mingw32-ar
STRIP = i686-w64-mingw32-strip
WINDRES = i686-w64-mingw32-windres

CPPFLAGS = \
	-DUSE_EDITOR \
	-I./ \
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
	../../src/microsoft/winpro.c \
	../../src/microsoft/dsound.c \
	../../src/microsoft/dialog_bg.c

SRCS_WIN_CC = \
	../../src/microsoft/dx9render.cc \
	../../src/microsoft/dsvideo.cc \
	../../src/microsoft/tts_sapi.cc

SRCS_R = resource.rc

#
# .c.o compilation rules
#

OBJS = \
	$(SRCS_MAIN:../../src/%.c=%.o) \
	$(SRCS_PACKAGE:../../src/%.c=%.o) \
	$(SRCS_WIN_C:../../src/microsoft/%.c=%.o) \
	$(SRCS_WIN_CC:../../src/microsoft/%.cc=%.o) \
	$(SRCS_R:%.rc=%.o)

%.o: ../../src/microsoft/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

%.o: ../../src/microsoft/%.cc
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $<

%.o: ../../src/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

%.o: %.rc
	$(WINDRES) $< $@

#
# Target
#

all: opennovel.exe

opennovel.exe: libroot $(OBJS) $(HDRS_MAIN) $(HDRS)
	@$(CC) -o opennovel.exe $(OBJS) $(CFLAGS) $(LDFLAGS)
	@$(STRIP) --strip-all opennovel.exe

libroot:
	@../../external/build-libs.sh "" $(CC) "-O3 -g0 -s -fno-stack-protector -fomit-frame-pointer -msse3 -ffast-math -ftree-vectorize -ffunction-sections -fdata-sections" $(AR)

#
# Phony
#

clean:
	@rm -rf *.o *~ editor.exe errors.txt save tmp libroot*
