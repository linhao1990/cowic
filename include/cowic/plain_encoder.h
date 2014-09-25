//plain_encoder.h

#ifndef _PLAIN_ENCODER_H_
#define _PLAIN_ENCODER_H_

#include "fresh_encoder.h"

class PlainEncoder : public FreshEncoder{
public:
    bool isFresh(const string& word) const;
    /* Fresh word is represent by a fresh mark followed by word length, then plain word
     * An empty string is used as word of fresh mark. Its frequncy will be estimated by 
     * words that appears once only. Those word appears once only will be filterd when build
     * huffman tree and will be treated as fresh word when decode.
     * */
    BitArray encode(const string& word) const;
    /* Reach here when we meet a FreshMarkCode. The following shall be an 8 bits number,
     * which is length of the fresh word. Then there are 8 * length of bits, which is 
     * the fresh word in ascii.
     * */
    int decode(BinaryCode& code, string& word) const;
    int decodeFresh(BinaryCode& code, string& word, bool isAuxiliary = false) const;
    void parse(string& dumpStr);
    string dump() const;
private:
};

#endif
