//
//  baseEncoder.cpp
//  arith_compress_int
//
//  Created by 七仔 on 7/11/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include "../includes/base.h"


// constructor for Base Encoder, default parameter code_value_bits = 30 set in .h file.
Base::Base (int16_t code_value_bits) : m_Code_value_bits(code_value_bits)
{
    // The following variables are only setup once when initialize, and NEVER CHANGE AFTER IT.
    m_Top_value = ((int64_t)1 << m_Code_value_bits) - 1;
    m_First_qtr = (m_Top_value / 4) + 1;
    m_Half      = 2 * m_First_qtr;
    m_Third_qtr = 3 * m_First_qtr;

    m_decode_value = 0;
}



Base::~Base () {
    // delete [] m_cum_freq;    // if use vector, no need to delete array.
}

void Base::startEncoding() {
    m_low = 0;
    m_high = m_Top_value;
}

void Base::startDecoding(InputBitStream& ibs) {
    // input bits to fill the code value.
    m_decode_value = 0;
    for (size_t ii = 0; ii < m_Code_value_bits; ii++) {
        m_decode_value = 2 * m_decode_value + ibs.inputBit();
    }

    // set original interval.
    m_low = 0;
    m_high = m_Top_value;
}

//  ------- END OF BASE -----
void Base::printBase() {
    std::cout << "code_value_bits = " << std::dec << m_Code_value_bits << '\n';
    std::cout << "top_value = " << std::hex << m_Top_value << '\n';
    std::cout << "quarter points: " << '\n' << m_First_qtr << '\n' << m_Half << '\n' << m_Third_qtr << '\n';
    std::cout << " ***** interval ***** " << "\n[" << m_low << ",\n" << m_high << ']' << '\n';
    std::cout << "decode value = " << m_decode_value << '\n';
}
