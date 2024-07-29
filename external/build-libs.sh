#!/bin/sh

set -eu

MACHINE=$1
CC=$2
CFLAGS=$3
AR=$4

if [ ! -z $MACHINE ]; then
    PREFIX=`pwd`/libroot-$MACHINE;
else
    PREFIX=`pwd`/libroot;
fi

rm -rf tmp $PREFIX
mkdir -p tmp $PREFIX
mkdir -p $PREFIX/include $PREFIX/lib

cd tmp

echo 'Building brotli...'
tar xzf ../../../external/brotli-1.1.0.tar.gz
cd brotli-1.1.0
$CC \
	-I./c/include \
	-fPIC \
	$CFLAGS \
	-c \
	c/common/constants.c \
	c/common/dictionary.c \
	c/common/shared_dictionary.c \
	c/common/context.c \
	c/common/platform.c \
	c/common/transform.c \
	c/dec/bit_reader.c \
	c/dec/decode.c \
	c/dec/huffman.c \
	c/dec/state.c
$AR \
	rcs \
	$PREFIX/lib/libbrotlicommon.a \
	constants.o \
	dictionary.o \
	shared_dictionary.o \
	context.o \
	platform.o \
	transform.o
$AR \
	rcs \
	$PREFIX/lib/libbrotlidec.a \
	bit_reader.o \
	decode.o \
	huffman.o \
	state.o
cp -R c/include/brotli $PREFIX/include/
cd ..

echo 'Building bzip2...'
tar xzf ../../../external/bzip2-1.0.6.tar.gz
cd bzip2-1.0.6
$CC \
	-I./c/include \
	-fPIC \
	$CFLAGS \
	-c \
	-Wno-pointer-sign \
	unzcrash.c \
	blocksort.c \
	compress.c \
	decompress.c \
	huffman.c \
	randtable.c \
	bzlib.c \
	crctable.c
$AR \
	rcs \
	$PREFIX/lib/libbz2.a \
	blocksort.o \
	compress.o \
	decompress.o \
	huffman.o \
	randtable.o \
	unzcrash.o \
	bzlib.o \
	crctable.o
cp bzlib.h $PREFIX/include/
cd ..

echo 'Building libwebp...'
tar xzf ../../../external/libwebp-1.3.2.tar.gz
cd libwebp-1.3.2
sed \
    -e 's|#define WEBP_USE_NEON|#undef WEBP_USE_NEON|g' \
    -e 's|#define WEBP_USE_SSE2|#undef WEBP_USE_SSE2|g' \
    < src/dsp/cpu.h \
    > src/dsp/cpu.h.new
