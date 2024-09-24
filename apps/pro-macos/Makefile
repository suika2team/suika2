FILES_MAIN = \
	../../src/anime.h \
	../../src/anime.c \
	../../src/ciel.h \
	../../src/ciel.c \
	../../src/conf.h \
	../../src/conf.c \
	../../src/event.h \
	../../src/event.c \
	../../src/file.h \
	../../src/file.c \
	../../src/glyph.h \
	../../src/glyph.c \
	../../src/gui.h \
	../../src/gui.c \
	../../src/history.h \
	../../src/history.c \
	../../src/image.h \
	../../src/image.c \
	../../src/log.h \
	../../src/log.c \
	../../src/main.h \
	../../src/main.c \
	../../src/mixer.h \
	../../src/mixer.c \
	../../src/readimage.c \
	../../src/readjpeg.c \
	../../src/readpng.c \
	../../src/readwebp.c \
	../../src/save.h \
	../../src/save.c \
	../../src/script.h \
	../../src/script.c \
	../../src/seen.h \
	../../src/seen.c \
	../../src/stage.h \
	../../src/stage.c \
	../../src/vars.h \
	../../src/vars.c \
	../../src/wave.h \
	../../src/wave.c \
	../../src/wms.h \
	../../src/wms_core.h \
	../../src/wms_core.c \
	../../src/wms_impl.c \
	../../src/wms_lexer.yy.c \
	../../src/wms_parser.tab.c \
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

FILES_MACOS = \
	pro-macos/main.m \
	pro-macos/AppDelegate.h \
	pro-macos/AppDelegate.m \
	pro-macos/ViewController.h \
	pro-macos/ViewController.m \
	pro-macos/GameView.h \
	pro-macos/GameView.m \
	../../src/apple/aunit.h \
	../../src/apple/aunit.c \
	../../src/apple/GameRenderer.h \
	../../src/apple/GameShaderTypes.h \
	../../src/apple/GameViewControllerProtocol.h \
	../../src/apple/GameRenderer.m \
	../../src/apple/GameShaders.metal

opennovel.dmg: libroot $(FILES_MAIN) $(FILES_MACOS)
	@echo "Building OpenNovel.app"
	@rm -rf build
	@echo 'Archiving...'
	@xcodebuild \
		-quiet \
		-scheme pro-macos \
		-project pro-macos.xcodeproj \
		-configuration Release \
		-archivePath `pwd`/build/Release/pro-macos.xcarchive \
		archive
	@echo 'Uploading...'
	@xcodebuild \
		-quiet \
		-exportArchive \
		-archivePath `pwd`/build/Release/pro-macos.xcarchive \
		-exportOptionsPlist ExportOptions.plist
	@rm -rf "build/Release/OpenNovel.app"
	@echo 'Exporting...'
	@until \
		xcodebuild \
			-quiet \
			-exportNotarizedApp \
			-archivePath `pwd`/build/Release/pro-macos.xcarchive \
			-exportPath `pwd`/build/Release > /dev/null 2>&1; \
	do \
		echo "Waiting 10 seconds for notarization..."; \
		sleep 10; \
	done
	@echo "Successfully notarized."
	@rm -rf tmp *.dmg
	@mkdir tmp
	@cp -Ra "build/Release/OpenNovel.app" "tmp/OpenNovel.app"
	@echo "Creating a dmg file."
	@hdiutil create -fs APFS -format UDBZ -srcfolder tmp -volname "OpenNovel" "opennovel.dmg"
	@echo "Signing the dmg file."
	@codesign --sign 'Developer ID Application' opennovel.dmg
	@rm -rf tmp

libroot:
	@echo 'Building arm64 libraries..'
	@MACOSX_DEPLOYMENT_TARGET=10.13 ../../external/build-libs.sh "arm64" clang "-O2 -g0 -arch arm64" ar
	@
	@echo 'Building x86_64 libraries..'
	@MACOSX_DEPLOYMENT_TARGET=10.13 ../../external/build-libs.sh "x86_64" clang "-O2 -g0 -arch x86_64" ar
	@
	@echo 'Merging into Univeral Binary libraries...'
	@mkdir -p libroot/include libroot/lib
	@cp -Ra libroot-arm64/include/* libroot/include/
	@lipo -create libroot-arm64/lib/libpng.a libroot-x86_64/lib/libpng.a -output libroot/lib/libpng.a
	@lipo -create libroot-arm64/lib/libjpeg.a libroot-x86_64/lib/libjpeg.a -output libroot/lib/libjpeg.a
	@lipo -create libroot-arm64/lib/libwebp.a libroot-x86_64/lib/libwebp.a -output libroot/lib/libwebp.a
	@lipo -create libroot-arm64/lib/libvorbis.a libroot-x86_64/lib/libvorbis.a -output libroot/lib/libvorbis.a
	@lipo -create libroot-arm64/lib/libogg.a libroot-x86_64/lib/libogg.a -output libroot/lib/libogg.a
	@lipo -create libroot-arm64/lib/libfreetype.a libroot-x86_64/lib/libfreetype.a -output libroot/lib/libfreetype.a
	@lipo -create libroot-arm64/lib/libbrotlicommon.a libroot-x86_64/lib/libbrotlicommon.a -output libroot/lib/libbrotlicommon.a
	@lipo -create libroot-arm64/lib/libbrotlidec.a libroot-x86_64/lib/libbrotlidec.a -output libroot/lib/libbrotlidec.a
	@lipo -create libroot-arm64/lib/libbz2.a libroot-x86_64/lib/libbz2.a -output libroot/lib/libbz2.a
	@lipo -create libroot-arm64/lib/libz.a libroot-x86_64/lib/libz.a -output libroot/lib/libz.a
	@rm -rf libroot-arm64 libroot-x86_64

clean:
	@rm -rf build libroot* opennovel.dmg
