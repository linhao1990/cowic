/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//number_encoder.cpp

#include <sstream>

#include "number_encoder.h"

void NumberEncoder::buildCoder(const unordered_map<string, int>& wordFreqDict){
    //do nothing
}

bool NumberEncoder::isFresh(const string& word) const{
    return false;
}

BitArray NumberEncoder::encode(const string& word) const{
    std::stringstream sstream(word);
    unsigned int num;
    sstream >> num;
    //int num = aoti(str.c_str());
    return encode(num);
}

int NumberEncoder::decode(BinaryCode& code, string & word) const{
    unsigned int num;
    int ret = decode(code, num);
    word = ret;
    std::stringstream sstream;
    sstream << num;
    word = sstream.str();
    return ret;
}

void NumberEncoder::parse(string& dumpStr){
    //do nothing
}

string NumberEncoder::dump() const{
    return "";
}
