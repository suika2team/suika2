include ../common.mk

SRCS = $(SRCS_MAIN_MINUS_FILE) $(SRCS_ANDROID) $(SRCS_OPENGL) $(SRCS_OPENSL)

src: libopennovel-aarch64.so libopennovel-armv7a.so libopennovel-x86_64.so libopennovel-i686.so
	@rm -rf app/build app/.cxx app/src/main/cpp app/src/main/java app/src/main/assets android-src
	@mkdir -p android-src
	@mkdir -p android-src/app/src/main/cpp
	@mkdir -p android-src/app/src/main/assets
	@
	@cp -R app android-src/
	@cp -R gradle.properties android-src/
	@cp -R build.gradle android-src/
	@cp -R gradlew android-src/
	@cp -R settings.gradle android-src/
	@cp -R gradlew.bat android-src/
	@cp -R gradle android-src/
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
	@../../external/build-libs.sh aarch64 `pwd`/ndk/bin/clang-18 "--target=aarch64-linux-android21 -O2 -g0 -fPIC" `pwd`/ndk/bin/llvm-ar
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
	@../../external/build-libs.sh armv7a `pwd`/ndk/bin/clang-18 "--target=armv7a-linux-androideabi21 -O2 -g0 -fPIC" `pwd`/ndk/bin/llvm-ar
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
	@../../external/build-libs.sh x86_64 `pwd`/ndk/bin/clang-18 "--target=x86_64-linux-android21 -O2 -g0 -fPIC" `pwd`/ndk/bin/llvm-ar
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
	@../../external/build-libs.sh i686 `pwd`/ndk/bin/clang-18 "--target=i686-linux-android21 -O2 -g0 -fPIC" `pwd`/ndk/bin/llvm-ar
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
