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
# Windows Build
#

echo 'Building the Windows engine...'
cd engines/windows
make
cp game.exe "$TARGET/game-win.exe"
cd ../..
echo 'Ok.'
echo ''

echo 'Building the Windows packager...'
cd apps/pack
make pack.exe
cp pack.exe "$TARGET/pack-win.exe"
cd ../..
echo 'Ok.'
echo ''

#
# Mac Build
#

echo "Building the macOS source..."
cd engines/macos
make game.dmg
cp game.dmg "$TARGET/game-mac.dmg"
cd ../../

echo 'Building the macOS packager...'
cd apps/pack
rm pack
make pack
cp pack "$TARGET/pack-mac"
cd ../..
echo 'Ok.'
echo ''

#
# Linux Build
#

echo 'Building the Linux engine...'
docker build -t ubuntu engines/linux
docker run -it -v `pwd`:/workspace ubuntu /bin/sh -c "cd /workspace/engines/linux && make clean && make"
docker run -it -v `pwd`:/workspace ubuntu /bin/sh -c "cd /workspace/apps/pack && rm -f pack && make pack"
cp engines/linux/game-linux "$TARGET/game-linux"
cp apps/pack/pack "$TARGET/pack-linux"
echo 'Ok.'
echo ''

#
# Documents
#

echo "Copying the documents..."
find . -name .DS_Store | xargs rm
cp -R doc "$TARGET/manual"

#
# Sample
#

# Copy the sample.
echo "Copying the sample..."
cp -R game "$TARGET/sample"

#
# Export Templates
#

echo "Copying the iOS source..."
cd engines/ios
make
cp -R ios-src "$TARGET/export-kit/"
cd ../../

echo "Copying the Android source..."
cd engines/android
make
cp -R android-src "$TARGET/export-kit/"
cd ../../

echo "Copying the macOS source..."
cd engines/macos
make src
cp -R macos-src "$TARGET/export-kit/"
cd ../../

echo "Copying the Wasm source..."
cd engines/wasm
make
cp -R html "$TARGET/export-kit/"
cd ../../

echo "Copying the Unity source..."
cd engines/unity
make
cp -R unity-src "$TARGET/export-kit/"
cd ../../

#
# ZIP
#

echo "Making a ZIP..."
7z a -tzip -mm=LZMA -mx9 -aoa "$TARGET.zip" "$TARGET"
rm -rf "$TARGET"

#
# GitHub Release
#

echo "Making a release on GitHub..."
yes "" | gh release create "$VERSION" --title "$VERSION" --notes "$NOTE" "$TARGET.zip"

echo "Done."
