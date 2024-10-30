#!/bin/bash

mkdir -p data/
rm -r data/*.dat

declare -A dataSizes
dataSizes[10]=10
dataSizes[1k]=1000
dataSizes[10k]=10000
dataSizes[100k]=100000
dataSizes[1M]=1000000
dataSizes[10M]=10000000
dataSizes[1B]=1000000000

for key in "${!dataSizes[@]}"; do
    value=${dataSizes[$key]}
    ./bin/FileGenerator "data/$key.dat" $value
done
