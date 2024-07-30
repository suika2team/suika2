include ../common.mk

SRCS_PROJECT = \
	engine-ios/AppDelegate.h \
	engine-ios/AppDelegate.m \
	engine-ios/SceneDelegate.h \
	engine-ios/SceneDelegate.m \
	engine-ios/ViewController.h \
	engine-ios/ViewController.m \
	engine-ios/GameView.h \
	engine-ios/GameView.m \
	engine-ios/main.m \

src: libroot-device libroot-sim $(SRCS_MAIN) $(SRCS_APPLE) $(SRCS_PROJECT)
	@rm -rf ios-src
	@mkdir ios-src
	@cp -R engine-ios engine-ios.xcodeproj build.sh ExportOptions.plist ios-src/
	@sed -e 's|../../src/apple/|engine-ios/apple/|g' < ios-src/engine-ios.xcodeproj/project.pbxproj > tmpfile && mv tmpfile ios-src/engine-ios.xcodeproj/project.pbxproj
	@sed -e 's|../../src/|engine-ios/|g' < ios-src/engine-ios.xcodeproj/project.pbxproj > tmpfile && mv tmpfile ios-src/engine-ios.xcodeproj/project.pbxproj
	@mkdir -p ios-src/engine-ios/apple
	@cp ../../src/apple/GameRenderer.h ios-src/engine-ios/apple/
	@cp ../../src/apple/GameRenderer.m ios-src/engine-ios/apple/
	@cp ../../src/apple/GameShaders.metal ios-src/engine-ios/apple/
	@cp ../../src/apple/GameShaderTypes.h ios-src/engine-ios/apple/
	@cp ../../src/apple/GameViewControllerProtocol.h ios-src/engine-ios/apple/
	@cp ../../src/apple/aunit.c ios-src/engine-ios/apple/
	@cp ../../src/apple/aunit.h ios-src/engine-ios/apple/
	@cp ../../src/anime.c ios-src/engine-ios/
	@cp ../../src/anime.h ios-src/engine-ios/
	@cp ../../src/ciel.c ios-src/engine-ios/
	@cp ../../src/ciel.h ios-src/engine-ios/
	@cp ../../src/conf.c ios-src/engine-ios/
	@cp ../../src/conf.h ios-src/engine-ios/
	@cp ../../src/event.c ios-src/engine-ios/
	@cp ../../src/event.h ios-src/engine-ios/
	@cp ../../src/file.h ios-src/engine-ios/
	@cp ../../src/file.c ios-src/engine-ios/
	@cp ../../src/glyph.c ios-src/engine-ios/
	@cp ../../src/glyph.h ios-src/engine-ios/
	@cp ../../src/gui.c ios-src/engine-ios/
	@cp ../../src/gui.h ios-src/engine-ios/
	@cp ../../src/history.c ios-src/engine-ios/
	@cp ../../src/history.h ios-src/engine-ios/
	@cp ../../src/image.c ios-src/engine-ios/
	@cp ../../src/image.h ios-src/engine-ios/
	@cp ../../src/key.h ios-src/engine-ios/
	@cp ../../src/log.c ios-src/engine-ios/
	@cp ../../src/log.h ios-src/engine-ios/
	@cp ../../src/main.c ios-src/engine-ios/
	@cp ../../src/main.h ios-src/engine-ios/
	@cp ../../src/mixer.c ios-src/engine-ios/
	@cp ../../src/mixer.h ios-src/engine-ios/
	@cp ../../src/motion.h ios-src/engine-ios/
	@cp ../../src/hal.h ios-src/engine-ios/
	@cp ../../src/readimage.c ios-src/engine-ios/
	@cp ../../src/readpng.c ios-src/engine-ios/
	@cp ../../src/readjpeg.c ios-src/engine-ios/
	@cp ../../src/readwebp.c ios-src/engine-ios/
	@cp ../../src/save.c ios-src/engine-ios/
	@cp ../../src/save.h ios-src/engine-ios/
	@cp ../../src/script.c ios-src/engine-ios/
	@cp ../../src/script.h ios-src/engine-ios/
	@cp ../../src/seen.c ios-src/engine-ios/
	@cp ../../src/seen.h ios-src/engine-ios/
	@cp ../../src/stage.c ios-src/engine-ios/
	@cp ../../src/stage.h ios-src/engine-ios/
	@cp ../../src/opennovel.h ios-src/engine-ios/
	@cp ../../src/types.h ios-src/engine-ios/
	@cp ../../src/vars.c ios-src/engine-ios/
	@cp ../../src/vars.h ios-src/engine-ios/
	@cp ../../src/wave.c ios-src/engine-ios/
	@cp ../../src/wave.h ios-src/engine-ios/
	@cp ../../src/wms_core.c ios-src/engine-ios/
	@cp ../../src/wms_core.h ios-src/engine-ios/
	@cp ../../src/wms.h ios-src/engine-ios/
	@cp ../../src/wms_impl.c ios-src/engine-ios/
	@cp ../../src/wms_lexer.yy.c ios-src/engine-ios/
	@cp ../../src/wms_parser.tab.c ios-src/engine-ios/
	@cp ../../src/wms_parser.tab.h ios-src/engine-ios/
	@cp ../../src/cmd_anime.c ios-src/engine-ios/
	@cp ../../src/cmd_bg.c ios-src/engine-ios/
	@cp ../../src/cmd_chapter.c ios-src/engine-ios/
	@cp ../../src/cmd_ch.c ios-src/engine-ios/
	@cp ../../src/cmd_chch.c ios-src/engine-ios/
	@cp ../../src/cmd_choose.c ios-src/engine-ios/
	@cp ../../src/cmd_click.c ios-src/engine-ios/
	@cp ../../src/cmd_config.c ios-src/engine-ios/
	@cp ../../src/cmd_gosub.c ios-src/engine-ios/
	@cp ../../src/cmd_goto.c ios-src/engine-ios/
	@cp ../../src/cmd_if.c ios-src/engine-ios/
	@cp ../../src/cmd_layer.c ios-src/engine-ios/
	@cp ../../src/cmd_menu.c ios-src/engine-ios/
	@cp ../../src/cmd_message.c ios-src/engine-ios/
	@cp ../../src/cmd_music.c ios-src/engine-ios/
	@cp ../../src/cmd_move.c ios-src/engine-ios/
	@cp ../../src/cmd_plugin.c ios-src/engine-ios/
	@cp ../../src/cmd_return.c ios-src/engine-ios/
	@cp ../../src/cmd_set.c ios-src/engine-ios/
	@cp ../../src/cmd_shake.c ios-src/engine-ios/
	@cp ../../src/cmd_skip.c ios-src/engine-ios/
	@cp ../../src/cmd_sound.c ios-src/engine-ios/
	@cp ../../src/cmd_story.c ios-src/engine-ios/
	@cp ../../src/cmd_text.c ios-src/engine-ios/
	@cp ../../src/cmd_time.c ios-src/engine-ios/
	@cp ../../src/cmd_video.c ios-src/engine-ios/
	@cp ../../src/cmd_volume.c ios-src/engine-ios/
	@cp -Ra libroot-device ios-src/
	@cp -Ra libroot-sim ios-src/
	@mkdir ios-src/Resources
	@mkdir ios-src/Resources/mov

