FROM ubuntu:22.04
RUN apt-get update
RUN apt-get -y install build-essential libasound2-dev libx11-dev libxpm-dev mesa-common-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev mingw-w64 llvm clang nsis git

RUN git clone https://github.com/emscripten-core/emsdk.git
WORKDIR emsdk
RUN ./emsdk install latest
RUN ./emsdk activate latest
WORKDIR ..

RUN git clone https://github.com/OpenNovelOrg/OpenNovel.git
