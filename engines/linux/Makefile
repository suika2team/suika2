CC=clang
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
	-I/usr/include/freetype2 \
	-I/usr/include/png

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
# Targets
#

all: game-linux

game-linux: $(SRCS) libroot
	$(CC) \
		-o game-linux \
		$(CPPFLAGS) \
		$(CFLAGS) \
		`pkg-config --cflags gstreamer-1.0 gstreamer-video-1.0` \
		$(SRCS) \
		 $(LDFLAGS) \
		`pkg-config --libs gstreamer-1.0 gstreamer-video-1.0`
	$(STRIP) game-linux

libroot:
	../../external/build-libs.sh "" $(CC) "" $(AR)

setup:
	sudo apt-get install build-essential libasound2-dev libx11-dev libxpm-dev mesa-common-dev libfreetype-dev libpng-dev libjpeg-dev libwebp-dev libvorbis-dev libogg-dev libbrotli-dev libbz2-dev libz-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev

clean:
	@rm -rf *~ *.o game-linux errors.txt save tmp libroot
