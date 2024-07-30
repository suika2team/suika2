CC = i686-w64-mingw32-gcc
STRIP = i686-w64-mingw32-strip

CFLAGS = \
	-O2 \
	-g0 \
	-s \
	-fno-exceptions \
	-fno-stack-protector \
	-fomit-frame-pointer \
	-municode \
	-finput-charset=utf-8 \
	-fexec-charset=utf-8 \
	-Wall \
	-Werror \
	-Wextra \
	-Wundef \
	-Wconversion

LDFLAGS = \
	-mwindows

exporter.exe: exporter.c ../../src/package.c ../../src/log.c
	$(CC) -o exporter.exe exporter.c ../../src/package.c ../../src/log.c $(CFLAGS) $(LDFLAGS)
	$(STRIP) --strip-all exporter.exe
