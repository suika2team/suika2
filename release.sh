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
TARGET_EXE="`pwd`/OpenNovel-Win-Installer-$VERSION.exe"
TARGET_ZIP="`pwd`/OpenNovel-Win-ZIP-Not-Recommended-$VERSION.zip"
TARGET_DMG="`pwd`/OpenNovel-Mac-$VERSION.dmg"
rm -rf "$TARGET_DIR" "$TARGET_ZIP"
mkdir "$TARGET_DIR"
mkdir "$TARGET_DIR/tools"

echo '...Done making a target directory.'
echo ''

#
# Docker container build
#

docker build -t opennovel-build .

#
# Windows build (Binary)
#

echo 'Building the Windows engine...'
docker run -it -v `pwd`:/workspace opennovel-build /bin/sh -c 'cd /workspace/engines/windows && make libroot && make -j$(nproc)'
cp engines/windows/game.exe "$TARGET_DIR/game.exe"
echo '...Done building the Windows engine.'

echo 'Building the Windows editor...'
docker run -it -v `pwd`:/workspace opennovel-build /bin/sh -c 'cd /workspace/apps/pro-windows && make libroot && make -j$(nproc)'
cp apps/pro-windows/editor.exe "$TARGET_DIR/editor.exe"
echo '...Done building the Windows editor.'

echo 'Building the Windows web-test tool...'
docker run -it -v `pwd`:/workspace opennovel-build /bin/sh -c 'cd /workspace/apps/web-test && make web-test.exe'
cp apps/web-test/web-test.exe "$TARGET_DIR/tools/web-test.exe"
echo '...Done building the Windows web-test tool.'

echo ''

#
# macOS engine build (Binary)
#

echo 'Building the macOS engine...'

cd engines/macos
make libroot
make game.dmg
cp game.dmg "$TARGET_DIR/tools/game-mac.dmg"
cd ../../

echo '...Done building the macOS engine.'
echo ''

#
# Linux build (Binary)
#

echo 'Building Linux binaries...'

docker run -it -v `pwd`:/workspace opennovel-build /bin/sh -c 'cd /workspace/engines/linux && make libroot && make -j$(nproc)'
cp engines/linux/game-linux "$TARGET_DIR/tools/game-linux"

docker run -it -v `pwd`:/workspace opennovel-build /bin/sh -c 'cd /workspace/apps/pack && rm -f pack && make pack'
cp apps/pack/pack "$TARGET_DIR/tools/pack-linux"

echo '...Done building Linux binaries.'
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
make
cp -R ios-src "$TARGET_DIR/tools/"
cd ../../

echo '...Done building the iOS source tree.'
echo ''

#
# Android source tree build (Source)
#

echo 'Building the Android source tree...'

cd engines/android
make
cp -R android-src "$TARGET_DIR/tools/"
cd ../../

echo '...Done building the Android source tree.'
echo ''

#
# macOS source tree build (Source)
#

echo 'Building the macOS source tree...'

cd engines/macos
make libroot
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

# Unity Windows
make -j$(nproc) libopennovel.dll

# Unity macOS
make -j$(nproc) libopennovel.dylib

# Unity Linux
docker run -it -v `pwd`/../..:/workspace opennovel-build /bin/sh -c 'cd /workspace/engines/unity && make libopennovel.so'

# Unity Switch
docker pull yesimnathan/switchdev
docker run -it -v `pwd`/../..:/workspace yesimnathan/switchdev /bin/bash -c "cd /workspace/engines/unity && make libopennovel.nso"

# src
make src

cp -R unity-src "$TARGET_DIR/tools/"
cd ../../

echo '...Done building the Unity source tree.'
echo ''

#
# macOS editor build (Binary)
#

echo 'Building the macOS editor...'

cd apps/pro-macos
make libroot
make opennovel.dmg
mv opennovel.dmg "$TARGET_DMG"
cd ../../

echo '...Done building the macOS editor.'
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
# Installer
#

echo 'Making an installer...'

cp "$TARGET_DIR"/game.exe apps/installer-windows/
cp "$TARGET_DIR"/editor.exe apps/installer-windows/
cp -R "$TARGET_DIR"/manual apps/installer-windows/
cp -R "$TARGET_DIR"/sample apps/installer-windows/
cp -R "$TARGET_DIR"/tools apps/installer-windows/

cd apps/installer-windows
make
cp opennovel-installer.exe "$TARGET_EXE"
rm opennovel-installer.exe
cd ../..

rm apps/installer-windows/game.exe
rm apps/installer-windows/editor.exe
rm -rf apps/installer-windows/manual
rm -rf apps/installer-windows/sample
rm -rf apps/installer-windows/tools

echo '...Done compressing.'
echo ''

#
# ZIP
#

echo 'Compressing...'

7z a -tzip -mx9 -aoa "$TARGET_ZIP" "$TARGET_DIR"

echo '...Done compressing.'
echo ''

read str

#
# GitHub Release
#

echo 'Making a release on GitHub...'

yes '' | gh release create "$VERSION" --title "$VERSION" --notes "$NOTE" "$TARGET_EXE" "$TARGET_ZIP" "$TARGET_DMG"
rm -rf "$TARGET_DIR" "$TARGET_EXE" "$TARGET_ZIP" "$TARGET_DMG"

echo '...Done.'
echo ''
echo 'Release completed!'
