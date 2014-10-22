#!/bin/bash
hasFailed=
isQuiet=
targetAndroidPlatform=android-18

GRAY=$(tput setaf 0)
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
BLUE=$(tput setaf 4)
TEAL=$(tput setaf 6)
NORMAL=$(tput sgr0)
BOLD=$(tput bold)

while test $# -gt 0; do
    case "$1" in
        -q|--quiet)
            shift
            isQuiet=yes
            ;;
        *)
            echo "Invalid parameter";
            exit
            ;;
    esac
done

# $1=text $2=status(0:ok, 1:warning, 2:failed)
function writeStatus {
    if [[ $2 -eq 0 ]] ; then
        color=$GREEN
    elif [[ $2 -eq 1 ]] ; then
        color=$YELLOW
    else
        color=$RED
    fi
    let COL=$(tput cols)-${#1}+${#color}+${#NORMAL}

    if [[ $2 -eq 0 ]] && [[ ! $isQuiet ]] ; then
        printf "%s%${COL}s" "$1" "$color[OK]$NORMAL        "
    elif [[ $2 -eq 1 ]] && [[ ! $isQuiet ]] ; then
        printf "%s%${COL}s" "$1" "$color[WARNING]$NORMAL   "
    elif [[ $2 -eq 2 ]] ; then
        printf "%s%${COL}s" "$1" "$color[FAILED]$NORMAL    "
    fi
}

# $1=text
function message {
    if [[ ! $isQuiet ]] ; then
        echo -e "$1"
    fi
}

# $1:=environment-variable
function validateEnvironmentVariable {
    if env | grep -q ^$1= ; then
        writeStatus "  - Environment variable $1 found" 0
        return 0
    else
        writeStatus "  - Environment variable $1 not set" 1
        return 1
    fi
}

# $1=path $2=libraryfile (lib[THIS].so)
function validateSharedLibrary {
    libraryPath=$1/lib${2}.so
    statusCode=0
    if [[ ! -e $libraryPath ]] ; then
        hasFailed=true
        statusCode=2
    fi
    writeStatus "  - $2 ($libraryPath)" $statusCode
}



# $1=name (e.g. boost) $2=searchFile  $3=requiredMatch [$4=specialExecutableArguments]
# If $4 is set, it assumes $3 in an executable finds paths with  `$2 $4`
# Both path/$2 and path/$3 must exist for the path to be valid.
# Value stored in $retvalue, if it finds a match.
function locateAny {
    message "  - Attempting to automatically find $1 (may take some time)"
    retvalue=
    searchFile=$2
    requiredMatch=$3
    possible=`locate "$searchFile" | grep ${searchFile}$ |  sed 's/.\{'${#searchFile}'\}$//'`
    if [[ $possible ]] ; then
        sorted=`echo "$possible" | awk '{ print length($0) " " $0; }' | sort -n | cut -d ' ' -f 2-`
        for i in $sorted ; do
            if [[ $# -eq 4 ]] ; then
                iexec=$i/$searchFile
                if [[ -x $iexec ]] && [[ -e `$iexec $4`/${requiredMatch} ]] ; then
                    retvalue=`$iexec $4`
                    return 0
                fi
            else
                if [[ -e ${i}/${requiredMatch} ]] ; then
                    retvalue=$i
                    return 0
                fi
            fi
        done
    fi
    return 1
}

accumVarDefines=
# $1=name (e.g. boost) $2=variableNameSet [$3=value, ${!TMP{2}} if not set]
function setEnvVariable {
    newPath=$3
    if [[ $# -eq 2 ]] ; then
        tmpprefix=TMP${2}
        newPath=${!tmpprefix}
    fi

    # echo ${!tmp}
    message "  - $1 found at ${BLUE}${newPath}${NORMAL}"
    message "  - Setting environment variable: $2=${newPath}"
    export ${2}=${newPath}
    accumVarDefines=${accumVarDefines}"\nexport "${2}=${newPath}
}

function findBoost {
    message "${GREEN}[boost library]${NORMAL}"
    TMPBOOST_DIR=
    if ! validateEnvironmentVariable BOOST_DIR ; then
        if validateEnvironmentVariable BOOST_ROOT ; then
            TMPBOOST_DIR=$BOOST_ROOT
        elif validateEnvironmentVariable BOOSTROOT ; then
            TMPBOOST_DIR=$BOOSTROOT
        else
            if locateAny "boost" \
                "/include/boost/algorithm/string.hpp" \
                "/include/boost/version.hpp" ; then
                TMPBOOST_DIR=$retvalue
            fi
        fi
        if [[ $TMPBOOST_DIR ]] ; then
            setEnvVariable "Boost library" "BOOST_DIR"
        else
            writeStatus "  - Failed to find boost library" 2
            hasFailed=true
            return
        fi
    fi

    # Test the actual content of the boost directory
    pathStatusCode=0
    [[ ! -e $BOOST_DIR/include/boost/version.hpp ]] && pathStatusCode=2
    writeStatus "  - Checking boost library ($BOOST_DIR)" $pathStatusCode
    [[ $pathStatusCode -eq 2 ]] && hasFailed=true
}

function findSDLlib {
    message "${GREEN}[sdl2 library]${NORMAL}"
    TMPSDL2_DIR=
    if ! validateEnvironmentVariable SDL2_DIR ; then
        if [[ ! $TMPSDL2_DIR ]] ; then
            if [[ -x `which sdl2-config` ]] ; then
                TMPSDL2_DIR=`sdl2-config --prefix`
                writeStatus "  - Found sdl2 path using sdl2-config utility" 0
            else
                writeStatus "  - Did not find sdl2-config utility in PATH" 1
            fi
        fi
        if [[ ! $TMPSDL2_DIR ]] ; then
            if pkg-config sdl2 ; then
                tmppath=`pkg-config sdl2 --variable=prefix`
                if [[ -d $tmppath ]] ; then
                    TMPSDL2_DIR=`pkg-config sdl2 --variable=prefix`
                    writeStatus "  - Found sdl2 path using pkg-config" 0
                fi
            fi
        fi
        if [[ ! $TMPSDL2_DIR ]] ; then
            if locateAny "sdl2" "bin/sdl2-config" "include/SDL2/SDL.h" "--prefix" ; then
                TMPSDL2_DIR=$retvalue
            fi
        fi

        if [[ $TMPSDL2_DIR ]] ; then
            setEnvVariable "SDL2 library" "SDL2_DIR"
        else
            writeStatus "  - Failed to find sdl2 library" 2
            hasFailed=true
            return
        fi
    fi

    # Test the actual content of the sdl2 directory
    pathStatusCode=0
    [[ ! -e $SDL2_DIR/include/SDL2/SDL.h ]] && pathStatusCode=2
    [[ ! -e $SDL2_DIR/lib/libSDL2-2.0.so.0 ]] && pathStatusCode=2
    writeStatus "  - Checking sdl2 library ($SDL2_DIR)" $pathStatusCode
    if [[ $pathStatusCode -eq 0 ]] ; then
        validateSharedLibrary $SDL2_DIR/lib SDL2_image
        validateSharedLibrary $SDL2_DIR/lib SDL2_mixer
        validateSharedLibrary $SDL2_DIR/lib SDL2_ttf
    else
        hasFailed=true
    fi
}

function findGLM {
    message "${GREEN}[GLM header files]${NORMAL}"
    TMPGLM_DIR=
    if ! validateEnvironmentVariable GLM_DIR ; then
        if [[ ! $TMPGLM_DIR ]] ; then
            if locateAny "GLM" "/glm/glm.hpp" "/glm/glm.hpp" ; then
                TMPGLM_DIR=$retvalue
            fi
        fi

        if [[ $TMPGLM_DIR ]] ; then
            setEnvVariable "GLM headers" "GLM_DIR"
        else
            writeStatus "  - Failed to find GLM headers" 2
            hasFailed=true
            return
        fi
    fi

    # Test the actual content of the sdl2 directory
    pathStatusCode=0
    [[ ! -e $GLM_DIR/glm/glm.hpp ]] && pathStatusCode=2
    writeStatus "  - Checking GLM path ($GLM_DIR)" $pathStatusCode
    [[ $pathStatusCode -eq 2 ]] && hasFailed=true
}

function findBulletLib {
    message "${GREEN}[Bullet library]${NORMAL}"
    TMPBULLET_DIR=
    if ! validateEnvironmentVariable BULLET_DIR ; then
        if [[ ! $TMPBULLET_DIR ]] ; then
            if pkg-config bullet ; then
                tmppath=`pkg-config bullet --variable=prefix`
                if [[ -d $tmppath ]] ; then
                    TMPBULLET_DIR=$tmppath
                    writeStatus "  - Found bullet path using pkg-config" 0
                fi
            fi
        fi
        if [[ ! $TMPBULLET_DIR ]] ; then
            file="/include/bullet/btBulletDynamicsCommon.h"
            if locateAny "bullet library" $file $file ; then
                TMPBULLET_DIR=$retvalue
            fi
        fi

        if [[ $TMPBULLET_DIR ]] ; then
            setEnvVariable "Bullet library" "BULLET_DIR"
        else
            writeStatus "  - Failed to find bullet library" 2
            hasFailed=true
            return
        fi
    fi

    # Test the actual content of the bullet directory
    pathStatusCode=0
    [[ ! -e $BULLET_DIR/include/bullet/btBulletDynamicsCommon.h ]] && pathStatusCode=2
    writeStatus "  - Checking bullet library ($BULLET_DIR)" $pathStatusCode
    if [[ $pathStatusCode -eq 0 ]] ; then
        validateSharedLibrary $BULLET_DIR/lib BulletDynamics
        validateSharedLibrary $BULLET_DIR/lib BulletCollision
        validateSharedLibrary $BULLET_DIR/lib LinearMath
    fi
    [[ $pathStatusCode -eq 2 ]] && hasFailed=true
}


function findNDK {
    message "${GREEN}[Android Native Development Kit]${NORMAL}"

    ndkBuildPath=`which ndk-build`

    TMPNDK_HOME=
    if ! validateEnvironmentVariable NDK_HOME ; then
        if [[ ! $TMPNDK_HOME ]] ; then
            if [[ -x $ndkBuildPath ]] ; then
                tmppath=`which ndk-build | sed 's/\/ndk-build//g'`
                if [[ -d $tmppath ]] ; then
                    TMPNDK_HOME=$tmppath
                    writeStatus "  - Found possible Android NDK path" 0
                fi
            fi
        fi

        if [[ $TMPNDK_HOME ]] ; then
            setEnvVariable "Android NDK" "NDK_HOME"
        else
            writeStatus "  - Failed to find Android NDK" 2
            hasFailed=true
            return
        fi
    fi

    # ndk-build
    if [[ -x $ndkBuildPath ]] ; then
        writeStatus "  - ndk-build utility ($ndkBuildPath)" 0
    else
        writeStatus "  - ndk-build utility not found in PATH" 2
        hasFailed=true
    fi

    targetPlatformPath=$NDK_HOME/platforms/${targetAndroidPlatform}
    statusCode=0
    androidLibPath=$targetPlatformPath/arch-arm/usr/lib/libandroid.so
    [[ ! -e $androidLibPath ]] && statusCode=2 && hasFailed=true
    writeStatus "  - NDK target platform (${targetAndroidPlatform})" $statusCode
}


function findSDK {
    message "${GREEN}[Android Software Development Kit]${NORMAL}"

    TMPSDK_HOME=
    if ! validateEnvironmentVariable SDK_HOME ; then
        if [[ ! $TMPSDK_HOME ]] ; then
            androidPath=`which android`
            if [[ -x $androidPath ]] ; then
                tmppath=`which android | sed 's/\/tools.*//g'`
                if [[ -d $tmppath ]] ; then
                    TMPSDK_HOME=$tmppath
                    writeStatus "  - Found possible Android SDK path" 0
                fi
            fi
        fi
        if [[ ! $TMPSDK_HOME ]] ; then
            if locateAny "Android SDK" "/platform-tools/adb" \
                "/platforms/$targetAndroidPlatform" ; then
                TMPSDK_HOME=$retvalue
            fi
        fi

        if [[ $TMPSDK_HOME ]] ; then
            setEnvVariable "Android SDK" "SDK_HOME"
        else
            writeStatus "  - Failed to find Android SDK" 2
            hasFailed=true
            return
        fi
    fi

    statusCode=0
    [[ ! -d $SDK_HOME/platforms/$targetAndroidPlatform/ ]] && statusCode=2 && hasFailed=true
    writeStatus "  - SDK target platform (${targetAndroidPlatform})" $statusCode
}


function findBulletSource {
    message "${GREEN}[Bullet source]${NORMAL}"
    TMPBULLET_SRC_DIR=
    if ! validateEnvironmentVariable BULLET_SRC_DIR ; then
        if [[ ! $TMPBULLET_SRC_DIR ]] ; then
            filepath="/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp"
            if locateAny "bullet library" $filepath $filepath ; then
                TMPBULLET_SRC_DIR=$retvalue
            fi
        fi

        if [[ $TMPBULLET_SRC_DIR ]] ; then
            setEnvVariable "Bullet source" "BULLET_SRC_DIR"
        else
            writeStatus "  - Failed to find bullet source" 2
            hasFailed=true
            return
        fi
    fi

    # Test the actual content of the bullet directory
    pathStatusCode=0
    [[ ! -e $BULLET_SRC_DIR/src/btBulletDynamicsCommon.h ]] && pathStatusCode=2
    [[ ! -e $BULLET_SRC_DIR/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp ]] && pathStatusCode=2
    writeStatus "  - Checking bullet source ($BULLET_SRC_DIR)" $pathStatusCode
    [[ $pathStatusCode -eq 2 ]] && hasFailed=true
}


function findSDL2Source {
    message "${GREEN}[sdl2 source]${NORMAL}"
    TMPSDL2_SRC_DIR=
    if ! validateEnvironmentVariable SDL2_SRC_DIR ; then
        if [[ ! $TMPSDL2_SRC_DIR ]] ; then
            if [[ -x `which sdl2-config` ]] ; then
                # check to see it is located common subdirectory?
                tmppath=`sdl2-config --prefix | sed 's/sdl2-x86/SDL/g'`
                if [[ -e $tmppath/src/SDL.c ]] ; then
                    writeStatus "  - Found possible sdl2 source path" 0
                    TMPSDL2_SRC_DIR=$tmppath
                fi
            fi
        fi

        if [[ ! $TMPSDL2_SRC_DIR ]] ; then
            filepath="/src/SDL.c"
            if locateAny "sdl2 source" $filepath $filepath ; then
                TMPSDL2_SRC_DIR=$retvalue
            fi
        fi

        if [[ $TMPSDL2_SRC_DIR ]] ; then
            setEnvVariable "SDL2 source" "SDL2_SRC_DIR"
        else
            writeStatus "  - Failed to find sdl2 source" 2
            hasFailed=true
            return
        fi
    fi

    # Test the actual content of the sdl2 directory
    pathStatusCode=0
    [[ ! -e $SDL2_SRC_DIR/Android.mk ]] && pathStatusCode=2
    [[ ! -e $SDL2_SRC_DIR/src/SDL.c ]] && pathStatusCode=2
    writeStatus "  - Checking sdl2 source ($SDL2_SRC_DIR)" $pathStatusCode
    [[ ! $pathStatusCode -eq 0 ]] && hasFailed=true
}

# $1={image,mixer,ttf} $2=requiredFile1 (e.g. IMG.c) $3=requiredFile2 (e.g. SDL_image.h)
function findSDL2UtilSource {
    message "${GREEN}[SDL2_$1 source]${NORMAL}"

    util=$1
    UTIL=${1^^}
    requiredFile1=$2
    requiredFile2=$3
    TMP_SRC_DIR=
    if ! validateEnvironmentVariable SDL2_${UTIL}_SRC_DIR ; then
        if [[ ! $TMP_SRC_DIR ]] ; then
            if [[ -x `which sdl2-config` ]] ; then
                # check to see it is located common subdirectory?
                tmppath=`sdl2-config --prefix | sed 's/sdl2-x86/SDL_'${util}'/g'`
                if [[ -e $tmppath/${requiredFile1} ]] ; then
                    writeStatus "  - Found possible SDL2_${util} source path" 0
                    TMP_SRC_DIR=$tmppath
                fi
            fi
        fi

        if [[ ! $TMP_SRC_DIR ]] ; then
            if locateAny "SDL2_${util} source" ${requiredFile1} ${requiredFile2} ; then
                TMP_SRC_DIR=$retvalue
            fi
        fi

        if [[ $TMP_SRC_DIR ]] ; then
            setEnvVariable "SDL2_${util} source" "SDL2_${UTIL}_SRC_DIR" $TMP_SRC_DIR
        else
            writeStatus "  - Failed to find SDL2_${util} source" 2
            hasFailed=true
            return
        fi
    fi

    # Test the actual content of the sdl2 directory
    pathStatusCode=0
    envVar="SDL2_${UTIL}_SRC_DIR"
    [[ ! -e ${!envVar}/${requiredFile1} ]] && pathStatusCode=2
    [[ ! -e ${!envVar}/${requiredFile2} ]] && pathStatusCode=2
    writeStatus "  - Checking SDL2_${util} source (${!envVar})" $pathStatusCode
    [[ ! $pathStatusCode -eq 0 ]] && hasFailed=true
}

function findOrGetWatchfile {
    message "${GREEN}[watchfile]${NORMAL}"
    if command -v watchfile > /dev/null ; then
        writeStatus "  - watchfile script found in PATH" 0
    else
        if [ -x utils/scripts/watchfile ] ; then
            writeStatus "  - watchfile script found in utils/scripts/" 0
        else
            writeStatus "  - watchfile script not found in PATH, downloading" 1
            echo $GRAY
            git clone https://gist.github.com/8963507.git utils/scripts/tmp && \
                mv utils/scripts/tmp/watchfile.sh utils/scripts/watchfile && \
                chmod +x utils/scripts/watchfile && \
                rm -rf utils/scripts/tmp
            echo $NORMAL
            if [ -x utils/scripts/watchfile ] ; then
                writeStatus "  - watchfile script now found in utils/scripts/" 0
            else
                writeStatus "  - watchfile script missing" 2
            fi
        fi
    fi
}


function findOrGetGlslangValidator {
    message "${GREEN}[glslangValidator]${NORMAL}"
    if command -v glslangValidator > /dev/null ; then
        writeStatus "  - glslangValidator script found in PATH" 0
    else
        if [ -x utils/scripts/glslangValidator ] ; then
            writeStatus "  - glslangValidator script found in utils/scripts/" 0
        else
            writeStatus "  - glslangValidator script not found in PATH, downloading" 1
            echo $GRAY
            wget https://cvs.khronos.org/svn/repos/ogl/trunk/ecosystem/public/sdk/tools/glslang/Install/Linux/glslangValidator -O utils/scripts/glslangValidator && \
                chmod +x utils/scripts/glslangValidator
            echo $NORMAL
            if [ -x utils/scripts/glslangValidator ] ; then
                writeStatus "  - glslangValidator script now found in utils/scripts/" 0
            else
                writeStatus "  - glslangValidator script missing" 2
            fi
        fi
    fi
}

# Create utils/scripts forlder if it doesn't already exist
if [ ! -d utils/scripts ] ; then mkdir utils/scripts ; fi

message "${TEAL}-----------------------------------${NORMAL}"
message "${TEAL}Checking necessary utilities${NORMAL}"
message "${TEAL}-----------------------------------${NORMAL}"
findOrGetWatchfile
findOrGetGlslangValidator

message "\n"
message "${TEAL}-----------------------------------${NORMAL}"
message "${TEAL}Checking desktop build dependencies${NORMAL}"
message "${TEAL}-----------------------------------${NORMAL}"
findBoost
findSDLlib
findGLM
findBulletLib

message "\n"
message "${TEAL}-----------------------------------${NORMAL}"
message "${TEAL}Checking android build dependencies${NORMAL}"
message "${TEAL}-----------------------------------${NORMAL}"
findNDK
findSDK
findBulletSource
findSDL2Source
findSDL2UtilSource image IMG.c Android.mk
findSDL2UtilSource mixer mixer.c Android.mk
findSDL2UtilSource ttf showfont.c Android.mk

message "\n"

if [[ $accumVarDefines ]] ; then
    message "${YELLOW}TIP:${NORMAL}
To speed up this script, (or better, make it unneccesary), you can add the
following lines to ${BLUE}~/.bashrc${NORMAL} (or similar), and then restart the terminal:"
    message "${accumVarDefines}\n\n"
fi

if [[ $hasFailed ]] ; then
    echo "${RED}Error occurred${NORMAL}"
    echo "Please amend the reported failure message(s) and try again"
else
    message "All environment variables and required libraries: ${GREEN}OK${NORMAL}"
    message "Configuring android project"

    # Creating symbolic link to SDL source path
    PATH_JNI=./android/jni
    FLITE_DIR="../../external/flite"
    NOISE_DIR="../../external/noise"
    POLYVOX_DIR="../../external/polyvox"
    [[ ! -e $PATH_JNI/SDL ]]        && ln -s $SDL2_SRC_DIR       $PATH_JNI/SDL
    [[ ! -e $PATH_JNI/SDL_image ]]  && ln -s $SDL2_IMAGE_SRC_DIR $PATH_JNI/SDL_image
    [[ ! -e $PATH_JNI/SDL_mixer ]]  && ln -s $SDL2_MIXER_SRC_DIR $PATH_JNI/SDL_mixer
    [[ ! -e $PATH_JNI/SDL_ttf ]]    && ln -s $SDL2_TTF_SRC_DIR   $PATH_JNI/SDL_ttf
    [[ ! -e $PATH_JNI/bullet-src ]] && ln -s $BULLET_SRC_DIR     $PATH_JNI/bullet-src
    [[ ! -e $PATH_JNI/flite ]]      && ln -s $FLITE_DIR          $PATH_JNI/flite
    [[ ! -e $PATH_JNI/noise ]]      && ln -s $NOISE_DIR          $PATH_JNI/noise
    [[ ! -e $PATH_JNI/polyvox ]]    && ln -s $POLYVOX_DIR        $PATH_JNI/polyvox

    # Create the local.properties file, with the path to the sdk
    m="# This file is automatically generated by initialize_project.sh
# Do not modify this file -- YOUR CHANGES WILL BE ERASED!
#
sdk.dir=$SDK_HOME
target=android-18
"
    echo "$m" > ./android/project.properties

    message "Done"
fi
