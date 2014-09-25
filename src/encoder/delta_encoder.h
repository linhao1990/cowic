/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
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
