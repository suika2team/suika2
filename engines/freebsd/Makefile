CC=cc
AR=ar

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
# Target
#

all: game-freebsd

game-freebsd: $(SRCS) libroot
	$(CC) \
		-o game-freebsd \
		$(CPPFLAGS) \
		$(CFLAGS) \
		$(SRCS) \
		$(LDFLAGS) \

libroot:
	../../external/build-libs.sh "" $(CC) "" $(AR)

setup:
	sudo pkg install git libX11 libXpm mesa-devel alsa-lib alsa-plugins webp libvorbis libogg freetype2 brotli gstreamer1 gstreamer1-plugins

clean:
	@rm -rf *~ *.o game-freebsd errors.txt save tmp libroot
