#
# Makefile for the Unity export template
#  - Note that the author does not have any copy of commercial SDKs
#  - This Makefile was written with his imagination
#

SRCS_UNITY = \
	../../src/halwrap.c \
	../../src/anime.c \
	../../src/conf.c \
	../../src/ciel.c \
	../../src/event.c \
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

OBJS = $(SRCS_UNITY:../../src/%.c=%.o)

#
# Make a source tree
#
all:
	@rm -rf unity-src
	@mkdir unity-src
	@mkdir unity-src/Assets
	@mkdir unity-src/Assets/StreamingAssets
	@mkdir unity-src/Assets/Resources
	@cp -v Assets/MainScene.unity unity-src/Assets/
	@cp -v Assets/OpenNovelScript.cs unity-src/Assets/
	@cp -v Assets/OpenNovelAudio.cs unity-src/Assets/
	@cp -v Assets/Resources/NormalShader.shader unity-src/Assets/Resources/
	@cp -v Assets/Resources/AddShader.shader unity-src/Assets/Resources/
	@cp -v Assets/Resources/DimShader.shader unity-src/Assets/Resources/
	@cp -v Assets/Resources/RuleShader.shader unity-src/Assets/Resources/
	@cp -v Assets/Resources/MeltShader.shader unity-src/Assets/Resources/
	@#cp libopennovel-win64.dll unity-src/Assets/libopennovel.dll
	@#cp libopennovel-macos.dylib unity-src/Assets/libopennovel.dylib
	@cp README.md unity-src/
	@mkdir unity-src/dll-src
	@cp build-libs.sh unity-src/dll-src/
	@cp console.mk unity-src/Makefile
	@cp $(SRCS_UNITY) unity-src/dll-src/
	@cp ps45.mk unity-src/dll-src/
	@cp xbox.mk unity-src/dll-src/
	@cp switch.mk unity-src/dll-src/
	@cp -R ../../external unity-src/libsrc

#
# Windows 64bit DLL for Test Run
#
libopennovel-win64.dll: $(SRCS_UNITY)
	../../external/build-libs.sh "win64" x86_64-w64-mingw32-gcc "" x86_64-w64-mingw32-ar
	x86_64-w64-mingw32-gcc \
		-shared \
		-fPIC \
		-o libopennovel-win64.dll \
		-O2 \
		-DUSE_UNITY \
		-DUSE_CSHARP \
		-DUSE_DLL \
		-I./libroot-win64/include \
		-I./libroot-win64/libroot/include/png \
		-I./libroot-win64/libroot/include/freetype2 \
		$(SRCS_UNITY) \
		./libroot-win64/lib/libfreetype.a \
		./libroot-win64/lib/libwebp.a \
		./libroot-win64/lib/libpng.a \
		./libroot-win64/lib/libjpeg.a \
		./libroot-win64/lib/libbrotlidec.a \
		./libroot-win64/lib/libbrotlicommon.a \
		./libroot-win64/lib/libvorbis.a \
		./libroot-win64/lib/libogg.a \
		./libroot-win64/lib/libbz2.a \
		./libroot-win64/lib/libz.a

#
# macOS DLL for Test Run
#
libopennovel-macos.dylib: ../macos/libroot $(SRCS_UNITY)
	../../external/build-libs.sh "macos" clang "-arch arm64 -arch x86_64 -Dmktemp=mkdtemp" ar
	/usr/bin/clang \
		-O2 \
		-arch arm64 \
		-arch x86_64 \
		-dynamiclib \
		-undefined dynamic_lookup \
		-fPIC \
		-o libopennovel-macos.dylib \
		-DUSE_UNITY \
		-DUSE_CSHARP \
		-DUSE_DLL \
		-I../macos/libroot/include \
		-I../macos/libroot/include/freetype2 \
		$(SRCS_UNITY) \
		../macos/libroot/lib/libfreetype.a \
		../macos/libroot/lib/libwebp.a \
		../macos/libroot/lib/libpng.a \
		../macos/libroot/lib/libjpeg.a \
		../macos/libroot/lib/libbrotlidec.a \
		../macos/libroot/lib/libbrotlicommon.a \
		../macos/libroot/lib/libvorbis.a \
		../macos/libroot/lib/libogg.a \
		../macos/libroot/lib/libbz2.a \
		../macos/libroot/lib/libz.a
	rm -rf libopennovel-macos.dylib.dSYM

#
# Cleanup
#
clean:
	@rm -rf unity-src libopennovel-win64.dll libopennovel-macos.dylib libroot* tmp
