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
    echo "validateshaders - Shorthand commandline aliases
Usage: $0 FILTERKEYWORD VERSIONSTRING

Example:
       $0 ES 300 es
       $0 GL 420 core

FILTERKEYWORD is used in a preprocessing step, allowing use of
#if FILTERKEYWORD
 included text
#else
 omitted
#endif

VERSIONSTRING is the GLSL version string to be prepended as:
#version VERSIONSTRING

Prerequisites:
  glslangValidator
  ./bin/simpletextpreprocess  (./compile.sh utils)
"
    exit
}


if [[ ! $# -eq 3 ]] ; then
    echo -e "Error: invalid arguments\n";
    showHelp
fi



keyword=$1
shift
if [[ ! -x `which glslangValidator` ]] ; then
    scriptsPath=`pwd`/utils/scripts
    if [ -d $scriptsPath ] ; then
        PATH=$PATH:$scriptsPath
    fi

    if [[ ! -x `which glslangValidator` ]] ; then
        echo "Could not find glslangValidator executable in PATH"
        exit 1
    fi
fi
if [[ ! -x ./bin/simpletextpreprocess ]] ; then
    echo "Could not find ./bin/simpletextpreprocess"
    echo "Build utilities using: ./compile.sh utils"
    exit 1
fi
if [[ ! -d ./assets/shaders ]] ; then
    echo "Could not find shader directory. Not in project root?"
    exit 1
fi
version=$@
tmpdir=$(mktemp -d)
echo "${BOLD}${BLUE}Validating shaders [ #version "${version}" ]"${NORMAL}
ignore="Warning, version $1 is not yet complete; most version-specific\
 features are present, but some are missing."
#            echo $ignore
for i in ./assets/shaders/* ; do
    # Perform shader validation
    tmpfile=$tmpdir/`basename $i`
    echo -e "#version $version\n" > $tmpfile
    ./bin/simpletextpreprocess $keyword $i >> $tmpfile
    out=`glslangValidator $tmpfile | sed "s/$ignore//g" | \
sed "s/ERROR/${RED}ERROR${NORMAL}/g" | \
sed "s/Warning/${YELLOW}Warning${NORMAL}/g"`
    if [[ "$out" == "" ]] ; then
        echo -n " [ ${GREEN}OK${NORMAL}  ]  "
        echo ${TEAL}$i${NORMAL}
    else
        echo -n " [ ${RED}${BOLD}BAD${NORMAL} ]  "
        echo ${TEAL}$i${NORMAL}
        echo "$out"
    fi
done
echo
rm -rf $tmpdir
