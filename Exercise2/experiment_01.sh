#!/bin/bash

#############################################
#              BUILD & PREPARE              #
#############################################

pushd ..

/bin/bash build.sh
echo -e "\n\n"

#############################################
#                  CONFIG                   #
# Uncomment options if you like to run them #
#############################################

ALGO=("merge" "quick")
SIZE=$((10 ** 6))
THREADS=$(grep -c ^processor /proc/cpuinfo)
CORES=$(grep ^cpu\\scores /proc/cpuinfo | uniq | awk '{print $4}')

#############################################
#                  EXECUTE                  #
#############################################

for algo in "${ALGO[@]}"; do
    for i in $(seq 2 $THREADS); do
        for c in $(seq 1 100); do
            ./bin/ParallelSort $SIZE $algo mta $i >/dev/null
        done
    done
done
