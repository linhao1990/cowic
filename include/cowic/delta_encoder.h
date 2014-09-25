//delta_encoder.h

#ifndef _DELTA_ENCODER_H_
#define _DELTA_ENCODER_H_

#include "gamma_encoder.h"

class DeltaEncoder : public NumberEncoder{
public:
    BitArray encode(unsigned int num) const;
    int decode(BinaryCode& code, unsigned int& num) const;
private:
    GammaEncoder gamma;
};

#endif
