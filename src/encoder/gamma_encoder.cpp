/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//gamma_encoder.cpp

#include "gamma_encoder.h"

BitArray GammaEncoder::encode(unsigned int num) const{
    BitArray numBinaryRemaining = getBinaryWithoutHighestOne(num);
    return unary.encode(numBinaryRemaining.size() + 1) + numBinaryRemaining;
}

int GammaEncoder::decode(BinaryCode& code, unsigned int& num) const{
    unsigned int len;
    int ret = unary.decode(code, len);
    if(ret == -1)
        return -1;

    unsigned int remaining = len - 1;
    num = 1;
    if(code.remainSize() < remaining)
        return -1;
    for(unsigned int i = 0; i < remaining; i++){
        num = (num << 1) + code.getFirstBitMoveToNext();
    }
    return 0;
}
