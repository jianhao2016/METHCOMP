//
//  intEncoder.h
//  arith_compress_int
//
//  Created by 七仔 on 7/16/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef intEncoder_h
#define intEncoder_h

#include <iostream>
#include <iomanip>		// for io format control like setw().
#include <fstream>
#include <string>
#include <sstream>		// for parsing the line.
#include <array>
#include "arithmeticInt.h"
#include "base.h"
#include "freqModel.h"
#include "baseEncoder.h"
#include "baseDecoder.h"
#include "outputBitStream.h"
#include "inputBitStream.h"
#include "strandEncoder.h"
#include "strandDecoder.h"
#include "intEncoder.h"

// ---------- some useful functions for integer encoder --------

typedef std::array<BaseEncoder, 4> Int32_Encoder;

Int32_Encoder getIntEncoder(const Base& base);
void startIntEncoderModel(Int32_Encoder& IntEncoderArray);
void encodeInt(int32_t x, Int32_Encoder& IntEncoderArray, Base& base, OutputBitStream& obs);



// Below are some Test functions.
void testOutputBits();
void encoderTest(std::string&& fileName);
void encoderTwoColumn(std::string&& fileName);
void encoderRandomNumber(std::string&& fileName, int cv_bits = 30, int num_char = 256);

void testInputBits();
void decoderTest();
void decoderTwoColumn();
void decoderRandomNumber(int cv_bits = 30, int num_char = 256);

void strandTest(std::string&& fileName);
void compareFile(std::string&& fileName);

ArithmeticInt getArInt(std::stringstream& ss);

#endif /* intEncoder_h */