libroot-device:
	@echo 'Building arm64 device libraries...'
	@IPHONEOS_DEPLOYMENT_TARGET=12.0 ../../external/build-libs.sh device clang "-O2 -g0 -arch arm64 -isysroot `xcrun -sdk iphoneos --show-sdk-path` -fembed-bitcode -mios-version-min=12.0" ar

libroot-sim:
	@echo 'Building arm64 simulator libraries...'
	@IPHONEOS_DEPLOYMENT_TARGET=12.0 ../../external/build-libs.sh sim-arm64 clang "-O2 -g0 -arch arm64 -isysroot `xcrun -sdk iphonesimulator --show-sdk-path`" ar
	@
	@echo 'Building x86_64 simulator libraries...'
	@IPHONEOS_DEPLOYMENT_TARGET=12.0 ../../external/build-libs.sh sim-x86_64 clang "-O2 -g0 -arch x86_64 -isysroot `xcrun -sdk iphonesimulator --show-sdk-path`" ar
	@
	@echo 'Merging into Universal Binary...'
	@mkdir -p libroot-sim/include libroot-sim/lib
	@cp -Ra libroot-sim-arm64/include/* libroot-sim/include/
	@lipo -create libroot-sim-arm64/lib/libpng.a libroot-sim-x86_64/lib/libpng.a -output libroot-sim/lib/libpng.a
	@lipo -create libroot-sim-arm64/lib/libjpeg.a libroot-sim-x86_64/lib/libjpeg.a -output libroot-sim/lib/libjpeg.a
	@lipo -create libroot-sim-arm64/lib/libwebp.a libroot-sim-x86_64/lib/libwebp.a -output libroot-sim/lib/libwebp.a
	@lipo -create libroot-sim-arm64/lib/libvorbis.a libroot-sim-x86_64/lib/libvorbis.a -output libroot-sim/lib/libvorbis.a
	@lipo -create libroot-sim-arm64/lib/libogg.a libroot-sim-x86_64/lib/libogg.a -output libroot-sim/lib/libogg.a
	@lipo -create libroot-sim-arm64/lib/libfreetype.a libroot-sim-x86_64/lib/libfreetype.a -output libroot-sim/lib/libfreetype.a
	@lipo -create libroot-sim-arm64/lib/libbrotlicommon.a libroot-sim-x86_64/lib/libbrotlicommon.a -output libroot-sim/lib/libbrotlicommon.a
	@lipo -create libroot-sim-arm64/lib/libbrotlidec.a libroot-sim-x86_64/lib/libbrotlidec.a -output libroot-sim/lib/libbrotlidec.a
	@lipo -create libroot-sim-arm64/lib/libbz2.a libroot-sim-x86_64/lib/libbz2.a -output libroot-sim/lib/libbz2.a
	@lipo -create libroot-sim-arm64/lib/libz.a libroot-sim-x86_64/lib/libz.a -output libroot-sim/lib/libz.a
	@rm -rf libroot-sim-arm64 libroot-sim-x86_64

clean:
	@rm -rf libroot-* ios-src
