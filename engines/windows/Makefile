CC = i686-w64-mingw32-gcc
CXX = i686-w64-mingw32-g++
AR = i686-w64-mingw32-ar
WINDRES = i686-w64-mingw32-windres

#
# CPPFLAGS
#

CPPFLAGS = \
	-I./ \
	-I./libroot/include \
	-I./libroot/include/freetype2

#
# CFLAGS
#

CFLAGS = \
	-O2 \
	-municode \
	-ffast-math \
	-finput-charset=utf-8 \
	-fexec-charset=utf-8 \
	-Wall \
	-Werror \
	-Wextra \
	-Wundef \
	-Wconversion

#
# LDFLAGS
#

LDFLAGS = \
	-mwindows \
	-municode \
	-lgdi32 \
	-lole32 \
	-ldsound \
	-ldxguid \
	-ld3d9 \
	-lstrmiids \
	-lsapi \
	-limagehlp \
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

include ../common.mk

SRCS_R = resource.rc

#
# .c.o compilation rules
#

OBJS = \
	$(SRCS_MAIN:../../src/%.c=%.o) \
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

all: game.exe

game.exe: libroot $(OBJS) $(HDRS_MAIN) $(HDRS)
	@$(CC) -s -o game.exe $(OBJS) $(LDFLAGS)

libroot:
	@../../external/build-libs.sh "" $(CC) "" $(AR)

#
# Phony
#

clean:
	@rm -rf *.o *~ game.exe log.txt sav tmp libroot*
