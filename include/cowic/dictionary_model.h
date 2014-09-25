//dictionary_model.h
#ifndef _DICTIONARY_MODEL_H
#define _DICTIONARY_MODEL_H

#include <memory>

#include "model.h"
#include "encoder.h"
#include "fresh_encoder.h"

using std::shared_ptr;

class DictionaryModel : public Model{
public:
    DictionaryModel();
    void updateWord(const string& word);
    /* Dump model into string.
     * Format:
     * <MODEL_NAME>
     * <#DICT_BYTE_LEN>
     * <WORD1> <#FREQ1>
     * ...
     * <WORDM> <#FREQM>
     * <#ENCODER_BYTE_LEN>
     * <ENCODER_DUMP_STR>
     * <#FRESH_ENCODER_BYTE_LEN>
     * <FRESH_ENCODER_DUMP_STR>
     * Explanation:
     * The first line is an integer denote dictionary length in bytes, the 
     * following #DICT_BYTE_LEN bytes are dictionary string. After the dictionary string is another
     * integer denote encoder length in byte and the following #ENCODER_BYTE_LEN bytes are encoder.
     * For the dictionary, each word info is stored in s seperate line,
     * each line contains a word, a space and an frequncy.
     * */
    string dump() const;
    void parse(string& dumpStr);
    bool isWordInEncoder(const string& word) const;
    BitArray compressWord(const string& word); 
    int decompressWord(BinaryCode& code, string& word) const;
    int decompressColumn(BinaryCode& code, string& columnStr, int column) const;
    int getFreq(const string& word) const;
protected:
    unordered_map<string, int> word2FreqDict;
    shared_ptr<Encoder> encoderPtr;
    shared_ptr<FreshEncoder> freshEncoderPtr;
};

inline bool isLowFreqWord(int freq){
    return freq <= 1;
}

#endif
