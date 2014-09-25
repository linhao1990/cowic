//unary_encoder.h

#ifndef _UNARY_ENCODER_H_
#define _UNARY_ENCODER_H_

#include "number_encoder.h"

class UnaryEncoder : public NumberEncoder{
public:
    BitArray encode(unsigned int num) const;
    int decode(BinaryCode& code, unsigned int& num) const;
private:
};

#endif
