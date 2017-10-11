//
//  intDecoder.h
//  arith_compress_int
//
//  Created by 七仔 on 7/16/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef intDecoder_h
#define intDecoder_h

#include <iostream>
#include <cstdlib>
#include <array>
#include <string>
#include <fstream>
#include "base.h"
#include "baseDecoder.h"
#include "inputBitStream.h"


typedef std::array<BaseDecoder, 4> Int32_Decoder;

Int32_Decoder getIntDecoder(const Base& base);

void startIntDecoderModel(Int32_Decoder& IntDecoderArray);

int32_t decodeInt(Int32_Decoder& IntDecoderArray, Base& base, InputBitStream& ibs, bool& getEOF);


#endif /* intDecoder_h */
