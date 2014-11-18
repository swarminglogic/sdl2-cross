#!/bin/bash

PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
CROSSPATH=$PREFIX/$TOOLSET
export CC="$TOOLSET-gcc -static-libgcc"
NP=5 #number of cores to use when making

tmpdir=/tmp/x-comp
if [ ! -d $tmpdir ] ; then mkdir $tmpdir ; fi

# $1 configure script
# ${@:2} additional parameters
function call_configure {
    $1 --target=$TOOLSET --host=$TOOLSET \
       --build=x86_64-linux --prefix=$CROSSPATH ${@:2}
}
function call_make_install {
    nice make -j${NP}
    sudo make install
}

# $1 Env variable to check if set
function require_env_xor_abort {
    if env | grep -q ^$1= ; then
        return 0
    else
        echo "Env variable $1 no set - aborting"
        exit
    fi
}

# $@ list of static libraries needed
function require_lib_xor_abort {
    for i in $@ ; do
        if [ ! -f $CROSSPATH/lib/$i ] ; then
            echo "Missing prerequisite $CROSSPATH/lib/$i aborting"
            isBad=yes
        fi
    done
    if [ "$isBad" == "yes" ] ; then
        exit
    fi
}

function x-compile-zlib {
    # We do this in /tmp/x-comp
    cd $tmpdir

    # Download zlib 1.2.8
    wget http://zlib.net/zlib128.zip
    unzip zlib128.zip
    cd zlib-1.2.8

    # Specify our toolchain in the makefile
    perl -i -pe 's,(PREFIX =)$,$1 x86_64-w64-mingw32-,' win32/Makefile.gcc

    # Call make
    make -f win32/Makefile.gcc -j${NP}

    # Install it to our cross-tools directory
    sudo DESTDIR=$CROSSPATH/ \
        INCLUDE_PATH=include  LIBRARY_PATH=lib  BINARY_PATH=bin \
        make install -f win32/Makefile.gcc
}


function x-compile-libpng {
    # We do this in /tmp/x-comp
    cd $tmpdir

    # Download libpng 1.2.50
    wget http://download.sourceforge.net/libpng/libpng-1.2.50.tar.gz
    tar xf libpng-1.2.50.tar.gz
    cd libpng-1.2.50

    # Configure
    export CFLAGS="-I${CROSSPATH}/include"
    export LDFLAGS="-L${CROSSPATH}/lib"
    call_configure ./configure
    call_make_install
}


function x-compile-libjpeg {
    # We do this in /tmp/x-comp
    cd $tmpdir

    # check if you have nasm assembler, if not, install it
    if ! command -v nasm ; then
        sudo apt-get install nasm
    fi

    # Download libjpeg-turbo 1.3.0
    wget "http://downloads.sourceforge.net/project/"\
"libjpeg-turbo/1.3.0/libjpeg-turbo-1.3.0.tar.gz"
    tar xf libjpeg-turbo-1.3.0.tar.gz
    cd libjpeg-turbo-1.3.0/

    export CFLAGS="-I${CROSSPATH}/include"
    export LDFLAGS="-L${CROSSPATH}/lib"
    call_configure ./configure
    call_make_install
}


function x-compile-libogg {
    # We do this in /tmp/x-comp
    cd $tmpdir

    # Download libogg 1.3.1
    wget http://downloads.xiph.org/releases/ogg/libogg-1.3.1.tar.gz
    tar xf libogg-1.3.1.tar.gz
    cd libogg-1.3.1/

    call_configure ./configure
    call_make_install
}


function x-compile-libvorbis {
    # We do this in /tmp/x-comp
    cd $tmpdir

    # # Download libvorbis 1.3.2
    wget http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.2.tar.gz
    tar xf libvorbis-1.3.2.tar.gz
    cd libvorbis-1.3.2/

    export CFLAGS="-I${CROSSPATH}/include"
    export LDFLAGS="-L${CROSSPATH}/lib"
    call_configure ./configure
    call_make_install
}


function x-compile-freetype {
    # We do this in /tmp/x-comp
    cd $tmpdir

    # Download freetype-2.5.2
    wget http://download.savannah.gnu.org/releases/freetype/freetype-2.5.2.tar.gz
    tar xf freetype-2.5.2.tar.gz
    cd freetype-2.5.2/

    PATH=${CROSSPATH}/bin:$PATH
    export CFLAGS="-I${CROSSPATH}/include"
    export LDFLAGS="-L${CROSSPATH}/lib"
    export LIBPNG="`libpng-config --libs`"
    export LIBPNG_CFLAGS="`libpng-config --cflags`"
    export LIBPNG_LDFLAGS="`libpng-config --ldflags`"

    call_configure ./configure
    call_make_install
}


