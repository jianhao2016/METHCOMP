//
//  baseDecoder.h
//
//  Derived from base class, add some common decoding function
//
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef baseDecoder_h
#define baseDecoder_h

#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include "base.h"
#include "freqModel.h"
#include "inputBitStream.h"

class BaseDecoder : public FreqModel{
protected:
    // std::ofstream m_outfile;

public:
    // fileName is the decoder output file. not the source file.
    BaseDecoder (const Base& base, int16_t no_of_chars = 256);
    virtual ~BaseDecoder ();

    // we need to update the m_decode_value in base class. Can't be const here.
    virtual int16_t decodeSymbol(Base& base, InputBitStream& ibs);
    int16_t decoder(Base& base, InputBitStream& ibs);

};

#endif /* baseDecoder_h */
