/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//fix_precision_number_model.h

#ifndef _FIX_PRECISION_NUMBER_MODEL_H_
#define _FIX_PRECISION_NUMBER_MODEL_H_

#include "abstract_number_model.h"
#include "number_encoder.h"

class FixPrecisionNumberModel : public AbstractNumberModel<unsigned int>{
public:
    FixPrecisionNumberModel();
    FixPrecisionNumberModel(const shared_ptr<DictionaryModel>& dictModelPtrVal, unsigned int precisionVal);
    string dump() const;
    void parse(string& dumpStr);
    BitArray compressNumber(unsigned int num);
    int decompressNumber(BinaryCode& code, unsigned int& num) const;
    shared_ptr<AbstractNumber<unsigned int> > parseColumnStr(const string& columnStr);
    string toPlainStr(unsigned int num, const string& before, const string& after) const;
    string getModelName() const;
private:
    unsigned int precision;

    shared_ptr<NumberEncoder> numberEncoderPtr;
};

#endif
