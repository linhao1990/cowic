/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//fix_precision_number_model.cpp

#include <iostream>

#include "helper.h"
#include "fix_precision_number.h"
#include "fix_precision_number_model.h"
#include "variable_byte_encoder.h"

FixPrecisionNumberModel::FixPrecisionNumberModel() 
    : AbstractNumberModel(), precision(0){
        numberEncoderPtr.reset(new VariableByteEncoder());
}

FixPrecisionNumberModel::FixPrecisionNumberModel(const shared_ptr<DictionaryModel>& dictModelPtrVal, unsigned int precisionVal) 
    : AbstractNumberModel(dictModelPtrVal), precision(precisionVal){
        numberEncoderPtr.reset(new VariableByteEncoder());
}


string FixPrecisionNumberModel::dump() const{
    string baseDumpStr = AbstractNumberModel::dump();
    string precisionStr = int2Str(precision);
    return baseDumpStr + precisionStr ;
}
    
void FixPrecisionNumberModel::parse(string& dumpStr){
    AbstractNumberModel::parse(dumpStr);
    string precisionStr = cutFirstLine(dumpStr);
    precision = atoi(precisionStr.c_str());
}

BitArray FixPrecisionNumberModel::compressNumber(unsigned int num){
    return numberEncoderPtr->encode(num);
}

int FixPrecisionNumberModel::decompressNumber(BinaryCode& code, unsigned int& num) const{
    return numberEncoderPtr->decode(code, num);
}

shared_ptr<AbstractNumber<unsigned int> > FixPrecisionNumberModel::parseColumnStr(const string& columnStr){
    FixPrecisionNumber number = FixPrecisionNumber::parse(columnStr, precision);
    return shared_ptr<AbstractNumber<unsigned int> >(new FixPrecisionNumber(number));
}

string FixPrecisionNumberModel::toPlainStr(unsigned int num, const string& before, const string& after) const{
    return FixPrecisionNumber(num, before, after, precision).to_plain_str();
}
    
string FixPrecisionNumberModel::getModelName() const{
    return "FixPrecisionNumber";
}

