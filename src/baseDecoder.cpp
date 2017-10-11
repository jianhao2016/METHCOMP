//
//  baseDecoder.cpp
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>
#include "../includes/baseDecoder.h"

BaseDecoder::BaseDecoder(const Base& base, int16_t no_of_chars) :
    FreqModel(base, no_of_chars) {

}

BaseDecoder::~BaseDecoder() {
    // nothing special here.
}

// ================ decoding functions. ========================
int16_t BaseDecoder::decodeSymbol(Base& base, InputBitStream& ibs) {
    int64_t range;
	range = (int64_t)(base.m_high - base.m_low) + 1;

	assert((m_cum_freq.size() > 0) && (m_cum_freq[0] != 0) && "ERROR: size of cum_freq = 0, start Decoder model first!");
    assert(range > 0 && "ERROR: range == 0, cannot keep Decoding.");

    int64_t decode_value_gap = base.m_decode_value - base.m_low + 1;
    int64_t product_cumFreq = decode_value_gap * m_cum_freq[0];
    assert((product_cumFreq/m_cum_freq[0] == decode_value_gap) && "** ERROR: overflow in <<baseDecoder>> cumFreq. Check decode_value and m_low.");

    int64_t cumFreq;
    cumFreq = (product_cumFreq - 1)/range;

	assert(cumFreq >= 0 && "*** ERROR: overflow in baseDecoder multiplication! Check cumFreq. ");

	int16_t symbol(0);  // return a symbol, initialize as 0, which does not exist if output.
	while (m_cum_freq[symbol] > cumFreq) {
		symbol++;
	}

	int64_t freqK_1 = static_cast<int64_t>(m_cum_freq[symbol - 1]);
	int64_t freqK = static_cast<int64_t>(m_cum_freq[symbol]);

	int64_t product_high = range * freqK_1;
	int64_t product_low = range * freqK;

	assert((product_high/range == freqK_1) && "** ERROR: overflow in << baseEncoder >> mul. Check m_high!");
	assert((product_low/range == freqK) && "*** ERROR: overflow in << baseEncoder >> mul. Check m_low!");

	base.m_high = base.m_low + product_high/m_cum_freq[0] - 1;
	base.m_low = base.m_low + product_low/m_cum_freq[0];

    while (true) {
        if (base.m_high < base.m_Half) {
            // expand lower half
        } else if (base.m_low >= base.m_Half) {
            base.m_decode_value -= base.m_Half;
            base.m_high -= base.m_Half;
            base.m_low -= base.m_Half;
        } else if (base.m_low >= base.m_First_qtr && base.m_high < base.m_Third_qtr){
            base.m_decode_value -= base.m_First_qtr;
            base.m_high -= base.m_First_qtr;
            base.m_low -= base.m_First_qtr;
        } else {
            break;
        }

        base.m_low = 2* base.m_low;
        base.m_high = 2 * base.m_high + 1;
        base.m_decode_value = 2 * base.m_decode_value + ibs.inputBit();
    }

    return symbol;
}

// if not EOF, the return value will be in [0,255]
// when meet EOF, return value will be 256.
int16_t BaseDecoder::decoder(Base& base, InputBitStream& ibs) {
    int16_t symbol;
    symbol = decodeSymbol(base, ibs);
    if (symbol == m_EOF_symbol) {
        std::cout << "*** End of File!! ***" << '\n';
        return 0x0100;      // positive number, larger than maximum char.
    }
    // so symbol is not EOF.

    int16_t ch(0);
    ch = m_index_to_char[symbol];
    updateModel(symbol);

    return ch;
}


//  ----------  END OF DECODER ----------------