mv src/dsp/cpu.h.new src/dsp/cpu.h
$CC \
	-I. \
	-I./src \
	-fPIC \
	$CFLAGS \
	-c \
	src/dec/alpha_dec.c \
	src/dec/buffer_dec.c \
	src/dec/frame_dec.c \
	src/dec/idec_dec.c \
	src/dec/io_dec.c \
	src/dec/quant_dec.c \
	src/dec/tree_dec.c \
	src/dec/vp8_dec.c \
	src/dec/vp8l_dec.c \
	src/dec/webp_dec.c \
	src/demux/anim_decode.c \
	src/demux/demux.c \
	src/dsp/alpha_processing.c \
	src/dsp/cost.c \
	src/dsp/cost_mips32.c \
	src/dsp/cpu.c \
	src/dsp/dec.c \
	src/dsp/dec_clip_tables.c \
	src/dsp/enc.c \
	src/dsp/filters.c \
	src/dsp/lossless.c \
	src/dsp/lossless_enc.c \
	src/dsp/rescaler.c \
	src/dsp/ssim.c \
	src/dsp/upsampling.c \
	src/dsp/yuv.c \
	src/enc/alpha_enc.c \
	src/enc/analysis_enc.c \
	src/enc/backward_references_cost_enc.c \
	src/enc/backward_references_enc.c \
	src/enc/config_enc.c \
	src/enc/cost_enc.c \
	src/enc/filter_enc.c \
	src/enc/frame_enc.c \
	src/enc/histogram_enc.c \
	src/enc/iterator_enc.c \
	src/enc/near_lossless_enc.c \
	src/enc/picture_csp_enc.c \
	src/enc/picture_enc.c \
	src/enc/picture_psnr_enc.c \
	src/enc/picture_rescale_enc.c \
	src/enc/picture_tools_enc.c \
	src/enc/predictor_enc.c \
	src/enc/quant_enc.c \
	src/enc/syntax_enc.c \
	src/enc/token_enc.c \
	src/enc/tree_enc.c \
	src/enc/vp8l_enc.c \
	src/enc/webp_enc.c \
	src/mux/anim_encode.c \
	src/mux/muxedit.c \
	src/mux/muxinternal.c \
	src/mux/muxread.c \
	src/utils/bit_reader_utils.c \
	src/utils/bit_writer_utils.c \
	src/utils/color_cache_utils.c \
	src/utils/filters_utils.c \
	src/utils/huffman_encode_utils.c \
	src/utils/huffman_utils.c \
	src/utils/quant_levels_dec_utils.c \
	src/utils/quant_levels_utils.c \
	src/utils/random_utils.c \
	src/utils/rescaler_utils.c \
	src/utils/thread_utils.c \
	src/utils/utils.c
$AR rcs $PREFIX/lib/libwebp.a *.o
cp -R src/webp $PREFIX/include/
cd ..

echo 'Building zlib...'
tar xzf ../../../external/zlib-1.2.11.tar.gz
cd zlib-1.2.11
$CC \
	-I. \
	-I./src \
	-fPIC \
	$CFLAGS \
	-std=c89 \
	-w \
	-c \
	adler32.c \
	infback.c \
	trees.c \
	compress.c \
	inffast.c \
	uncompr.c \
	crc32.c \
	inflate.c \
	deflate.c \
	inftrees.c \
	zutil.c
$AR \
	rcs \
	$PREFIX/lib/libz.a \
	adler32.o \
	infback.o \
	trees.o \
	compress.o \
	inffast.o \
	uncompr.o \
	crc32.o \
	inflate.o \
	deflate.o \
	inftrees.o \
	zutil.o
cp zlib.h zconf.h $PREFIX/include/
cd ..

echo 'Building libpng...'
tar xzf ../../../external/libpng-1.6.43.tar.gz
cd libpng-1.6.43
sed \
    -e 's|/\*#undef PNG_ARM_NEON_API_SUPPORTED\*/|#undef PNG_ARM_NEON_API_SUPPORTED|g' \
    -e 's|/\*#undef PNG_ARM_NEON_CHECK_SUPPORTED\*/|#undef PNG_ARM_NEON_CHECK_SUPPORTED|g' \
    < scripts/pnglibconf.h.prebuilt \
    > pnglibconf.h
$CC \
	-I. \
	-I./src \
	-I$PREFIX/include \
	-fPIC \
	$CFLAGS \
	-DPNG_ARM_NEON_OPT=0 \
	-c \
	pngget.c \
	pngread.c \
	pngrutil.c \
	pngtrans.c \
	pngwtran.c \
	png.c \
	pngmem.c \
	pngrio.c \
	pngset.c \
	pngwio.c \
	pngwutil.c \
	pngerror.c \
	pngpread.c \
	pngrtran.c \
	pngwrite.c
$AR \
	rcs \
	$PREFIX/lib/libpng.a \
	pngget.o \
	pngread.o \
	pngrutil.o \
	pngtrans.o \
	pngwtran.o \
	png.o \
	pngmem.o \
	pngrio.o \
	pngset.o \
	pngwio.o \
	pngwutil.o \
	pngerror.o \
	pngpread.o \
	pngrtran.o \
	pngwrite.o
mkdir $PREFIX/include/png
cp *.h $PREFIX/include/png/
cd ..

