//
//  baseWrapper.cpp
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
#include "../includes/baseEncoder.h"


BaseEncoder::BaseEncoder(const Base& base, int16_t no_of_chars):
    FreqModel(base, no_of_chars){
}

BaseEncoder::~BaseEncoder(){
    // nothing special here.
}


// ============= encoding funcitons =======================

void BaseEncoder::encoder(int16_t ch, Base& base, OutputBitStream& obs) {
    // no need to check ch == EOF since this is used inside the while(!readLine) loop, everything inside is 'legit'.
    int16_t symbol;
    symbol = m_char_to_index[ch];
    encodeSymbol(symbol, base, obs);
    updateModel(symbol);
}

void BaseEncoder::encodeSymbol(int16_t symbol, Base& base, OutputBitStream& obs){

    int64_t range;          // size of the current range.
    range = (base.m_high - base.m_low) + 1;   // base.m_high and base.m_low are all int64_t.

    // make sure that we have start the frequency model.
    assert((m_cum_freq.size() > 0) && "ERROR: size of cum_freq = 0, start Encoder model first!");
    assert(range > 0 && "ERROR: range == 0, cannot keep encoding.");

    int64_t freqK_1 = static_cast<int64_t>(m_cum_freq[symbol - 1]);
    int64_t freqK = static_cast<int64_t>(m_cum_freq[symbol]);

    int64_t product_high = range * freqK_1;
    int64_t product_low = range * freqK;

    assert((product_high/range == freqK_1) && "** ERROR: overflow in << baseEncoder >> mul. Check m_high!");
    assert((product_low/range == freqK) && "*** ERROR: overflow in << baseEncoder >> mul. Check m_low!");

    base.m_high = base.m_low + product_high/m_cum_freq[0] - 1;
    base.m_low = base.m_low + product_low/m_cum_freq[0];

    // keep looping until none of E1, E2, E3 holds.
    while(true) {
        if (base.m_high < base.m_Half) {
            obs.outputBitPlusFollow(0); // output 0 if in base.m_low half;
        } else if (base.m_low >= base.m_Half) {
            obs.outputBitPlusFollow(1); // output 1 if in upper half.
            base.m_low -= base.m_Half;        // update range by subtracting offset
            base.m_high -= base.m_Half;
        } else if ((base.m_low >= base.m_First_qtr) && (base.m_high < base.m_Third_qtr)) {
            obs.increaseFollowBits();        // output an opposite later
            base.m_low -= base.m_First_qtr;           // IF lay in the middle. E3.
            base.m_high -= base.m_First_qtr;
        } else {
            break;              // otherwise exit loop;
        }

        base.m_low = 2 * base.m_low;          // scale up code range.
        base.m_high = 2 * base.m_high + 1;    // since we subtracted the offset, it's safe. Care is taken to shift ones in when scaled m_high.
    }
    // so after the termination of while loop, we can make sure that:
    // base.m_low <  base.m_First_qtr <   base.m_Half      <   base.m_high, or
    // base.m_low <  base.m_Half      <   base.m_Third_qtr <   base.m_high
}

/**
 * doneEncoding should only be called once and once ONLY after no more bits.
 * @param obs : the bit flow go through the whole process.
 */
void BaseEncoder::doneEncoding(Base& base, OutputBitStream& obs) {
    // FIRST we need to encode the EOF_symbol.
    encodeSymbol(m_EOF_symbol, base, obs);    // we have the previous inequalities hold. Then the only thing to do is to transmit 01 for first one, 10 for second one.

    // 2nd step, send two more bits to end.
    obs.increaseFollowBits();
    if (base.m_low < base.m_First_qtr) {
        obs.outputBitPlusFollow(0);
    } else {
        obs.outputBitPlusFollow(1);
    }

    // The last step is to finishing output stream.
    obs.doneOutputingBits();
}
// ----------------- end of Encoding Functions -----------------------------
