#!/bin/bash

mkdir -p bin/
mkdir -p build/

cmake -S src/ -B build/
cmake --build src/
