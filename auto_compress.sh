#! /bin/sh
#
# auto_compress.sh
# Copyright (C) 2018 qizai <jianhao2@illinois.edu>
#
# Distributed under terms of the MIT license.
#


PATH_2FILE=$1
OUTPUT_PATH=$2
LINE_IN_BLOCK=${3:-5000}

# ./src/output compress $PATH_2FILE $OUTPUT_PATH 5000
./src/output compress $PATH_2FILE $OUTPUT_PATH $LINE_IN_BLOCK
# rm $PATH_2FILE
