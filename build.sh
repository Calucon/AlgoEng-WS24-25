#!/bin/bash

mkdir -p bin/
mkdir -p build/

GCC=$(command -v gcc)
if [ -z "${GCC}" ]; then
    echo "GCC missing! Please add it to your path"
    exit
fi

GPP=$(command -v g++)
if [ -z "${GPP}" ]; then
    echo "G++ missing! Please add it to your path"
    exit
fi

SC=$PWD
BC="$SC/build/"

cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=$GCC -DCMAKE_CXX_COMPILER:FILEPATH=$GPP -S$SC -B$BC -G "Unix Makefiles"
cmake --build build/ --config Release --target all --
