/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//unary_encoder.cpp

#include "unary_encoder.h"

BitArray UnaryEncoder::encode(unsigned int num) const{
    BitArray ret;
    while(num > 1){
        ret.push_back(0);
        num--;
    }
    ret.push_back(1);
    return ret;
}

int UnaryEncoder::decode(BinaryCode& code, unsigned int& num) const{
    bool meetOne = false;
    num = 0;

    while(!code.finish() ){
        num++;
        if(code.getFirstBitMoveToNext()){
            meetOne = true;
            break;
        }
    }

    if(!meetOne)
        return -1;
    return 0;
}
