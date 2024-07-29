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
all: libopennovel.dll libopennovel.dylib libopennovel.nso
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
	@cp libopennovel.dll unity-src/Assets/libopennovel.dll
	@cp libopennovel.dylib unity-src/Assets/libopennovel.dylib
	@cp libopennovel.so unity-src/Assets/libopennovel.so
	@cp libopennovel.nso unity-src/Assets/libopennovel.nso
	@cp README.md unity-src/

#
# Windows / Xbox Series X|S x86_64 DLL
#
libopennovel.dll: $(SRCS_UNITY)
	@../../external/build-libs.sh "win64" x86_64-w64-mingw32-gcc "" x86_64-w64-mingw32-ar
	@x86_64-w64-mingw32-gcc \
		-shared \
		-fPIC \
		-o libopennovel.dll \
		-O2 \
		-DUSE_UNITY \
		-DUSE_CSHARP \
		-DUSE_DLL \
		-I./libroot-win64/include \
		-I./libroot-win64/include/png \
		-I./libroot-win64/include/freetype2 \
		$(SRCS_UNITY) \
		-L./libroot-win64/lib \
		-lfreetype \
		-lwebp \
		-lpng \
		-ljpeg \
		-lbrotlidec \
		-lbrotlicommon \
		-lvorbis \
		-logg \
		-lbz2 \
		-lz
	@rm -rf libroot-win64

#
# macOS Universal Binary DLL
#
libopennovel.dylib: $(SRCS_UNITY)
	@echo 'Building macOS DLL...'
	@echo 'Building for arm64...'
	@../../external/build-libs.sh "macos-arm64" clang "-arch arm64 -Dmktemp=mkdtemp" ar
	@/usr/bin/clang \
		-O2 \
		-arch arm64 \
		-dynamiclib \
		-undefined dynamic_lookup \
		-fPIC \
		-o libopennovel-macos-arm64.dylib \
		-DUSE_UNITY \
		-DUSE_CSHARP \
		-DUSE_DLL \
		-I./libroot-macos-arm64/include \
		-I./libroot-macos-arm64/include/freetype2 \
		$(SRCS_UNITY) \
		./libroot-macos-arm64/lib/libfreetype.a \
		./libroot-macos-arm64/lib/libwebp.a \
		./libroot-macos-arm64/lib/libpng.a \
		./libroot-macos-arm64/lib/libjpeg.a \
		./libroot-macos-arm64/lib/libbrotlidec.a \
		./libroot-macos-arm64/lib/libbrotlicommon.a \
		./libroot-macos-arm64/lib/libvorbis.a \
		./libroot-macos-arm64/lib/libogg.a \
		./libroot-macos-arm64/lib/libbz2.a \
		./libroot-macos-arm64/lib/libz.a
	@rm -rf libopennovel-macos-arm64.dylib.dSYM
	@echo 'Building for x86_64...'
	@../../external/build-libs.sh "macos-x86_64" clang "-arch x86_64 -Dmktemp=mkdtemp" ar
	@/usr/bin/clang \
		-O2 \
		-arch x86_64 \
		-dynamiclib \
		-undefined dynamic_lookup \
		-fPIC \
		-o libopennovel-macos-x86_64.dylib \
		-DUSE_UNITY \
		-DUSE_CSHARP \
		-DUSE_DLL \
		-I./libroot-macos-x86_64/include \
		-I./libroot-macos-x86_64/include/freetype2 \
		$(SRCS_UNITY) \
		./libroot-macos-x86_64/lib/libfreetype.a \
		./libroot-macos-x86_64/lib/libwebp.a \
		./libroot-macos-x86_64/lib/libpng.a \
		./libroot-macos-x86_64/lib/libjpeg.a \
		./libroot-macos-x86_64/lib/libbrotlidec.a \
		./libroot-macos-x86_64/lib/libbrotlicommon.a \
		./libroot-macos-x86_64/lib/libvorbis.a \
		./libroot-macos-x86_64/lib/libogg.a \
		./libroot-macos-x86_64/lib/libbz2.a \
		./libroot-macos-x86_64/lib/libz.a
	@rm -rf libopennovel-macos-x86_64.dylib.dSYM
	@echo 'Merging into a Universal Binary...'
	@lipo -create libopennovel-macos-arm64.dylib libopennovel-macos-x86_64.dylib -output libopennovel.dylib
	@rm -rf libopennovel-macos-arm64.dylib libopennovel-macos-x86_64.dylib libroot-macos-arm64 libroot-macos-x86_64

#
# Linux x86_64 DLL
#  - Please recompile on the Orbis SDK to use with PlayStation 4/5
#
libopennovel.so: $(SRCS_UNITY)
	@echo 'Building Linux DLL...'
	@../../external/build-libs.sh linux clang "-Dmktemp=mkdtemp" llvm-ar
	@clang \
		-shared \
		-fPIC \
		-o libopennovel.so \
		-O2 \
		-DUSE_UNITY \
		-DUSE_CSHARP \
		-DUSE_DLL \
		-DNO_CDECL \
		-I./libroot-linux/include \
		-I./libroot-linux/include/png \
		-I./libroot-linux/include/freetype2 \
		$(SRCS_UNITY) \
		-L./libroot-linux/lib \
		-lfreetype \
		-lwebp \
		-lpng \
		-ljpeg \
		-lbrotlidec \
		-lbrotlicommon \
		-lvorbis \
		-logg \
		-lbz2 \
		-lz
	@rm -rf libroot-linux

#
# Switch DLL
#
libopennovel.nso: $(SRCS_UNITY)
	@echo 'Building Switch DLL...'
	@../../external/build-libs.sh switch /opt/devkitpro/devkitA64/bin/aarch64-none-elf-gcc "" /opt/devkitpro/devkitA64/bin/aarch64-none-elf-ar
	@/opt/devkitpro/devkitA64/bin/aarch64-none-elf-gcc \
		-shared \
		-fPIC \
		-o libopennovel.nso \
		-O2 \
		-DUSE_UNITY \
		-DUSE_CSHARP \
		-DUSE_DLL \
		-DNO_CDECL \
		-I./libroot-switch/include \
		-I./libroot-switch/include/png \
		-I./libroot-switch/include/freetype2 \
		$(SRCS_UNITY) \
		-L./libroot-switch/lib \
		-lfreetype \
		-lwebp \
		-lpng \
		-ljpeg \
		-lbrotlidec \
		-lbrotlicommon \
		-lvorbis \
		-logg \
		-lbz2 \
		-lz
	@rm -rf libroot-switch

#
# Cleanup
#
clean:
	@rm -rf unity-src libopennovel.dll libopennovel.dylib libopennovel.so libopennovel.nso libroot* tmp
