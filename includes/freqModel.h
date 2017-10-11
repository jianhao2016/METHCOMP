//
//  charDecoder.h
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef freqModel_h
#define freqModel_h

#include "base.h"
#include <cstdlib>
#include <vector>

class FreqModel {
protected:
    const int16_t m_No_of_chars;    // to setup the frequency vectors.
    int16_t m_EOF_symbol;           // special symbol for EOF, not included in conversion relationships.
    int16_t m_No_of_symbols;        // all chars + EOF.
    int64_t m_Max_frequency;        // used in updateModel. shrink the freq[];

    // frequency vectors. Base on symbols/indexs, not chars.
    std::vector<int64_t> m_cum_freq;
    std::vector<int64_t> m_freq;

    // conversion relation, in order to sort freq list for performance benefits.
    std::vector<int16_t> m_char_to_index;
    std::vector<int16_t> m_index_to_char;

public:
    FreqModel (const Base& base, int16_t no_of_chars = 256);
    virtual ~FreqModel ();

    // Model manipulate function are not related to output/input. Thus don't need OutputBitStream or Base.
    virtual void startModel();
    virtual void updateModel(int16_t symbol);

    virtual void printCharIndex();
    virtual void printFreq();
};

#endif /* freqModel_h */
