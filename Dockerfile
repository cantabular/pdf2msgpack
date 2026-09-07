# syntax = docker/dockerfile:experimental

FROM alpine:3.24 AS cachebase
RUN mkdir -p /tmp/ccache \
 && chown nobody:nogroup /tmp/ccache

FROM alpine:3.24

ARG BUILD_CONCURRENCY=4

RUN --mount=type=cache,target=/etc/apk/cache,id=apk-cache \
    --mount=type=cache,target=/var/lib/apk,id=apk-lib \
    apk add \
      build-base \
      bzip2-dev \
      bzip2-static \
      ccache \
      cmake \
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
      linux-headers \
      meson \
      ninja \
      pkgconf \
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

# FreeType (Meson)
RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    cd vendor/gitlab.freedesktop.org/freetype/freetype/ \
 && meson setup build --prefix=$PWD/build/install --default-library=static \
 && ninja -C build -j${BUILD_CONCURRENCY} install

ENV PKG_CONFIG_PATH="/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install/lib $LINKFLAGS" \
    FREETYPE_DIR=/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install

# Fontconfig (Meson)
RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    cd vendor/gitlab.freedesktop.org/fontconfig/fontconfig/ \
 && meson setup build --prefix=$PWD/build/install --default-library=static \
 && ninja -C build -j${BUILD_CONCURRENCY} install

ENV PKG_CONFIG_PATH="/src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install/lib $LINKFLAGS"

# Little-CMS (Meson)
RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    cd vendor/github.com/mm2/Little-CMS/ \
 && meson setup build --prefix=$PWD/build/install --default-library=static \
 && ninja -C build -j${BUILD_CONCURRENCY} install

ENV PKG_CONFIG_PATH="/src/vendor/github.com/mm2/Little-CMS/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/github.com/mm2/Little-CMS/build/install/lib $LINKFLAGS"

# OpenJPEG (CMake + Ninja)
RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    cd vendor/github.com/uclouvain/openjpeg/ \
 && cmake -B build -G Ninja -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/build/install \
 && ninja -C build -j${BUILD_CONCURRENCY} install

ENV PKG_CONFIG_PATH="/src/vendor/github.com/uclouvain/openjpeg/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    CXXFLAGS="-I/src/vendor/github.com/uclouvain/openjpeg/build/install/include $CXXFLAGS" \
    LDFLAGS="-L/src/vendor/github.com/uclouvain/openjpeg/build/install/lib $LDFLAGS" \
    LINKFLAGS="-L/src/vendor/github.com/uclouvain/openjpeg/build/install/lib $LINKFLAGS" \
    OpenJPEG_DIR="/src/vendor/github.com/uclouvain/openjpeg/build/install/lib/cmake/openjpeg-2.5"

# Poppler (CMake + Ninja - tests disabled to prevent target linking failures)
RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    cd vendor/github.com/cantabular/poppler/ \
 && cmake -B build -G Ninja \
          -DCMAKE_INSTALL_PREFIX=$PWD/build/install \
          -DBUILD_TESTS=OFF \
          -DBUILD_MANUAL_TESTS=OFF \
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
 && ninja -C build -j${BUILD_CONCURRENCY} install

ENV PKG_CONFIG_PATH="/src/vendor/github.com/cantabular/poppler/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/github.com/cantabular/poppler/build/install/lib $LINKFLAGS" \
    CXXFLAGS="-I/src/vendor/github.com/cantabular/poppler/build/install/include $CXXFLAGS"

# Application Build (Waf)
COPY --chown=nobody:nogroup ./src /src/src
COPY --chown=nobody:nogroup waf wscript .

RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    ./waf configure --static --release || { cat build/config.log; exit 1; }

RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    ./waf build

ENTRYPOINT ["/src/build/pdf2msgpack"]
