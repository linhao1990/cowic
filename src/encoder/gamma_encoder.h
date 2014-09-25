/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
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
