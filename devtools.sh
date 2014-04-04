#!/bin/bash

function showHelp() {
    echo "devtools - Shorthand commandline aliases
Possible targets:
    h,help       Shows this message.
    b            General build
    ba           Builds android
    bap          Builds android, pushes .apk on completion
    bl           Builds linux
    bal          Builds all
    p            Pushes Android APK to device
    wa           Auto-rebuild android on changes.
    wl           Auto-rebuild linux on changes.
    lg,log       adb logcat with SDL and SDL/APP filter
    lga,loga     adb logcat only SDL/APP filter
    lgall,logall adb logcat with no filter
"
#    wal          Auto-rebuilds either on change.
    exit
}



function echoerr() {
    echo -e "$@" 1>&2;
}

if test $# -eq 0 ; then
    showHelp
    exit
fi

while test $# -gt 0; do
    case "$1" in
        h|help)
            shift
            showHelp
            exit
            ;;
        b)
            shift
            ./build.sh $@
            exit
            ;;
        ba)
            shift
            ./build.sh a $@
            exit
            ;;
        bap)
            shift
            ./build.sh a -p $@
            exit
            ;;
        bl)
            shift
            ./build.sh l $@
            exit
            ;;
        bal)
            shift
            ./build.sh all $@
            exit
            ;;
        p)
            shift
            (cd android && ant debug install)
            exit
            ;;
        wa)
            shift
            watchfile -s "find . \
            | grep -P '\./src/[^\.](.*\.mk|.*\.cpp$|.*\.h$)' \
            | xargs cat | md5sum" -e ./build.sh a $@
             exit
             ;;
        wl)
            shift
             watchfile -s "find . \
             | grep -P '(\./src/[^\.](.*\.cpp$|.*\.h$)|\./(src/|)SCons.*)' \
             | xargs cat | md5sum" -e ./build.sh l $@
             exit
             ;;
        # wal)
        #     shift
        #      watchfile --no-clear -s "find . \
        #      | grep -P '(\./src/[^\.](.*\.cpp$|.*\.h$)|\./(src/|)SCons.*)' \
        #      | xargs cat | md5sum" -e ./build.sh l&
        #      watchfile --no-clear -s "find . \
        #      | grep -P '\./src/[^\.](.*\.mk|.*\.cpp$|.*\.h$)' \
        #      | xargs cat | md5sum" -e ./build.sh a&
        #      wait $(jobs -p)
        #      exit
        #      ;;
        lg|log)
            shift
            adb logcat -c && adb logcat -s "SDL","SDL/APP"
            exit
            ;;
        lga|loga)
            shift
            adb logcat -c && adb logcat -s "SDL/APP"
            exit
            ;;
        lgall|logall)
            shift
            adb logcat -c && adb logcat
            exit
            ;;
        *)
            echoerr "Error: Invalid parameter: $1\n"
            showHelp
            exit
            ;;
    esac
done
