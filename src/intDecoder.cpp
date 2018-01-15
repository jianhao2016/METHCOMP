//
//  intDecoder.cpp
//  arith_compress_int
//
//  Created by 七仔 on 7/16/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include "../includes/intDecoder.h"

Int32_Decoder getIntDecoder(const Base& base){
    BaseDecoder Int_bd1(base, 256);
    BaseDecoder Int_bd2(base, 256);
    BaseDecoder Int_bd3(base, 256);
    BaseDecoder Int_bd4(base, 256);

    Int32_Decoder Int_bd_array{ Int_bd1, Int_bd2, Int_bd3, Int_bd4 };

    return Int_bd_array;
}

int32_t decodeInt(Int32_Decoder& IntDecoderArray, Base& base, InputBitStream& ibs, bool& getEOF) {
    int32_t decode_v(0);
    int16_t ch[4];

    ch[0] = IntDecoderArray[0].decoder(base, ibs);
    // see if we meet the EOF.
    if (ch[0] > 0x00ff) {
        std::cout << "base decoder symbol >= 0x00ff, break in decodeInt." << '\n';
        getEOF = true;
        return 0;
    }
    decode_v |= ((ch[0] & (0x00ff)) << 24);

    for (size_t ii = 1; ii < 4; ii++) {
        ch[ii] = IntDecoderArray[ii].decoder(base, ibs);
        decode_v |= ((ch[ii] & (0x00ff)) << ((3 - ii) * 8));
    }

    return decode_v;
}

void startIntDecoderModel(Int32_Decoder& IntDecoderArray) {
    for (auto & elem : IntDecoderArray) {
        elem.startModel();
    }
}

///////////////////////////
Int24_Decoder getInt24Decoder(const Base& base) {
	BaseDecoder Int_bd1(base, 256);
	BaseDecoder Int_bd2(base, 256);
	BaseDecoder Int_bd3(base, 256);

	Int24_Decoder Int_bd_array{Int_bd1, Int_bd2, Int_bd3};
	return Int_bd_array;
}

int32_t decodeInt24(Int24_Decoder& IntDecoderArray, Base& base, InputBitStream& ibs, bool& getEOF) {
    int32_t decode_v(0);
    int16_t ch[3];

    ch[0] = IntDecoderArray[0].decoder(base, ibs);
    // see if we meet the EOF.
    if (ch[0] > 0x00ff) {
        std::cout << "base decoder symbol >= 0x00ff, break in decodeInt." << '\n';
        getEOF = true;
        return 0;
    }
    decode_v |= ((ch[0] & (0x00ff)) << 8);

    for (size_t ii = 1; ii < 3; ii++) {
        ch[ii] = IntDecoderArray[ii].decoder(base, ibs);
        decode_v |= ((ch[ii] & (0x00ff)) << ((2 - ii) * 8));
    }

    return decode_v;
}

void startInt24DecoderModel(Int24_Decoder& IntDecoderArray) {
    for (auto & elem : IntDecoderArray) {
        elem.startModel();
    }
}

