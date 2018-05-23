#! /bin/sh
#
# auto_script.sh
# Copyright (C) 2018 qizai <jianhao2@illinois.edu>
#
# Distributed under terms of the MIT license.
#
# usage of the script:
# source auto_decompress.sh <name of file to be decompressed> <path to file> <save path for decompress file> 
#                           <lines in each block>
# the end mark '&' in the loop is to enable parallel script

FILE_NAME=$1
PATH_2FILE=$2
SAVE_PATH=$3
TOTAL_BLOCKS=$(ls -1 $PATH_2FILE| wc -l)
TOTAL_BLOCKS=$((TOTAL_BLOCKS/3))
NUM_PER_BLOCK=${4:-5000}

echo $TOTAL_BLOCKS
while [ $TOTAL_BLOCKS -gt 0 ]
do
    ((TOTAL_BLOCKS--))
    start_idx=$((TOTAL_BLOCKS * $NUM_PER_BLOCK * 1000 + 1))
    # echo $start_idx
    # ./src/output decompress $PATH_2FILE$FILE_NAME $SAVE_PATH $NUM_PER_BLOCK $start_idx &
    ./src/output decompress $PATH_2FILE"${FILE_NAME}_outfileArInt-0" $SAVE_PATH $NUM_PER_BLOCK $start_idx &
done

wait
echo "All complete."

cat $(ls "${SAVE_PATH}/"reconstructed_* | sort -n -t - -k 2) > "${SAVE_PATH}/decompressed_${FILE_NAME}"
