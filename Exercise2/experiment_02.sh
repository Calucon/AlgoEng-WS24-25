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
TYPE=("sta" "mta")

read -p "Number of threads to use: " THREADS
if ! [[ "$THREADS" =~ ^[0-9]+$ ]]; then
    echo "Invalid Input!"
    exit
fi
if [ "$THREADS" -lt "2" ]; then
    echo "Thread count must be at least 2!"
    exit
fi

#############################################
#                  EXECUTE                  #
#############################################

for algo in "${ALGO[@]}"; do
    for type in "${TYPE[@]}"; do
        for sizeExp in $(seq 3 9); do
            size=$((10 ** sizeExp))
            ./bin/ParallelSort $size $algo $type $THREADS >/dev/null
        done
    done
done