echo 'Building jpeg9...'
tar xzf ../../../external/jpegsrc.v9e.tar.gz
cd jpeg-9e
cp jconfig.txt jconfig.h
$CC \
	-fPIC \
	$CFLAGS \
	-c \
	cdjpeg.c \
	jaricom.c \
	jcapimin.c \
	jcapistd.c \
	jcarith.c \
	jccoefct.c \
	jccolor.c \
	jcdctmgr.c \
	jchuff.c \
	jcinit.c \
	jcmainct.c \
	jcmarker.c \
	jcmaster.c \
	jcomapi.c \
	jcparam.c \
	jcprepct.c \
	jcsample.c \
	jctrans.c \
	jdapimin.c \
	jdapistd.c \
	jdarith.c \
	jdatadst.c \
	jdatasrc.c \
	jdcoefct.c \
	jdcolor.c \
	jddctmgr.c \
	jdhuff.c \
	jdinput.c \
	jdmainct.c \
	jdmarker.c \
	jdmaster.c \
	jdmerge.c \
	jdpostct.c \
	jdsample.c \
	jdtrans.c \
	jerror.c \
	jfdctflt.c \
	jfdctfst.c \
	jfdctint.c \
	jidctflt.c \
	jidctfst.c \
	jidctint.c \
	jmemansi.c \
	jmemmgr.c \
	jmemnobs.c \
	jquant1.c \
	jquant2.c \
	jutils.c \
	transupp.c
$AR \
	rcs \
	$PREFIX/lib/libjpeg.a \
	cdjpeg.o \
	jaricom.o \
	jcapimin.o \
	jcapistd.o \
	jcarith.o \
	jccoefct.o \
	jccolor.o \
	jcdctmgr.o \
	jchuff.o \
	jcinit.o \
	jcmainct.o \
	jcmarker.o \
	jcmaster.o \
	jcomapi.o \
	jcparam.o \
	jcprepct.o \
	jcsample.o \
	jctrans.o \
	jdapimin.o \
	jdapistd.o \
	jdarith.o \
	jdatadst.o \
	jdatasrc.o \
	jdcoefct.o \
	jdcolor.o \
	jddctmgr.o \
	jdhuff.o \
	jdinput.o \
	jdmainct.o \
	jdmarker.o \
	jdmaster.o \
	jdmerge.o \
	jdpostct.o \
	jdsample.o \
	jdtrans.o \
	jerror.o \
	jfdctflt.o \
	jfdctfst.o \
	jfdctint.o \
	jidctflt.o \
	jidctfst.o \
	jidctint.o \
	jmemansi.o \
	jmemmgr.o \
	jmemnobs.o \
	jquant1.o \
	jquant2.o \
	jutils.o \
	transupp.o
mkdir $PREFIX/include/jpeg
cp *.h $PREFIX/include/jpeg/
cd ..

echo 'Building libogg...'
tar xzf ../../../external/libogg-1.3.3.tar.gz
cd libogg-1.3.3
sed -e 's/@INCLUDE_INTTYPES_H@/1/g' \
    -e 's/@INCLUDE_STDINT_H@/1/g' \
    -e 's/@INCLUDE_SYS_TYPES_H@/1/g' \
    -e 's/@SIZE16@/int16_t/g' \
    -e 's/@USIZE16@/uint16_t/g' \
    -e 's/@SIZE32@/int32_t/g' \
    -e 's/@USIZE32@/uint32_t/g' \
    -e 's/@SIZE64@/int64_t/g' \
    < include/ogg/config_types.h.in \
    > include/ogg/config_types.h
$CC \
	-I./include \
	-fPIC \
	$CFLAGS \
	-c \
	src/bitwise.c \
	src/framing.c
$AR rcs $PREFIX/lib/libogg.a bitwise.o framing.o
cp -R include/ogg $PREFIX/include/
cd ..

