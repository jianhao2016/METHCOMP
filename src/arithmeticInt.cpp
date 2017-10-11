//
//  arithmeticInt.cpp
//  meth_compression
//
//  Created by 七仔 on 7/9/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include "../includes/arithmeticInt.h"

int32_t ArithmeticInt::s_chromStartOld(0);

ArithmeticInt::ArithmeticInt(int32_t chromStartDiff, char strand, int32_t readCount, uint8_t percentage){
	Init(chromStartDiff, strand, readCount, percentage);
}

void ArithmeticInt::Init(int32_t chromStartDiff, char strand, int32_t readCount, uint8_t percentage){
	m_chromStartDiff = chromStartDiff;
	m_strand = strand;
	m_readCount = readCount;
	m_percentage = percentage;
}

std::ostream& operator<< (std::ostream& out, ArithmeticInt& ArInt){
	out << ArInt.m_chromStartDiff << '\t' << ArInt.m_strand << '\t' << ArInt.m_readCount << '\t' << static_cast<unsigned int>(ArInt.m_percentage);
    return out;
}

void ArithmeticInt::dumpArithmeticInt(std::ofstream& outfile){
	outfile << m_chromStartDiff << '\t' << m_strand << '\t' << m_readCount << '\t' << static_cast<unsigned int>(m_percentage) <<'\n';
}

// --------------- end of class member functions --------
