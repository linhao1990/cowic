//number_encoder.h

#ifndef _NUMBER_ENCODER_H_
#define _NUMBER_ENCODER_H_

#include "encoder.h"

class NumberEncoder : public Encoder{
public:
    void buildCoder(const unordered_map<string, int>& wordFreqDict);
    bool isFresh(const string& word) const;
    /** @word is in fact an positive integer. 
     */
    BitArray encode(const string& word) const;
    int decode(BinaryCode& code, string& word) const;
    void parse(string& dumpStr);
    string dump() const;
    /* Two function do the real work. encode & decode integer.
     * */
    virtual BitArray encode(unsigned int num) const = 0;
    virtual int decode(BinaryCode& code, unsigned int& num) const = 0;
private:
};

#endif
