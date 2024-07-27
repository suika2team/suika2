SRC = \
	../../src/wasm/emmain.c \
	../../src/wasm/alsound.c \
	../../src/khronos/glrender.c \
	../../src/cmd_anime.c \
	../../src/cmd_bg.c \
	../../src/cmd_ch.c \
	../../src/cmd_chapter.c \
	../../src/cmd_chch.c \
	../../src/cmd_click.c \
	../../src/cmd_choose.c \
	../../src/cmd_config.c \
	../../src/cmd_gosub.c \
	../../src/cmd_goto.c \
	../../src/cmd_if.c \
	../../src/cmd_layer.c \
	../../src/cmd_message.c \
	../../src/cmd_menu.c \
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
	../../src/cmd_volume.c \
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
	../../src/wms_impl.c \
	../../src/wms_lexer.yy.c \
	../../src/wms_parser.tab.c

all: html

html: $(SRC)
	@tar xzf ../../external/libwebp-1.3.2.tar.gz && \
	rm -f html/index.* && \
	mkdir -p html && \
	emcc $(SRC) -I./libwebp-1.3.2 -I./libwebp-1.3.2/src libwebp-1.3.2/src/**/*.c -o html/index.html -O3 -DUSE_OPENGL -s USE_ZLIB=1 -s USE_LIBPNG=1 -s USE_OGG=1 -s USE_VORBIS=1 -s USE_FREETYPE=1 -s USE_LIBJPEG=1 -s TOTAL_MEMORY=536870912 -s NO_EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=["ccall","UTF8ToString"] -lz -lpng -logg -lvorbis -lfreetype -lopenal -lidbfs.js --shell-file shell.html --pre-js pre.js --use-preload-plugins

clean:
	@rm -rf html
