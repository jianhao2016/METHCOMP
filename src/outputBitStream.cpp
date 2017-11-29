//
//  outputBitStream.cpp
//  arith_compress_int
//
//  Created by 七仔 on 7/12/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include "../includes/outputBitStream.h"

OutputBitStream::OutputBitStream(std::string&& fileName) {
    // create a output file to store the code.
    // std::string path = fileName;
    // std::ofstream outfile(path, std::ios::out);
    // if (outfile.is_open()) {
    //     m_outfile = std::move(outfile);
    std::string path = fileName;
    m_outfile.open(path, std::ios::out);
    if (!m_outfile.is_open()) {
        std::cout << "Unable to open " << path << '\n';
        exit(1);
    }
    startOutputingBits();
}

OutputBitStream::~OutputBitStream() {
    m_outfile.close();
}

void OutputBitStream::startOutputingBits() {
    m_bits_to_follow = 0;   // intial no E3 at all.
    m_buffer = 0x00;       // clean the buffer;
    m_bits_to_go = 8;   // the m_buffer has 8 bits remain unused.
}

void OutputBitStream::outputBit(bool bit) {
    // std::cout << std::dec << bit << ' '; // not necessary, just show bits when processing.

    m_buffer <<= 1;       // left shift the buffer, so the new bit store in LSB
    if (bit) {
        m_buffer |= 0x01; // if bit = 1, write 1 to LSB, otherwise 0 by default.
    }
    m_bits_to_go -= 1;    // decrease one bit in buffer space.
    if (m_bits_to_go == 0) {
        // putc(buffer, stdout)
        // here is something to write to file.
        m_outfile << m_buffer;

        // std::cout << '*' << "buffer = " << std::hex << static_cast<int>(m_buffer) << '*' << ' ';

        m_bits_to_go = 8;
        m_buffer &= 0x00;       // clean the buffer.
    }
}

void OutputBitStream::increaseFollowBits() {
    ++ m_bits_to_follow;
}

/**
 * if E1 or E2 condition holds, output current bit, plus the following bits from E3.
 * @param bit :E1 -> 1, E2 -> 0
 */
void OutputBitStream::outputBitPlusFollow(bool bit){
    outputBit(bit);
    while (m_bits_to_follow > 0) {
        outputBit(!bit);        // output the complement bit.
        --m_bits_to_follow;
    }
}

void OutputBitStream::doneOutputingBits(){
    // putc(m_buffer>>m_bits_to_go, stdout);    // flush the buffer.
    m_buffer = m_buffer << m_bits_to_go;        // move all bits to left

    m_outfile << m_buffer;

    std::cout << "\nfinishing... last byte = " << std::hex << static_cast<int>(m_buffer) << '\n';

    m_bits_to_go = 8;                           // reset bits_to_go.
    m_buffer &= 0x00;           // flush buffer. cause we are done here.
    m_outfile.close();
}

void OutputBitStream::printOBS() {
    std::cout << "bits_to_follow = " << m_bits_to_follow << '\n';
    std::cout << "bits_to_to = " << m_bits_to_go << '\n';
    std::cout << "m_buffer = " << std::hex << m_buffer << '\n';
}
