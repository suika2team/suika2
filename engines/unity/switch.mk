CC=cc
AR=ar
CFLAGS=

SRCS = \
	halwrap.c \
	anime.c \
	conf.c \
	ciel.c \
	event.c \
	glyph.c \
	gui.c \
	history.c \
	image.c \
	log.c \
	main.c \
	mixer.c \
	readimage.c \
	readpng.c \
	readjpeg.c \
	readwebp.c \
	save.c \
	script.c \
	seen.c \
	stage.c \
	vars.c \
	wave.c \
	wms_core.c \
	wms_lexer.yy.c \
	wms_parser.tab.c \
	wms_impl.c \
	cmd_anime.c \
	cmd_bg.c \
	cmd_ch.c \
	cmd_chapter.c \
	cmd_chch.c \
	cmd_choose.c \
	cmd_click.c \
	cmd_config.c \
	cmd_gosub.c \
	cmd_goto.c \
	cmd_if.c \
	cmd_layer.c \
	cmd_menu.c \
	cmd_message.c \
	cmd_move.c \
	cmd_music.c \
	cmd_plugin.c \
	cmd_return.c \
	cmd_set.c \
	cmd_shake.c \
	cmd_skip.c \
	cmd_sound.c \
	cmd_story.c \
	cmd_text.c \
	cmd_time.c \
	cmd_video.c \
	cmd_volume.c

all:
	rm -rf *.o libroot-switch ../Assets/libopennovel.nso
	./build-libs.sh \
		"switch" \
		"$CC" \
		"$CFLAGS" \
		"$AR"
	"$CC" \
		-shared \
		-o ../Assets/libopennovel.nso \
		-O2 \
		-fPIC \
		-DUSE_UNITY \
		-DUSE_CSHARP \
		-DUSE_DLL \
		-I./libroot-switch/include \
		-I./libroot-switch/include/png \
		-I./libroot-switch/include/freetype \
		$(SRCS) \
		libroot-switch/lib/libwebp.a \
		libroot-switch/lib/libfreetype.a \
		libroot-switch/lib/libbrotlidec.a \
		libroot-switch/lib/libbrotlicommon.a \
		libroot-switch/lib/libpng.a \
		libroot-switch/lib/libjpeg.a \
		libroot-switch/lib/libvorbis.a \
		libroot-switch/lib/libogg.a \
		libroot-switch/lib/libbz2.a \
		libroot-switch/lib/libz.a
