CPPFLAGS=-DUSE_EDITOR -DUSE_STANDALONE_PACK -I../../src
CFLAGS=-O2 -w
LDFLAGS=

SRC=../../src/package.c ../../src/log.c

pack: $(SRC)
	$(CC) -o pack $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(SRC)
