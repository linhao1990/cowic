/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//fresh_encoder.cpp

#include "fresh_encoder.h"

void FreshEncoder::buildCoder(const unordered_map<string, int>& wordFreqDict){
    //do nothing
}

BitArray FreshEncoder::getFreshCode() const{
    return freshCode;
}

void FreshEncoder::setFreshCode(const BitArray& freshCodeValue, const BitArray& auxiliaryCodeValue){
    freshCode = freshCodeValue;
}
