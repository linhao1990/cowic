/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//delta_encoder.cpp

#include "delta_encoder.h"

BitArray DeltaEncoder::encode(unsigned int num) const{
    BitArray numBinaryRemaining = getBinaryWithoutHighestOne(num);
    return gamma.encode(numBinaryRemaining.size() + 1) + numBinaryRemaining;
}

int DeltaEncoder::decode(BinaryCode& code, unsigned int& num) const{
    unsigned int len;
    int ret = gamma.decode(code, len);
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
