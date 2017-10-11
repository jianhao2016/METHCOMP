//
//  outputBitStream.h
//  arith_compress_int
//
//  Created by 七仔 on 7/12/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef outputBitStream_h
#define outputBitStream_h

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

class OutputBitStream {
private:
    int32_t m_bits_to_follow;   // keep an track on E3 conditions. How many complement bits to output. 2^(2^16) is huge.
    int16_t m_bits_to_go;       // indicate how many bits remain unused in buffer.
    uint8_t m_buffer;           // an 8bit buffer.
    std::ofstream m_outfile;

public:
    OutputBitStream (std::string&& fileName);         // default constructor call startOutputingBits.
    virtual ~OutputBitStream ();

    void startOutputingBits();  // set up all three member variabes. 0, 8, 0
    void increaseFollowBits();  // increase E3 condition number.
    void outputBit(bool bit);   // write a bit to flow.
    void outputBitPlusFollow(bool bit);   // output bits when E1 || E2 holds. Plus all the implement bits cause by E3.
    void doneOutputingBits();   // flushing the buffer to complete output. And close the output file.

    void printOBS();
};

#endif /* outputBitStream_h */
