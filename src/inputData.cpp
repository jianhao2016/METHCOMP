//
//  inputData.cpp
//  meth_compression
//
//  Created by 七仔 on 7/8/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include "../includes/inputData.h"
#include "../includes/row.h"
#include <iostream>

std::vector<std::string> InputData::s_chromList;
std::vector<uint32_t> InputData::s_chromCountList;
std::vector<std::string> InputData::s_nameList;
std::vector<uint32_t> InputData::s_nameCountList;

InputData::InputData(Row& row) {
    dataUpdate(row);
}

void InputData::dataUpdate(Row& row) {
    if (row.m_column1 != s_chromList.back()) {
        s_chromList.push_back(row.m_column1);
        s_chromCountList.push_back(1);
    } else {
        ++s_chromCountList.back();
    }
    if (row.m_column4 != s_nameList.back()) {
        s_nameList.push_back(row.m_column4);
        s_nameCountList.push_back(1);
    } else {
        ++s_nameCountList.back();
    }

    m_chromStartDiff = row.m_column2 - s_chromStartOld;
    s_chromStartOld = row.m_column2;
    // s_chromStartOld = 0;

    m_strand = row.m_column6;
    m_readCount = row.m_column10;
    m_percentage = row.m_column11;
}

void InputData::setFirstRow(Row& row) {
    s_chromList.push_back(row.m_column1);
    s_chromCountList.push_back(1);

    s_nameList.push_back(row.m_column4);
    s_nameCountList.push_back(1);

    m_chromStartDiff = row.m_column2 - s_chromStartOld;
    s_chromStartOld = row.m_column2;

    m_strand = row.m_column6;
    m_readCount = row.m_column10;
    m_percentage = row.m_column11;
} // only use once at the first line.

std::ostream& operator<< (std::ostream& out, const InputData& input){
    if (input.s_chromList.size() == 0 || input.s_nameList.size() == 0) {
        out << "Data doesnt exist! FATAL!";
        return out;
    }
    out << input.s_chromList.back() << '\t' << input.s_chromCountList.back() << '\n';
    out << input.s_nameList.back() << '\t' << input.s_nameCountList.back() << '\n';
    out << input.m_chromStartDiff << '\t' << input.m_strand << '\t' << input.m_readCount << '\t' << static_cast<unsigned int>(input.m_percentage);
    out << "\n--------";
     return out;
}

void InputData::printLists() {
    std::cout << "\n-------\n" << "Here is the runLength list." << '\n';
    for (size_t ii = 0; ii < s_chromList.size(); ii++) {
        std::cout << s_chromList[ii] << '\t' << s_chromCountList[ii] << '\n';
    }

    for (size_t jj = 0; jj < s_nameList.size(); jj++) {
        std::cout << s_nameList[jj] << '\t' << s_nameCountList[jj] << '\n';
    }
}

void InputData::dumpLists(std::ofstream& outfileChrom, std::ofstream& outfileName){
    for (size_t ii = 0; ii < s_chromList.size(); ii++) {
        outfileChrom << s_chromList[ii] << '\t' << s_chromCountList[ii] << '\n';
    }

    for (size_t jj = 0; jj < s_nameList.size(); jj++) {
        outfileName << s_nameList[jj] << '\t' << s_nameCountList[jj] << '\n';
    }
}
