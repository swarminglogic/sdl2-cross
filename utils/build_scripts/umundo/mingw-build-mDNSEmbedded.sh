#!/bin/bash


# USER DEFINES
# TODO swarminglogic, 2015-03-04: Refactor these defines
PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
CROSSPATH=$PREFIX/$TOOLSET
CC="$TOOLSET-gcc -static-libgcc"
CXX="${TOOLSET}-g++ -static-libgcc"
AR="${TOOLSET}-ar"
CC="${TOOLSET}-gcc"
LD="${TOOLSET}-ld"
CPP="${TOOLSET}-cpp"

SOURCES="mDNSCore/DNSCommon.c
mDNSCore/DNSDigest.c
mDNSCore/mDNS.c
mDNSCore/uDNS.c
mDNSMacOSX/LegacyNATTraversal.c
mDNSShared/DebugServices.c
mDNSShared/dnssd_clientshim.c
mDNSShared/dnssd_ipc.c
mDNSShared/GenLinkedList.c
mDNSShared/mDNSDebug.c
mDNSWindows/mDNSWin32.c
mDNSWindows/Poll.c
mDNSWindows/Secret.c"

# mDNSWindows/SystemService/Firewall.cpp


# TODO swarminglogic, 2015-03-04: add exit if not correct path
BP=mingw-build
cd mDNSResponder-333.10 && mkdir -p $BP

CFLAGS="-DDISC_BONJOUR_EMBED -DWIN32 -DNDEBUG -D_CONSOLE -D_WIN32_WINNT=0x0501 \
-DTARGET_OS_WIN32 -DWIN32_LEAN_AND_MEAN -DUSE_TCP_LOOPBACK -DPLATFORM_NO_STRSEP \
-DPLATFORM_NO_EPIPE -DPLATFORM_NO_RLIMIT -DPID_FILE="" -DUNICODE -D_UNICODE \
-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1 \
-D_LEGACY_NAT_TRAVERSAL_ \
-ImDNSCore -ImDNSWindows -ImDNSShared \
"


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

function cpp_source {
    if [ ! -e "$BP/${filename/.c/.o}" ] ; then
        tmpoutfile=$(tempfile)
        ${CXX} ${CFLAGS} -c ${file} \
            -MMD -MP -MF $BP/${filename/.c/.o.d} \
            -o $BP/${filename/.c/.o} \
            2> $tmpoutfile
        isOk=$?
        echo "  CXX ${file/.c/.o}"
        if [ ! $isOk -eq 0 ] ; then
            cat $tmpoutfile
        fi
        rm $tmpoutfile
    else
        echo "  CXX ${file/.c/.o}"
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
    elif [ "${file/.cpp/}" != "$file" ] ; then
		    (cpp_source)&
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

libname=libmDNSEmbedded.a
echo "  AR $libname"
if ! ls $OBJS &> /dev/null  ; then
    echo "${RED} Error, failed to build some of the sources${NORMAL}"
else
    # Create library
    ${AR} crsD ${BP}/${libname} ${OBJS}
    sudo install -m 644 $BP/${libname} $CROSSPATH/lib/${libname}
fi

