//abstract_number_model.h

#ifndef _ABSTRACT_NUMBER_MODEL_H_
#define _ABSTRACT_NUMBER_MODEL_H_

#include <memory>

#include "abstract_number.h"
#include "model.h"
#include "dictionary_model.h"
#include "huffman_encoder.h"

using std::shared_ptr;

class AbstractNumberModel : public Model{
public:
    AbstractNumberModel();
    AbstractNumberModel(const shared_ptr<DictionaryModel>& dictModelPtrVal);
    /* <MODEL_NAME>
     * <#MODEL_BYTE_LEN>
     * <MODEL_DUMP_STR>
     * */
    virtual string dump() const;
    virtual void parse(string& dumpStr);
    void updateColumn(const string& columnStr, int column);
    BitArray compressColumn(const string& columnStr, int column);
    int decompressColumn(BinaryCode& code, string& columnStr, int column) const;
    virtual BitArray compressNumber(unsigned int num);
    virtual int decompressNumber(BinaryCode& code, unsigned int& num) const;
    virtual shared_ptr<AbstractNumber> parseColumnStr(const string& columnStr) = 0;
    virtual string toPlainStr(unsigned int num, const string& before, const string& after) const = 0;
    static int legalBit;
    static int beforeBit;
    static int afterBit;
protected:
    /* The model used to process string when the timestamp cannot be parsed successfully.
     */
    shared_ptr<DictionaryModel> dictModelPtr;
    BitArray compressBeforeAfter(const string& beforeAfter);
    int decompressBeforeAfter(BinaryCode& code, string& beforeAfter) const;
    /* The model used to encode illegal bit, before bit, after bit.
     * */
    unordered_map<int, int> flag2FreqDict;
    shared_ptr<Encoder> flagEncoderPtr;
    void initFlag2FreqDict();
    string dumpFlagEncoder() const;
};

#endif
