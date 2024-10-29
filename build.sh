#!/bin/bash

mkdir -p bin/
mkdir -p build/

cmake -B build/ .
cmake --build build/