echo 'Building libvorbis...'
tar xzf ../../../external/libvorbis-1.3.6.tar.gz
cd libvorbis-1.3.6
$CC \
	-I./include \
	-I./lib \
	-I$PREFIX/include \
	-fPIC \
	$CFLAGS \
	-c \
	lib/analysis.c \
	lib/barkmel.c \
	lib/bitrate.c \
	lib/block.c \
	lib/codebook.c \
	lib/envelope.c \
	lib/floor0.c \
	lib/floor1.c \
	lib/info.c \
	lib/lookup.c \
	lib/lpc.c \
	lib/lsp.c \
	lib/mapping0.c \
	lib/mdct.c \
	lib/psy.c \
	lib/registry.c \
	lib/res0.c \
	lib/sharedbook.c \
	lib/smallft.c \
	lib/synthesis.c \
	lib/tone.c \
	lib/vorbisenc.c \
	lib/vorbisfile.c \
	lib/window.c
$AR \
	rcs \
	$PREFIX/lib/libvorbis.a \
	analysis.o \
	barkmel.o \
	bitrate.o \
	block.o \
	codebook.o \
	envelope.o \
	floor0.o \
	floor1.o \
	info.o \
	lookup.o \
	lpc.o \
	lsp.o \
	mapping0.o \
	mdct.o \
	psy.o \
	registry.o \
	res0.o \
	sharedbook.o \
	smallft.o \
	synthesis.o \
	tone.o \
	vorbisenc.o \
	vorbisfile.o \
	window.o
cp -R include/vorbis $PREFIX/include/
cd ..

echo 'Building freetype2...'
tar xzf ../../../external/freetype-2.13.2.tar.gz
cd freetype-2.13.2
sed -e 's/FONT_MODULES += type1//' \
    -e 's/FONT_MODULES += cid//' \
    -e 's/FONT_MODULES += pfr//' \
    -e 's/FONT_MODULES += type42//' \
    -e 's/FONT_MODULES += pcf//' \
    -e 's/FONT_MODULES += bdf//' \
    -e 's/FONT_MODULES += pshinter//' \
    -e 's/FONT_MODULES += raster//' \
    -e 's/FONT_MODULES += psaux//' \
    -e 's/FONT_MODULES += psnames//' \
    < modules.cfg > modules.cfg.new
