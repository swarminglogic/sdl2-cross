#!/bin/bash

function showHelp() {
    echo "build - help utility for initializing various platform builds
Possible targets:
    a, android
    android-tests
    l, linux
    w, windows
    windows-tests
    utils
    all          (build each of the above, if possible)

Options
 General:
    -c, --clean   Cleans build files for all specified targets.
    -v, --verbose Verbose compilation output
    -s, --serial  Only use one core when compiling.
 For android:
    -p, --push    Pushes the compiled apk to a connected device.

 # TODO swarminglogic, 2014-03-25:
 # For linux:
 #   -t, --tests   Compile & run the unit test system
"
    exit
}

function echoerr() {
    echo -e "$@" 1>&2;
}


function writeGitRevHeader() {
echo "#ifndef UTIL_GITREV_H
#define UTIL_GITREV_H

#define CURRENT_GIT_REV \"$1\"

#endif  // UTIL_GITREV_H" > src/util/gitrev.h
}

function prepareGitRevHeader() {
    gitrev=`git rev-parse --short HEAD`
    if [ ! -e src/util/gitrev.h ] ; then
        echo "src/util/gitrev.h not found. Attempting to create."
        writeGitRevHeader $gitrev
        if [ $? -eq 0 ]; then
            echo "src/util/gitrev.h was created successfully."
        else
            echo "Failed to create src/util/gitrev.h. Aborting!"
            exit
        fi
    else
        writeGitRevHeader $gitrev
    fi
}


while test $# -gt 0; do
    case "$1" in
        -h|--help)
            showHelp 0
            exit
            ;;
        -c|--clean)
            shift
            cleanTarget=yes
            ;;
        -v|--verbose)
            shift
            isVerbose=yes
            ;;
        -s|--serial)
            shift
            isSerial=yes
            ;;
        -p|--push)
            shift
            pushToAndroid=yes
            ;;
        -t|--tests)
            shift
            buildTests=yes
            ;;
        a|android)
            shift
            android=yes
            ;;
        android-tests)
            shift
            android=yes
            androidTests=yes
            ;;
        l|linux)
            shift
            linux=yes
            ;;
        w|windows)
            shift
            windows=yes
            ;;
        windows-tests)
            shift
            windows=yes
            windowsTests=yes
            ;;
        utils)
            shift
            utils=yes
            ;;
        all)
            shift
            android=yes
            linux=yes
            utils=yes
            ;;
        *)
            echoerr "Error: Invalid parameter: $1\n"
            showHelp
            exit
#            shift
#            linux=yes
            ;;
    esac
done

# $1: Runner filename
function create_unit_test_main_file {
    local main=$1
    local testlist=$(cd src/ && find . -name 'Test*.h' | \
        grep -Ev '_flymake.*' | grep -v 'TestEmpty')
    (cd src/ && ../utils/cxxtest/bin/cxxtestgen \
        --error-print -o $main $testlist)
    if [ ! -e src/$main ] ; then
        echo "Failed to generate CxxTest runner"
        exit 0;
    fi

    # Patch the generated runner file
    local main_entry="int main\( int argc\, char \*argv\[\] \) \{"
    local add_headers="#include <audio/SDL_mixer.h>
#include <core/MainManager.h>
#include <graphics/SDL_image.h>
#include <util/SDL.h>
"
    local return_entry="return status\;"
    local return_replace="IMG_Quit\(\)\;
    const int nOpenAudio = Mix_QuerySpec\(nullptr\, nullptr\, nullptr\)\;
    for \(int i = 0 \; i < nOpenAudio \; ++i\)
      Mix_CloseAudio\(\)\;
    while \(Mix_Init\(0\)\)
      Mix_Quit\(\)\;
    SDL_Quit\(\)\;
    return status\;"

    perl -i -pe "s,${main_entry},${add_headers}${main_entry},;" \
        -pe "s,${return_entry},${return_replace},g" src/$main
}


serial='-j6'
if [[ $isSerial ]] ; then
    serial="-j1"
fi

if [[ $android ]] ; then
    if [[ $isVerbose ]] ; then
        verbose="V=1"
    fi

    if [[ $androidTests ]] ; then
        # Configure to build
        export SWL_MAIN_FILE=main_unit_tests.cpp
        create_unit_test_main_file $SWL_MAIN_FILE
    else
        export SWL_MAIN_FILE=main.cpp
    fi

    if [[ $cleanTarget ]] ; then
        echo "Cleaning Android build ..."
        (cd android && ndk-build clean $verbose);
    else
        prepareGitRevHeader
        if [[ ! -e android/project.properties ]] ; then
            echo "Running initialize_project.sh ..."
            (./initialize_project.sh --quiet)
        fi

        (cd android && nice ndk-build $serial $verbose)
        if [[ $pushToAndroid ]] ; then
            (cd android && ant debug install)
        fi
    fi
fi

if [[ $linux ]] ; then
    if [[ $cleanTarget ]] ; then
        echo "Cleaning Linux build ..."
        scons -c;
    else
        prepareGitRevHeader
        if [[ $buildTests ]] ; then
            nice scons $serial --tests
        else
            nice scons $serial
        fi
    fi
fi


if [[ $windows ]] ; then
    if [[ $cleanTarget ]] ; then
        echo "Cleaning Windows build ..."
        scons --win64 -c;
    else
       prepareGitRevHeader
        if [[ $buildTests ]] ; then
            export SWL_TARGET_SUFFIX=-unittests
            export SWL_MAIN_FILE=main_unit_tests.cpp
            create_unit_test_main_file $SWL_MAIN_FILE
            nice scons $serial --win64
        else
            nice scons $serial --win64
            export SWL_MAIN_FILE=main.cpp
        fi
    fi
fi


function buildUtil {
    target=$1
    echo -e "\nBuilding utils/$target"
    echo "--------------------"
    sconsfile="utils/${target}/SConstruct"
    if [[ $cleanTarget ]] ; then
        echo "Cleaning utils/${target} build ..."
        scons -f $sconsfile -c;
    else
        nice scons -f $sconsfile $serial
        # preliminary cleanup
        if [[ -x ./bin/${target} ]] ; then
            mv ./bin/${target}{,-tmp}
        fi
        scons -f $sconsfile -c
        if [[ -x ./bin/${target}-tmp ]] ; then
            mv ./bin/${target}{-tmp,}
        fi
    fi
}

if [[ $utils ]]; then
    buildUtil simpletextpreprocess
    buildUtil obj2cobj
    buildUtil obj2info
fi
