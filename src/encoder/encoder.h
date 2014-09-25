/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//encoder.h
#ifndef _ENCODER_H
#define _ENCODER_H

#include <unordered_map>

#include "bit_array.h"

using std::unordered_map;

class Encoder{
public:
    Encoder();
    virtual void buildCoder(const unordered_map<string, int>& wordFreqDict) = 0;
    virtual bool isFresh(const string& word) const = 0;
    virtual BitArray encode(const string& word) const = 0;
    /** consume @code and store the first word in @word
     *  @return 0 on success, -1 means no word can be decoded until reach end of code
     */
    virtual int decode(BinaryCode& code, string& word) const = 0;
    virtual void parse(string& dumpStr) = 0;
    virtual string dump() const = 0;
    virtual ~Encoder();
private:
};

#endif
