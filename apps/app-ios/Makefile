all: libroot-ios-device libroot-ios-sim libroot-macos

libroot-ios-device:
	@../../external/build-libs.sh ios-device clang "-O2 -arch arm64 -isysroot `xcrun -sdk iphoneos --show-sdk-path` -fembed-bitcode -mios-version-min=8.0" ar

libroot-ios-sim:
	@../../external/build-libs.sh sim clang "-O2 -arch arm64 -arch x86_64 -isysroot `xcrun -sdk iphonesimulator --show-sdk-path`" ar

libroot-macos:
	@../../external/build-libs.sh macos clang "-O2 -arch arm64 -arch x86_64" ar

clean:
	@rm -rf build libroot libroot-ios-device libroot-ios-sim libroot-macos
