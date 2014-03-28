#!/bin/bash

function showHelp() {
    echo "build - help utility for initializing various platform builds
Possible targets:
    a, android
    l, linux
    all          (build each of the above, if possible)

Options
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

while test $# -gt 0; do
    case "$1" in
        -h|--help)
            showHelp 0
            exit
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
            shift
            linux=yes
            ;;
    esac
done

if [[ $android ]] ; then
    if [[ $pushToAndroid ]] ; then
        # TODO swarminglogic, 2014-03-25: write code that pushes apk to android
        true
    fi
    (cd android && nice ndk-build -j6)
fi

if [[ $linux ]] ; then
    scons
fi
