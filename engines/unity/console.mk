help:
	@echo 'Usage:'
	@echo '  make ps45   ... make a DLL for PlayStation 4/5'
	@echo '  make xbox   ... make a DLL for Xbox Series X|S'
	@echo '  make switch ... make a DLL for Switch'

ps45:
	rm -f Assets/*.so Assets/*.dll Assets/*.nso
	cd dll-src && make -f ps45.mk && cd ..

xbox
	rm -f Assets/*.so Assets/*.dll Assets/*.nso
	cd dll-src && make -f xbox.mk && cd ..

switch:
	rm -f Assets/*.so Assets/*.dll Assets/*.nso
	cd dll-src && make -f switch.mk && cd ..
