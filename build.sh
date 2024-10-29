#!/bin/bash

mkdir -p bin/

cmake .
cmake --build . --clean-first
