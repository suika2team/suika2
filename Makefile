all:
	@echo 'Preparing iOS engine...' && cd engines/ios && make && cd ../..
	@echo 'Building macOS engine...' && cd engines/macos && make && cd ../..
	@echo 'Preparing Android engine...' && cd engines/android && make && cd ../..
	@echo 'Building Windows engine...' && cd engines/windows && make && cd ../..
	@echo 'Building Web engine...' && cd engines/wasm && make && cd ../..
	@echo 'Preparing Unity engine...' && cd engines/unity && make && cd ../..
	@cp -Ra engines/ios/libroot-sim apps/app-ios/
	@cp -Ra engines/ios/libroot-device apps/app-ios/
	@#echo 'Building Development app...' && cd apps/app-ios && make && cd ../..

clean:
	@echo 'Cleaning iOS engine...' && cd engines/ios && make clean && cd ../..
	@echo 'Cleaning macOS engine...' && cd engines/macos && make clean && cd ../..
	@echo 'Cleaning Android engine...' && cd engines/android && make clean && cd ../..
	@echo 'Cleaning Windows engine...' && cd engines/windows && make clean && cd ../..
	@echo 'Cleaning Wasm engine...' && cd engines/wasm && make clean && cd ../..
	@echo 'Cleaning Unity engine...' && cd engines/unity && make clean && cd ../..
	@echo 'Cleaning iOS app...' && cd apps/app-ios && make clean && cd ../..

release:
	./release.sh
