# syntax = docker/dockerfile:1.4

FROM alpine:3.24 AS cachebase
RUN mkdir -p /tmp/ccache \
 && chown nobody:nogroup /tmp/ccache

# Base build stage containing required packages
FROM alpine:3.24 AS build-base
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

RUN mkdir -p /src && chown nobody:nogroup /src
WORKDIR /src
USER nobody:nogroup

# --- PARALLEL STAGE 1A: FreeType ---
FROM build-base AS build-freetype
COPY --chown=nobody:nogroup ./vendor/gitlab.freedesktop.org/freetype/freetype /src/vendor/gitlab.freedesktop.org/freetype/freetype
RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    cd vendor/gitlab.freedesktop.org/freetype/freetype/ \
 && meson setup build --prefix=$PWD/build/install --default-library=static \
 && ninja -C build -j${BUILD_CONCURRENCY} install


# --- PARALLEL STAGE 1B: Little-CMS ---
FROM build-base AS build-lcms
COPY --chown=nobody:nogroup ./vendor/github.com/mm2/Little-CMS /src/vendor/github.com/mm2/Little-CMS
RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    cd vendor/github.com/mm2/Little-CMS/ \
 && meson setup build --prefix=$PWD/build/install --default-library=static \
 && ninja -C build -j${BUILD_CONCURRENCY} install


# --- PARALLEL STAGE 1C: OpenJPEG ---
FROM build-base AS build-openjpeg
COPY --chown=nobody:nogroup ./vendor/github.com/uclouvain/openjpeg /src/vendor/github.com/uclouvain/openjpeg
RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    cd vendor/github.com/uclouvain/openjpeg/ \
 && cmake -B build -G Ninja -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/build/install \
 && ninja -C build -j${BUILD_CONCURRENCY} install


# --- STAGE 2: Fontconfig (Requires FreeType) ---
FROM build-base AS build-fontconfig
COPY --from=build-freetype /src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install /src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install
COPY --chown=nobody:nogroup ./vendor/gitlab.freedesktop.org/fontconfig/fontconfig /src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig

ENV PKG_CONFIG_PATH="/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install/lib $LINKFLAGS" \
    FREETYPE_DIR=/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install

RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    cd vendor/gitlab.freedesktop.org/fontconfig/fontconfig/ \
 && meson setup build --prefix=$PWD/build/install --default-library=static \
 && ninja -C build -j${BUILD_CONCURRENCY} install


# --- STAGE 3: Poppler (Requires FreeType, Fontconfig, LCMS, OpenJPEG) ---
FROM build-base AS build-poppler
COPY --from=build-freetype /src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install /src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install
COPY --from=build-fontconfig /src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install /src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install
COPY --from=build-lcms /src/vendor/github.com/mm2/Little-CMS/build/install /src/vendor/github.com/mm2/Little-CMS/build/install
COPY --from=build-openjpeg /src/vendor/github.com/uclouvain/openjpeg/build/install /src/vendor/github.com/uclouvain/openjpeg/build/install
COPY --chown=nobody:nogroup ./vendor/github.com/cantabular/poppler /src/vendor/github.com/cantabular/poppler

ENV PKG_CONFIG_PATH="/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install/lib/pkgconfig:/src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install/lib/pkgconfig:/src/vendor/github.com/mm2/Little-CMS/build/install/lib/pkgconfig:/src/vendor/github.com/uclouvain/openjpeg/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install/lib -L/src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install/lib -L/src/vendor/github.com/mm2/Little-CMS/build/install/lib -L/src/vendor/github.com/uclouvain/openjpeg/build/install/lib $LINKFLAGS" \
    CXXFLAGS="-I/src/vendor/github.com/uclouvain/openjpeg/build/install/include $CXXFLAGS" \
    LDFLAGS="-L/src/vendor/github.com/uclouvain/openjpeg/build/install/lib $LDFLAGS" \
    OpenJPEG_DIR="/src/vendor/github.com/uclouvain/openjpeg/build/install/lib/cmake/openjpeg-2.5" \
    FREETYPE_DIR=/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install

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
          -DENABLE_HARFBUZZ=OFF \
 && ninja -C build -j${BUILD_CONCURRENCY} install


# --- FINAL STAGE: Application Build (Waf) ---
FROM build-base AS final
COPY --from=build-freetype /src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install /src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install
COPY --from=build-fontconfig /src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install /src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install
COPY --from=build-lcms /src/vendor/github.com/mm2/Little-CMS/build/install /src/vendor/github.com/mm2/Little-CMS/build/install
COPY --from=build-openjpeg /src/vendor/github.com/uclouvain/openjpeg/build/install /src/vendor/github.com/uclouvain/openjpeg/build/install
COPY --from=build-poppler /src/vendor/github.com/cantabular/poppler/build/install /src/vendor/github.com/cantabular/poppler/build/install

COPY --chown=nobody:nogroup ./vendor/github.com/msgpack/msgpack-c /src/vendor/github.com/msgpack/msgpack-c
COPY --chown=nobody:nogroup ./src /src/src
COPY --chown=nobody:nogroup waf wscript .

ENV PKG_CONFIG_PATH="/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install/lib/pkgconfig:/src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install/lib/pkgconfig:/src/vendor/github.com/mm2/Little-CMS/build/install/lib/pkgconfig:/src/vendor/github.com/uclouvain/openjpeg/build/install/lib/pkgconfig:/src/vendor/github.com/cantabular/poppler/build/install/lib/pkgconfig:$PKG_CONFIG_PATH" \
    LINKFLAGS="-L/src/vendor/gitlab.freedesktop.org/freetype/freetype/build/install/lib -L/src/vendor/gitlab.freedesktop.org/fontconfig/fontconfig/build/install/lib -L/src/vendor/github.com/mm2/Little-CMS/build/install/lib -L/src/vendor/github.com/uclouvain/openjpeg/build/install/lib -L/src/vendor/github.com/cantabular/poppler/build/install/lib $LINKFLAGS" \
    CXXFLAGS="-I/src/vendor/github.com/cantabular/poppler/build/install/include $CXXFLAGS"

RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    ./waf configure --static --release || { cat build/config.log; exit 1; }

RUN --mount=type=cache,src=/tmp/ccache,target=/tmp/ccache,id=ccache,from=cachebase \
    ./waf build

ENTRYPOINT ["/src/build/pdf2msgpack"]
