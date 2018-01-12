//
//  arithmeticInt.h
//  meth_compression
//
//  Created by 七仔 on 7/9/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef arithmeticInt_h
#define arithmeticInt_h

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

class ArithmeticInt {
protected:
    static int32_t s_chromStartOld; // for the first number in col2; initialize as 0. Will not be update here. Instead, all the update are done in child class InputData.
    int32_t m_chromStartDiff;
    char m_strand;
    int32_t m_readCount;
    uint8_t m_percentage;

public:
    ArithmeticInt (){

    };
    // constructor with input from user.
    ArithmeticInt(int32_t chromStartDiff, char strand, int32_t readCount, uint8_t percentage);

    // destructor. Nothing here.
    virtual ~ArithmeticInt (){

    };

    // set the value of member variables to use input.
    void Init(int32_t chromStartDiff, char strand, int32_t readCount, uint8_t percentage);

    // dump arInt object to file. NOT USING IT HERE.
    void dumpArithmeticInt(std::ofstream& outfile);

    friend std::ostream& operator<< (std::ostream& out, ArithmeticInt& ArInt);
    // friend void assembleLine(std::ofstream& reassembleFile, ArithmeticInt& arInt, std::string& chromString, std::string& nameString);
    friend void assembleLine(std::FILE* reassembleFile, ArithmeticInt& arInt, std::string& chromString, std::string& nameString);

    int32_t getChromDiff()  {return m_chromStartDiff;}
    char getStrand()        {return m_strand;}
    int32_t getReadCount()  {return m_readCount;}
    uint8_t getPercent()    {return m_percentage;}
};

#endif /* arithmeticInt_h */
