//
//  intEncoder.cpp
//  arith_compress_int
//
//  Created by 七仔 on 7/16/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include "../includes/intEncoder.h"

Int32_Encoder getIntEncoder(const Base& base){
	BaseEncoder Int_be1(base, 256);
	BaseEncoder Int_be2(base, 256);
	BaseEncoder Int_be3(base, 256);
	BaseEncoder Int_be4(base, 256);

	Int32_Encoder Int_be_array{ Int_be1, Int_be2, Int_be3, Int_be4 };

	// Int_be_array[0].startModel();
	return Int_be_array;
}

void startIntEncoderModel(Int32_Encoder& IntEncoderArray) {
    for (auto & elem : IntEncoderArray) {
        elem.startModel();
    }
}

void encodeInt(int32_t x, Int32_Encoder& IntEncoderArray, Base& base, OutputBitStream& obs) {
    uint8_t bytes[4]{0};
    for (size_t ii = 0; ii < 4; ii++) {
        bytes[ii] |= (x >> ((3 - ii) * 8));
        IntEncoderArray[ii].encoder(bytes[ii], base, obs);
    }
}


/**
 * getArInt is an assisstant function to help extract arInt from line of file.
 * @param  ss Stringstream that holds the line string, so that we can parse the line easily. NO worries here, Juse leave it.
 * @return    A arInt object with values in a line.
 */
ArithmeticInt getArInt(std::stringstream& ss) {
	// local variables used to initialize the arInt.
	int32_t chromStartDiff;
	char strand;
	int32_t readCount;
	int16_t percentage;
	// ss won't regonize the uint type, have to explicitly cast into uint8_t.
	uint8_t percentage_8bit;

	ss >> chromStartDiff >> strand >> readCount >> percentage;
	// flush the ss for next line.
	ss.str(std::string());
	ss.clear();

	// construct a arInt object with the input line.
	percentage_8bit = static_cast<uint8_t>(percentage);
	ArithmeticInt arInt(chromStartDiff, strand, readCount, percentage_8bit);

	return arInt;
}// not necessary in future application. won't change after all.


// ================ end of Integer encoder functions ==========
