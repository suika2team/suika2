#!/bin/sh

set -eu

MACHINE=$1
CC=$2
CFLAGS=$3
AR=$4

echo "Building libroot with CC=$CC AR=$AR"

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
make -j$(nproc) -f ../../../../external/brotli.mk CC="$CC" CFLAGS="$CFLAGS" AR="$AR" PREFIX="$PREFIX"
cp -R c/include/brotli $PREFIX/include/
cd ..

echo 'Building bzip2...'
tar xzf ../../../external/bzip2-1.0.6.tar.gz
cd bzip2-1.0.6
make -j$(nproc) -f ../../../../external/bzip2.mk CC="$CC" CFLAGS="$CFLAGS" AR="$AR" PREFIX="$PREFIX"
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
make -j$(nproc) -f ../../../../external/libwebp.mk CC="$CC" CFLAGS="$CFLAGS" AR="$AR" PREFIX="$PREFIX"
cp -R src/webp $PREFIX/include/
cd ..

echo 'Building zlib...'
tar xzf ../../../external/zlib-1.2.11.tar.gz
cd zlib-1.2.11
make -j$(nproc) -f ../../../../external/zlib.mk CC="$CC" CFLAGS="$CFLAGS" AR="$AR" PREFIX="$PREFIX"
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
make -j$(nproc) -f ../../../../external/libpng.mk CC="$CC" CFLAGS="$CFLAGS" AR="$AR" PREFIX="$PREFIX"
mkdir $PREFIX/include/png
cp *.h $PREFIX/include/png/
cd ..

echo 'Building jpeg9...'
tar xzf ../../../external/jpegsrc.v9e.tar.gz
cd jpeg-9e
cp jconfig.txt jconfig.h
make -j$(nproc) -f ../../../../external/jpeg9.mk CC="$CC" CFLAGS="$CFLAGS" AR="$AR" PREFIX="$PREFIX"
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
make -j$(nproc) -f ../../../../external/libogg.mk CC="$CC" CFLAGS="$CFLAGS" AR="$AR" PREFIX="$PREFIX"
cp -R include/ogg $PREFIX/include/
cd ..

echo 'Building libvorbis...'
tar xzf ../../../external/libvorbis-1.3.6.tar.gz
cd libvorbis-1.3.6
make -j$(nproc) -f ../../../../external/libvorbis.mk CC="$CC" CFLAGS="$CFLAGS" AR="$AR" PREFIX="$PREFIX"
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
make -j$(nproc) -f ../../../../external/freetype2.mk CC="$CC" CFLAGS="$CFLAGS" AR="$AR" PREFIX="$PREFIX"
cp -R include/freetype $PREFIX/include/freetype
cp include/ft2build.h $PREFIX/include/
cd ..

cd ..
rm -rf tmp
echo 'Finished.'
