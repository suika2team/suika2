SRCS = \
	../../src/opennovel.h \
	../../src/anime.h \
	../../src/conf.h \
	../../src/event.h \
	../../src/file.h \
	../../src/glyph.h \
	../../src/gui.h \
	../../src/history.h \
	../../src/image.h \
	../../src/log.h \
	../../src/main.h \
	../../src/mixer.h \
	../../src/motion.h \
	../../src/save.h \
	../../src/script.h \
	../../src/seen.h \
	../../src/stage.h \
	../../src/vars.h \
	../../src/wave.h \
	../../src/wms.h \
	\
	../../src/anime.c \
	../../src/conf.c \
	../../src/ciel.c \
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
	\
	engine-macos/main.m \
	engine-macos/AppDelegate.h \
	engine-macos/AppDelegate.m \
	engine-macos/ViewController.h \
	engine-macos/ViewController.m \
	engine-macos/GameView.h \
	engine-macos/GameView.m \
	\
	../../src/apple/aunit.h \
	../../src/apple/aunit.c \
	../../src/apple/GameRenderer.h \
	../../src/apple/GameShaderTypes.h \
	../../src/apple/GameViewControllerProtocol.h \
	../../src/apple/GameRenderer.m \
	../../src/apple/GameShaders.metal

all: src game.dmg

game.dmg: libroot $(SRCS)
	@echo "Building Game.app"
	@rm -rf build
	@echo 'archiving...'
	@xcodebuild \
		-quiet \
		-scheme engine-macos \
		-project engine-macos.xcodeproj \
		-configuration Release \
		-archivePath `pwd`/build/Release/engine-macos.xcarchive \
		archive
	@echo 'uploading...'
	@xcodebuild \
		-quiet \
		-exportArchive \
		-archivePath `pwd`/build/Release/engine-macos.xcarchive \
		-exportOptionsPlist ExportOptions.plist
	@echo 'exporting...'
	@until \
		xcodebuild \
			-quiet \
			-exportNotarizedApp \
			-archivePath `pwd`/build/Release/engine-macos.xcarchive \
			-exportPath `pwd`/build/Release; \
	do \
		echo "Waiting 10 seconds for notarization..."; \
		sleep 10; \
	done
	@echo "Successfully notarized."
	@rm -f game.dmg
	@mkdir tmp
	@cp -Rv build/Release/Game.app tmp/
	@hdiutil create -fs HFS+ -format UDBZ -srcfolder tmp -volname Game game.dmg
	@codesign --sign 'Developer ID Application' game.dmg
	@rm -rf tmp

