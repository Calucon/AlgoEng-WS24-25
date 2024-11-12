#!/bin/bash

mkdir -p bin/
mkdir -p build/

cmake -B build/ -D CMAKE_BUILD_TYPE=Release .
cmake --build build/
