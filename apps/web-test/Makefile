CC = i686-w64-mingw32-gcc
STRIP = i686-w64-mingw32-strip

CFLAGS = \
	-O2 \
	-g0 \
	-s \
	-fno-exceptions \
	-fno-stack-protector \
	-fomit-frame-pointer \
	-finput-charset=utf-8 \
	-fexec-charset=utf-8

LDFLAGS = \
	-lwsock32 \
	-lws2_32

web-test.exe: web-test.c
	$(CC) -o web-test.exe $(CFLAGS) web-test.c $(LDFLAGS) 
	$(STRIP) --strip-all web-test.exe
