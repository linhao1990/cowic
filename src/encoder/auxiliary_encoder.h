/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//auxiliary_encoder.h

#ifndef _AUXILIARY_ENCODER_H_
#define _AUXILIARY_ENCODER_H_

#include "fresh_encoder.h"
#include "plain_encoder.h"
#include "word_list_encoder.h"

class AuxiliaryEncoder : public FreshEncoder{
public:
    bool isFresh(const string& word) const;
    void updateWord(const string& word);
    void setFreshCode(const BitArray& freshCodeValue, const BitArray& auxiliaryCodeValue);
    BitArray encode(const string& word) const;
    int decode(BinaryCode& code, string& word) const;
    void parse(string& dumpStr);
    /* Format:
     * <#AUXILIARY_ENCODER_BYTE_LEN>
     * <AUXILIARY_ENCODER_DUMP_STR>
     * */
    string dump() const;
    int decodeFresh(BinaryCode& code, string& word, bool isAuxiliary) const;
private:
    BitArray auxiliaryCode;
    shared_ptr<WordListEncoder> wordlistEncoderPtr;
    shared_ptr<PlainEncoder> plainEncoderPtr;
};

#endif
