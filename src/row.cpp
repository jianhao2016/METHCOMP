//
//  row.cpp
//  meth_compression
//
//  Created by 七仔 on 7/8/17.
//  Copyright © 2017 qizai. All rights reserved.
//
// column1  -   2   -   3   -   4   -   5   -   6   -   7   -   8   -   9
// chrom        start   end     name    score   strand  start   end     color
//
// column10 -   11
// readCount    Percentage
//
// score is min(readCount, 1000), color is determined by Percentage. strand can only be +/-/.

#include <stdio.h>
#include <iostream>
#include <typeinfo> // checking storage type.
#include <map> // for rgbDict;
#include "../includes/row.h"



Row::Row(std::string& readLine){
    rowUpdate(readLine);
}

void Row::rowUpdate(std::string& readLine) {
    std::string col[11];
    std::string delimiter("\t");  // parse the line with tab
    size_t pos(0), ii(0);
    while ((pos = readLine.find(delimiter)) != std::string::npos) {
        col[ii] = readLine.substr(0, pos);
        ++ii;
        readLine.erase(0, pos + delimiter.length());
    }
    col[10] = readLine;

    m_column1 = col[0];
    m_column2 = fast_atoi<int32_t>(col[1].c_str());
    m_column3 = fast_atoi<int32_t>(col[2].c_str());
    m_column4 = col[3];
    m_column5 = fast_atoi<int32_t>(col[4].c_str());
    m_column6 = col[5][0];
    m_column9 = col[8];
    m_column10 = fast_atoi<int32_t>(col[9].c_str());
    m_column11 = fast_atoi<uint8_t>(col[10].c_str());
}

std::ifstream& operator>>(std::ifstream& infile, Row& row){
    std::string strInput;
    std::getline(infile, strInput);

    std::string col[11];
    std::string delimiter("\t");
    size_t pos(0), ii(0);
    while ((pos = strInput.find(delimiter)) != std::string::npos) {
        col[ii] = strInput.substr(0, pos);
        // std::cout << "column " << ii+1 << " = " << col[ii] << '\n';
        ++ii;
        strInput.erase(0, pos + delimiter.length());
    }
    col[10] = strInput;
    // std::cout << "column 11 = " << col[10] << "\n---------" << '\n';

    row.m_column1 = col[0];
    row.m_column2 = fast_atoi<int32_t>(col[1].c_str());
    row.m_column3 = fast_atoi<int32_t>(col[2].c_str());
    row.m_column4 = col[3];
    row.m_column5 = fast_atoi<int32_t>(col[4].c_str());
    row.m_column6 = col[5][0];
    row.m_column9 = col[8];
    row.m_column10 = fast_atoi<int32_t>(col[9].c_str());
    row.m_column11 = fast_atoi<uint8_t>(col[10].c_str());

    return infile;
}

std::ostream& operator<<(std::ostream& out, const Row& row){
    out << row.m_column1 << " size = " << sizeof(row.m_column1) << '\n';
    out << row.m_column2 << " size = " << sizeof(row.m_column2) << '\n';
    out << row.m_column3 << " size = " << sizeof(row.m_column3) << '\n';
    out << row.m_column4 << " size = " << sizeof(row.m_column4) << '\n';
    out << row.m_column5 << " size = " << sizeof(row.m_column5) << '\n';
    out << row.m_column6 << " size = " << sizeof(row.m_column6) << '\n';
    out << row.m_column9 << " size = " << sizeof(row.m_column9) << '\n';
    out << row.m_column10 << " size = " << sizeof(row.m_column10) << '\n';
    out << static_cast<unsigned int>(row.m_column11) << " size = " << sizeof(row.m_column11) << '\n';
    out << "------------";
    return out;
}

int32_t Row::getScore(){
    return min(m_column10,1000);
}

std::string Row::getRGB(){
    std::string rgb;
    if (m_column11 >= 0 && m_column11 <= 5) {
        rgb = "0,255,0";
    } else if (m_column11 >= 6 && m_column11 <= 15) {
        rgb = "55,255,0";
    } else if (m_column11 >= 16 && m_column11 <= 25) {
        rgb = "105,255,0";
    } else if (m_column11 >= 26 && m_column11 <= 35) {
        rgb = "155,255,0";
    } else if (m_column11 >= 36 && m_column11 <= 45) {
        rgb = "205,255,0";
    } else if (m_column11 >= 46 && m_column11 <= 55) {
        rgb = "255,255,0";
    } else if (m_column11 >= 56 && m_column11 <= 65) {
        rgb = "255,205,0";
    } else if (m_column11 >= 66 && m_column11 <= 75) {
        rgb = "255,155,0";
    } else if (m_column11 >= 76 && m_column11 <= 85) {
        rgb = "255,105,0";
    } else if (m_column11 >= 86 && m_column11 <= 95) {
        rgb = "255,55,0";
    } else if (m_column11 >= 96 && m_column11 <= 100) {
        rgb = "255,0,0";
    } else {
        std::cout << "Percentage error!" << '\n';
        rgb = "???,???,???";
    }
    return rgb;
}

bool Row::checkFail(){
    if (m_column5 != getScore()) {
        std::cout << "min func error!" << '\n';
        return true;
    }
    std::string rgb = getRGB();
    if (rgb == "???,???,???") {
        return true;
    }

    if (rgb != m_column9) {
        std::cout << "Rgb error!" << '\n';
        return true;
    }
    return false;
}
