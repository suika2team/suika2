all:
	@echo 'Usage:'
	@echo '  make engine-windows'
	@echo '  make engine-macos'
	@echo '  make engine-linux'
	@echo '  make engine-freebsd'
	@echo '  make engine-wasm'
	@echo '  make engine-unity'
	@echo '  make pro-windows'

engine-windows:
	@cd engines/windows && make libroot && make -j$(nproc) && cp engine.exe ../../
	@echo 'See engine.exe'

engine-macos:
	@cd engines/macos && make libroot && make && cp engine.dmg ../../
	@echo 'See engine.dmg'

engine-linux:
	@cd engines/linux && make libroot && make -j$(nproc) && cp engine-linux ../../
	@echo 'See engine-linux'

engine-freebsd:
	@cd engines/freebsd && make libroot && make -j$(nproc) && cp engine-freebsd ../../
	@echo 'See engine-freebsd'

engine-wasm:
	@cd engine/wasm && make && cp -R html ../../engine-wasm
	@echo 'See engine-wasm'

engine-unity:
	@cd engine/unity && make && cp -R unity-src ../../engine-unity
	@echo 'See engine-unity'

pro-windows:
	@cd apps/pro-windows && make libroot && make -j$(nproc) && cp editor.exe ../../
	@echo 'See editor.exe'

release:
	./release.sh

clean:
	@echo 'Cleaning Windows engine...' && cd engines/windows && make clean && cd ../.. && rm -rf engine.exe
	@echo 'Cleaning macOS engine...' && cd engines/macos && make clean && cd ../.. && rm -rf engine.dmg
	@echo 'Cleaning Linux engine...' && cd engines/linux && make clean && cd ../.. && rm -rf engine-linux
	@echo 'Cleaning FreeBSD engine...' && cd engines/freebsd && make clean && cd ../.. && rm -rf engine-freebsd
	@echo 'Cleaning iOS engine...' && cd engines/ios && make clean && cd ../..
	@echo 'Cleaning Android engine...' && cd engines/android && make clean && cd ../..
	@echo 'Cleaning Wasm engine...' && cd engines/wasm && make clean && cd ../.. && rm -rf engine-wasm
	@echo 'Cleaning Unity engine...' && cd engines/unity && make clean && cd ../.. && rm -rf engine-unity
	@echo 'Cleaning Windows editor...' && cd apps/pro-windows && make clean && cd ../.. && rm -rf editor.exe
