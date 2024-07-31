CPPFLAGS=-DUSE_EDITOR -DUSE_STANDALONE_PACK -I../../src
CFLAGS=-O2 -w
LDFLAGS=

SRC=../../src/package.c ../../src/log.c

all:
	@echo 'Usage:'
	@echo '  make pack'
	@echo '  make pack.exe'
	@echo

pack: $(SRC)
	$(CC) -o pack $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(SRC)

pack.exe: $(SRC)
	i686-w64-mingw32-gcc -o pack.exe -municode $(CPPFLAGS) $(CFLAGS) $(SRC) $(LDFLAGS)
