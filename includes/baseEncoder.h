//
//  baseWrapper.h
//  derived from basa class, ADD some common encoding function. like:
//      - startEncoding();      set up high and low.
//      - encodeSymbol(symbol, obs);    transmit a single symbol.
//      - doneEncoding();       finishing encoding and send EOF;
//
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef baseEncoder_h
#define baseEncoder_h
#include <cstdlib>
#include <vector>
#include "base.h"
#include "freqModel.h"
#include "outputBitStream.h"

// according to different no_of_chars, each instance of BaseEncoder will have their own different member variables, i.e.
//      - m_No_of_chars
//      - m_EOF_symbol
//      - m_No_of_symbols
//      - m_Max_frequency, is a little special, since we wanna update the
//                          model yet this is determined by value of bits.
//      - frequency lists.
//      - two conversion lists.
//
// But all instances of BaseEncoder share the functions. So functions won't take extra space.

class BaseEncoder : public FreqModel{
protected:
    // nothing new here.

public:
    BaseEncoder (const Base& base, int16_t no_of_chars = 256);
    virtual ~BaseEncoder ();

    // Encoding function need to use m_low, m_high, and quarter seperators. Also has influence on output sequence.
    virtual void encodeSymbol(int16_t symbol, Base& base, OutputBitStream& obs);
    virtual void doneEncoding(Base& base, OutputBitStream& obs);    // encode EOF -> send last two bits -> clean bit stream obs.

    virtual void encoder(int16_t ch, Base& base, OutputBitStream& obs); // take a ch from user, and the output bit stream. Feed data into the bit stream.

};


#endif /* baseEncoder_h */
