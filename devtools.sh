#!/bin/bash

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
TEAL=$(tput setaf 6)
NORMAL=$(tput sgr0)
BOLD=$(tput bold)


function showHelp() {
    echo "devtools - Shorthand commandline aliases
Possible targets:
    h,help                 Shows this message.
    b                      General build
    ba                     Builds android
    bap                    Builds android, pushes .apk on completion
    bl                     Builds linux
    bu                     Builds utils
    bal,bla,ball           Builds android and linux
    p                      Pushes android APK to device
    ra                     Runs android app on device
    ua                     Updates android assets only
    ia                     Takes screenshot of android
    sa                     Stops android app if running on device
    wa                     Auto-rebuild android on changes.
    wl                     Auto-rebuild linux on changes.
    ws                     Auto-validate shaders on changes.
    wt                     Watch test directory, execute changed tests
    testl                  compile and run unit tests on linux
    rtestl                 run all unit tests on linux
    lg,log                 adb logcat with SWL filter
    lga,loga               adb logcat with SWL, SDL, SDL/* filter
    lgall,logall           adb logcat with no filter
    lgt,logtests           adb logcat with CxxTest filter
    clean                  removal of all build directories (use with caution)
    cleanl                 removal of all linux build directories (use with caution)
    cleana                 removal of all android build directories (use with caution)
    validate-gl-shaders    validates shaders for used OpenGL 4.3
    validate-es-shaders    validates shaders for used OpenGL 3.0 ES
    validate-shaders       validates shaders for used OpenGL 4.3 and OpenGL 3.0 ES
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

scriptsPath=`pwd`/utils/scripts
if [ -d $scriptsPath ] ; then
    PATH=$PATH:$scriptsPath
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
        bu)
            shift
            ./compile.sh utils $@
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
        ia) shift
            screencapfilename="sdl2-android-screencapture-"$(date +'%Y_%m_%d__%H_%M_%S.png')
            adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > $screencapfilename
            convert $screencapfilename -rotate -90  $screencapfilename
            echo "Screenshot saved to: "$screencapfilename
            exit
            ;;
        ua) shift
            apk=android/bin/SDLActivity-debug.apk
            if [ ! -e $apk ] ; then
                echo "APK not found. Building first once with ./devtools p"
                ./devtools.sh p
            fi
            assetsToUpdate=$(find assets/ -newer $apk -type f)
            if [[ $assetsToUpdate ]] ; then
                echo zip $apk $assetsToUpdate
                zip $apk $assetsToUpdate
                echo zip -d $apk META-INF/*
                zip -d $apk META-INF/*
                jarsigner -verbose -storepass android -sigalg \
                    SHA1withRSA -digestalg SHA1 -keystore \
                    ~/.android/debug.keystore $apk \
                    androiddebugkey
                adb install -r $apk
            else
                echo "All assets up to date. Nothing to do."
                adb shell input keyevent 80
            fi
            exit
            ;;
        ra)
            shift
            (cd android && adb shell am start -n com.swarminglogic.swldev/.SWLdev)
            adb shell input keyevent 80
            exit
            ;;
        sa) shift
            (cd android && adb shell am force-stop com.swarminglogic.swldev)
            exit
            ;;
        wa)
            shift
            watchfile -s "find ./src ./android/jni/ \
                -name '[!\.]*.cpp' -or \
                -name '[!\.]*.h'   -or \
                -name '[!\.]*.tpp' -or \
                -name '[!\.]*.mk'
               | grep -Ev '_flymake.*' | xargs cat | md5sum" -e ./compile.sh a $@
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
               | grep -Ev '_flymake.*' \
               | grep -Ev 'main_.*' \
               | xargs cat | md5sum" -e ./compile.sh l $@
             exit
             ;;
        ws)
            shift
            watchfile -s "find ./assets/shaders \
                -name '[!\.]*.frag' -or \
                -name '[!\.]*.vert' \
               | grep -Ev '_flymake.*' | xargs cat | md5sum" -e ./devtools.sh validate-shaders
             exit
             ;;
        wt)
            shift
            # Cached md5sum of files in bin/tests/*
            tmpfile=/tmp/.sdl2-cross.tests.md5sum
            # Create this file if it doesn't already exist
            if [ ! -e $tmpfile ] ; then touch $tmpfile ; fi
            while true; do
                # Wait until some file in ./bin/tests/ has changed
                watchfile --oneshot -s "find ./bin/tests/ \
                      -executable -type f -exec stat {} \;" -e true
                # One second grace period for finalizing files
                sleep 1
                # Get tests and matching md5sum
                currentMd5=$(find ./bin/tests/ -executable -type f -exec md5sum {} \; | \
                    awk '{print $2,$1}' | sort)
                # Compare w/cached md5sums and get current checksums
                newTests=$(<<< "$currentMd5" diff $tmpfile - | grep -P "^>" | awk '{print $2}')
                # File for accumulating failed tests output
                failAccumFile=/tmp/.sdl2-cross.test.failure
                if [ -e $failAccumFile ] ; then echo "" > $failAccumFile; fi
                for i in $newTests ; do
                    testLog=/tmp/.sdl2-cross.test.$(basename $i)
                    ./$i > $testLog
                    cat $testLog | grep --color=never "OK"
                    cat $testLog | grep -v --color=never "OK" >> $failAccumFile
                done
                echo "$currentMd5" > $tmpfile
                # Outputs failed tests with colouring:
                # 1. Removes reduntant "In TestFoo::testFoo:"
                # 2. Colors line TestFoo:testBarFunction
                # 3. Colors path to failed line, break into new line before error
                # 4. Color instances of "Error: "
                cat $failAccumFile | \
                    grep -vP "In Test.*::test.*:$" | \
                    sed "s/^\(Test.*\)::\(.*\)/${RED}${BOLD}\1${NORMAL}::${YELLOW}\2${NORMAL}/g" | \
                    sed "s/\(^\/.*\.h:[[:digit:]]\+:\)\(.*\)/${TEAL}\1${NORMAL}\n  \2\n/g" | \
                    sed "s/\(Error:\)/${RED}\1${NORMAL}/g"
            done
            ;;
        rtestl)
            shift
            LD_LIBRARY_PATH=./lib/linux/debug/:$LD_LIBRARY_PATH
            failAccumFile=/tmp/.sdl2-cross.alltest.failure
            echo "" > $failAccumFile;
            tests=$(find ./bin/tests/ -executable -type f)
            for i in $tests ; do
                testLog=/tmp/.sdl2-cross.alltest.$(basename $i)
                ./$i > $testLog
                cat $testLog | grep --color=never "OK"
                cat $testLog | grep -v --color=never "OK" >> $failAccumFile
            done
            cat $failAccumFile | \
                grep -vP "In Test.*::test.*:$" | \
                sed "s/^\(Test.*\)::\(.*\)/${RED}${BOLD}\1${NORMAL}::${YELLOW}\2${NORMAL}/g" | \
                sed "s/\(^\/.*\.h:[[:digit:]]\+:\)\(.*\)/${TEAL}\1${NORMAL}\n  \2\n/g" | \
                sed "s/\(Error:\)/${RED}\1${NORMAL}/g"
            ;;
        testl)
            shift
            ./compile.sh l -t
            LD_LIBRARY_PATH=./lib/linux/debug/:$LD_LIBRARY_PATH
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
        lgt|logtests)
            shift
            adb logcat -c && adb logcat -s "CxxTest"
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
        validate-gl-shaders)
            shift
            ./validateshaders.sh GL 430 core
            exit
            ;;
        validate-es-shaders)
            shift
            ./validateshaders.sh ES 300 es
            exit
            ;;
        validate-shaders)
            shift
            ./devtools.sh validate-gl-shaders && \
            ./devtools.sh validate-es-shaders
            exit
            ;;
        *)
            echoerr "Error: Invalid parameter: $1\n"
            showHelp
            exit
            ;;
    esac
done
