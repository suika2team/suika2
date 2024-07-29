help:
	@echo 'Usage:'
	@echo '  make ps45   ... make a DLL for PlayStation 4/5'
	@echo '  make xbox   ... make a DLL for Xbox Series X|S'
	@echo '  make switch ... make a DLL for Switch'

ps45:
	rm -f Assets/*.so
	cd dll-src && make -f ps45.mk && cd ..

xbox
	rm -f Assets/*.dll
	cd dll-src && make -f xbox.mk && cd ..

switch:
	rm -f Assets/*.nso
	../../external/build-libs.sh switch /opt/devkitpro/devkitA64/bin/aarch64-none-elf-gcc \"\" /opt/devkitpro/devkitA64/bin/aarch64-none-elf-ar"
