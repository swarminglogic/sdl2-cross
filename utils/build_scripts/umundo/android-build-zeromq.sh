#!/bin/bash

# USER DEFINES
# TODO swarminglogic, 2015-03-04: Refactor these defines
HOST_OS=linux-x86_64
ARCH=arm
NDK_PATH=${NDK_HOME}
APP_PLATFORM=android-18
NDK_TOOLCHAIN_VERSION=4.8
APP_ABI=armeabi

# CONSTRUCTED DEFINES
SYSROOT=${NDK_PATH}/platforms/${APP_PLATFORM}/arch-${ARCH}
PREBUILT=${NDK_PATH}/toolchains/arm-linux-androideabi-${NDK_TOOLCHAIN_VERSION}/prebuilt
BIN=${PREBUILT}/${HOST_OS}/bin/arm-linux-androideabi

# EXTRA
export CPPFLAGS="--sysroot=${SYSROOT}"
export CXXCPPFLAGS="${CPPFLAGS}"

# BUILD TOOLS
export CC=${BIN}-gcc
export RANLIB=${BIN}-ranlib
export CPP=${BIN}-cpp
export CXX=${BIN}-g++
export LD=${BIN}-ld
export CXXCPP=$CPP
export STRIP=${BIN}-strip
export AR=${BIN}-ar
export AS=${BIN}-as


# Aremabi specific flags and other stuff
CC_ARMEABI_FLAGS="${CC_ARMEABI_FLAGS} \
-D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5__ \
-fsigned-char \
-march=armv5te \
-mtune=xscale \
-msoft-float \
-mfpu=vfp \
-mfloat-abi=softfp \
-fPIC \
-finline-limit=64"


ARMEABI_COMMON_FLAGS="\
-isystem ${NDK_PATH}/sources/android/support/include \
-isystem ${NDK_PATH}/platforms/${APP_PLATFORM}/arch-${ARCH}/usr/include \
-isystem ${NDK_PATH}/sources/cxx-stl/llvm-libc++/libcxx/include/ \
-isystem ${NDK_PATH}/sources/cxx-stl/gabi++/include \
"
echo "$ARMEABI_COMMON_FLAGS"
ARMEABI_CPP_STDLIB="${NDK_PATH}/sources/cxx-stl/llvm-libc++/libs/${APP_ABI}"


export CXXFLAGS="-Os -s ${CC_ARMEABI_FLAGS} --sysroot=${SYSROOT} ${ARMEABI_COMMON_FLAGS}" \
export CFLAGS="-Os -s ${CC_ARMEABI_FLAGS} --sysroot=${SYSROOT} ${ARMEABI_COMMON_FLAGS}" \
# export LDFLAGS="--sysroot=${SYSROOT} -L${ARMEABI_CPP_STDLIB} -lgnustl_static"
export LDFLAGS="--sysroot=${SYSROOT} -L${ARMEABI_CPP_STDLIB} -lc++_shared"
export CPPFLAGS="--sysroot=${SYSROOT}"
export CXXCPPFLAGS="${CPPFLAGS}"
export CC=${BIN}-gcc
export RANLIB=${BIN}-ranlib
export CPP=${BIN}-cpp
export CXX=${BIN}-g++
export LD=${BIN}-ld
export CXXCPP=$CPP
export STRIP=${BIN}-strip
export AR=${BIN}-ar
export AS=${BIN}-as

# Executing configure script
builddir=zeromq-4.1.0/android-build
mkdir -p $builddir 2>/dev/null
cd $builddir

../configure --prefix="$(pwd)/android-prefix" \
--disable-dependency-tracking \
--host=arm-linux-androideabi \
--enable-static \
--disable-shared \
--without-documentation

make install -C src # VERBOSE=1 V=1
LIB_DEST_DIR="../../../../android/libs/${APP_ABI}/"
mkdir -p $LIB_DEST_DIR
install -m 644 android-prefix/lib/libzmq.a $LIB_DEST_DIR
