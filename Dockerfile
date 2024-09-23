FROM --platform=linux/amd64 ubuntu:22.04
RUN apt-get update
RUN apt-get -y install build-essential libasound2-dev libx11-dev libxpm-dev mesa-common-dev libfreetype-dev libpng-dev libjpeg-dev libwebp-dev libvorbis-dev libogg-dev libbrotli-dev libbz2-dev libz-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev llvm clang mingw-w64
