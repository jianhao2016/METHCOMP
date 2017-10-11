//
//  base.h
//  provide some basic variables for both ENCODER and DECODER.
//      - const m_Top_value
//      - const m_First_qtr, m_Half, m_Third_qtr
//      - m_low and m_high, which should go through the process.
//  arith_compress_int
//
//  Created by 七仔 on 7/11/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef base_h
#define base_h

#include <cstdlib>
#include <vector>
#include "inputBitStream.h"

class BaseEncoder;
class BaseDecoder;

// member variables are initialize once declared, functions are used by all instance.
// Base will only be declared once. and hold for the whole precess.

class Base {
private:
    const int16_t m_Code_value_bits;  // THE ONLY THING NEED TO BE SET IN BASE. how many bits to represent the code value in the interger implementation. wont larger than 32767;
    int64_t m_Top_value;        // the upper end, i.e: 0.11111111xxx, has multiplication operator, so int64_t might be better.
    int64_t m_First_qtr, m_Half, m_Third_qtr;   // conditions number.
    int64_t m_low, m_high;      // 64 bits interger to store the current interval. [need at least 30 cuz the code_value_bits = 30]

    int64_t m_decode_value;     // use for DECODER only!!!

public:
    Base (int16_t code_value_bits);
    virtual ~Base ();// need to delete the dynamic allocated array.
    void startEncoding();   // set m_low and m_high to original value.
    void startDecoding(InputBitStream& ibs);   // set m_decode_value to initial, and set m_low, m_high to original.

    friend class FreqModel;
    friend class BaseEncoder;
    friend class BaseDecoder;

    void printBase();
};

#endif /* base_h */
