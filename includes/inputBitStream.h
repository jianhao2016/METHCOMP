//
//  inputBitStream.h
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef inputBitStream_h
#define inputBitStream_h

#include <cstdlib>
#include <fstream>
#include <string>
#include <deque>

class InputBitStream {
private:
    std::ifstream m_infile;      // the input file
    std::deque<uint8_t> m_bufferList;    // read all the bytes to bufferList.
    uint8_t m_buffer;       // read a bite into the buffer.

    const int16_t m_Code_value_bits;  // determined how many bits to store at first.
    int16_t m_bits_to_go;   // number of bits still stays in buffer.
    int16_t m_garbage_bits;   // number of bits past EOF
public:
    InputBitStream (int16_t code_value_bits, std::string&& fileName);   // including startInputingBits.
    virtual ~InputBitStream ();

    void startInputingBits();
    void increaseGarbageBits();
    int16_t inputBit();     // return 0 or 1.

    void printBuffer();

};

#endif /* inputBitStream_h */
