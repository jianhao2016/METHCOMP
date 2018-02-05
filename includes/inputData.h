//
//  inputData.hpp
//  meth_compression
//
//  Created by 七仔 on 7/8/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef inputData_hpp
#define inputData_hpp

#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "arithmeticInt.h"
class Row;

class InputData : public ArithmeticInt{
private:
    //it has a list to storage the name, and chrom[column1], and they are static so we can keep updating them.
    static std::vector<std::string> s_chromList;
    static std::vector<uint32_t> s_chromCountList;
    static std::vector<std::string> s_nameList;
    static std::vector<uint32_t> s_nameCountList;

public:
    InputData ();
    InputData (Row& row);
    void setFirstRow(Row& row); // only use once at the first line.
    void dataUpdate(Row& row);
    static void printLists();
    static void dumpLists(std::ofstream& outfileChrom, std::ofstream& outfileName);
    virtual ~InputData () {};
    friend std::ostream& operator<< (std::ostream& out, const InputData& input);
};

#endif /* inputData_hpp */
