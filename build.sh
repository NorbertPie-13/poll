#! /bin/bash

USAGE="Usage: $(basename $0) [ debug | release ]"
CMAKE=cmake
BUILD=./build
TYPE=Debug
TEST=./test
SERVER=$TEST/server

DIR=( $BUILD $TEST $SERVER)

for arg; do
    case "$arg" in
        --help|-h) echo $USAGE; exit 0;;
        debug) TYPE=Debug; BUILD_DIR=$BIN ;;
        release) TYPE=Release; BUILD_DIR=$BIN ;;
        *) echo $USAGE; exit 1;;
    esac
done

for i in "${DIR[@]}"
do
    if [ -d "$i" ];
    then
      echo " $i does exist."
    else
      mkdir $i
    fi
done

cd build
cmake ..  $BUILD_DIR -DCMAKE_BUILD_TYPE=$TYPE 
make
