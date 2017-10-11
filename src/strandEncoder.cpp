//
//  strandEncoder.cpp
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "../includes/strandEncoder.h"

StrandEncoder::StrandEncoder(const Base& base, int16_t no_of_chars) :
    BaseEncoder(base, no_of_chars) {
        m_strandList.reserve(no_of_chars);
        m_strandList.push_back('+');
        m_strandList.push_back('-');
        m_strandList.push_back('.');
    }

StrandEncoder::~StrandEncoder(){
    // nothing here.
}

void StrandEncoder::encoder(char strand, Base& base, OutputBitStream& obs) {
    int16_t ch(-1);
    for (size_t ii = 0; ii < m_strandList.size(); ii++) {
        if (strand == m_strandList[ii]) {
            ch = ii;
            break;
        }
    }
    if (ch == -1) {
        std::cout << "Strand " << strand << " does NOT EXIST" << '\n';
        exit(1);
    }
    // convert strand to ch.

    int16_t symbol;
    symbol =  m_char_to_index[ch];
    encodeSymbol(symbol, base, obs);
    updateModel(symbol);
}

void StrandEncoder::printStrand() {
    for (const auto & elem : m_strandList) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';
}
