/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//variable_byte_encoder.cpp

#include "variable_byte_encoder.h"

BitArray VariableByteEncoder::encode(unsigned int num) const{
    BitArray ret;
    bool endByte = true;
    do
    {
        unsigned char c = num & 127;
        num = num >> 7;
        if(endByte){
            c += 128;
            endByte = false;
        }
        ret = BitArray(c) + ret;
    }
    while (num != 0);
    return ret;
}

int decodeFirstByte(BinaryCode& code, unsigned char& c){
    if(code.remainSize() < 8)
        return -1;
    c = 0;
    for(int i = 0; i < 8; i++){
        c = (c << 1) + code.getFirstBitMoveToNext();
    }
    return 0;
}

int VariableByteEncoder::decode(BinaryCode& code, unsigned int& num) const{
    num = 0;
    while(true){
        unsigned char c;
        int status = decodeFirstByte(code, c);
        if(status == -1)
            return -1;
        num = (num << 7) + c;
        if(c & 128){
            num -= 128;
            break;
        }
    }
    return 0;
}