mv modules.cfg.new modules.cfg
$CC \
	-DFT2_BUILD_LIBRARY \
	-Dft_memcpy=memcpy \
	-I./include \
	-I$PREFIX/include \
	-fPIC \
	$CFLAGS \
	-c \
	-w \
	src/base/ftadvanc.c \
	src/base/ftbase.c \
	src/base/ftbbox.c \
	src/base/ftbdf.c \
	src/base/ftbitmap.c \
	src/base/ftcalc.c \
	src/base/ftcid.c \
	src/base/ftcolor.c \
	src/base/ftdbgmem.c \
	src/base/ftdebug.c \
	src/base/fterrors.c \
	src/base/ftfntfmt.c \
	src/base/ftfstype.c \
	src/base/ftgasp.c \
	src/base/ftgloadr.c \
	src/base/ftglyph.c \
	src/base/ftgxval.c \
	src/base/fthash.c \
	src/base/ftinit.c \
	src/base/ftlcdfil.c \
	src/base/ftmac.c \
	src/base/ftmm.c \
	src/base/ftobjs.c \
	src/base/ftotval.c \
	src/base/ftoutln.c \
	src/base/ftpatent.c \
	src/base/ftpfr.c \
	src/base/ftpsprop.c \
	src/base/ftrfork.c \
	src/base/ftsnames.c \
	src/base/ftstream.c \
	src/base/ftstroke.c \
	src/base/ftsynth.c \
	src/base/ftsystem.c \
	src/base/fttrigon.c \
	src/base/fttype1.c \
	src/base/ftutil.c \
	src/base/ftwinfnt.c \
	src/base/md5.c \
	src/bdf/bdf.c \
	src/bdf/bdfdrivr.c \
	src/bdf/bdflib.c \
	src/bzip2/ftbzip2.c \
	src/cache/ftcache.c \
	src/cache/ftcbasic.c \
	src/cache/ftccache.c \
	src/cache/ftccmap.c \
	src/cache/ftcglyph.c \
	src/cache/ftcimage.c \
	src/cache/ftcmanag.c \
	src/cache/ftcmru.c \
	src/cache/ftcsbits.c \
	src/cff/cff.c \
	src/cff/cffcmap.c \
	src/cff/cffdrivr.c \
	src/cff/cffgload.c \
	src/cff/cffload.c \
	src/cff/cffobjs.c \
	src/cff/cffparse.c \
	src/cid/cidgload.c \
	src/cid/cidload.c \
	src/cid/cidobjs.c \
	src/cid/cidparse.c \
	src/cid/cidriver.c \
	src/cid/type1cid.c \
	src/otvalid/otvalid.c \
	src/otvalid/otvbase.c \
	src/otvalid/otvcommn.c \
	src/otvalid/otvgdef.c \
	src/otvalid/otvgpos.c \
	src/otvalid/otvgsub.c \
	src/otvalid/otvjstf.c \
	src/otvalid/otvmath.c \
	src/otvalid/otvmod.c \
	src/pfr/pfr.c \
	src/pfr/pfrcmap.c \
	src/pfr/pfrdrivr.c \
	src/pfr/pfrgload.c \
	src/pfr/pfrload.c \
	src/pfr/pfrobjs.c \
	src/pfr/pfrsbit.c \
	src/psaux/afmparse.c \
	src/psaux/cffdecode.c \
	src/psaux/psarrst.c \
	src/psaux/psaux.c \
	src/psaux/psauxmod.c \
	src/psaux/psblues.c \
	src/psaux/psconv.c \
	src/psaux/pserror.c \
	src/psaux/psfont.c \
	src/psaux/psft.c \
	src/psaux/pshints.c \
	src/psaux/psintrp.c \
	src/psaux/psobjs.c \
	src/psaux/psread.c \
	src/psaux/psstack.c \
	src/pshinter/pshalgo.c \
	src/pshinter/pshglob.c \
	src/pshinter/pshinter.c \
	src/pshinter/pshmod.c \
	src/pshinter/pshrec.c \
	src/psnames/psmodule.c \
	src/psnames/psnames.c \
	src/raster/ftraster.c \
	src/sdf/ftbsdf.c \
	src/sdf/ftsdf.c \
	src/sdf/ftsdfcommon.c \
	src/sdf/ftsdfrend.c \
	src/sfnt/pngshim.c \
	src/sfnt/sfdriver.c \
	src/sfnt/sfobjs.c \
	src/sfnt/ttbdf.c \
	src/sfnt/ttcmap.c \
	src/sfnt/ttcolr.c \
	src/sfnt/ttcpal.c \
	src/sfnt/ttkern.c \
	src/sfnt/ttload.c \
	src/sfnt/ttmtx.c \
	src/sfnt/ttpost.c \
	src/sfnt/ttsbit.c \
	src/sfnt/ttsvg.c \
	src/sfnt/sfwoff.c \
	src/smooth/ftgrays.c \
	src/smooth/ftsmooth.c \
	src/svg/ftsvg.c \
	src/truetype/truetype.c \
	src/truetype/ttdriver.c \
	src/truetype/ttgload.c \
	src/truetype/ttgxvar.c \
	src/truetype/ttinterp.c \
	src/truetype/ttobjs.c \
	src/truetype/ttpload.c \
	src/winfonts/winfnt.c \
	src/gzip/adler32.c \
	src/gzip/crc32.c \
	src/gzip/ftgzip.c \
	src/gzip/inffast.c \
	src/gzip/inflate.c \
	src/gzip/inftrees.c \
	src/gzip/zutil.c \
	src/lzw/ftlzw.c \
	src/pcf/pcf.c \
	src/autofit/autofit.c \
	src/type1/type1.c \
	src/type42/type42.c \
	src/raster/raster.c \
	src/svg/svg.c
