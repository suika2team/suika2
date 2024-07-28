SRCS = \
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
	../../src/cmd_volume.c \
	../../src/khronos/glrender.c \
	../../src/khronos/slsound.c \
	../../src/google/ndkmain.c \
	../../src/google/ndkfile.c

src: libopennovel-aarch64.so libopennovel-armv7a.so libopennovel-x86_64.so libopennovel-i686.so
	@rm -rf app/build app/.cxx app/src/main/cpp app/src/main/java app/src/main/assets android-src
	@mkdir -p android-src
	@mkdir -p android-src/app/src/main/cpp
	@
	@cp -R app android-src/
	@cp -R gradle.properties android-src/
	@cp -R build.gradle android-src/
	@cp -R gradlew android-src/
	@cp -R settings.gradle android-src/
	@cp -R gradlew.bat android-src/
	@cp -R gradle android-src/
	@
	@cp build.sh android-src/
	@cp build.bat android-src/
	@
	@mkdir -p android-src/app/src/main/java/org/opennovel/engineandroid
	@cp ../../src/google/MainActivity.java android-src/app/src/main/java/org/opennovel/engineandroid/
	@
	@mkdir -p android-src/app/src/main/jniLibs/arm64-v8a
	@cp libopennovel-aarch64.so android-src/app/src/main/jniLibs/arm64-v8a/libopennovel.so
	@mkdir -p android-src/app/src/main/jniLibs/armeabi-v7a
	@cp libopennovel-armv7a.so android-src/app/src/main/jniLibs/armeabi-v7a/libopennovel.so
	@mkdir -p android-src/app/src/main/jniLibs/x86_64
	@cp libopennovel-armv7a.so android-src/app/src/main/jniLibs/x86_64/libopennovel.so
	@mkdir -p android-src/app/src/main/jniLibs/x86
	@cp libopennovel-armv7a.so android-src/app/src/main/jniLibs/x86/libopennovel.so

libopennovel-aarch64.so: ndk
	@echo 'Building aarch64 library...'
	@../../external/build-libs.sh aarch64 `pwd`/ndk/bin/clang-18 "--target=aarch64-linux-android21 -O2" `pwd`/ndk/bin/llvm-ar
	@`pwd`/ndk/bin/clang-18 \
		--target=aarch64-linux-android21 \
		-O2 \
		-shared \
		-fPIC \
		-o libopennovel-aarch64.so \
		-DUSE_DLL \
		-I../../src \
		-I../../src/khronos \
		-I./libroot-aarch64/include \
		-I./libroot-aarch64/include/png \
		-I./libroot-aarch64/include/freetype2 \
		$(SRCS) \
		-lm \
		-lGLESv3 \
		-lOpenSLES \
		-llog \
		-L./libroot-aarch64/lib \
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

libopennovel-armv7a.so: ndk
	@echo 'Building armv7a library...'
	@../../external/build-libs.sh armv7a `pwd`/ndk/bin/clang-18 "--target=armv7a-linux-androideabi21 -O2" `pwd`/ndk/bin/llvm-ar
	@`pwd`/ndk/bin/clang-18 \
		--target=armv7a-linux-android21 \
		-O2 \
		-shared \
		-fPIC \
		-o libopennovel-armv7a.so \
		-DUSE_DLL \
		-I../../src \
		-I../../src/khronos \
		-I./libroot-armv7a/include \
		-I./libroot-armv7a/include/png \
		-I./libroot-armv7a/include/freetype2 \
		$(SRCS) \
		-lm \
		-lGLESv3 \
		-lOpenSLES \
		-llog \
		-L./libroot-armv7a/lib \
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

libopennovel-x86_64.so: ndk
	@echo 'Building x86_64 library...'
	@../../external/build-libs.sh x86_64 `pwd`/ndk/bin/clang-18 "--target=x86_64-linux-android21 -O2" `pwd`/ndk/bin/llvm-ar
	@`pwd`/ndk/bin/clang-18 \
		--target=x86_64-linux-android21 \
		-O2 \
		-shared \
		-fPIC \
		-o libopennovel-x86_64.so \
		-DUSE_DLL \
		-I../../src \
		-I../../src/khronos \
		-I./libroot-x86_64/include \
		-I./libroot-x86_64/include/png \
		-I./libroot-x86_64/include/freetype2 \
		$(SRCS) \
		-lm \
		-lGLESv3 \
		-lOpenSLES \
		-llog \
		-L./libroot-x86_64/lib \
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

libopennovel-i686.so: ndk
	@echo 'Building i686 library...'
	@../../external/build-libs.sh i686 `pwd`/ndk/bin/clang-18 "--target=i686-linux-android21 -O2" `pwd`/ndk/bin/llvm-ar
	@`pwd`/ndk/bin/clang-18 \
		--target=i686-linux-android21 \
		-O2 \
		-shared \
		-fPIC \
		-o libopennovel-i686.so \
		-DUSE_DLL \
		-I../../src \
		-I../../src/khronos \
		-I./libroot-i686/include \
		-I./libroot-i686/include/png \
		-I./libroot-i686/include/freetype2 \
		$(SRCS) \
		-lm \
		-lGLESv3 \
		-lOpenSLES \
		-llog \
		-L./libroot-i686/lib \
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

ndk:
	@curl -O https://dl.google.com/android/repository/android-ndk-r27-darwin.dmg
	@7z x android-ndk-r27-darwin.dmg
	@mv 'Android NDK r27/AndroidNDK12077973.app/Contents/NDK/toolchains/llvm/prebuilt/darwin-x86_64' ndk
	@chmod -R +x ./ndk/bin/*
	@rm ndk/bin/ld.lld
	@ln -s lld ndk/bin/ld.lld
	@rm -rf 'Android NDK r27'

clean:
	@rm -rf android-src android-ndk-r27-darwin.dmg ndk libroot-* libopennovel-*.so
