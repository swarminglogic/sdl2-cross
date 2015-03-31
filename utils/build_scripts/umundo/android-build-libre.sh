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
SYSROOT=${NDK_PATH}/platforms/${APP_PLATFORM}/arch-${ARCH}/usr
PREBUILT=${NDK_PATH}/toolchains/arm-linux-androideabi-${NDK_TOOLCHAIN_VERSION}/prebuilt
BIN=${PREBUILT}/${HOST_OS}/bin
CC=${BIN}/arm-linux-androideabi-gcc
RANLIB=${BIN}/arm-linux-androideabi-ranlib

# BUILD DEFINES
CFLAGS="-I${SYSROOT}/include/ \
-Wno-cast-align \
-Wno-shadow \
-Wno-nested-externs \
-march=armv7-a"
LFLAGS="-L${SYSROOT}/lib --sysroot=${NDK_PATH}/platforms/${APP_PLATFORM}/arch-${ARCH}"

# MAKEFILE VARIABLES
export COMMON_FLAGS="CC=${CC} \
RANLIB=${RANLIB} \
EXTRA_CFLAGS=\"${CFLAGS}\" \
EXTRA_LFLAGS=\"${LFLAGS}\" \
SYSROOT=${SYSROOT} \
SYSROOT_ALT= \
HAVE_LIBRESOLV= \
HAVE_PTHREAD=1 \
HAVE_PTHREAD_RWLOCK=1 \
HAVE_LIBPTHREAD= \
HAVE_INET_PTON=1 \
HAVE_INET6= \
PEDANTIC= \
OS=linux ARCH=${ARCH} \
USE_OPENSSL= \
USE_OPENSSL_DTLS= \
USE_OPENSSL_SRTP= \
ANDROID=yes \
PREFIX=\"./android-prefix\""

# CALL MAKEFILE TO BUILD
eval "make install -f Makefile_static -C re-0.4.7/ ${COMMON_FLAGS}"

# INSTALL TO CORRECT ANDROID DIR
INSTALLDIR=../../android/libs/${APP_ABI}
mkdir -p $INSTALLDIR
install -m 644 re-0.4.7/android-prefix/lib/libre.a $INSTALLDIR/libre.a && \
    echo "Android ${APP_ABI} build of libre.a was successful"
