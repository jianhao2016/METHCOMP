//
//  convertFunction.cpp
//  meth_compression
//
//  Created by 七仔 on 7/9/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <sys/types.h>  // for determining wether a argument is file or not.
#include <sys/stat.h>
#include "../includes/convertFunction.h"

std::string convertRGB(uint8_t percentage){
    std::string rgb;
    if (percentage >= 0 && percentage <= 5) {
        rgb = "0,255,0";
    } else if (percentage >= 6 && percentage <= 15) {
        rgb = "55,255,0";
    } else if (percentage >= 16 && percentage <= 25) {
        rgb = "105,255,0";
    } else if (percentage >= 26 && percentage <= 35) {
        rgb = "155,255,0";
    } else if (percentage >= 36 && percentage <= 45) {
        rgb = "205,255,0";
    } else if (percentage >= 46 && percentage <= 55) {
        rgb = "255,255,0";
    } else if (percentage >= 56 && percentage <= 65) {
        rgb = "255,205,0";
    } else if (percentage >= 66 && percentage <= 75) {
        rgb = "255,155,0";
    } else if (percentage >= 76 && percentage <= 85) {
        rgb = "255,105,0";
    } else if (percentage >= 86 && percentage <= 95) {
        rgb = "255,55,0";
    } else if (percentage >= 96 && percentage <= 100) {
        rgb = "255,0,0";
    } else {
        std::cout << "Percentage error!" << '\n';
        rgb = "???,???,???";
    }
    return rgb;
}

// void assembleLine(std::ofstream& reassembleFile, ArithmeticInt& arInt, std::string& chromString, std::string& nameString){
void assembleLine(std::FILE* reassembleFile, ArithmeticInt& arInt, std::string& chromString, std::string& nameString){

    static int32_t chromStartOld(0);
    int32_t chromStart = arInt.m_chromStartDiff + chromStartOld;
    int32_t chromEnd = chromStart + 1;
    chromStartOld = chromStart;
    // reassembleFile << chromString << '\t' << chromStart << '\t' << chromEnd <<'\t';
    int32_t score = min(arInt.m_readCount, 1000);
    // reassembleFile << nameString << '\t' << score << '\t' << arInt.m_strand << '\t';

    // reassembleFile << chromStart << '\t' << chromEnd <<'\t';
    std::string rgb = convertRGB(arInt.m_percentage);
    // unsigned int percentage = static_cast<unsigned int>(arInt.m_percentage)
    // reassembleFile << rgb << '\t' << arInt.m_readCount << '\t' << static_cast<unsigned int>(arInt.m_percentage) << '\n';

    // std::fprintf(reassembleFile, "%s\t%d\t%d\n", chromString.c_str(), chromStart, chromEnd);
    std::fprintf(reassembleFile, "%s\t%d\t%d\t%s\t%d\t%c\t%d\t%d\t%s\t%d\t%d\n", 
            chromString.c_str(), chromStart, chromEnd, nameString.c_str(), score, arInt.m_strand, chromStart, chromEnd,
            rgb.c_str(), arInt.m_readCount, static_cast<unsigned int>(arInt.m_percentage));
}

void parseString(std::string& str, std::string col[]) {
    std::string delimiter("\t");  // parse the line with tab
    size_t pos(0), ii(0);
    while ((pos = str.find(delimiter)) != std::string::npos) {
        col[ii] = str.substr(0, pos);
        ++ii;
        str.erase(0, pos + delimiter.length());
    }
}

