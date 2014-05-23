#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/lib
export LDFLAGS="-lgcov -fprofile-arcs"
export CPPFLAGS="-fprofile-arcs -ftest-coverage"

# Cleaning all build files
echo y | ./devtools.sh cleanl

# Compiling whole project, including tests
./compile.sh l -t

# Generating initial coverage information
lcov -b . -c -i -d ./build -o .coverage.wtest.base

# Running tests
for i in ./bin/tests/* ; do $i ; done

# Generating coverage based on executed tests
lcov -b . -c -d ./build -o .coverage.wtest.run

# Mergin coverage tracefiles
lcov -b . -a .coverage.wtest.base -a .coverage.wtest.run  -o .coverage.total

# Filtering, extracting project files
lcov -b . -e .coverage.total "`pwd`/*" -o .coverage.total.filtered

# Filtering, removing TestFiles and main.cpp
lcov -b . -r .coverage.total.filtered "`pwd`/build/main.cpp" -o .coverage.total.filtered
lcov -b . -r .coverage.total.filtered "`pwd`/*/Test*.*" -o .coverage.total.filtered

# Replacing /build/ with /src/ to unify directories
sed 's/\/build\//\/src\//g' .coverage.total.filtered > .coverage.total

# Clear up previous data
if [[ -d ./html/ ]] ; then
    rm -rf ./html/*
else
    mkdir html
fi

# Generating webpage
genhtml -q -o ./html/ .coverage.total

# Preserve coverage file in backup folder
[[ -d ./coveragehistory/ ]] || mkdir coveragehistory
cp .coverage.total ./coveragehistory/`date +'%Y.%m.%d-coverage'`

# Cleaning up tracefiles
rm .coverage.*
