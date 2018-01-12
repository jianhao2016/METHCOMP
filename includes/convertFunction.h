//
//  convertFunction.h
//  meth_compression
//
//  Created by 七仔 on 7/9/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef convertFunction_h
#define convertFunction_h

#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include "arithmeticInt.h"
#include "row.h"
#include "inputData.h"
#include "base.h"
#include "inputBitStream.h"
#include "intDecoder.h"
#include "strandDecoder.h"
#include "baseDecoder.h"

class ArithmeticInt;
class Row;

template <typename T>
T min(T x, T y){
    return ((x > y) ? y : x);
}

template <typename T>
T fast_atoi( const char * str )
{
    T val = 0;
    bool neg = false;
    if(*str == '-') {
        neg = true;
        ++str;
    }
    while( *str >= '0' && *str <= '9' ) {
        val = val*10 + (*str - '0');
        ++str;
    }
    if (neg) {
        val = -val;
    }
    return val;
}

std::string convertRGB(Row& row);
// void assembleLine(std::ofstream& reassembleFile, ArithmeticInt& arInt, std::string& chromString, std::string& nameString);
// void assembleFile(std::ofstream& reassembleFile, std::ifstream& chromFile, std::ifstream& nameFile, std::string& arithmeticFile, int code_value_bits, int16_t num_of_strands, int16_t num_of_base_chars);
void assembleLine(std::FILE* reassembleFile, ArithmeticInt& arInt, std::string& chromString, std::string& nameString);
void assembleFile(std::FILE* reassembleFile, std::ifstream& chromFile, std::ifstream& nameFile, std::string& arithmeticFile, int code_value_bits, int16_t num_of_strands, int16_t num_of_base_chars);
int is_regular_file(const char *path);
std::string getFileName(std::string pathToFile, std::string delimiter);
std::string getPath(std::string pathToFile);

#endif /* convertFunction_h */