$AR \
	rcs \
	$PREFIX/lib/libfreetype.a \
	ftadvanc.o \
	ftbase.o \
	ftbbox.o \
	ftbdf.o \
	ftbitmap.o \
	ftcalc.o \
	ftcid.o \
	ftcolor.o \
	ftdbgmem.o \
	ftdebug.o \
	fterrors.o \
	ftfntfmt.o \
	ftfstype.o \
	ftgasp.o \
	ftgloadr.o \
	ftglyph.o \
	ftgxval.o \
	fthash.o \
	ftinit.o \
	ftlcdfil.o \
	ftmac.o \
	ftmm.o \
	ftobjs.o \
	ftotval.o \
	ftoutln.o \
	ftpatent.o \
	ftpfr.o \
	ftpsprop.o \
	ftrfork.o \
	ftsnames.o \
	ftstream.o \
	ftstroke.o \
	ftsynth.o \
	ftsystem.o \
	fttrigon.o \
	fttype1.o \
	ftutil.o \
	ftwinfnt.o \
	md5.o \
	bdf.o \
	bdfdrivr.o \
	bdflib.o \
	ftbzip2.o \
	ftcache.o \
	ftcbasic.o \
	ftccache.o \
	ftccmap.o \
	ftcglyph.o \
	ftcimage.o \
	ftcmanag.o \
	ftcmru.o \
	ftcsbits.o \
	cff.o \
	cffcmap.o \
	cffdrivr.o \
	cffgload.o \
	cffload.o \
	cffobjs.o \
	cffparse.o \
	cidgload.o \
	cidload.o \
	cidobjs.o \
	cidparse.o \
	cidriver.o \
	type1cid.o \
	otvalid.o \
	otvbase.o \
	otvcommn.o \
	otvgdef.o \
	otvgpos.o \
	otvgsub.o \
	otvjstf.o \
	otvmath.o \
	otvmod.o \
	pfr.o \
	pfrcmap.o \
	pfrdrivr.o \
	pfrgload.o \
	pfrload.o \
	pfrobjs.o \
	pfrsbit.o \
	afmparse.o \
	cffdecode.o \
	psarrst.o \
	psaux.o \
	psauxmod.o \
	psblues.o \
	psconv.o \
	pserror.o \
	psfont.o \
	psft.o \
	pshints.o \
	psintrp.o \
	psobjs.o \
	psread.o \
	psstack.o \
	pshalgo.o \
	pshglob.o \
	pshinter.o \
	pshmod.o \
	pshrec.o \
	psmodule.o \
	psnames.o \
	ftraster.o \
	ftbsdf.o \
	ftsdf.o \
	ftsdfcommon.o \
	ftsdfrend.o \
	pngshim.o \
	sfdriver.o \
	sfobjs.o \
	ttbdf.o \
	ttcmap.o \
	ttcolr.o \
	ttcpal.o \
	ttkern.o \
	ttload.o \
	ttmtx.o \
	ttpost.o \
	ttsbit.o \
	ttsvg.o \
	sfwoff.o \
	ftgrays.o \
	ftsmooth.o \
	ftsvg.o \
	truetype.o \
	ttdriver.o \
	ttgload.o \
	ttgxvar.o \
	ttinterp.o \
	ttobjs.o \
	ttpload.o \
	winfnt.o \
	adler32.o \
	crc32.o \
	ftgzip.o \
	inffast.o \
	inflate.o \
	inftrees.o \
	zutil.o \
	ftlzw.o \
	pcf.o \
	autofit.o \
	type1.o \
	type42.o \
	raster.o \
	svg.o
cp -R include/freetype $PREFIX/include/freetype
cp include/ft2build.h $PREFIX/include/
cd ..

cd ..
rm -rf tmp
echo 'Finished.'
