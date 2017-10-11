//
//  inputBitStream.cpp
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <iterator> // for istream_iterator
#include <deque>     // for std::deque
#include "../includes/inputBitStream.h"

InputBitStream::InputBitStream(int16_t code_value_bits, std::string&& fileName) : m_Code_value_bits(code_value_bits) {
    std::ifstream input(fileName, std::ios::in | std::ios::binary);
    // std::ifstream input( "../data/" + fileName, std::ios::in | std::ios::binary | std::ios::ate );   // for std::read() only.
    if (input.is_open()) {

        // std::ifstream::pos_type fileSize = input.tellg();
        // input.seekg(0, std::ios::beg);
        //
        // std::deque<uint8_t> buffer(fileSize);
        // input.read( (char*)( &buffer[0]), fileSize ) ;

        std::istreambuf_iterator<char> begin(input), end;
        std::deque<uint8_t> buffer (begin, end);

        m_infile = std::move(input);
        // move all bytes to bufferList.
        m_bufferList = std::move(buffer);

        startInputingBits();
    } else {
        std::cout << "Unable to open binary File." << '\n';
        exit(1);
    }
}

InputBitStream::~InputBitStream(){
    m_infile.close();
}

void InputBitStream::startInputingBits() {
    m_bits_to_go = 0;
    m_garbage_bits = 0;
}

void InputBitStream::increaseGarbageBits() {
    ++ m_garbage_bits;
}

int16_t InputBitStream::inputBit() {
    // no more bits in buffer, need to read a new buffer.
    if (m_bits_to_go == 0) {
        if (!m_bufferList.empty()) {
            // get a new byte.
            m_buffer = m_bufferList.front();
            m_bufferList.pop_front();

        } else {
            // bufferList is empty, already pass the end of input file. Put 0 in buffer.
            // m_buffer = 0x00;

            if (m_garbage_bits > m_Code_value_bits) {
                // the code file was all 0
                // std::cout << "bad input file!!" << '\n';
                // exit(1);
            }
            m_garbage_bits += 8;
        }
        m_bits_to_go = 8;
    }

    int16_t bit;
    bit = ((m_buffer & 0x80) == 0x80);
    m_buffer = m_buffer << 1;
    -- m_bits_to_go;

    return bit;
}

void InputBitStream::printBuffer() {
    std::cout << "Reading " << std::dec << m_bufferList.size() << " bytes..." << '\n';
    // for(const auto& elem : m_bufferList) {
    //     std::cout << std::hex << static_cast<int>(elem) << ' ';
    // }
    // std::cout << '\n';
}
