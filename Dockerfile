# syntax = docker/dockerfile:experimental

FROM alpine:3.22 AS cachebase
RUN mkdir -p /tmp/ccache \
 && chown nobody:nogroup /tmp/ccache

FROM alpine:3.22

ARG BUILD_CONCURRENCY=4

RUN --mount=type=cache,target=/etc/apk/cache,id=apk-cache \
    --mount=type=cache,target=/var/lib/apk,id=apk-lib \
    \
    apk add \
      autoconf \
      automake \
      build-base \
      bzip2-dev \
      bzip2-static \
      cmake \
      ccache \
      expat-dev \
      expat-static \
      gettext-dev \
      gettext-static \
      git \
      gperf \
      libjpeg-turbo-dev \
      libjpeg-turbo-static \
      libpng-dev \
      libpng-static \
      libtool \
      linux-headers \
      python3 \
      util-linux-dev \
      util-linux-static \
      zlib-dev \
      zlib-static

ENV PATH=/usr/lib/ccache/bin:$PATH \
    CCACHE_DIR=/tmp/ccache

COPY --chown=nobody:nogroup ./vendor /src/vendor
WORKDIR /src
USER nobody:nogroup

RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    \
    cd vendor/gitlab.freedesktop.org/freetype/freetype/ \
 && NOCONFIGURE=1 ./autogen.sh \
 # workaround for docker #9547 (Text file busy) \
 && sync \
 && mkdir build && cd build \
 && ../configure --prefix=$PWD/install --enable-static \
 && make -j${BUILD_CONCURRENCY} \
 && make install

ENV PKG_CONFIG_PATH="/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install/lib $LINKFLAGS" \
    # Required for poppler cmake \
    FREETYPE_DIR=/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install


RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    \
    cd vendor/gitlab.freedesktop.org/fontconfig/fontconfig/ \
 && NOCONFIGURE=1 ./autogen.sh \
 && mkdir build && cd build \
 && ../configure --prefix=$PWD/install --enable-static \
 && make -j${BUILD_CONCURRENCY} \
 && make install

ENV PKG_CONFIG_PATH="/src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install/lib $LINKFLAGS"


RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    \
    cd vendor/github.com/mm2/Little-CMS/ \
 && mkdir build && cd build \
 && ../configure --prefix=$PWD/install --enable-static \
 && make -j${BUILD_CONCURRENCY} \
 && make install

ENV PKG_CONFIG_PATH="/src/vendor/github.com/mm2/Little-CMS/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/github.com/mm2/Little-CMS/build/install/lib $LINKFLAGS"


RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    \
    cd vendor/github.com/uclouvain/openjpeg/ \
 && mkdir build && cd build \
 && cmake .. -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/install \
 && make -j${BUILD_CONCURRENCY} \
 && make install

ENV PKG_CONFIG_PATH="/src/vendor/github.com/uclouvain/openjpeg/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    CXXFLAGS="-I/src/vendor/github.com/uclouvain/openjpeg/build/install/include $CXXFLAGS" \
    LDFLAGS="-L/src/vendor/github.com/uclouvain/openjpeg/build/install/lib $LDFLAGS" \
    LINKFLAGS="-L/src/vendor/github.com/uclouvain/openjpeg/build/install/lib $LINKFLAGS" \
    OpenJPEG_DIR="/src/vendor/github.com/uclouvain/openjpeg/build/install/lib/cmake/openjpeg-2.5"


RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    \
    cd vendor/github.com/sensiblecodeio/poppler/ \
 && mkdir build && cd build \
 && cmake .. \
          -DCMAKE_INSTALL_PREFIX=$PWD/install \
          -DENABLE_GLIB:BOOL=OFF \
          -DENABLE_CPP:BOOL=OFF \
          -DENABLE_UTILS:BOOL=OFF \
          -DENABLE_BOOST:BOOL=OFF \
          -DBUILD_SHARED_LIBS:BOOL=OFF \
          -DENABLE_UNSTABLE_API_ABI_HEADERS:BOOL=ON \
          -DCMAKE_BUILD_TYPE:STRING=release \
          -DENABLE_LIBOPENJPEG:STRING=openjpeg2 \
          -DENABLE_NSS3:BOOL=OFF \
          -DENABLE_GPGME:BOOL=OFF \
          -DENABLE_LIBTIFF:BOOL=OFF \
          -DENABLE_QT5:BOOL=OFF \
          -DENABLE_QT6:BOOL=OFF \
          -DENABLE_LIBCURL:BOOL=OFF \
 && make V=1 -j${BUILD_CONCURRENCY} \
 && make install

ENV PKG_CONFIG_PATH="/src/vendor/github.com/sensiblecodeio/poppler/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/github.com/sensiblecodeio/poppler/build/install/lib $LINKFLAGS" \
    CXXFLAGS="-I/src/vendor/github.com/sensiblecodeio/poppler/build/install/include $CXXFLAGS"


COPY --chown=nobody:nogroup ./src /src/src
COPY --chown=nobody:nogroup waf wscript .

RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    \
    ./waf configure --static --release || { cat build/config.log; exit 1; }

RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    \
    ./waf build

ENTRYPOINT ["/src/build/pdf2msgpack"]