src: libroot
	@rm -rf macos-src
	@mkdir macos-src
	@cp -R engine-macos engine-macos.xcodeproj macos-src/
	@sed -e 's|../../src/apple/|engine-macos/apple/|g' < macos-src/engine-macos.xcodeproj/project.pbxproj > tmp && mv tmp macos-src/engine-macos.xcodeproj/project.pbxproj
	@sed -e 's|../../src/|engine-macos/|g' < macos-src/engine-macos.xcodeproj/project.pbxproj > tmp && mv tmp macos-src/engine-macos.xcodeproj/project.pbxproj
	@mkdir -p macos-src/engine-macos/apple
	@cp ../../src/apple/GameRenderer.h macos-src/engine-macos/apple/
	@cp ../../src/apple/GameRenderer.m macos-src/engine-macos/apple/
	@cp ../../src/apple/GameShaders.metal macos-src/engine-macos/apple/
	@cp ../../src/apple/GameShaderTypes.h macos-src/engine-macos/apple/
	@cp ../../src/apple/GameViewControllerProtocol.h macos-src/engine-macos/apple/
	@cp ../../src/apple/aunit.c macos-src/engine-macos/apple/
	@cp ../../src/apple/aunit.h macos-src/engine-macos/apple/
	@cp ../../src/anime.c macos-src/engine-macos/
	@cp ../../src/anime.h macos-src/engine-macos/
	@cp ../../src/ciel.c macos-src/engine-macos/
	@cp ../../src/ciel.h macos-src/engine-macos/
	@cp ../../src/conf.c macos-src/engine-macos/
	@cp ../../src/conf.h macos-src/engine-macos/
	@cp ../../src/event.c macos-src/engine-macos/
	@cp ../../src/event.h macos-src/engine-macos/
	@cp ../../src/file.h macos-src/engine-macos/
	@cp ../../src/file.c macos-src/engine-macos/
	@cp ../../src/glyph.c macos-src/engine-macos/
	@cp ../../src/glyph.h macos-src/engine-macos/
	@cp ../../src/gui.c macos-src/engine-macos/
	@cp ../../src/gui.h macos-src/engine-macos/
	@cp ../../src/history.c macos-src/engine-macos/
	@cp ../../src/history.h macos-src/engine-macos/
	@cp ../../src/image.c macos-src/engine-macos/
	@cp ../../src/image.h macos-src/engine-macos/
	@cp ../../src/key.h macos-src/engine-macos/
	@cp ../../src/log.c macos-src/engine-macos/
	@cp ../../src/log.h macos-src/engine-macos/
	@cp ../../src/main.c macos-src/engine-macos/
	@cp ../../src/main.h macos-src/engine-macos/
	@cp ../../src/mixer.c macos-src/engine-macos/
	@cp ../../src/mixer.h macos-src/engine-macos/
	@cp ../../src/motion.h macos-src/engine-macos/
	@cp ../../src/hal.h macos-src/engine-macos/
	@cp ../../src/readimage.c macos-src/engine-macos/
	@cp ../../src/readpng.c macos-src/engine-macos/
	@cp ../../src/readjpeg.c macos-src/engine-macos/
	@cp ../../src/readwebp.c macos-src/engine-macos/
	@cp ../../src/save.c macos-src/engine-macos/
	@cp ../../src/save.h macos-src/engine-macos/
	@cp ../../src/script.c macos-src/engine-macos/
	@cp ../../src/script.h macos-src/engine-macos/
	@cp ../../src/seen.c macos-src/engine-macos/
	@cp ../../src/seen.h macos-src/engine-macos/
	@cp ../../src/stage.c macos-src/engine-macos/
	@cp ../../src/stage.h macos-src/engine-macos/
	@cp ../../src/opennovel.h macos-src/engine-macos/
	@cp ../../src/types.h macos-src/engine-macos/
	@cp ../../src/vars.c macos-src/engine-macos/
	@cp ../../src/vars.h macos-src/engine-macos/
	@cp ../../src/wave.c macos-src/engine-macos/
	@cp ../../src/wave.h macos-src/engine-macos/
	@cp ../../src/wms_core.c macos-src/engine-macos/
	@cp ../../src/wms_core.h macos-src/engine-macos/
	@cp ../../src/wms.h macos-src/engine-macos/
	@cp ../../src/wms_impl.c macos-src/engine-macos/
	@cp ../../src/wms_lexer.yy.c macos-src/engine-macos/
	@cp ../../src/wms_parser.tab.c macos-src/engine-macos/
	@cp ../../src/wms_parser.tab.h macos-src/engine-macos/
	@cp ../../src/cmd_anime.c macos-src/engine-macos/
	@cp ../../src/cmd_bg.c macos-src/engine-macos/
	@cp ../../src/cmd_chapter.c macos-src/engine-macos/
	@cp ../../src/cmd_ch.c macos-src/engine-macos/
	@cp ../../src/cmd_chch.c macos-src/engine-macos/
	@cp ../../src/cmd_choose.c macos-src/engine-macos/
	@cp ../../src/cmd_click.c macos-src/engine-macos/
	@cp ../../src/cmd_config.c macos-src/engine-macos/
	@cp ../../src/cmd_gosub.c macos-src/engine-macos/
	@cp ../../src/cmd_goto.c macos-src/engine-macos/
	@cp ../../src/cmd_if.c macos-src/engine-macos/
	@cp ../../src/cmd_layer.c macos-src/engine-macos/
	@cp ../../src/cmd_menu.c macos-src/engine-macos/
	@cp ../../src/cmd_message.c macos-src/engine-macos/
	@cp ../../src/cmd_move.c macos-src/engine-macos/
	@cp ../../src/cmd_music.c macos-src/engine-macos/
	@cp ../../src/cmd_return.c macos-src/engine-macos/
	@cp ../../src/cmd_set.c macos-src/engine-macos/
	@cp ../../src/cmd_sound.c macos-src/engine-macos/
	@cp ../../src/cmd_story.c macos-src/engine-macos/
	@cp ../../src/cmd_shake.c macos-src/engine-macos/
	@cp ../../src/cmd_skip.c macos-src/engine-macos/
	@cp ../../src/cmd_text.c macos-src/engine-macos/
	@cp ../../src/cmd_time.c macos-src/engine-macos/
	@cp ../../src/cmd_video.c macos-src/engine-macos/
	@cp ../../src/cmd_volume.c macos-src/engine-macos/
	@cp ../../src/cmd_plugin.c macos-src/engine-macos/
	@mkdir macos-src/Resources/
	@cp -Ra libroot macos-src/

libroot:
	@echo 'Building arm64 libraries..'
	@MACOSX_DEPLOYMENT_TARGET=10.13 ../../external/build-libs.sh "arm64" clang "-arch arm64 -Dmktemp=mkdtemp" ar
	@echo 'Building x86_64 libraries..'
	@MACOSX_DEPLOYMENT_TARGET=10.13 ../../external/build-libs.sh "x86_64" clang "-arch x86_64 -Dmktemp=mkdtemp" ar
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
	@rm -rf build libroot* macos-src game.dmg

