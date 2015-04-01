#!/bin/bash

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
TEAL=$(tput setaf 6)
NORMAL=$(tput sgr0)
BOLD=$(tput bold)

scriptsPath=`pwd`/utils/scripts
if [ -d $scriptsPath ] ; then
    PATH=$PATH:$scriptsPath
fi

# imports function prepareGitRevHeader
source utils/scripts/write_gitrev_header.sh

P=$(basename $0)
function showHelp() {
    echo "
${BOLD}NAME${NORMAL}
     $P - Short hand command line helper

${BOLD}SYNPOSIS${NORMAL}
     ${BOLD}$P${NORMAL} init
     ${BOLD}$P${NORMAL} b|build [PLATFORM] [TARGET] [OPTIONS]
     ${BOLD}$P${NORMAL} c|clean [PLATFORM] [TARGET] [OPTIONS|--brute]
     ${BOLD}$P${NORMAL} r|run   [PLATFORM] [RUN_TARGET] [OPTIONS]
     ${BOLD}$P${NORMAL} a|android log [LOG_FILTER]
     ${BOLD}$P${NORMAL} a|android push [OPTIONS]
     ${BOLD}$P${NORMAL} a|android [screenshot|run|stop]
     ${BOLD}$P${NORMAL} v|validate-shaders [SHADER_TYPE] [OPTIONS]

     ${BOLD}$P${NORMAL} [COMMAND]...
     ${BOLD}$P${NORMAL} a|android [ANDROID_COMMAND]...


${BOLD}DESCRIPTION${NORMAL}
     PLATFORM:
        l, linux
        a, android
        w, windows         Not valid for 'run' command

     TARGET:
        m, main            sdl2-cross engine (default when unspecified)
        t, tests           CxxTest unit tests
        u, utils           standalone utilities (see utils/standalones/)

     OPTIONS:
        -r, --release   optimizes build
        -s, --serial    command serialized when possible
        -v, --verbose   verbose output when possible
        -w, --watch     monitors relevant files and repeats command on change
        --assets-only   applies only to 'android push'
        --brute         applies only to clean

     LOG_FILTER:
        swl             adb logcat with SWL filter (default)
        sdl             adb logcat with SWL, SDL, SDL/* filter
        all             adb logcat with no filter
        unit            adb logcat with CxxTest unit testing filter

     SHADER_TYPE:
        gles            OpenGL + OpenGL ES
        gl              OpenGL only
        es              OpenGL ES only

     COMMAND:
        init                   Initializes and configures repository
        h, help                Shows this message
        b, build               Build a TARGET
        c, clean               Clean TARGET build files
                               With --brute, build directories are removed
        r, run                 Run linux test, linux main or android apk
        a, android             Various android specific commands
        v, validate-shaders    OpenGL/ES Shader Validation

     ANDROID_COMMAND:
        p, push                Packages apk, and pushes to connected device
                               With --assets-only, it repackages ./assets folder
        l, log                 Starts an adb logcat session
        r, run                 Same as 'run android'
        s, stop                Stops application if running on connected device
        screenshot             Takes screenshot of connected device

${BOLD}NOTES${NORMAL}
     ${BOLD}$P${NORMAL} run [PLATFORM] [RUN_TARGET] [OPTIONS]
     Really only supports: ${BOLD}$P${NORMAL} run linux main|tests [--release] [--watch]
                           ${BOLD}$P${NORMAL} run android main [--watch]

     If COMMAND is unrecognized, it attempts to split it up with spaces,
     allowing grouped short-hand notation.
         For example:        ${BOLD}$P${NORMAL} blt -r -w
         is equivalent to:   ${BOLD}$P${NORMAL} b l t -r -w
         and more verbosely: ${BOLD}$P${NORMAL} build linux tests --release --watch

     Multi-char single-dash options can also be grouped together.
         For example:        ${BOLD}$P${NORMAL} blt -rvw
         is equivalent to:   ${BOLD}$P${NORMAL} b l t -r -v -w
         and more verbosely: ${BOLD}$P${NORMAL} build linux tests --release --verbose --watch

${BOLD}EXAMPLES${NORMAL}
     Linux debug build
       \$ $P build linux
       \$ $P bl

     Windows release build
       \$ $P build windows main --release
       \$ $P bw -r

     Monitor source files and build release linux tests upon changes
       \$ $P build linux tests --release --watch
       \$ $P blt -r -w

     Monitor tests and run non-debug tests whenever one is modified
       \$ $P run tests --release --watch
       \$ $P rt -r -w

"
    exit
}

if test $# -lt 1 ; then
    showHelp ; exit
fi

function echoerr() {
    echo -e "$@" 1>&2;
}

THIS=$0
COMMAND=$1
shift

# Splits up COMMAND if unrecognized
if [ ${#COMMAND} -gt 1 ] ; then
    case "$COMMAND" in
        help|init|build|clean|run|android|validate-shaders)
            ;;
        *-)
            echoerr "Invalid COMMAND value"
            exit 1
            ;;
        *) # Consider COMMAND to be a grouped command.
            split=""
            while [ ! -z "$COMMAND" ] ; do
                c=${COMMAND::1}
                COMMAND=${COMMAND:1}
                split="$split $c"
            done
            $THIS $split $@
            exit
            ;;
    esac
fi

# Splits up -XYZ arguments to -X -Y -Z
args=
for i in $@ ; do
    case "$i" in
        --*)
            args="$args $i" ;;
        -*)
            if [ ${#i} -gt 2 ] ; then
                split=""
                i=${i:1}
                while [ ! -z "$i" ] ; do
                    c=${i::1}
                    i=${i:1}
                    split="$split -$c"
                done
                args="$args $split"
            else
                args="$args $i"
            fi
            ;;
        *)
            args="$args $i" ;;
    esac
done
set -- $args

while test $# -gt 0; do
    case "$1" in
        l|linux)           PLATFORM=linux     ;
                           ANDROID_COMMAND=log; shift ;; # Aliasing
        a|android)         PLATFORM=android   ; shift ;;
        w|windows)         PLATFORM=windows   ; shift ;;
        m|main)            TARGET=main        ; shift ;;
        t|tests)           TARGET=tests       ; shift ;;
        u|utils)           TARGET=utils       ; shift ;;
        -r|--release)      RELEASE=true       ; shift ;;
        -s|--serial)       SERIAL=true        ; shift ;;
        -v|--verbose)      VERBOSE=true       ; shift ;;
        -w|--watch)        WATCH=true         ; shift ;;
        --assets-only)     ASSETSONLY=true    ; shift ;;
        --brute)           BRUTE=true         ; shift ;;
        swl|sdl|all|unit)  LOG_FILTER=$1      ; shift ;;
        gles|gl|es)        SHADER_TYPE=$1     ; shift ;;
        log|push|screenshot|run|stop)
                           ANDROID_COMMAND=$1   ; shift ;;
        p)                 ANDROID_COMMAND=push ; shift ;;
        s)                 ANDROID_COMMAND=stop ; shift ;;
        r)                 ANDROID_COMMAND=run  ; shift ;;
        *)  echoerr "Error: Unkown parameter: '$1'\n" ; exit ;;
    esac
done


# Defaults
PLATFORM=${PLATFORM:-linux}
TARGET=${TARGET:-main}
LOG_FILTER=${LOG_FILTER:-swl}
SHADER_TYPE=${SHADER_TYPE:-gles}

# Forwarding
[ ! -z "$RELEASE" ] && FW_RELEASE="--release"
[ ! -z "$WATCH" ] && FW_WATCH="--watch"
[ ! -z "$ASSETSONLY" ] && FW_ASSETSONLY="--assets-only"
[ ! -z "$SERIAL" ] && FW_SERIAL="--serial"
[ ! -z "$VERBOSE" ] && FW_VERBOSE="--verbose"


# Helpers
H_SERIAL=$FW_SERIAL
H_RELEASE=$FW_RELEASE

if [ ! -z "$VERBOSE" ] ; then
#   TODO swarminglogic, 2015-04-01: implement default quiet scons output
#   H_SCONS_VERBOSE="--verbose"
    H_MAKE_VERBOSE="V=1"
fi

[ "$PLATFORM" == "windows" ] && H_WINDOWS="--win64"

if [ ! -z "$SERIAL" ] ; then
    H_SERIAL="-j1"
else
    H_SERIAL="-j7"
fi

# Others
if [ -z "$RELEASE" ] ; then
    BUILD_TYPE="debug"
else
    BUILD_TYPE="release"
fi


# WORKER FUNCTIONS
########################################
# $1: Runner filename
function create_unit_test_main_file {
    local main=$1
    local testlist=$(find ./src -name 'Test*.h' | \
        grep -Ev '_flymake.*' | grep -v 'TestEmpty')
    ./utils/cxxtest/bin/cxxtestgen --error-print -o src/$main $testlist
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

function execute_build {
    if [ -z "$WATCH" ] ; then
        # Prepare surrounding files
        case "$TARGET" in
            main) prepareGitRevHeader ;;
            tests)
                case "$PLATFORM" in
                    android|windows)
                        export SWL_TARGET_SUFFIX=-unittests
                        SWL_MAIN_FILE=main_unit_tests.cpp
                        create_unit_test_main_file $SWL_MAIN_FILE
                        ;;
                esac
                ;;

        esac
        SWL_MAIN_FILE=${SWL_MAIN_FILE:-main.cpp}
        export SWL_MAIN_FILE

        case "$PLATFORM" in
            linux|windows)
                case "$TARGET" in
                    main)   scons $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL ;;
                    tests)  scons $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL --tests ;;
                    utils)  SS=utils/standalones/SConstruct
                        scons -f $SS $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL simpletextpreprocess
                        scons -f $SS $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL obj2cobj
                        scons -f $SS $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL obj2info
                        scons -f $SS $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL wordgen
                        ;;
                esac
                ;;
            android)
                case "$TARGET" in
                    main|tests) cd android && ndk-build $H_SERIAL $H_MAKE_VERBOSE ;;
                esac
                ;;
        esac
    else
        case "$PLATFORM" in
            linux|windows)
                watchfile -s "find ./utils ./src ./SConstruct \
                -name '[!\.]*.cpp' -or \
                -name '[!\.]*.h'   -or \
                -name '[!\.]*.tpp' -or \
                -name '*_build.py' -or \
                -name 'SConscript*' -or \
                -name 'SConstruct' \
               | grep -Ev '_flymake.*' \
               | grep -Ev 'main_.*' \
               | xargs cat | md5sum" -e \
                    $THIS build $PLATFORM $TARGET $FW_SERIAL $FW_VERBOSE $FW_RELEASE
                ;;
            android)
                watchfile -s "find ./src ./android/jni/ \
                -name '[!\.]*.cpp' -or \
                -name '[!\.]*.h'   -or \
                -name '[!\.]*.tpp' -or \
                -name '[!\.]*.mk'
               | grep -Ev '_flymake.*' | xargs cat | md5sum" -e \
                   $THIS build android $TARGET $FW_SERIAL $FW_VERBOSE $FW_RELEASE
                ;;
        esac

        case "$TARGET" in
            main)   scons $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL ;;
            tests)  scons $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL --tests ;;
        esac
    fi
}


# $1 directories
function interactive_brute_rm {
    while true; do
        echo "Directories: $@"
        read -p "Remove these $PLATFORM build directories:? [y/n] " yn
        case $yn in
            [Yy]* )
                rm -rf $@;
                break
                ;;
            [Nn]* )
                exit;;
            * ) echo "Please answer yes or no.";;
        esac
    done
}

function execute_clean {
    if [ -z "$BRUTE" ] ; then
        case "$PLATFORM" in
            linux|windows)
                scons $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL -c
                scons $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL --tests -c
                SS=utils/standalones/SConstruct
                scons -f $SS $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL simpletextpreprocess -c
                scons -f $SS $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL obj2cobj -c
                scons -f $SS $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL obj2info -c
                scons -f $SS $H_WINDOWS $H_SCONS_VERBOSE $H_RELEASE $H_SERIAL wordgen -c
                ;;
            android)
                cd android && ndk-build clean $H_SERIAL $H_MAKE_VERBOSE
                ;;
        esac
    else
        case "$PLATFORM" in
            linux)
                interactive_brute_rm {bin,lib,build}/linux ;;
            windows)
                interactive_brute_rm {bin,lib,build}/windows ;;
            android)
                interactive_brute_rm android/{bin,gen,libs,objs} ;;
        esac
    fi
}


function execute_run {
    if [ -z "$WATCH" ] ; then
        case "$PLATFORM" in
            android)
                cd android && adb shell am start -n com.swarminglogic.swldev/.SWLdev
                adb shell input keyevent 80
                ;;
            linux)
                local LD_LIBRARY_PATH="lib/linux/${BUILD_TYPE}:${LD_LIBRARY_PATH}"
                case "$TARGET" in
                    main)
                        target=./bin/linux/${BUILD_TYPE}/main
                        if [ -x $target ] ; then
                            $target
                        else
                            echoerr "Error: Could not find executable: $target"
                        fi
                        ;;
                    tests)
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
                esac
                ;;
        esac
    else
        # Perform watch
        case "$PLATFORM" in
            android)
                echo "Warning, monitoring will also push new apk's to device"
                $THIS android run
                watchfile -i android/libs/armeabi/* -e \
                    $THIS android push $FW_ASSETSONLY \&\& \
                    $THIS android run
                ;;
            linux)
                case "$TARGET" in
                    main)
                        target=./bin/linux/${BUILD_TYPE}/main
                        $THIS run linux main $FW_RELEASE
                        watchfile -i $target -e $THIS run linux main $FW_RELEASE
                        ;;
                    tests)
                        local LD_LIBRARY_PATH="lib/linux/${BUILD_TYPE}:${LD_LIBRARY_PATH}"
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
                esac
                ;;
        esac
    fi
}


function execute_android_log {
    filter=""
    case "$LOG_FILTER" in
        swl) filter="-s SWL" ;;
        sdl) filter="-s SWL,SWL,SDL,SDL/APP,\
SDL/ERROR,SDL/SYSTEM,SDL/AUDIO,SDL/VIDEO,\
SDL/RENDER,SDL/INPUTSDL,SDL/APP" ;;
        unit) filter="-s CxxTest" ;;
        all) ;;
        *) echoerr "Logic error." ;;
    esac
    adb logcat -c
    adb logcat $filter
}


function execute_android_push {
    if [ -z "$WATCH" ] ; then
        if [ -z "$ASSETSONLY" ] ; then
            cd android && ant debug install
        else
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
        fi
    else
        $THIS android push $FW_ASSETSONLY
        watchfile -i android/libs/armeabi/* -e \
            $THIS android push $FW_ASSETSONLY
    fi
}


function execute_android_screenshot {
    screencapfilename="sdl2-android-screencapture-"$(date +'%Y_%m_%d__%H_%M_%S.png')
    adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > $screencapfilename
    convert $screencapfilename -rotate -90  $screencapfilename
    echo "Screenshot saved to: "$screencapfilename
}


function execute_android_stop {
    cd android && adb shell am force-stop com.swarminglogic.swldev
}


function execute_shader_validation {
    if [ -z "$WATCH" ] ; then
        case "$SHADER_TYPE" in
            gles) $THIS validate-shaders gl && $THIS validate-shaders es ;;
            gl) utils/scripts/validateshaders.sh GL 430 core ;;
            es) utils/scripts/validateshaders.sh ES 300 es ;;
            *) echoerr "Logic error" ;;
        esac
    else
        $THIS validate-shaders $SHADER_TYPE
        watchfile -s "find ./assets/shaders \
                -name '[!\.]*.frag' -or \
                -name '[!\.]*.vert' \
               | grep -Ev '_flymake.*' | xargs cat | md5sum" -e \
            $THIS validate-shaders $SHADER_TYPE
    fi
}


# MAIN DECISION LOGIC
########################################

case "$COMMAND" in
    h|help)  showHelp ; exit ;;

    # Initializes and configures repository
    init) utils/scripts/initialize_project.sh ;;

    # Build a TARGET
    b|build) execute_build ;;

    # Clean TARGET build files
    c|clean) execute_clean ;;

    # Run linux test, linux main or android apk
    r|run) execute_run ;;

    # Various android specific commands
    a|android)
        case "$ANDROID_COMMAND" in
            log)
                execute_android_log
                ;;
            push)
                execute_android_push
                ;;
            screenshot)
                execute_android_screenshot
                ;;
            run)
                $THIS run android main $FW_WATCH
                ;;
            stop)
                execute_android_stop
                ;;
            *)
        esac
        ;;

    # OpenGL/ES Shader Validation
    v|validate-shaders) execute_shader_validation ;;
    *) echoerr "Error: Invalid command: $COMMAND" ; exit ;;
esac
