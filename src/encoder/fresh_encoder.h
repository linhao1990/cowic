/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//fresh_encoder.h

#ifndef _FRESH_ENCODER_H_
#define _FRESH_ENCODER_H_

#include "encoder.h"

class FreshEncoder : public Encoder{
public:
    void buildCoder(const unordered_map<string, int>& wordFreqDict);
    BitArray getFreshCode() const;
    virtual void updateWord(const string& word){}
    virtual void setFreshCode(const BitArray& freshCodeValue, const BitArray& auxiliaryCodeValue);
    virtual int decodeFresh(BinaryCode& code, string& word, bool isAuxiliary = false) const = 0;
private:
    BitArray freshCode;
};

#endif
