all: exporter.dmg

exporter.dmg: libroot
	@echo "Building Exporter.app"
	@rm -rf build
	@echo 'archiving...'
	@xcodebuild \
		-quiet \
		-scheme exporter-macos \
		-project exporter-macos.xcodeproj \
		-configuration Release \
		-archivePath `pwd`/build/Release/exporter-macos.xcarchive \
		archive
	@echo 'uploading...'
	@xcodebuild \
		-quiet \
		-exportArchive \
		-archivePath `pwd`/build/Release/exporter-macos.xcarchive \
		-exportOptionsPlist ExportOptions.plist
	@echo 'exporting...'
	@until \
		xcodebuild \
			-quiet \
			-exportNotarizedApp \
			-archivePath `pwd`/build/Release/exporter-macos.xcarchive \
			-exportPath `pwd`/build/Release; \
	do \
		echo "Waiting 10 seconds for notarization..."; \
		sleep 10; \
	done
	@echo "Successfully notarized."
	@rm -f exporter.dmg
	@mkdir tmp
	@cp -Rv build/Release/Exporter.app tmp/
	@hdiutil create -fs HFS+ -format UDBZ -srcfolder tmp -volname Exporter exporter.dmg
	@codesign --sign 'Developer ID Application' exporter.dmg
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
	@rm -rf build libroot* exporter.dmg
