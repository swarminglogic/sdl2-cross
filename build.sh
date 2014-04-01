#!/bin/bash

function showHelp() {
    echo "build - help utility for initializing various platform builds
Possible targets:
    a, android
    l, linux
    all          (build each of the above, if possible)

Options
 General:
    -c, --clean   Cleans build files for all specified targets.
 For android:
    -p, --push    Pushes the compiled apk to a connected device.

 # TODO swarminglogic, 2014-03-25:
 # For linux:
 #   -t, --tests   Runs the unit test system todo
"
    exit
}

function buildAndroid() {
    (cd android && nice ndk-build -j6)
}

function buildLinux() {
    scons
}

function echoerr() {
    echo -e "$@" 1>&2;
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
        -p|--push)
            shift
            pushToAndroid=yes
            ;;
        a|android)
            shift
            android=yes
            ;;
        l|linux)
            shift
            linux=yes
            ;;
        all)
            shift
            android=yes
            linux=yes
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

if [[ $android ]] ; then
    if [[ $cleanTarget ]] ; then
        echo "Cleaning Android build ..."
        (cd android && ndk-build clean);
    else
        if [[ ! -e android/project.properties ]] ; then
            echo "Running android/initialize_project.sh ..."
            (cd android && ./initialize_project.sh)
        fi

        (cd android && nice ndk-build -j6)
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
        nice scons -j6
    fi
fi
