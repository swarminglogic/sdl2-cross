#!/bin/bash

function writeGitRevHeader() {
tmpfile=$(tempfile)
gitrevfile=src/util/gitrev.h

echo "#ifndef UTIL_GITREV_H
#define UTIL_GITREV_H

#define CURRENT_GIT_REV \"$1\"

#endif  // UTIL_GITREV_H" > $tmpfile

if [ ! -e $gitrevfile ] ; then
    mv $tmpfile $gitrevfile
elif cmp --silent $tmpfile $gitrevfile ; then
    # Files are the same
    rm $tmpfile
else
    # Files differ
    mv $tmpfile $gitrevfile
fi
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
