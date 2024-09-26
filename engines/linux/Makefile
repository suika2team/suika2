CC=gcc
AR=ar
STRIP=strip

#
# Source
#

include ../common.mk

SRCS = $(SRCS_MAIN) $(SRCS_LINUX) $(SRCS_OPENGL)

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
	-O2 \
	-g0 \
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
# Targets
#

all: engine-linux

engine-linux: $(SRCS) libroot
	@$(CC) \
		-o engine-linux \
		$(CPPFLAGS) \
		$(CFLAGS) \
		`pkg-config --cflags gstreamer-1.0 gstreamer-video-1.0` \
		$(SRCS) \
		 $(LDFLAGS) \
		`pkg-config --libs gstreamer-1.0 gstreamer-video-1.0`
	@$(STRIP) engine-linux

libroot:
	@../../external/build-libs.sh "" $(CC) "-O2 -g0" $(AR)

setup:
	sudo apt-get install build-essential libasound2-dev libx11-dev libxpm-dev mesa-common-dev libfreetype-dev libpng-dev libjpeg-dev libwebp-dev libvorbis-dev libogg-dev libbrotli-dev libbz2-dev libz-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev

clean:
	@rm -rf *~ *.o engine-linux errors.txt save tmp libroot
