/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//plain_encoder.cpp

#include "plain_encoder.h"

bool PlainEncoder::isFresh(const string& word) const{
    // plain encoder store no state, everything is fresh
    return true;
}

BitArray PlainEncoder::encode(const string& word) const{
    unsigned char len = word.size(); //Assume maximu word lenght < 256
    BitArray lenCode = BitArray(len);
    BitArray plainStrCode = BitArray(word);
    return getFreshCode() + lenCode + plainStrCode;
}

int PlainEncoder::decode(BinaryCode& code, string& word) const{
    // shall not reach here.
    return -1;
}

int PlainEncoder::decodeFresh(BinaryCode& code, string& word, bool isAuxiliary) const{
    unsigned char length;
    int ret = decodeChar(code, length);
    if(ret < 0)
        return ret;
    return decodePlainStr(code, word, length);
}

void PlainEncoder::parse(string& dumpStr){
    // do nothing
}

string PlainEncoder::dump() const{
    return "";
}

