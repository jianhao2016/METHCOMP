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
#include <sys/types.h>  // for determining wether a argument is file or not.
#include <sys/stat.h>
#include <cstdio>

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
    if (argc < 3) {
        // The first 3 arguments are crutial to run the program.
        if (argv[0]) {
            std::cout << "Usage: " << argv[0] << " <mode> <path to file> [optional <path to output folder> <num of line per block> <line to decode>]" << '\n';
        } else {
            std::cout << "Usage: <program name> <mode> <path to file> [optional <path to output folder> <num of line per block> <line to decode>]" << '\n';
        }
        std::cout << "-----\n** <path to folder> is optional. If not specified the output folder will be in the same directory of input file." << '\n';
        exit(1);
    }

    if (!((strcmp(argv[1], "full") == 0) || (strcmp(argv[1], "compress") == 0) || (strcmp(argv[1], "decompress") == 0))) {
        std::cout << "** <mode> ERROR: mode can be only chosen from \"full\", \"compress\" or \"decompress\"." << '\n';
        exit(1);
    }

    std::string full_path_to_file(argv[2]);
    if (!is_regular_file(full_path_to_file.c_str())) {
        std::cout << "\"" << full_path_to_file << "\" is not a valid file! program terminated."  << '\n';
        exit(1);
    }
    // By far, we have check the input of first three arguments.

    std::string running_mode;
    if (strcmp(argv[1], "full") == 0) {
        running_mode = "Running compresion and decompression on ";
    } else if (strcmp(argv[1], "compress") == 0) {
        running_mode = "Running compression on ";
    } else if (strcmp(argv[1], "decompress") == 0){
        running_mode = "Running decompression on ";
    }

    std::string file_name;
    file_name = getFileName(full_path_to_file, "."); // return name of file only. Without extension.

    std::cout << "+++++++++++\n" << running_mode << file_name << '\n' << "+++++++++++\n";

    std::string file_path;
    file_path = getPath(full_path_to_file); // return the directory only, with '/' ending.

    std::string path_to_output;  // path to output folder, with '/' ending.
    if (argc >= 4) {
    // if the user enter last argv <path to output folder>
        if (strcmp(argv[3], ".") != 0) {
            // user specifies output path.
            path_to_output = argv[3];
            if (path_to_output.back() != '/') {
                path_to_output += '/';
            }
        } else {
            // user enter '.' so path_to_output = current directory;
            path_to_output = "DerivedData/";
        }
    } else {
        // user enter 3 arguments only. output in the same directory of input
        path_to_output = file_path;
        path_to_output += "DerivedData/";
    }
    std::string path_to_output_compressed(path_to_output);
    path_to_output_compressed += "compressed_";
    path_to_output_compressed += file_name;
    path_to_output_compressed += '/';  // path to compression output folder, with '/' ending.

    // std::string makeDir("mkdir -p " + path_to_output_compressed);
    std::string makeDir("mkdir -p ");
    if (strcmp(argv[1], "decompress") == 0) {
        makeDir += path_to_output;
    } else {
        makeDir += path_to_output_compressed;
    }

    // std::cout << "full_path_to_file = " << full_path_to_file  << '\n';
    // std::cout << "file_name = " << file_name << '\n'; // name only. no extension
    // std::cout << "file_path = " << file_path << '\n'; // path only. with '/'.
    // std::cout << "path_to_output = " << path_to_output << '\n'; // end with '/'
    // std::cout << "path_to_output_compressed = " << path_to_output_compressed << '\n'; // end with '/'
    // std::cout << "makeDir = " << makeDir << '\n';

    system(makeDir.c_str());

    int num_of_line_per_block(-1);
    if (argc >= 5) {
        std::istringstream argv_4(argv[4]);
        if (!(argv_4 >> num_of_line_per_block)) {
            std::cerr << "Invalid number of lines per block!" << std::endl;
            exit(1);
        }
        num_of_line_per_block = num_of_line_per_block * 1000;
    }

    int line_to_decode(-1);
    if (argc >= 6) {
        std::istringstream argv_5(argv[5]);
        if (!(argv_5 >> line_to_decode)) {
            std::cerr << "Invalid line number to decode!" << std::endl;
            exit(1);
        }
    }

    std::cout << "line per block = " << num_of_line_per_block << '\n';
    std::cout << "line to decode = " << line_to_decode << '\n';

    // std::string block_name;
    // block_name = generateBlockName("file_test", line_to_decode/num_of_line_per_block);
    // std::cout << "block name in enc = " << block_name << std::endl;

    // block_name = getBlockName("file_test", num_of_line_per_block, line_to_decode);
    // std::cout << "block name in dec = " << block_name << std::endl;
    
    // integer implementation of arithmetic coding needs to specific a bit string. the code_value_bits is the length of that string.
    int16_t code_value_bits(32);
    int16_t num_of_strands(3);
    // each interger in [-2^31, +2^31)  will be split into 4 * 2^8 base value.
    // int16_t num_of_base_chars(256);
    int16_t num_of_percentage(101);   // [0, 100]

    if (strcmp(argv[1], "compress") ==0 || strcmp(argv[1], "full") == 0) {
        std::cout << "COMPRESSION!" << '\n';

        std::string fileName(full_path_to_file);
        std::cout << "compressing " << fileName << "..." << '\n';
        std::ifstream infile(fileName);
        if (!infile)
        {
            // Print an error and exit
            std::cerr << fileName << " could not be opened for reading!" << '\n';
            exit(1);
        }

        bool file_end = false;
        int block_idx = 0;
        int rowNum(1);   // since we skip the first line.
        do 
        {
            std::cout << "---\n" << "processing block " << block_idx;
            std::string outputBitFile(path_to_output_compressed + file_name + "_outfileArInt");
            outputBitFile = generateBlockName(outputBitFile, block_idx);
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
                BaseEncoder		be(base, num_of_percentage);

                startIntEncoderModel(firstInt_be_array);
                se.startModel();
                startIntEncoderModel(secondInt_be_array);
                be.startModel();

                // encoder the first line.
                encodeInt(arInt.getChromDiff(), firstInt_be_array, base, obs);
                se.encoder(arInt.getStrand(), base, obs);
                encodeInt(arInt.getReadCount(), secondInt_be_array, base, obs);

                be.encoder(arInt.getPercent(), base, obs);

                // encoding happens when:
                //  - we can read a file.
                //  - block mode: yes, and row count < rows in a block
                //              : not block mode
                while (num_of_line_per_block == -1 || rowNum < num_of_line_per_block) {
                    if (!std::getline(infile, readLine)) {
                        break;
                    }
                    row.rowUpdate(readLine);  // update the row.
                    inputdata.dataUpdate(row);  // update the input data.
                    arInt = inputdata;          // slice the interger to new object.

                    encodeInt(arInt.getChromDiff(), firstInt_be_array, base, obs);
                    se.encoder(arInt.getStrand(), base, obs);
                    encodeInt(arInt.getReadCount(), secondInt_be_array, base, obs);

                    be.encoder(arInt.getPercent(), base, obs);

                    ++rowNum;           // increase the row counts.
                }

                // end of encoding.
                firstInt_be_array[0].doneEncoding(base, obs);

                // output the chrom list and name list.
                std::string outfileChrom_str = path_to_output_compressed + file_name + "_outfileChrom";
                std::string outfileName_str = path_to_output_compressed + file_name + "_outfileName";
                outfileChrom_str = generateBlockName(outfileChrom_str, block_idx);
                outfileName_str = generateBlockName(outfileName_str, block_idx);

                std::ofstream outfileChrom(outfileChrom_str, std::ios::out);
                std::ofstream outfileName(outfileName_str, std::ios::out);

                if (outfileChrom.is_open() && outfileName.is_open()) {
                    InputData::dumpLists(outfileChrom, outfileName);
                } else {
                    std::cout << "fail saving Lists data!" << '\n';
                }
            }
            // if rowNum is not number of line per block, then it must mean we meet the end of file.
            if (rowNum != num_of_line_per_block) {
                file_end = true;
            } else {
                file_end = false;
                rowNum = 1;
            }
            block_idx++;
        } while((num_of_line_per_block != -1) && (file_end != true));
        std::cout << "==== end of encoding function ====" << '\n';
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

        if (strcmp(argv[1], "decompress") == 0) {
            path_to_output_compressed = file_path;
            file_name = getFileName(full_path_to_file, "_");
        }

        try {

            std::string reassemble_str = path_to_output + "reconstructed_" + file_name;
            reassemble_str = getBlockName(reassemble_str, num_of_line_per_block, line_to_decode);

            std::FILE* reassembleF_c = std::fopen(reassemble_str.c_str(), "w");

            std::string chromFile_str = path_to_output_compressed + file_name + "_outfileChrom";
            chromFile_str = getBlockName(chromFile_str, num_of_line_per_block, line_to_decode);
            std::cout << "reading file: " << chromFile_str << std::endl;
            std::ifstream chromFile(chromFile_str, std::ios::in);
            if (!chromFile.is_open()) {
                std::cout << "outfileChrom open failed." << '\n';
                throw -1;
            }

            std::string nameFile_str = path_to_output_compressed + file_name + "_outfileName";
            nameFile_str = getBlockName(nameFile_str, num_of_line_per_block, line_to_decode);
            std::ifstream nameFile(nameFile_str, std::ios::in);
            if (!nameFile.is_open()) {
                std::cout << "outfileName open failed." << '\n';
                throw -1;
            }


            std::string arithmeticFile(path_to_output_compressed + file_name + "_outfileArInt");
            arithmeticFile = getBlockName(arithmeticFile, num_of_line_per_block, line_to_decode);
            // std::string arithmeticFile = dataPath???? + "DerivedData/compressed_";
            // arithmeticFile += argv[2];
            // arithmeticFile += "/outfileArInt_";
            // arithmeticFile += argv[2];
            // assembleFile(reassembleF_c, chromFile, nameFile, arithmeticFile, code_value_bits, num_of_strands, num_of_base_chars);
            assembleFile(reassembleF_c, chromFile, nameFile, arithmeticFile, code_value_bits, num_of_strands, num_of_percentage);
            // reassembleFile.close();
            std::fclose(reassembleF_c);
            chromFile.close();
            nameFile.close();

        }
        catch(int){
            std::cout << "cant complete reassembling." << '\n';
        }

    }

    std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
    std::cout << "Decompression takes = " << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - end1).count() << "ms" << "\n-------" <<std::endl;

    return 0;
}
