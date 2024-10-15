#!/bin/sh

set -eu

#
# Banner
#

# Get the version string.
VERSION=`grep -a1 '<!-- BEGIN-LATEST -->' ChangeLog | tail -n1`
VERSION=`echo $VERSION | cut -d ' ' -f 2`
NOTE=`cat ChangeLog | awk '/BEGIN-LATEST/,/END-LATEST/' | tail -n +2 | head -n -1`

# Show the version and the release note.
echo "Going to release the version $VERSION of the Suika2 project."
echo ''
echo '[Note]'
echo "$NOTE"
echo ''
echo 'Press enter to continue.'

read str

#
# Make Directories
#

echo 'Preparing target directories...'

TARGET_DIR="`pwd`/suika2-$VERSION"
TARGET_ZIP="`pwd`/suika2-$VERSION.zip"
rm -rf "$TARGET_DIR" "$TARGET_ZIP"
mkdir "$TARGET_DIR"
mkdir "$TARGET_DIR/tools"

echo '...Done preparing target directories.'
echo ''

#
# Windows build (Binary)
#

echo 'Building the Windows engine...'
cd engines/windows
make -j$(nproc)
cd ../..
cp engines/windows/engine.exe "$TARGET_DIR/engine.exe"
echo '...Done building the Windows engine.'

echo 'Building the Windows editor...'
cd apps/pro-windows
make -j$(nproc)
cd ../../
cp apps/pro-windows/editor.exe "$TARGET_DIR/editor.exe"
echo '...Done building the Windows editor.'

echo 'Building the Windows web-test tool...'
cd apps/web-test
make web-test.exe
cd ../..
cp apps/web-test/web-test.exe "$TARGET_DIR/tools/web-test.exe"
echo '...Done building the Windows web-test tool.'

echo ''

#
# Wasm build (Binary)
#

echo 'Building Wasm binaries...'

cd engines/wasm
make
cp -R html "$TARGET_DIR/tools/wasm-src"
cd ../../

echo '...Done building Wasm binaries.'
echo ''

#
# iOS source tree build (Source)
#

echo 'Building the iOS source tree...'

cd engines/ios
tar xzf ../../external/libroot-ios.tar.gz
make src
cp -R ios-src "$TARGET_DIR/tools/"
cd ../../

echo '...Done building the iOS source tree.'
echo ''

#
# Android source tree build (Source)
#

echo 'Building the Android source tree...'

cd engines/android
make src
cp -R android-src "$TARGET_DIR/tools/"
cd ../../

echo '...Done building the Android source tree.'
echo ''

#
# macOS source tree build (Source)
#

echo 'Building the macOS source tree...'

cd engines/macos
tar xzf ../../external/libroot-macos.tar.gz
make src
cp -R macos-src "$TARGET_DIR/tools/"
cd ../../

echo '...Done building the macOS source tree.'
echo ''

#
# Unity source tree build (Source)
#

echo 'Building the Unity source tree...'

cd engines/unity
make -j$(nproc) libopennovel.dll
make src
cd ../../
cp -R engines/unity/unity-src "$TARGET_DIR/tools/"

echo '...Done building the Unity source tree.'
echo ''

#
# Documents and Sample
#

echo 'Copying documents and sample...'

cp -R docs "$TARGET_DIR/manual"
cp -R games "$TARGET_DIR/games"

echo '...Done copying documents and sample.'
echo ''

#
# ZIP
#

zip -9 -r "$TARGET_ZIP" "suika2-$VERSION"

#
# GitHub Release
#

echo 'Making a release on GitHub...'

yes '' | gh release create "$VERSION" --title "$VERSION" --notes "$NOTE" "$TARGET_ZIP"
rm -rf "$TARGET_DIR" "$TARGET_ZIP"

echo '...Done releasing on GitHub.'
echo ''
echo 'Release completed!'