// void assembleFile(std::ofstream& reassembleFile, std::ifstream& chromFile, std::ifstream& nameFile, std::string& arithmeticFile, int code_value_bits, int16_t num_of_strands, int16_t num_of_base_chars) {
void assembleFile(std::FILE* reassembleFile, std::ifstream& chromFile, std::ifstream& nameFile, std::string& arithmeticFile, int code_value_bits, int16_t num_of_strands, int16_t num_of_base_chars) {

    std::string arithmeticLine, chromFileLine, nameFileLine;

    // For chrom file.
    std::getline(chromFile, chromFileLine); // read the first line of
    std::string chromFileLineCol[2];        // declare array store chrom pair.
    parseString(chromFileLine, chromFileLineCol);
    chromFileLineCol[1] = chromFileLine;
    std::string chromString = chromFileLineCol[0];
    int32_t chromCount = fast_atoi<int32_t>(chromFileLineCol[1].c_str());

    // For name file.
    std::getline(nameFile, nameFileLine);
    std::string nameFileLineCol[2];
    parseString(nameFileLine, nameFileLineCol);
    nameFileLineCol[1] = nameFileLine;
    std::string nameString = nameFileLineCol[0];
    int32_t nameCount = fast_atoi<int32_t>(nameFileLineCol[1].c_str());

    // For ArithmeticInt file.
    Base base_decode(code_value_bits);
    // inputBitStream actually read the whole file into buffer.
    InputBitStream ibs(code_value_bits, std::move(arithmeticFile));
    ibs.printBuffer();
    base_decode.startDecoding(ibs);

    // initialized decoders.
    Int32_Decoder firstInt_bd_array = getIntDecoder(base_decode);
    StrandDecoder sd(base_decode, num_of_strands);
    Int32_Decoder secondInt_bd_array = getIntDecoder(base_decode);
    // Int24_Decoder secondInt_bd_array = getInt24Decoder(base_decode);
    BaseDecoder bd(base_decode, num_of_base_chars);

    startIntDecoderModel(firstInt_bd_array);
    sd.startModel(); 
    startIntDecoderModel(secondInt_bd_array);
    // startInt24DecoderModel(secondInt_bd_array);
    bd.startModel();

    bool getEOF(false);

    int32_t d_chromDiff(0);
    char d_strand('?');
    int32_t	d_readCount(0);
    uint8_t d_percentage(0);
    ArithmeticInt arInt;
    bool decoderFlag(true);
    int rowNum(0);

    // no need to output a file. Directly decode to AriInt.
    while (decoderFlag) {
        d_chromDiff = decodeInt(firstInt_bd_array, base_decode, ibs, getEOF);
        d_strand = sd.decoder(base_decode, ibs);
        d_readCount = decodeInt(secondInt_bd_array, base_decode, ibs, getEOF);
        // d_readCount = decodeInt24(secondInt_bd_array, base_decode, ibs, getEOF);
        d_percentage = bd.decoder(base_decode, ibs);

        // d_chromDiff = 0;
        // d_strand = '*';
        // d_readCount = 0;
        // d_percentage = 0;

        arInt.Init(d_chromDiff, d_strand, d_readCount, d_percentage);

        assembleLine(reassembleFile, arInt, chromString, nameString);
        --nameCount;
        --chromCount;

        if (chromCount == 0) {
            std::getline(chromFile, chromFileLine);
            if (chromFileLine.empty() == false) {
                // decoderFlag = true;
                parseString(chromFileLine, chromFileLineCol);
                chromFileLineCol[1] = chromFileLine;
                chromString = chromFileLineCol[0];
                chromCount = fast_atoi<int32_t>(chromFileLineCol[1].c_str());
            }
            else {
                decoderFlag = false;
                std::cout << "End of ChromCout file." << '\n';
            }
        }

        if (nameCount == 0) {
            std::getline(nameFile, nameFileLine);
            if (nameFileLine.empty() == false) {
                parseString(nameFileLine, nameFileLineCol);
                nameFileLineCol[1] = nameFileLine;
                nameString = nameFileLineCol[0];
                nameCount = fast_atoi<int32_t>(nameFileLineCol[1].c_str());
            } else {
                decoderFlag = false;
                std::cout << "End of nameCount file." << '\n';
            }
        }

        rowNum++;
    }
    // end of Arimetic Decoder setup.
    std::cout << "Decompression finished. Number of lines in reassembleFile = " << std::dec << rowNum << '\n';
}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

std::string getFileName(std::string pathToFile, std::string delimiter) {
    size_t sep = pathToFile.find_last_of("\\/");
    std::string fileName;
    if (sep != std::string::npos){
        fileName = pathToFile.substr(sep + 1, pathToFile.size() - sep - 1);
    }
    else {
        // when the pathToFile contains on '/'. It was directly the file.
        fileName = pathToFile;
    }

    std::string name;
    size_t dot = fileName.find_last_of(delimiter);
    // size_t dot = fileName.find_last_of(".");
    if (dot != std::string::npos)
    {
        name = fileName.substr(0, dot);
    }
    else
    {
        name = fileName;
    }
    return name;
}

std::string getPath(std::string pathToFile) {
    size_t sep = pathToFile.find_last_of("\\/");
    std::string file_path;
    if (sep != std::string::npos){
        file_path = pathToFile.substr(0, sep + 1);
    }
    else {
        // when the pathToFile contains on '/', which means inside current folder.
        file_path = "";
    }
    // file_path += '/';
    return file_path;
}
