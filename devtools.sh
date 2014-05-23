#!/bin/bash

function showHelp() {
    echo "devtools - Shorthand commandline aliases
Possible targets:
    h,help       Shows this message.
    b            General build
    ba           Builds android
    bap          Builds android, pushes .apk on completion
    bl           Builds linux
    bal,bla,ball Builds android and linux
    p            Pushes android APK to device
    wa           Auto-rebuild android on changes.
    wl           Auto-rebuild linux on changes.
    testl        compile and run unit tests on linux
    lg,log       adb logcat with SWL filter
    lga,loga     adb logcat with SWL, SDL, SDL/* filter
    lgall,logall adb logcat with no filter
    clean        removal of all build directories (use with caution)
    cleanl       removal of all linux build directories (use with caution)
    cleana       removal of all android build directories (use with caution)
"
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
            ./compile.sh $@
            exit
            ;;
        ba)
            shift
            ./compile.sh a $@
            exit
            ;;
        bap)
            shift
            ./compile.sh a -p $@
            exit
            ;;
        bl)
            shift
            ./compile.sh l $@
            exit
            ;;
        bal|bla|ball)
            shift
            ./compile.sh all $@
            exit
            ;;
        p)
            shift
            (cd android && ant debug install)
            exit
            ;;
        wa)
            shift
            watchfile -s "find ./src ./android/jni/ \
                -name '[!\.]*.cpp' -or \
                -name '[!\.]*.h'   -or \
                -name '[!\.]*.tpp' -or \
                -name '[!\.]*.mk'
               | xargs cat | md5sum" -e ./compile.sh a $@
             exit
             ;;
        wl)
            shift
            watchfile -s "find ./src ./SConstruct \
                -name '[!\.]*.cpp' -or \
                -name '[!\.]*.h'   -or \
                -name '[!\.]*.tpp' -or \
                -name 'SConscript*' -or \
                -name 'SConstruct' \
               | xargs cat | md5sum" -e ./compile.sh l $@
             exit
             ;;
        testl)
            shift
            ./compile.sh l -t
            LD_LIBRARY_PATH=./lib/:$LD_LIBRARY_PATH
            if [[ $? ]] ; then
                for t in ./bin/tests/* ; do
                    ./$t
                done
            fi
            ;;
        lg|log)
            shift
            adb logcat -c && adb logcat -s "SWL"
            exit
            ;;
        lga|loga)
            shift
            adb logcat -c && adb logcat -s "SWL","SDL","SDL/APP",\
            "SDL/ERROR","SDL/SYSTEM","SDL/AUDIO","SDL/VIDEO",\
            "SDL/RENDER","SDL/INPUT"
            exit
            ;;
        lgall|logall)
            shift
            adb logcat -c && adb logcat
            exit
            ;;
        clean)
            shift
            ./devtools.sh cleanl
            ./devtools.sh cleana
            ;;
        cleanl)
            shift
            while true; do
                read -p "Remove linux build directories: \
./{bin,build,lib}/ ? [y/n] " yn
                case $yn in
                    [Yy]* )
                        rm -rf ./{bin,build,lib}/ ;
                        break
                        ;;
                    [Nn]* )
                        exit;;
                    * ) echo "Please answer yes or no.";;
                esac
            done
            exit
            ;;
        cleana)
            shift
            while true; do
                read -p "Remove android build directories: \
./android/{bin,gen,libs,obj}/ ? [y/n] " yn
                case $yn in
                    [Yy]* )
                        rm -rf android/{bin,gen,libs,obj}/ ;
                        break
                        ;;
                    [Nn]* )
                        exit;;
                    * ) echo "Please answer yes or no.";;
                esac
            done
            exit
            ;;
        *)
            echoerr "Error: Invalid parameter: $1\n"
            showHelp
            exit
            ;;
    esac
done
