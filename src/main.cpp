//
//  main.cpp
//  meth_compression
//
//  Created by 七仔 on 6/14/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <iomanip>		// for io format control like setw().
#include <sstream>		// for parsing the line.
#include <array>
#include <chrono>		// for timing

#include "../includes/row.h"
#include "../includes/inputData.h"
#include "../includes/arithmeticInt.h"
#include "../includes/convertFunction.h"

#include "../includes/base.h"
#include "../includes/freqModel.h"
#include "../includes/baseEncoder.h"
#include "../includes/baseDecoder.h"
#include "../includes/outputBitStream.h"
#include "../includes/inputBitStream.h"
#include "../includes/strandEncoder.h"
#include "../includes/strandDecoder.h"
#include "../includes/intEncoder.h"
#include "../includes/intDecoder.h"

// three modes.
// full => finish the complete compress/decompress process.
// compress => compress the file to DerivedData/compressed/outfileArInt.
// decompress => decompress the outfileArInt, chromFile, nameFile to DerivedData/reassembleFile.


int main(int argc, char const *argv[]) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    if (argc <= 3) {
        // user forget to input the test fileName!
        if (argv[0]) {
            std::cout << "Usage: " << argv[0] << " <mode> <fileName> <path_to_file>" << '\n';
        } else {
            std::cout << "Usage: <program name> <mode> <fileName> <path_to_file>" << '\n';
        }
        exit(1);
    }

    std::string dataPath("../data/");
    std::string inputFilePath(dataPath);
    if (argc >= 4) {
        if (strcmp(argv[3], ".") != 0) {
            inputFilePath += argv[3];
            // inputFilePath += "/";
        }
    }
    std::string makeDir("mkdir -p " + dataPath + "DerivedData/compressed_" + argv[2]);
    system(makeDir.c_str());
    // system("mkdir -p ../data/DerivedData");


    // integer implementation of arithmetic coding needs to specific a bit string. the code_value_bits is the length of that string.
    int16_t code_value_bits(32);
    int16_t num_of_strands(3);
    // each interger in [-2^31, +2^31)  will be split into 4 * 2^8 base value.
    int16_t num_of_base_chars(256);
    std::string outputBitFile(dataPath + "DerivedData/compressed_" + argv[2] +  "/outfileArInt_" + argv[2]);

    if (strcmp(argv[1], "compress") ==0 || strcmp(argv[1], "full") == 0) {
        std::cout << "COMPRESSION!" << '\n';

        std::string fileName(argv[2]);
        fileName = inputFilePath + fileName;
        std::ifstream infile(fileName);
        if (!infile)
        {
            // Print an error and exit
            std::cerr << "File could not be opened for reading!" << '\n';
            exit(1);
        }

        int rowNum(1);   // since we skip the first line.
        // try{
        {
            std::string readLine;
            // get the first line!!
            std::getline(infile, readLine);
            Row row(readLine);  // instantize a row.
            ArithmeticInt arInt;
            InputData inputdata;
            // function setFirstRow is important to store the initial value of differential coding.
            inputdata.setFirstRow(row);
            arInt = inputdata;

            // initialize encoders & output bit stream.
            Base base(code_value_bits);
            base.startEncoding();

            OutputBitStream obs(std::move(outputBitFile));
            obs.startOutputingBits();

            Int32_Encoder 	firstInt_be_array = getIntEncoder(base);
            Int32_Encoder 	secondInt_be_array = getIntEncoder(base);
            StrandEncoder 	se(base, num_of_strands);
            BaseEncoder		be(base, num_of_base_chars);

            startIntEncoderModel(firstInt_be_array);
            se.startModel();
            startIntEncoderModel(secondInt_be_array);
            be.startModel();

            // encoder the first line.
            encodeInt(arInt.getChromDiff(), firstInt_be_array, base, obs);
            se.encoder(arInt.getStrand(), base, obs);
            encodeInt(arInt.getReadCount(), secondInt_be_array, base, obs);
            be.encoder(arInt.getPercent(), base, obs);

            while (std::getline(infile, readLine)) {
                row.rowUpdate(readLine);  // update the row.
                inputdata.dataUpdate(row);  // update the input data.
                arInt = inputdata;          // slice the interger to new object.

                encodeInt(arInt.getChromDiff(), firstInt_be_array, base, obs);
                se.encoder(arInt.getStrand(), base, obs);
                encodeInt(arInt.getReadCount(), secondInt_be_array, base, obs);
                be.encoder(arInt.getPercent(), base, obs);

                ++rowNum;           // increase the row counts.
                // if (row.checkFail()) {
                //     std::cout << row << '\n';   // print the fail line.
                //     throw rowNum;
                // }
            }

            // end of encoding.
            firstInt_be_array[0].doneEncoding(base, obs);
            std::cout << "==== end of encoding function ====" << '\n';

            // output the chrom list and name list.
            std::ofstream outfileChrom(dataPath + "DerivedData/compressed_" + argv[2] + "/outfileChrom_" + argv[2], std::ios::out);
            std::ofstream outfileName(dataPath + "DerivedData/compressed_" + argv[2] + "/outfileName_" + argv[2], std::ios::out);
            if (outfileChrom.is_open() && outfileName.is_open()) {
                InputData::dumpLists(outfileChrom, outfileName);
            } else {
                std::cout << "fail saving Lists data!" << '\n';
            }
        }
        // catch(int rowNum){
        //     std::cout << "Miss match in row: " << rowNum << '\n';
        //     rowNum = 0;
        //     infile.close();
        // }
        // if (rowNum > 0) {
        //     std::cout << "pass the check test!! Num of rows = " << std::dec << rowNum << '\n';
        // }
        infile.close();
    }


    std::chrono::steady_clock::time_point end1= std::chrono::steady_clock::now();
    std::cout << "Compression takes = " << std::dec << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin).count() << "ms" << "\n--------" <<std::endl;

    // decompression part.
    if (strcmp(argv[1], "decompress") ==0 || strcmp(argv[1], "full") == 0) {
        std::cout << "DECOMPRESSION!" << '\n';

        std::cout << "\nStart Decoding..." << '\n';
        std::cout << "-------------------------------------" << '\n';

        try {
            std::ofstream reassembleFile(dataPath + "/reassembleFile_" + argv[2], std::ios::out);
            if (!reassembleFile.is_open()) {
                std::cout << "reassembleFile open failed." << '\n';
                throw -1;
            }

            std::ifstream chromFile(dataPath + "DerivedData/compressed_" + argv[2] + "/outfileChrom_" + argv[2], std::ios::in);
            if (!chromFile.is_open()) {
                std::cout << "outfileChrom open failed." << '\n';
                throw -1;
            }

            std::ifstream nameFile(dataPath + "DerivedData/compressed_" + argv[2] + "/outfileName_" + argv[2], std::ios::in);
            if (!nameFile.is_open()) {
                std::cout << "outfileName open failed." << '\n';
                throw -1;
            }


            std::string arithmeticFile = dataPath + "DerivedData/compressed_";
            arithmeticFile += argv[2];
            arithmeticFile += "/outfileArInt_";
            arithmeticFile += argv[2];
            assembleFile(reassembleFile, chromFile, nameFile, arithmeticFile, code_value_bits, num_of_strands, num_of_base_chars);
            reassembleFile.close();
            chromFile.close();
            // arithmeticFile.close();
            nameFile.close();

        }
        catch(int){
            std::cout << "cant complete reassembling." << '\n';
        }

    }

    std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
    std::cout << "Decompression takes = " << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - end1).count() << "ms" << "\n-------" <<std::endl;
    // int code_value_bits = 34;
    // int num_of_base_chars = 256;
    // encoderRandomNumber("randomNum256", code_value_bits, num_of_base_chars);
    // decoderRandomNumber(code_value_bits, num_of_base_chars);
    return 0;
}
