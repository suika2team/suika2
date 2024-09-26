CC=i686-w64-mingw32-gcc
CPPFLAGS=-DUSE_EDITOR -DUSE_STANDALONE_PACK -I../../src
CFLAGS=-O2 -w -municode
LDFLAGS=

SRC=../../src/package.c ../../src/log.c

pack.exe: $(SRC)
	$(CC) -o pack.exe $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(SRC)
