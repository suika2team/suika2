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

#
# Windows Build (Binary)
#

echo 'Building the Windows binaries...'

cd engines/windows
make
cp game.exe "$TARGET/game-win.exe"
cd ../..
echo 'Ok.'
echo ''

cd apps/pack
make pack.exe
cp pack.exe "$TARGET/pack-win.exe"
cd ../..
echo 'Ok.'
echo ''

#
# Mac Build (Binary)
#

echo "Building the macOS binaries..."

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

#
# Linux Build (Binary)
#

echo 'Building the Linux binaries...'

docker build -t ubuntu-build engines/linux
docker run -it -v `pwd`:/workspace ubuntu-build /bin/sh -c "cd /workspace/engines/linux && make clean && make"
docker run -it -v `pwd`:/workspace ubuntu-build /bin/sh -c "cd /workspace/apps/pack && rm -f pack && make pack"
cp engines/linux/game-linux "$TARGET/game-linux"
cp apps/pack/pack "$TARGET/pack-linux"
echo 'Ok.'
echo ''

#
# Wasm Build (Binary)
#

echo "Building the Wasm binaries..."

cd engines/wasm
make
cp -R html "$TARGET/export-kit/"
cd ../../

#
# iOS Build (Source)
#

echo 'Building the iOS source...'

cd engines/ios
rm -rf ios-src libroot-*
make
cp -R ios-src "$TARGET/export-kit/"
cd ../../

#
# Android Build (Source)
#

echo 'Building the Android source...'

cd engines/android
rm -rf libroot-* libopennovel-* android-src
make
cp -R android-src "$TARGET/export-kit/"
cd ../../

#
# macOS Build (Source)
#

echo "Building the macOS source..."

cd engines/macos
make src
cp -R macos-src "$TARGET/export-kit/"
cd ../../

#
# Unity Build (Source)
#

echo "Building the Unity source..."

cd engines/unity

docker pull yesimnathan/switchdev
docker run -it -v `pwd`/../..:/workspace yesimnathan/switchdev /bin/bash -c "cd /workspace/engines/unity && make libopennovel.nso"

make all

cp -R unity-src "$TARGET/export-kit/"
cd ../../

#
# Documents
#

echo "Copying the documents..."

find . -name .DS_Store | xargs rm
cp -R doc "$TARGET/manual"

#
# Sample
#

echo "Copying the sample..."

cp -R game "$TARGET/sample"

#
# ZIP
#

echo "Making a ZIP..."

7z a -tzip -mx9 -aoa "$TARGET.zip" "$TARGET"
rm -rf "$TARGET"

#
# GitHub Release
#

echo "Making a release on GitHub..."

yes "" | gh release create "$VERSION" --title "$VERSION" --notes "$NOTE" "$TARGET.zip"
rm "$TARGET.zip"

echo "Done."
