include ../common.mk

SRCS = $(SRCS_MAIN_MINUS_FILE) $(SRCS_CSHARP)

#
# Make a source tree
#
src:
	@echo 'Making Unity source tree...'
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
	@echo 'Done making Unity source tree...'
	@echo ''

#
# Windows / Xbox Series X|S x86_64 DLL
#
libopennovel.dll: $(SRCS)
	@echo 'Building Unity Win64 DLL...'
	@../../external/build-libs.sh "win64" x86_64-w64-mingw32-gcc "-O2 -g0 -fPIC" x86_64-w64-mingw32-ar
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
	@echo 'Done building libopennovel.dll for Win64.'
	@echo ''

#
# macOS Universal Binary DLL
#
libopennovel.dylib: $(SRCS)
	@echo 'Building Unity macOS DLL...'
	@
	@echo '1. Building for arm64...'
	@../../external/build-libs.sh "macos-arm64" clang "-O2 -g0 -arch arm64 -fPIC" ar
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
	@
	@echo '2. Building for x86_64...'
	@../../external/build-libs.sh "macos-x86_64" clang "-O2 -g0 -arch x86_64 -fPIC" ar
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
	@
	@echo 'Merging into a Universal Binary...'
	@lipo -create libopennovel-macos-arm64.dylib libopennovel-macos-x86_64.dylib -output libopennovel.dylib
	@rm -rf libopennovel-macos-arm64.dylib libopennovel-macos-x86_64.dylib libroot-macos-arm64 libroot-macos-x86_64
	@
	@echo 'Done building macOS DLL.'
	@echo ''

#
# Linux x86_64 DLL
#  - Please recompile on the Orbis SDK to use with PlayStation 4/5
#
libopennovel.so: $(SRCS)
	@echo 'Building Unity Linux DLL...'
	@../../external/build-libs.sh linux gcc "-O2 -g0 -fPIC" ar
	@gcc \
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
	@echo 'Done building Linux DLL...'
	@echo ''

#
# Switch DLL
#
libopennovel.nso: $(SRCS)
	@echo 'Building Unity Switch DLL...'
	@../../external/build-libs.sh switch /opt/devkitpro/devkitA64/bin/aarch64-none-elf-gcc "-O2 -g0 -fPIC" /opt/devkitpro/devkitA64/bin/aarch64-none-elf-ar
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
	@echo 'Done building Unity Switch DLL.'
	@echo ''

#
# Cleanup
#
clean:
	@rm -rf unity-src libopennovel.dll libopennovel.dylib libopennovel.so libopennovel.nso libroot* tmp
