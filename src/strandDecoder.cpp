//
//  strandDecoder.cpp
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include "../includes/strandDecoder.h"

StrandDecoder::StrandDecoder(const Base& base, int16_t no_of_chars) :
    BaseDecoder(base, no_of_chars) {
        m_strandList.reserve(no_of_chars);
        m_strandList.push_back('+');
        m_strandList.push_back('-');
        m_strandList.push_back('.');
    }

StrandDecoder::~StrandDecoder() {
    // nothing here.
    // m_outfile.close();
}

char StrandDecoder::decoder(Base& base, InputBitStream& ibs) {
    int16_t symbol;
    symbol = decodeSymbol(base, ibs);
    if (symbol == m_EOF_symbol) {
        std::cout << "*** stradDecoder End of File!! ***" << '\n';
        return '?';
    }
    // so symbol is not EOF.
    int16_t ch(0);
    ch = m_index_to_char[symbol];
    updateModel(symbol);

    char strand;
    for (size_t ii = 0; ii < m_strandList.size(); ii++) {
        if (ch == ii) {
            strand = m_strandList[ii];

            return strand;
        }
    }

    std::cout << "** ERROR ***: strandDecoder Fail!" << '\n';
    return '0';
}
