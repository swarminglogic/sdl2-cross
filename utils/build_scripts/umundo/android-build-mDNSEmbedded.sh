#!/bin/bash


# USER DEFINES
# TODO swarminglogic, 2015-03-04: Refactor these defines
# TODO swarminglogic, 2015-03-04: Learn how to properly write makefiles ^_^
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
cd mDNSResponder-333.10 && mkdir -p $BP

CFLAGS=" -ffunction-sections -funwind-tables -fstack-protector \
-no-canonical-prefixes -march=armv5te -mtune=xscale -msoft-float -mthumb \
-Os -DNDEBUG \
-fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 \
-marm -fno-omit-frame-pointer \
-DANDROID -fwrapv \
-W -Wall -DPID_FILE=\"/var/run/mdnsd.pid\" \
-DMDNS_UDS_SERVERPATH=\"/var/run/mdnsd\" \
-DTARGET_OS_ANDROID -DNOT_HAVE_SA_LEN -DHAVE_LINUX \
-DUSES_NETLINK -DUNICAST_DISABLED -DDNSSEC_DISABLED \
-fpic -Wdeclaration-after-statement \
-DMDNS_DEBUGMSGS=0 \
-isystem .//mDNSShared \
-isystem .//mDNSCore \
-Wa,--noexecstack -Wformat -Werror=format-security \
-I${NDK_PATH}/platforms/${APP_PLATFORM}/arch-${ARCH}/usr/include"

SOURCES="mDNSShared/dnssd_clientshim.c
mDNSPosix/mDNSPosix.c
mDNSCore/mDNS.c
mDNSCore/DNSCommon.c
mDNSShared/mDNSDebug.c
mDNSShared/GenLinkedList.c
mDNSCore/uDNS.c
mDNSShared/PlatformCommon.c
mDNSPosix/mDNSUNP.c
mDNSCore/DNSDigest.c"

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

# echo "PIDS: ${RED}$pIDs${TEAL}"
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

echo "  AR libmDNSEmbedded.a"
if ! ls $OBJS &> /dev/null  ; then
    echo "${RED} Error, failed to build some of the sources${NORMAL}"
else
    # Create library
    ${AR} crsD $BP/libmDNSEmbedded.a $OBJS
    install -m 644 $BP/libmDNSEmbedded.a \
        ../../../android/libs/${APP_ABI}/libmDNSEmbedded.a
fi

