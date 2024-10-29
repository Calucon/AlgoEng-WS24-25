#!/bin/bash

mkdir -p bin/
mkdir -p build/

cmake .
cmake --build .
