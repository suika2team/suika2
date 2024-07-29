#!/bin/sh

set -eu

#
# Initialization
#

# Get the version string.
VERSION=`grep -a1 '<!-- BEGIN-LATEST -->' ChangeLog | tail -n1`
VERSION=`echo $VERSION | cut -d ' ' -f 2`
NOTE=`cat ChangeLog | awk '/BEGIN-LATEST/,/END-LATEST/' | tail -n +2 | ghead -n -1`

echo "Going to release the version $VERSION of the OpenNovel project."
echo ""
echo "[Note]"
echo "$NOTE"
echo ""

echo 'Press enter to continue.'
read str

echo ''
echo "Making opennovel-$VERSION.zip ..."
echo ''

echo 'Making a target directory...'
TARGET="`pwd`/opennovel-$VERSION"
rm -rf "$TARGET" "$TARGET.zip"
mkdir "$TARGET"
mkdir "$TARGET/export-kit"
echo ''

#
# Windows Build (Binary)
#

echo 'Building Windows binaries...'

cd engines/windows
make
cp game.exe "$TARGET/game-win.exe"
cd ../..

cd apps/pack
make pack.exe
cp pack.exe "$TARGET/pack-win.exe"
cd ../..

echo 'Done building Windows binaries.'
echo ''

#
# Mac Build (Binary)
#

echo "Building macOS binaries..."

cd engines/macos
make game.dmg
cp game.dmg "$TARGET/game-mac.dmg"
cd ../../

cd apps/pack
rm pack
make pack
cp pack "$TARGET/pack-mac"
cd ../..
echo 'Ok.'
echo ''

echo 'Done building macOS binaries.'
echo ''

#
# Linux Build (Binary)
#

echo 'Building the Linux binaries...'

docker build -t ubuntu-build engines/linux
docker run -it -v `pwd`:/workspace ubuntu-build /bin/sh -c "cd /workspace/engines/linux && make clean && make"
docker run -it -v `pwd`:/workspace ubuntu-build /bin/sh -c "cd /workspace/apps/pack && rm -f pack && make pack"
cp engines/linux/game-linux "$TARGET/game-linux"
cp apps/pack/pack "$TARGET/pack-linux"

echo 'Done building Linux binaries.'
echo ''

#
# Wasm Build (Binary)
#

echo "Building the Wasm binaries..."

cd engines/wasm
make
cp -R html "$TARGET/export-kit/"
cd ../../

echo 'Done building Wasm binaries.'
echo ''

#
# iOS Build (Source)
#

echo 'Building iOS source tree...'

cd engines/ios
rm -rf ios-src libroot-*
make
cp -R ios-src "$TARGET/export-kit/"
cd ../../

echo 'Done building iOS source tree.'
echo ''

#
# Android Build (Source)
#

echo 'Building Android source tree...'

cd engines/android
rm -rf libroot-* libopennovel-* android-src
make
cp -R android-src "$TARGET/export-kit/"
cd ../../

echo 'Done building Android source tree.'
echo ''

#
# macOS Build (Source)
#

echo "Building macOS source tree...\n"

cd engines/macos
make src
cp -R macos-src "$TARGET/export-kit/"
cd ../../

echo 'Done building macOS source tree.'
echo ''

#
# Unity Build (Source)
#

echo "Building Unity source tree...\n"

cd engines/unity

docker pull yesimnathan/switchdev
docker run -it -v `pwd`/../..:/workspace yesimnathan/switchdev /bin/bash -c "cd /workspace/engines/unity && make libopennovel.nso"

make all

cp -R unity-src "$TARGET/export-kit/"
cd ../../

echo 'Done building Unity source tree.'
echo ''

#
# Documents and Sample
#

echo "Copying documents and sample..."

find . -name .DS_Store | xargs rm

cp -R doc "$TARGET/manual"
cp -R game "$TARGET/sample"

echo 'Done copying documents and sample.'
echo ''

#
# ZIP
#

echo "Compressing..."

7z a -tzip -mx9 -aoa "$TARGET.zip" "$TARGET"
rm -rf "$TARGET"

echo 'Done compressing.'
echo ''

#
# GitHub Release
#

echo "Making a release on GitHub..."

yes "" | gh release create "$VERSION" --title "$VERSION" --notes "$NOTE" "$TARGET.zip"
rm "$TARGET.zip"

echo "Done."
