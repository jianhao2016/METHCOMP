CXX = clang++
CXXFLAGS = -c -std=c++11 -stdlib=libc++
HEADERPATH = ../includes/
SRC = main.o row.o inputData.o arithmeticInt.o convertFunction.o base.o freqModel.o outputBitStream.o inputBitStream.o baseEncoder.o baseDecoder.o strandEncoder.o strandDecoder.o intEncoder.o intDecoder.o

output: $(SRC)
	$(CXX) $(SRC) -o output

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

row.o: row.cpp  $(HEADERPATH)row.h
	$(CXX) $(CXXFLAGS) row.cpp

inputData.o: inputData.cpp  $(HEADERPATH)inputData.h
	$(CXX) $(CXXFLAGS) inputData.cpp

arithmeticInt.o: arithmeticInt.cpp  $(HEADERPATH)arithmeticInt.h
	$(CXX) $(CXXFLAGS) arithmeticInt.cpp

convertFunction.o: convertFunction.cpp  $(HEADERPATH)convertFunction.h
	$(CXX) $(CXXFLAGS) convertFunction.cpp

base.o: base.cpp $(HEADERPATH)base.h
	$(CXX) $(CXXFLAGS) base.cpp

freqModel.o: freqModel.cpp $(HEADERPATH)freqModel.h
	$(CXX) $(CXXFLAGS) freqModel.cpp

outputBitStream.o: outputBitStream.cpp $(HEADERPATH)outputBitStream.h
	$(CXX) $(CXXFLAGS) outputBitStream.cpp

inputBitStream.o: inputBitStream.cpp $(HEADERPATH)inputBitStream.h
	$(CXX) $(CXXFLAGS) inputBitStream.cpp

baseEncoder.o: baseEncoder.cpp $(HEADERPATH)baseEncoder.h $(HEADERPATH)freqModel.h $(HEADERPATH)outputBitStream.h
	$(CXX) $(CXXFLAGS) baseEncoder.cpp

baseDecoder.o: baseDecoder.cpp $(HEADERPATH)baseDecoder.h $(HEADERPATH)freqModel.h $(HEADERPATH)inputBitStream.h
	$(CXX) $(CXXFLAGS) baseDecoder.cpp

strandEncoder.o: strandEncoder.cpp $(HEADERPATH)strandEncoder.h $(HEADERPATH)baseEncoder.h
	$(CXX) $(CXXFLAGS) strandEncoder.cpp

strandDecoder.o: strandDecoder.cpp $(HEADERPATH)strandDecoder.h $(HEADERPATH)baseDecoder.h
	$(CXX) $(CXXFLAGS) strandDecoder.cpp

intEncoder.o: intEncoder.cpp $(HEADERPATH)intEncoder.h
	$(CXX) $(CXXFLAGS) intEncoder.cpp

intDecoder.o: intDecoder.cpp $(HEADERPATH)intDecoder.h
	$(CXX) $(CXXFLAGS) intDecoder.cpp

clean:
	rm *.o output
