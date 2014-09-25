//gamma_encoder.h

#ifndef _GAMMA_ENCODER_H_
#define _GAMMA_ENCODER_H_

#include "unary_encoder.h"

class GammaEncoder : public NumberEncoder{
public:
    BitArray encode(unsigned int num) const;
    int decode(BinaryCode& code, unsigned int& num) const;
private:
    UnaryEncoder unary;
};

#endif
