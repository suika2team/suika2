#!/bin/sh

set -eu

# Get the version string.
VERSION=`grep -a1 '<!-- BEGIN-LATEST -->' ChangeLog | tail -n1`
VERSION=`echo $VERSION | cut -d ' ' -f 2`
NOTE=`cat ChangeLog | awk '/BEGIN-LATEST/,/END-LATEST/' | tail -n +2 | ghead -n -1`

# Show a version and a note.
echo "Going to release the version $VERSION"
echo ""
echo "[Note]"
echo "$NOTE"
echo ""

# Wait for input.
echo 'Press enter to continue.'
read str
echo ''
echo 'Making opennovel-$VERSION.zip ...'
echo ''

# Make a target directory.
TARGET="`pwd`/opennovel-$VERSION"
rm -rf "$TARGET" "$TARGET.zip"
mkdir "$TARGET"
mkdir "$TARGET/export-kit"

# Build the Windows engine.
echo 'Building the Windows engine...'
cd engines/windows
make
cp game.exe "$TARGET/game-win.exe"
cd ../..
echo 'Ok.'
echo ''

# Build the Windows packager.
echo 'Building the Windows packager...'
cd apps/pack
make pack.exe
cp pack.exe "$TARGET/pack-win.exe"
cd ../..
echo 'Ok.'
echo ''

# Build the macOS engine.
echo "Building the macOS source..."
cd engines/macos
make game.dmg
cp game.dmg "$TARGET/game-mac.dmg"
cd ../../

# Build the macOS packager.
echo 'Building the macOS packager...'
cd apps/pack
make pack
cp pack "$TARGET/pack-mac"
cd ../..
echo 'Ok.'
echo ''

# Copy the documents.
echo "Copying the documents..."
find . -name .DS_Store | xargs rm
cp -R doc "$TARGET/manual"

# Copy the sample.
echo "Copying the sample..."
cp -R game "$TARGET/sample"

# Copy the iOS source.
echo "Copying the iOS source..."
cd engines/ios
make
cp -R ios-src "$TARGET/export-kit/"
cd ../../

# Copy the Android source.
echo "Copying the Android source..."
cd engines/android
make
cp -R android-src "$TARGET/export-kit/"
cd ../../

# Copy the macOS source.
echo "Copying the macOS source..."
cd engines/macos
make src
cp -R macos-src "$TARGET/export-kit/"
cd ../../

# Copy the Wasm source.
echo "Copying the Wasm source..."
cd engines/wasm
make
cp -R html "$TARGET/export-kit/"
cd ../../

# Copy the Unity source.
echo "Copying the Unity source..."
cd engines/unity
make
cp -R unity-src "$TARGET/export-kit/"
cd ../../

# Make a ZIP.
echo "Making a ZIP..."
7z a -tzip -mm=LZMA -mx9 -aoa "$TARGET.zip" "$TARGET"
rm -rf "$TARGET"

echo "Done."
