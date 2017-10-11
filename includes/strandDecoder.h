//
//  strandDecoder.h
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef strandDecoder_h
#define strandDecoder_h

#include <vector>
#include "base.h"
#include "inputBitStream.h"
#include "baseDecoder.h"

class StrandDecoder : public BaseDecoder{
private:
    // std::ofstream m_outfile;
    std::vector<char> m_strandList;

public:
    StrandDecoder (const Base& base, int16_t no_of_chars = 3);
    virtual ~StrandDecoder ();

    char decoder(Base& base, InputBitStream& ibs);
};

#endif /* strandDecoder_h */
