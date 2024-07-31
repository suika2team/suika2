include ../common.mk

SRCS = $(SRCS_MAIN) $(SRCS_WASM) $(SRCS_OPENGL)

all: html

html: $(SRCS)
	@tar xzf ../../external/libwebp-1.3.2.tar.gz
	@rm -f html/index.*
	@mkdir -p html
	@emcc \
		-O2 \
		-o html/index.html \
		-I./libwebp-1.3.2 \
		-I./libwebp-1.3.2/src \
		$(SRCS) \
		libwebp-1.3.2/src/**/*.c \
		-s USE_ZLIB=1 \
		-s USE_LIBPNG=1 \
		-s USE_OGG=1 \
		-s USE_VORBIS=1 \
		-s USE_FREETYPE=1 \
		-s USE_LIBJPEG=1 \
		-s TOTAL_MEMORY=536870912 \
		-s NO_EXIT_RUNTIME=1 \
		-s EXPORTED_RUNTIME_METHODS=["ccall","UTF8ToString"] \
		-lz \
		-lpng \
		-logg \
		-lvorbis \
		-lfreetype \
		-lopenal \
		-lidbfs.js \
		--shell-file shell.html \
		--pre-js pre.js \
		--use-preload-plugins

clean:
	@rm -rf html
