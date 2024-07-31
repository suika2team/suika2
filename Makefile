all:
	@echo 'Usage:'
	@echo '  make engine-windows'
	@echo '  make engine-macos'
	@echo '  make engine-linux'
	@echo '  make engine-freebsd'
	@echo '  make engine-wasm'

engine-windows:
	@cd engines/windows && make && cp game.exe ../../
	@echo 'See game.exe'

engine-macos:
	@cd engines/macos && make && cp game.dmg ../../
	@echo 'See game.dmg'

engine-linux:
	@cd engines/linux && make && cp game-linux ../../
	@echo 'See game-linux'

engine-freebsd:
	@cd engines/freebsd && make && cp game-freebsd ../../
	@echo 'See game-freebsd'

engine-wasm:
	@cd engine/wasm && make && cp -R html ../../game-wasm
	@echo 'See game-wasm'

release:
	./release.sh

clean:
	@echo 'Cleaning Windows engine...' && cd engines/windows && make clean && cd ../..
	@echo 'Cleaning macOS engine...' && cd engines/macos && make clean && cd ../..
	@echo 'Cleaning Linux engine...' && cd engines/linux && make clean && cd ../..
	@echo 'Cleaning FreeBSD engine...' && cd engines/freebsd && make clean && cd ../..
	@echo 'Cleaning iOS engine...' && cd engines/ios && make clean && cd ../..
	@echo 'Cleaning Android engine...' && cd engines/android && make clean && cd ../..
	@echo 'Cleaning Wasm engine...' && cd engines/wasm && make clean && cd ../..
	@echo 'Cleaning Unity engine...' && cd engines/unity && make clean && cd ../..
	@echo 'Cleaning iOS app...' && cd apps/app-ios && make clean && cd ../..
