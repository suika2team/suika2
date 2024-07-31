CC = i686-w64-mingw32-gcc
STRIP = i686-w64-mingw32-strip

exporter.exe: exporter.c ../../src/package.c ../../src/log.c
	$(CC) -o exporter.exe -DUSE_EDITOR -O2 -municode -mwindows exporter.c ../../src/package.c ../../src/log.c
