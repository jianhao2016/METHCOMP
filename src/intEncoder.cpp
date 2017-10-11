//
//  intEncoder.cpp
//  arith_compress_int
//
//  Created by 七仔 on 7/16/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#include <stdio.h>
#include "../includes/intEncoder.h"

Int32_Encoder getIntEncoder(const Base& base){
	BaseEncoder Int_be1(base, 256);
	BaseEncoder Int_be2(base, 256);
	BaseEncoder Int_be3(base, 256);
	BaseEncoder Int_be4(base, 256);

	Int32_Encoder Int_be_array{ Int_be1, Int_be2, Int_be3, Int_be4 };

	// Int_be_array[0].startModel();
	return Int_be_array;
}

void startIntEncoderModel(Int32_Encoder& IntEncoderArray) {
    for (auto & elem : IntEncoderArray) {
        elem.startModel();
    }
}

void encodeInt(int32_t x, Int32_Encoder& IntEncoderArray, Base& base, OutputBitStream& obs) {
    uint8_t bytes[4]{0};
    for (size_t ii = 0; ii < 4; ii++) {
        bytes[ii] |= (x >> ((3 - ii) * 8));
        IntEncoderArray[ii].encoder(bytes[ii], base, obs);
    }
}


/**
 * getArInt is an assisstant function to help extract arInt from line of file.
 * @param  ss Stringstream that holds the line string, so that we can parse the line easily. NO worries here, Juse leave it.
 * @return    A arInt object with values in a line.
 */
ArithmeticInt getArInt(std::stringstream& ss) {
	// local variables used to initialize the arInt.
	int32_t chromStartDiff;
	char strand;
	int32_t readCount;
	int16_t percentage;
	// ss won't regonize the uint type, have to explicitly cast into uint8_t.
	uint8_t percentage_8bit;

	ss >> chromStartDiff >> strand >> readCount >> percentage;
	// flush the ss for next line.
	ss.str(std::string());
	ss.clear();

	// construct a arInt object with the input line.
	percentage_8bit = static_cast<uint8_t>(percentage);
	ArithmeticInt arInt(chromStartDiff, strand, readCount, percentage_8bit);

	return arInt;
}// not necessary in future application. won't change after all.


// ================ end of Integer encoder functions ==========


