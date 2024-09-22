#!/bin/sh

set -eu

#
# Banner
#

# Get the version string.
VERSION=`grep -a1 '<!-- BEGIN-LATEST -->' ChangeLog | tail -n1`
VERSION=`echo $VERSION | cut -d ' ' -f 2`
NOTE=`cat ChangeLog | awk '/BEGIN-LATEST/,/END-LATEST/' | tail -n +2 | ghead -n -1`

# Show the version and the release note.
echo "Going to release the version $VERSION of the OpenNovel project."
echo ''
echo '[Note]'
echo "$NOTE"
echo ''
echo 'Press enter to continue.'

read str

#
# Make Directories
#

echo 'Making a target directories...'

TARGET_DIR="`pwd`/OpenNovel-$VERSION"
TARGET_ZIP="`pwd`/OpenNovel-$VERSION.zip"
rm -rf "$TARGET_DIR" "$TARGET_ZIP"
mkdir "$TARGET_DIR"
mkdir "$TARGET_DIR/tools"

echo '...Done making a target directory.'
echo ''

#
# Windows Build (Binary)
#

echo 'Building Windows binaries...'

docker build -t opennovel-mingw-build engines/windows

docker run -it -v `pwd`:/workspace opennovel-mingw-build /bin/sh -c 'cd /workspace/engines/windows && make libroot && make -j$(nproc)'
cp engines/windows/game.exe "$TARGET_DIR/game.exe"

docker run -it -v `pwd`:/workspace opennovel-mingw-build /bin/sh -c 'cd /workspace/apps/pack && make pack.exe'
cp apps/pack/pack.exe "$TARGET_DIR/tools/pack-win.exe"

docker run -it -v `pwd`:/workspace opennovel-mingw-build /bin/sh -c 'cd /workspace/apps/exporter && make exporter.exe'
cp apps/exporter/exporter.exe "$TARGET_DIR/exporter.exe"

docker run -it -v `pwd`:/workspace opennovel-mingw-build /bin/sh -c 'cd /workspace/apps/web-test && make web-test.exe'
cp apps/web-test/web-test.exe "$TARGET_DIR/tools/web-test.exe"

echo '...Done building Windows binaries.'
echo ''

#
# Mac Build (Binary)
#

echo 'Building macOS binaries...'

cd engines/macos
make libroot
make game.dmg
cp game.dmg "$TARGET_DIR/tools/game-mac.dmg"
cd ../../

cd apps/pack
rm -f pack
make pack
cp pack "$TARGET_DIR/tools/pack-mac"
cd ../..
echo 'Ok.'
echo ''

echo '...Done building macOS binaries.'
echo ''

#
# Linux Build (Binary)
#

echo 'Building Linux binaries...'

docker build -t opennovel-linux-build engines/linux

docker run -it -v `pwd`:/workspace opennovel-linux-build /bin/sh -c 'cd /workspace/engines/linux && make libroot && make -j$(nproc)'
cp engines/linux/game-linux "$TARGET_DIR/tools/game-linux"

docker run -it -v `pwd`:/workspace opennovel-linux-build /bin/sh -c 'cd /workspace/apps/pack && rm -f pack && make pack'
cp apps/pack/pack "$TARGET_DIR/tools/pack-linux"

echo '...Done building Linux binaries.'
echo ''

#
# Wasm Build (Binary)
#

echo 'Building Wasm binaries...'

cd engines/wasm
make
cp -R html "$TARGET_DIR/tools/wasm-src"
cd ../../

echo '...Done building Wasm binaries.'
echo ''

#
# iOS Build (Source)
#

echo 'Building iOS source tree...'

cd engines/ios
make
cp -R ios-src "$TARGET_DIR/tools/"
cd ../../

echo '...Done building iOS source tree.'
echo ''

#
# Android Build (Source)
#

echo 'Building Android source tree...'

cd engines/android
make
cp -R android-src "$TARGET_DIR/tools/"
cd ../../

echo '...Done building Android source tree.'
echo ''

#
# macOS Build (Source)
#

echo 'Building macOS source tree...'

cd engines/macos
make libroot
make src
cp -R macos-src "$TARGET_DIR/tools/"
cd ../../

echo '...Done building macOS source tree.'
echo ''

#
# Unity Build (Source)
#

echo 'Building Unity source tree...'

cd engines/unity

# Unity Windows
make -j$(nproc) libopennovel.dll

# Unity macOS
make -j$(nproc) libopennovel.dylib

# Unity Linux
docker run -it -v `pwd`/../..:/workspace opennovel-linux-build /bin/sh -c 'cd /workspace/engines/unity && make libopennovel.so'

# Unity Switch
docker pull yesimnathan/switchdev
docker run -it -v `pwd`/../..:/workspace yesimnathan/switchdev /bin/bash -c "cd /workspace/engines/unity && make libopennovel.nso"

# src
make src

cp -R unity-src "$TARGET_DIR/tools/"
cd ../../

echo '...Done building Unity source tree.'
echo ''

#
# Documents and Sample
#

echo 'Copying documents and sample...'

find . -name .DS_Store | xargs rm

cp -R doc "$TARGET_DIR/manual"
cp -R game "$TARGET_DIR/sample"

echo '...Done copying documents and sample.'
echo ''

#
# ZIP
#

echo 'Compressing...'

7z a -tzip -mx9 -aoa "$TARGET_ZIP" "$TARGET_DIR"
rm -rf "$TARGET_DIR"

echo '...Done compressing.'
echo ''

#
# GitHub Release
#

echo 'Making a release on GitHub...'

yes '' | gh release create "$VERSION" --title "$VERSION" --notes "$NOTE" "$TARGET_ZIP"
rm -f "$TARGET_ZIP"

echo '...Done.'
echo ''
echo 'Release completed!'
