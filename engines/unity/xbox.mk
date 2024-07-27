CC=cc
AR=ar
CFLAGS= \
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
	rm -rf *.o libroot-xbox ../Assets/libopennovel.dll
	./build-libs.sh \
		"xbox" \
		"$CC" \
		"$CFLAGS" \
		"$AR"
	"$CC" \
		-shared \
		-o ../Assets/libopennovel.dll \
		-O2 \
		-fPIC \
		-DUSE_UNITY \
		-DUSE_CSHARP \
		-DUSE_DLL \
		-I./libroot-xbox/include \
		-I./libroot-xbox/include/png \
		-I./libroot-xbox/include/freetype \
		$(SRCS) \
		libroot-xbox/lib/libwebp.a \
		libroot-xbox/lib/libfreetype.a \
		libroot-xbox/lib/libbrotlidec.a \
		libroot-xbox/lib/libbrotlicommon.a \
		libroot-xbox/lib/libpng.a \
		libroot-xbox/lib/libjpeg.a \
		libroot-xbox/lib/libvorbis.a \
		libroot-xbox/lib/libogg.a \
		libroot-xbox/lib/libbz2.a \
		libroot-xbox/lib/libz.a
