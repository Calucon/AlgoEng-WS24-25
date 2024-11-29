#!/bin/bash

#############################################
#              BUILD & PREPARE              #
#############################################

pushd ..

/bin/bash build.sh
echo -e "\n\n"

read -p "Do you want to (re-)generate input data files? [y/N]: " -n 1 -r
echo # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]; then
    /bin/bash Exercise1/gendata.sh
    echo -e "\n\n"
fi

#############################################
#                  CONFIG                   #
# Uncomment options if you like to run them #
#############################################

#INPUT_FILE="data/10.dat"
#INPUT_FILE="data/10k.dat"
INPUT_FILE="data/100k.dat"
#INPUT_FILE="data/1M.dat"  # 4MB file
#INPUT_FILE="data/10M.dat" # 40MB file
#INPUT_FILE="data/1B.dat"  # 4GB file

BLOCK_SIZE=10000 # large B

PRINT_PROGRESS="silent"
#PRINT_PROGRESS="progress" # uncomment to print progress to console

#############################################
#                  EXECUTE                  #
#############################################

# run merge sort
./bin/EM_MergeSort $INPUT_FILE "data/out.dat" $BLOCK_SIZE $PRINT_PROGRESS
# analyze file
./bin/FileAnalyzer "data/out.dat" silent
