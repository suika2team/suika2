HDRS_MAIN = \
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
	../../src/wms.h

SRCS_MAIN = \
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
	../../src/cmd_volume.c

FILES_IOS= \
	engine-ios/main.m \
	engine-ios/AppDelegate.h \
	engine-ios/AppDelegate.m \
	engine-ios/SceneDelegate.h \
	engine-ios/SceneDelegate.m \
	engine-ios/ViewController.h \
	engine-ios/ViewController.m \
	engine-ios/GameView.h \
	engine-ios/GameView.m \
	../../src/apple/aunit.h \
	../../src/apple/aunit.c \
	../../src/apple/GameRenderer.h \
	../../src/apple/GameShaderTypes.h \
	../../src/apple/GameViewControllerProtocol.h \
	../../src/apple/GameRenderer.m \
	../../src/apple/GameShaders.metal

src: libroot-device libroot-sim
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
	@IPHONEOS_DEPLOYMENT_TARGET=12.0 ../../external/build-libs.sh device clang "-O2 -arch arm64 -isysroot `xcrun -sdk iphoneos --show-sdk-path` -fembed-bitcode -mios-version-min=8.0" ar

libroot-sim:
	@IPHONEOS_DEPLOYMENT_TARGET=12.0 ../../external/build-libs.sh sim clang "-O2 -arch arm64 -isysroot `xcrun -sdk iphonesimulator --show-sdk-path`" ar

clean:
	@rm -rf libroot-* ios-src
