#!/bin/bash

function showHelp() {
    echo "build - help utility for initializing various platform builds
Possible targets:
    a, android
    l, linux
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
            runTests=yes
            ;;
        a|android)
            shift
            android=yes
            ;;
        l|linux)
            shift
            linux=yes
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

serial='-j6'
if [[ $isSerial ]] ; then
    serial="-j1"
fi

if [[ $android ]] ; then
    if [[ $isVerbose ]] ; then
        verbose="V=1"
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
        if [[ $runTests ]] ; then
            nice scons $serial --tests
        else
            nice scons $serial
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
