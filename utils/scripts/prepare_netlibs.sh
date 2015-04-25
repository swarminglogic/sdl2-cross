#!/bin/bash

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
TEAL=$(tput setaf 6)
NORMAL=$(tput sgr0)
BOLD=$(tput bold)
MODEST=${YELLOW}${BOLD}

if [ ! -f utils/scripts/prepare_netlibs.sh ] ; then
    echo "${RED}Error:${NORMAL} execute script from base directory"
    exit 1
fi

cd external/netlibs/

function get_source {
    echo "
${GREEN}[ Preparing ${BOLD}$1${NORMAL}${GREEN} ]${NORMAL}"
    if [ -d "$1" ] ; then
        echo " - Directory $1 already exists. ${YELLOW}Skipping.${NORMAL}"
    else
        echo " - Downloading $1..."
        eval "$2"
    fi
}

# $1 filepath
# $2 command to download and prepare the file
function get_file {
    echo "
${GREEN}[ Preparing ${BOLD}$1${NORMAL}${GREEN} ]${NORMAL}"
    if [ -f "$1" ] ; then
        echo " - File $1 already exists. ${YELLOW}Skipping.${NORMAL}"
    else
        echo " - Downloading $1..."
        eval "$2"
    fi
}


function say_done {
    echo "${GREEN} - done${NORMAL}"
}


function unpack {
    if [ -e "$1" ] ; then
        echo "${TEAL} - unpacking"
        tar xf "$1" && rm "$1"
    fi
}


# $1 type (scons, android)
# $2 source
# $3 destination
function add_build_script {
    if [ -e "$3" ] ; then
        echo " - Found $1 build script ${BOLD}$3${NORMAL} ${YELLOW}Skipping${NORMAL}"
    else
        echo "${TEAL} - adding $1 build script ${BOLD}$3${NORMAL}"
        if [ ! -d $(dirname "$3") ] ; then mkdir -p $(dirname "$3") ; fi
        nsub=$(($(grep -o '/' <<< "$3" | wc -l) + 2))
        subdepth=$(yes "../" | head -n $nsub | xargs)
        subdepth=${subdepth// /}
        ln -s ${subdepth}utils/build_scripts/netlibs/"$2" "$3"
    fi
}

# $1 file
function remove_build_file {
    if [ ! -e "$1" ] ; then
        echo " - File ${BOLD}$1${NORMAL} already removed \
${BOLD}$3${NORMAL} ${YELLOW}Skipping${NORMAL}"
    else
        echo "${TEAL} - removing file ${BOLD}$1${NORMAL}"
        rm "$1"
    fi
}


# zeromq
get_source zeromq-4.1.0 \
"wget http://download.zeromq.org/zeromq-4.1.0-rc1.tar.gz"
unpack "zeromq-4.1.0-rc1.tar.gz"
if [ -e zeromq-4.1.0/.patched ] ; then
    echo " - Already patched. ${YELLOW}Skipping${NORMAL}"
else
    echo "${TEAL} - patching ${NORMAL}"
    patch -p1 < ../../utils/patches/zeromq-4.1.0.mingw.patch
    touch zeromq-4.1.0/.patched
fi
add_build_script "SCons" SConscript_zeromq "zeromq-4.1.0/SConscript"
add_build_script "header file" zmq_platform_linux.hpp \
    "zeromq-4.1.0/include_linux/platform.hpp"
add_build_script "header file" zmq_platform_windows.hpp \
    "zeromq-4.1.0/include_windows/platform.hpp"
add_build_script "header file" zmq_platform_android.hpp \
    "zeromq-4.1.0/include_android/platform.hpp"
add_build_script "Android.mk" Android_zmq.mk "zeromq-4.1.0/Android.mk"
say_done

get_file zeromq-4.1.0/include/zmq.hpp \
"wget https://raw.githubusercontent.com/zeromq/cppzmq/master/zmq.hpp -O zeromq-4.1.0/include/zmq.hpp"
say_done

# re
get_source czmq-3.0.0 \
"wget http://download.zeromq.org/czmq-3.0.0-rc1.tar.gz"
unpack "czmq-3.0.0-rc1.tar.gz"
if [ -e czmq-3.0.0/.patched ] ; then
    echo " - Already patched. ${YELLOW}Skipping${NORMAL}"
else
    echo "${TEAL} - patching ${NORMAL}"
    patch -p1 < ../../utils/patches/czmq-3.0.0.mingw.patch
    touch czmq-3.0.0/.patched
fi
remove_build_file "czmq-3.0.0/src/platform.h"
add_build_script "SCons" SConscript_czmq "czmq-3.0.0/SConscript"
add_build_script "header file" czmq_platform_windows.h \
    "czmq-3.0.0/include_windows/platform.h"
add_build_script "header file" czmq_platform_linux.h \
    "czmq-3.0.0/include_linux/platform.h"
add_build_script "header file" czmq_platform_android.h \
    "czmq-3.0.0/include_android/platform.h"
add_build_script "Android.mk" Android_czmq.mk "czmq-3.0.0/Android.mk"


function build_static_android {
    echo "${GREEN}[ Building static ${BOLD}$1${NORMAL}${GREEN} for android]${NORMAL}"
    ../../utils/build_scripts/netlibs/$2
    say_done
}

# Check all dependencies, ask to compile everything
if  [ -d zeromq-4.1.0 ] && \
    [ -d czmq-3.0.0/ ] && \
    [ ! -e ../../android/libs/*/libzmq.a ] && \
    [ ! -e ../../android/libs/*/libczmq.a ] ; then

    echo "
 ${GREEN}All dependencies met.${NORMAL}"
    while true; do
        read -p " Do you want to build static android libraries now? [y/n] " yn
        case $yn in
            [Yy]* )
                build_static_android zeromq-4.1.0         android-build-zeromq.sh
                build_static_android czmq-3.0.0           android-build-czmq.sh
                break;
                ;;
            [Nn]* )
                break;
                ;;
            * ) echo "Please answer yes or no.";;
        esac
    done
fi
