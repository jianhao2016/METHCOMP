//
//  charDecoder.cpp
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "../includes/freqModel.h"

FreqModel::FreqModel(const Base& base, int16_t no_of_chars) :
    m_No_of_chars(no_of_chars) {

        m_EOF_symbol = m_No_of_chars + 1;   // if [1,256] is the index of char, then the index[aka symbol] of EOF will be 257.

        m_No_of_symbols = m_No_of_chars + 1; // plus a EOF symbol.
        m_Max_frequency = ((int64_t)1 << (base.m_Code_value_bits - 2)) - 1;

        m_char_to_index.reserve(m_No_of_chars); // no need to transfer EOF to index, cause it will be sent independently.
        m_index_to_char.reserve(m_No_of_chars + 1);     // index 0 doesn't exist. then from 1 to 256, add up to 257 elements. EOF doesn't have a index!!! its symbol is defined as m_EOF_symbol;

        // frequency information are base on symbols. Not chars.
        m_freq.reserve(m_No_of_symbols + 1);      // freq[0] is special, reserved for comparing. It will always be 0. the rest is 1 - 257. This represent the counts for each symbol. freq[257] is the EOF.
        m_cum_freq.reserve(m_No_of_symbols + 1);  // m_cum_freq[0] is the total counts. m_cum_freq[i] is the lower end of the interval that was assigned to symbol i.
    }

FreqModel::~FreqModel() {
    // nothing here.
}

void FreqModel::startModel() {
    // index 0 is NOTHING. so here I set to -1 since no char is -1;
    m_index_to_char.push_back(-1);
    for (size_t ii = 0; ii < m_No_of_chars; ii++) {
        m_char_to_index.push_back(ii+1);
        m_index_to_char.push_back(ii);
    }

    // set up initial frequency counts to be one for all symbols.
    for (size_t jj = 0; jj <= m_No_of_symbols; jj++) {
        m_freq.push_back(1);
        m_cum_freq.push_back(m_No_of_symbols - jj);
    }
    m_freq[0] = 0;
}

void FreqModel::updateModel(int16_t symbol) {
    // constraint the total counts smaller than the Maximum.
    if (m_cum_freq[0] >= m_Max_frequency) {
        int cum;
        cum = 0;
        for (int i = m_No_of_symbols; i >= 0; i--) {
            m_freq[i] = (m_freq[i] + 1) / 2;
            m_cum_freq[i] = cum;
            cum += m_freq[i];         // make sure that elements in m_freq differ at least one. Since m_freq[i] >= 1;
        }
    }

    // find symbol's "new index".
    int16_t i(0);
    for (i = symbol; m_freq[i] == m_freq[i-1]; i--);

    // update the translation if the symbol has moved.
    // swap the symbol-pair and i-pair

    // now we have two indexs, i & symbol. (i - 1) represent the smallest index whose freq is larger than symbol. [more freqently than symbol.]
    // i and symbol has the same freq. switch them won't change the original i's freq.
    if (i < symbol) {
        // find the char of corresponding symbols.
        int16_t ch_i, ch_symbol;
        ch_i = m_index_to_char[i];
        ch_symbol = m_index_to_char[symbol];

        // first, switch their representing chars.
        // Now the occured symbol goes to a smaller index i;
        m_index_to_char[i] = ch_symbol;
        m_index_to_char[symbol] = ch_i;

        // then, switch their index.
        m_char_to_index[ch_i] = symbol;
        m_char_to_index[ch_symbol] = i;

        // when next time 'symbol' occurs, it points to index i. the smaller one.
        // and 'i' now points to where 'symbol' originally stays.
        // Since we haven't change the freq[] yet. freq[i] == freq[symbol].
        // so next time 'i' occures, its freq is the same as before.
    }

    // int16_t i = symbol;

    // now, 'symbol' has already been moved to where 'i' stays.
    m_freq[i] += 1;         // increment the frequency.
    while (i > 0) {         // update the cumulative table.
        --i;
        m_cum_freq[i] += 1;
    }
}


/**
 * printCharIndex   First print out the m_index_to_char.
 *                  Then print out the m_char_to_index.
 */
void FreqModel::printCharIndex() {
    int i(-1);
    for(const auto& elem : m_index_to_char){
        if (elem == i) {
            i++;
            continue;
        } else {
            i++;
            std::cout << '[' << i << ',' << elem << ']' << ' ';
        }
    }
    std::cout << '\n';
    int j(1);
    for(const auto& elem : m_char_to_index){
        if (elem == j) {
            j++;
            continue;
        } else {
            j++;
            std::cout << '[' << j << ',' << elem << ']' << ' ';
        }
    }
    std::cout << '\n';
}

// print out the freq/counts for each SYMBOL.
void FreqModel::printFreq() {
    std::cout << "m_Max_frequency = " << std::dec << m_Max_frequency << '\n';
    std::cout << "\n---- freq -----" << '\n';
    for (const auto& elem : m_freq) {
        std::cout << std::dec << elem << ' ';
    }
    std::cout << '\n';
    for (const auto& elem: m_cum_freq)  {
        std::cout << std::dec << elem << ' ';
    }
    std::cout << '\n';
    std::cout << "-- end of freq --" << '\n';
}
