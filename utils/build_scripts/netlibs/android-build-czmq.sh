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
CPPFLAGS="--sysroot=${SYSROOT}"
CXXCPPFLAGS="${CPPFLAGS}"

# BUILD TOOLS
CC=${BIN}-gcc
RANLIB=${BIN}-ranlib
CPP=${BIN}-cpp
CXX=${BIN}-g++
LD=${BIN}-ld
CXXCPP=$CPP
STRIP=${BIN}-strip
AR=${BIN}-ar
AS=${BIN}-as

# TODO swarminglogic, 2015-03-04: add exit if not correct path
BP=android-build
cd czmq-3.0.0 && mkdir -p $BP



CFLAGS="-std=gnu99 -O2 -DNDEBUG -Wall -DHAVE_CONFIG_H \
    -D_CRT_ERRNO_DEFINED \
	  -D_GNU_SOURCE=1 -D_REENTRANT -DLIBCZMQ_STATIC -D_THREAD_SAFE -DZMQ_STATIC \
	  -fPIC -fvisibility=hidden \
    -isystem ../zeromq-4.1.0/include \
    -Iinclude
    -Iinclude_android
    -I${NDK_PATH}/platforms/${APP_PLATFORM}/arch-${ARCH}/usr/include"


SOURCES="src/czmq_selftest.c
src/zbeacon.c
src/zchunk.c
src/zdigest.c
src/zframe.c
src/zlist.c
src/zmsg.c
src/zproxy_v2.c
src/zsocket.c
src/zsys.c
src/zactor.c
src/zclock.c
src/zdir.c
src/zgossip.c
src/zhash.c
src/zloop.c
src/zmutex.c
src/zrex.c
src/zsockopt.c
src/zthread.c
src/zauth.c
src/zcert.c
src/zconfig.c
src/zdir_patch.c
src/zmonitor.c
src/zpoller.c
src/zring.c
src/zsock_option.c
src/zuuid.c
src/zauth_v2.c
src/zcertstore.c
src/zctx.c
src/zfile.c
src/zgossip_msg.c
src/ziflist.c
src/zmonitor_v2.c
src/zproxy.c
src/zsock.c
src/zstr.c"


function cc_source {
    if [ ! -e "$BP/${filename/.c/.o}" ] ; then
        tmpoutfile=$(tempfile)
        ${CC} ${CFLAGS} -c ${file} \
            -MMD -MP -MF $BP/${filename/.c/.o.d} \
            -o $BP/${filename/.c/.o} \
            2> $tmpoutfile
        isOk=$?
        echo "  CC ${file/.c/.o}"
        if [ ! $isOk -eq 0 ] ; then
            cat $tmpoutfile
        fi
        rm $tmpoutfile
    else
        echo "  CC ${file/.c/.o}"
    fi
}

# Build sources
OBJS=""
pIDs=""
while read file ; do
    filename=$(basename "$file")
    if [ "${file/.c/}" != "$file" ] ; then
		    (cc_source)&
        pIDs="$! $pIDs"
		    OBJS="$OBJS ${BP}/${filename/.c/.o}"
    else
        echo " Error, unspported filetype in sources"
        exit 0;
    fi
done <<< "$SOURCES"
wait $pIDs

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
TEAL=$(tput setaf 6)
NORMAL=$(tput sgr0)
BOLD=$(tput bold)
MODEST=${YELLOW}${BOLD}

echo "  AR libczmq.a"
if ! ls $OBJS &> /dev/null  ; then
    echo "${RED} Error, failed to build some of the sources${NORMAL}"
else
    # Create library
    ${AR} crsD $BP/libczmq.a $OBJS
    install -m 644 $BP/libczmq.a \
        ../../../android/libs/${APP_ABI}/libczmq.a
fi
