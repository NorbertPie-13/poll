#! /bin/bash

USAGE="Usage: $(basename $0) [ debug | release ]"
CMAKE=cmake
BUILD=./build
TYPE=Debug


DIR=( $BUILD $TEST $SERVER)

for arg; do
    case "$arg" in
        --help|-h) echo $USAGE; exit 0;;
        debug) TYPE=Debug; BUILD_DIR=$BIN ;;
        release) TYPE=Release; BUILD_DIR=$BIN ;;
        *) echo $USAGE; exit 1;;
    esac
done


cd build
cmake ..  $BUILD_DIR -DCMAKE_BUILD_TYPE=$TYPE 
make
