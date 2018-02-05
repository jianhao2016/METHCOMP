#! /bin/sh
#
# auto_script.sh
# Copyright (C) 2018 qizai <jianhao2@illinois.edu>
#
# Distributed under terms of the MIT license.
#

PATH_2FILE=$1
SAVE_PATH=$2

# ./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 10000 1 &
# ./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 10000 10000001 &
# ./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 10000 20000001 &
# ./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 10000 30000001 &
# ./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 10000 40000001 &

./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 5000 1 &
./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 5000 5000001 &
./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 5000 10000001 &
./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 5000 15000001 &
./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 5000 20000001 &
./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 5000 25000001 &
./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 5000 30000001 &
./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 5000 35000001 &
./src/output decompress data/DerivedData/compressed_ENCFF751DLO/ENCFF751DLO_outfileArInt-0 data/DerivedData/ 5000 40000001 &

wait
echo "All complete."
