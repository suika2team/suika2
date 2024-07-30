all: analysis.o barkmel.o bitrate.o block.o codebook.o envelope.o floor0.o floor1.o info.o lookup.o lpc.o lsp.o mapping0.o mdct.o psy.o registry.o res0.o sharedbook.o smallft.o synthesis.o tone.o vorbisenc.o vorbisfile.o window.o
	$(AR) rcs $(PREFIX)/lib/libvorbis.a analysis.o barkmel.o bitrate.o block.o codebook.o envelope.o floor0.o floor1.o info.o lookup.o lpc.o lsp.o mapping0.o mdct.o psy.o registry.o res0.o sharedbook.o smallft.o synthesis.o tone.o vorbisenc.o vorbisfile.o window.o

analysis.o: lib/analysis.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

barkmel.o: lib/barkmel.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

bitrate.o: lib/bitrate.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

block.o: lib/block.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

codebook.o: lib/codebook.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

envelope.o: lib/envelope.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

floor0.o: lib/floor0.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

floor1.o: lib/floor1.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

info.o: lib/info.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

lookup.o: lib/lookup.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

lpc.o: lib/lpc.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

lsp.o: lib/lsp.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

mapping0.o: lib/mapping0.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

mdct.o: lib/mdct.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

psy.o: lib/psy.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

registry.o: lib/registry.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

res0.o: lib/res0.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

sharedbook.o: lib/sharedbook.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

smallft.o: lib/smallft.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

synthesis.o: lib/synthesis.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

tone.o: lib/tone.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

vorbisenc.o: lib/vorbisenc.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

vorbisfile.o: lib/vorbisfile.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<

window.o: lib/window.c
	$(CC) -I./include -I./lib -I$(PREFIX)/include -fPIC $(CFLAGS) -c $<