function x-compile-glew {
    # We do this in /tmp/x-comp
    cd $tmpdir

    # Download glew 1.11.0
    wget http://downloads.sourceforge.net/project/glew/glew/1.11.0/glew-1.11.0.tgz
    tar xf  glew-1.11.0.tgz
    cd glew-1.11.0/

    # Slightly trickier, since it doesn't use configure
    make SYSTEM=linux-mingw64            \
        CC=${TOOLSET}-gcc LD=${TOOLSET}-ld \
        LDFLAGS.EXTRA=-L${CROSSPATH}/lib   \
        GLEW_PREFIX=${CROSSPATH}           \
        GLEW_DEST=${CROSSPATH} -j${NP}

    sudo make SYSTEM=linux-mingw64       \
        CC=${TOOLSET}-gcc LD=${TOOLSET}-ld \
        LDFLAGS.EXTRA=-L${CROSSPATH}/lib   \
        GLEW_PREFIX=${CROSSPATH}           \
        GLEW_DEST=${CROSSPATH} install
}


function x-compile-bullet {
    cd "$1"

    if [ ! -f "src/btBulletCollisionCommon.h" ] ; then
        echo "`pwd` not valid Bullet source dir. Aborting"
        exit
    fi

    if [ -d 'build-w64' ] ; then rm -rf build-w64 ; fi
    mkdir build-w64 && cd $_

    call_configure ../configure --disable-demos
    call_make_install
}



# $1 Name of Repo (SDL, SDL_image, etc)
# $2 Path to repo
# $3 Required file to find in repo
# ${@:4} Additional configure commands
function x-compile-sdl-any {
    cd "$2"

    if [ ! -f "$3" ] ; then
        echo "`pwd` not valid $1 source dir. Aborting"
        exit
    fi

    if [ -d 'build-w64' ] ; then rm -rf build-w64 ; fi
    mkdir build-w64 && cd $_

    export PKG_CONFIG_PATH=${CROSSPATH}/lib/pkgconfig
    export PATH=${CROSSPATH}/bin:$PATH
    export CFLAGS="-I${CROSSPATH}/include"
    export LDFLAGS="-L${CROSSPATH}/lib"

    call_configure ../configure ${@:4}
    call_make_install
}

while test $# -gt 0; do
    case "$1" in
       --list-targets)
            echo "--x-comp-zlib
--x-comp-libpng
--x-comp-libjpeg
--x-comp-libogg
--x-comp-libvorbis
--x-comp-freetype
--x-comp-glew
--x-comp-sdl
--x-comp-sdl-image
--x-comp-sdl-mixer
--x-comp-sdl-ttf"
            exit
            ;;
        --x-comp-zlib)
            (x-compile-zlib)
            exit
            ;;
        --x-comp-libpng)
            require_lib_xor_abort libz.a
            (x-compile-libpng)
            exit
            ;;
        --x-comp-libjpeg)
            (x-compile-libjpeg)
            exit
            ;;
        --x-comp-libogg)
            (x-compile-libogg)
            exit
            ;;
        --x-comp-libvorbis)
            require_lib_xor_abort libogg.a
            (x-compile-libvorbis)
            exit
            ;;
        --x-comp-freetype)
            require_lib_xor_abort libz.a
            (x-compile-freetype)
            exit
            ;;
        --x-comp-glew)
            (x-compile-glew)
            exit
            ;;
        --x-comp-bullet)
            require_env_xor_abort BULLET_SRC_DIR
            (x-compile-bullet "$BULLET_SRC_DIR")
            exit
            ;;
        --x-comp-sdl)
            shift
            require_env_xor_abort SDL2_SRC_DIR
            (x-compile-sdl-any "SDL" "$SDL2_SRC_DIR" "src/SDL.c")
            exit
            ;;
        --x-comp-sdl-image)
            shift
            require_lib_xor_abort libpng.a libjpeg.a libz.a libSDL2.a
            require_env_xor_abort SDL2_IMAGE_SRC_DIR
            (x-compile-sdl-any "SDL_image" "$SDL2_IMAGE_SRC_DIR" "IMG.c" --disable-webp)
            exit
            ;;
        --x-comp-sdl-mixer)
            shift
            require_lib_xor_abort libvorbis.a libogg.a
            require_env_xor_abort SDL2_MIXER_SRC_DIR
            (x-compile-sdl-any "SDL_mixer" "$SDL2_MIXER_SRC_DIR" "mixer.c" --disable-music-mp3-smpeg)
            exit
            ;;
        --x-comp-sdl-ttf)
            shift
            require_lib_xor_abort libfreetype.a
            require_env_xor_abort SDL2_TTF_SRC_DIR
            (x-compile-sdl-any "SDL_ttf" "$SDL2_TTF_SRC_DIR" "SDL_ttf.c")
            exit
            ;;
        --x-comp-sdl-all)
            $0 --x-comp-sdl
            $0 --x-comp-sdl-image
            $0 --x-comp-sdl-mixer
            $0 --x-comp-sdl-ttf
            exit
            ;;
        --x-comp-all)
            $0 --x-comp-zlib
            $0 --x-comp-libpng
            $0 --x-comp-libjpeg
            $0 --x-comp-libogg
            $0 --x-comp-libvorbis
            $0 --x-comp-freetype
            $0 --x-comp-glew
            $0 --x-comp-sdl-all
            exit
            ;;
        *)
            echo "Unkown flag: "$1
            exit
            ;;
    esac
done
