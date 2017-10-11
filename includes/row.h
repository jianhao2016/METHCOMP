//
//  row.h
//  meth_compression
//
//  Created by 七仔 on 7/7/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef row_h
#define row_h

#include <string>
#include <fstream>
#include "convertFunction.h" // for min, convertRGB and fast_atoi function.

class Row {
private:
    std::string m_column1;  // column1 is the chrom, start with chr*
    int32_t m_column2, m_column3; // column2 and 3 are the Chrom Start and end.
    std::string m_column4; // column4 is the name of it.
    int32_t m_column5; // column5 is min(readCount, 1000)
    char m_column6; // column6 is strand, can only be +/-/.
    // column 7 & 8 are just duplicate of 2 & 3;
    std::string m_column9; // column 9 is the rgbitem, it was assigned from a fixed dictionary
    int32_t m_column10; // column10 is readCount, so 5 can be determined by 10;
    uint8_t m_column11; // column11 is the Percentage of reads that show methylation, between 1 & 100.

public:
    Row (std::string& readLine);
    virtual ~Row (){};
    void rowUpdate(std::string& readLine);
    bool checkFail();
    friend std::ifstream& operator>>(std::ifstream& infile, Row& row);
    friend std::ostream& operator<<(std::ostream& out, const Row& row);
    int32_t getScore();
    std::string getRGB();
    friend std::string convertRGB(Row& row);
	friend class ArithmeticInt;
	friend class InputData;
};

#endif /* row_h */