// void strandTest(std::string&& fileName){
// 	Base base(8);
// 	base.startEncoding();
//
// 	OutputBitStream obs("DerivedData/strandEncodeSeq");
//
// 	StrandEncoder se(base, 3);
// 	se.startModel();
//
// 	std::ifstream infile("../data/DerivedData/" + fileName, std::ios::in);
// 	if (infile.is_open()) {
// 		std::string line;
// 		while (std::getline(infile, line)) {
// 			std::stringstream ss;
// 			ss << line;
// 			char s_chr;
// 			ss >> s_chr;
// 			std::cout << "\ns = " << s_chr << ", code seq:\t";
// 			se.encoder(s_chr, base, obs);
// 		}
//
// 		infile.close();
// 	}
// 	else {
// 		std::cerr << "** Unable to open file for StrandTest\n";
// 	}
// 	std::cout << "\n strand list = ";
// 	se.printStrand();
// 	se.doneEncoding(base, obs);
//
// 	// start decoding.
// 	std::cout << "\n ------ decoding... -------" << '\n';
// 	Base base_decode(8);
// 	InputBitStream ibs(8, "DerivedData/strandEncodeSeq");
// 	ibs.printBuffer();
//
// 	base_decode.startDecoding(ibs);
//
// 	StrandDecoder sd(base_decode, 3, "DerivedData/StrandDecoder_output");
// 	sd.startModel();
//
// 	char ch_decode;
// 	ch_decode = sd.decoder(base_decode, ibs);
//
// 	while(ch_decode != '?') {
// 		std::cout << ch_decode << '\n';
// 		ch_decode = sd.decoder(base_decode, ibs);
// 	}
//
// }
//
// void decoderTest() {
// 	std::cout << "I am decoderTest. Enter Name of compressed file:";
// 	std::string fileName;
// 	// std::cin.clear();
// 	// std::cin.ignore(32767, '\n');
// 	std::cin >> fileName;;
//
// 	if (fileName.compare(0, 3, "row") == 0) {
// 		std::cout << "Check 2 column file." << '\n';
// 		decoderTwoColumn();
//
// 	} else if (fileName.compare(0, 9, "randomNum") == 0) {
// 		std::cout << "Check 1 column rand num file." << '\n';
//
// 		std::cout << "Enter # of chars in file: ";
// 		int nchar;
// 		std::cin >> nchar;
// 		std::cout << "Enter Bits to Encode file: ";
// 		int cvBits;
// 		std::cin >> cvBits;
//
// 		std::cout << nchar << '\t' << cvBits << '\n';
//
// 		decoderRandomNumber(cvBits, nchar);
//
// 	} else {
// 		std::cout << "** Unable to open decoderTest file." << '\n';
// 	}
// }
//
// void decoderRandomNumber(int cv_bits, int num_char) {
// 	Base base(cv_bits);
// 	InputBitStream ibs(cv_bits, "DerivedData/EncoderSeq_Random_Num_" + std::to_string(num_char));
// 	ibs.printBuffer();
// 	base.startDecoding(ibs);
//
// 	BaseDecoder bd(base, num_char);
// 	bd.startModel();
//
// 	std::cout << "._._._._. decode test ._._._._._." << '\n';
//
// 	std::ofstream outfile("../data/DerivedData/Decoder_test_randn_" + std::to_string(num_char), std::ios::out);
// 	if (!outfile.is_open()) {
// 		std::cout << "** Can't open Decoder ouput file!" << '\n';
// 		exit(1);
// 	}
//
// 	int16_t ch;
// 	ch = bd.decoder(base, ibs);
//
//
// 	int rowNum(0);
// 	while (ch != 0x0100) {
// 		outfile << ch << '\n';
// 		rowNum++;
// 		// if (((rowNum >= 70662467) && (rowNum <= 70662477))|| rowNum <= 5) {
// 		// 	std::cout << std::dec << "\n===================\nrow = " << rowNum << '\n';
// 		// 	std::cout << "ch = " << ch << '\n';
// 		// 	bd.printFreq();
// 		// 	bd.printCharIndex();
// 		// 	base.printBase();
// 		// 	ibs.printBuffer();
// 		// }
//
// 		ch = bd.decoder(base, ibs);
// 	}
// }
//
// void decoderTwoColumn(){
// 	Base base(37);
// 	InputBitStream ibs(37);		// value of bits = 30. only used at the end.
// 	ibs.printBuffer();
// 	base.startDecoding(ibs);	// read no_of_chars bits to decode value, set m_low and m_high to intial.
//
// 	BaseDecoder bd1(base, 101);
// 	bd1.startModel();
//
// 	BaseDecoder bd2(base, 256);
// 	bd2.startModel();
//
// 	std::cout << "._._._._. decode test ._._._._._." << '\n';
//
// 	std::ofstream outfile("../data/DerivedData/decoderTestOutput", std::ios::out);
// 	if (!outfile.is_open()) {
// 		std::cout << "Can't open Decoder output file!" << '\n';
// 		exit(1);
// 	}
//
// 	int16_t ch1, ch2;
// 	ch1 = bd1.decoder(base, ibs);
// 	ch2 = bd2.decoder(base, ibs);
// 	while(ch1 != 0x0100 && ch2 != 0x0100) {
// 		// std::cout << ch1 << '\t';
// 		// std::cout << ch2 << '\n';
// 		outfile << ch1 << '\t';
// 		outfile << ch2 << '\n';
//
// 		ch1 = bd1.decoder(base, ibs);
// 		ch2 = bd2.decoder(base, ibs);
// 	}
// }
//
// void testInputBits() {
// 	InputBitStream ibs(30, "DerivedData/testBinInput");
// 	ibs.printBuffer();
// 	for (size_t jj = 0; jj < 8; jj++) {
// 		for (size_t ii = 0; ii < 8; ii++) {
// 			std::cout << ibs.inputBit() << ' ';
// 		}
// 		std::cout << '\n';
// 	}
// }
//
// void encoderTest(std::string&& fileName) {
// 	if (fileName.compare(0, 3, "row") == 0) {
// 		std::cout << "Check 2 column file." << '\n';
// 		encoderTwoColumn(std::move(fileName));
//
// 	} else if (fileName.compare(0, 9, "randomNum") == 0) {
// 		std::cout << "Check 1 column rand num file." << '\n';
//
// 		std::cout << "Enter Bits to Encode file: ";
// 		int cvBits;
// 		std::cin >> cvBits;
//
// 		std::cout << "Enter # of chars in file: ";
// 		int nchar;
// 		std::cin >> nchar;
//
// 		encoderRandomNumber(std::move(fileName), cvBits, nchar);
// 	} else {
// 		std::cout << "** Unable to open encoderTest file." << '\n';
// 	}
// }
//
// void encoderRandomNumber(std::string&& fileName, int cv_bits, int num_char) {
// 	Base base(cv_bits);
// 	base.startEncoding();
//
// 	BaseEncoder be(base, num_char);
// 	be.startModel();
//
// 	std::cout << "._._._._. encode test ._._._._._." << '\n';
//
// 	OutputBitStream obs("DerivedData/EncoderSeq_Random_Num_" + std::to_string(num_char));
//
// 	std::ifstream infile("../data/test_single_compress/" + fileName, std::ios::in);
// 	if (infile.is_open()) {
// 		std::string line;
//
// 		int rowNum(0);
// 		while (std::getline(infile, line)) {
// 			std::stringstream ss;
// 			ss << line;
// 			int16_t v;
// 			ss >> v;
// 			be.encoder(v, base, obs);
//
// 			rowNum++;
// 			// if (((rowNum >= 70662467) && (rowNum <= 70662477)) || rowNum <= 5) {
// 			// 	std::cout <<std::dec<< "\n===================\nrow = " << rowNum << '\n';
// 			// 	be.printFreq();
// 			// 	be.printCharIndex();
// 			// 	base.printBase();
// 			// 	obs.printOBS();
// 			// }
// 		}
// 		infile.close();
// 		be.doneEncoding(base, obs);
// 	}
// 	else {
// 		std::cout << "** Unable to open file for Random Number Encoder Test.\n";
// 	}
// }
//
// void encoderTwoColumn(std::string&& fileName){
// 	Base base(37);				// set code_value_bits = 37.
// 	base.startEncoding();		// setup m_low and m_high.
// 	base.startEncoding();		// setup m_low and m_high.
//
// 	BaseEncoder be1(base, 101);	// setup model parameters. 6 chars.
// 	be1.startModel();		// call startModel() in CharEncoder.
//
// 	BaseEncoder be2(base, 256);
// 	be2.startModel();
//
// 	std::cout << "._._._._. encode test ._._._._._." << '\n';
//
// 	OutputBitStream obs;			// obs has been initialized onece created.
// 	// obs.startOutputingBits();	// explicitly initialize bit stream here.
//
// 	std::ifstream infile("../data/" + fileName, std::ios::in);
// 	if (infile.is_open()) {
// 		std::string line;
// 		while (std::getline(infile, line)) {
// 			std::stringstream ss;
// 			ss << line;
// 			int16_t v1, v2;
// 			ss >> v1 >> v2;
// 			// std::cout << '\n' << "v1 = " << v1 << "\t v2 =  " << v2 << '\t';
// 			be1.encoder(v1, base, obs);
// 			be2.encoder(v2, base, obs);
// 		}
// 		infile.close();
// 	}
// 	else {
// 		std::cout << "** Unable to open file\n";
// 	}
//
// 	be1.doneEncoding(base, obs);
//
// }
//
// void testOutputBits(){
// 	OutputBitStream obs;
// 	obs.outputBitPlusFollow(1);
// 	for (size_t i = 0; i < 4; i++) {
// 		obs.increaseFollowBits();
// 	}
// 	obs.outputBitPlusFollow(0);
// 	obs.outputBit(1);
//
// 	obs.outputBitPlusFollow(1);
// 	for (size_t i = 0; i < 4; i++) {
// 		obs.increaseFollowBits();
// 	}
// 	obs.outputBitPlusFollow(0);
// 	obs.outputBit(0);
// 	for (size_t i = 0; i < 3; i++) {
// 		obs.increaseFollowBits();
// 	}
// 	obs.outputBitPlusFollow(0);
//
// 	for (size_t i = 0; i < 4; i++) {
// 		obs.increaseFollowBits();
// 	}
// 	obs.outputBitPlusFollow(0);
// 	// obs.outputBitPlusFollow(1);
// 	obs.outputBit(0);
// 	obs.outputBit(1);
// 	obs.outputBit(0);
// 	obs.outputBit(1);
// 	obs.outputBit(1);
// 	obs.outputBit(1);
// 	obs.outputBit(0);
// 	obs.outputBit(0);
// 	obs.outputBit(0);
// 	// obs.outputBit(1);
//
// 	std::cout << "\n--- end of binary output ---\n";
// 	obs.doneOutputingBits();
// }
//
// void compareFile(std::string&& fileName) {
// 	// Read the content of the DIZ file into memory.
// 	// This goes through the file only once.
// 	std::deque<std::string> originFile;
// 	std::ifstream inputData("../data/" + fileName, std::ios::in);
// 	if (inputData.is_open()) {
// 		std::string line;
// 		while (std::getline(inputData, line)) {
// 			originFile.push_back(line);
// 		}
// 		inputData.close();
// 	}
// 	else {
// 		std::cerr << "** Unable to open file\n";
// 	}
//
// 	std::ifstream recoverData("../data/DerivedData/decoderTestOutput", std::ios::in);
// 	if (recoverData.is_open()) {
// 		std::string line;
// 		int numOfLine(0);
// 		while (std::getline(recoverData, line)) {
// 			++numOfLine;
//
// 			if(line != originFile.front()) {
// 				std::cout << "different in line : " << numOfLine << '\n';
// 				std::cout << " output > " << line << '\n';
// 				std::cout << " origin < " << originFile.front() << '\n';
// 				break;
// 			}
// 			originFile.pop_front();
// 		}
// 		std::cout << "# different Lines in " << fileName << " = " << originFile.size() << '\n';
// 		std::cout << "# Lines in decoderTestOutput = " << numOfLine << '\n';
// 		recoverData.close();
// 	}
// 	else {
// 		std::cerr << "** Unable to open file\n";
// 	}
// }
