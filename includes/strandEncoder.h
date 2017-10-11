//
//  strandEncoder.h
//  arith_compress_int
//
//  Created by 七仔 on 7/13/17.
//  Copyright © 2017 qizai. All rights reserved.
//

#ifndef strandEncoder_h
#define strandEncoder_h

#include <vector>
#include "base.h"
#include "baseEncoder.h"
#include "outputBitStream.h"

class StrandEncoder : public BaseEncoder{
private:
    std::vector<char> m_strandList;

public:
    StrandEncoder (const Base& base, int16_t no_of_chars = 3);
    virtual ~StrandEncoder ();

    virtual void encoder(char strand, Base& base, OutputBitStream& obs);
    void printStrand();
};

#endif /* strandEncoder_h */
