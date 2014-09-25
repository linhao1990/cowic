/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//number_model.cpp

#include "number.h"
#include "number_model.h"

NumberModel::NumberModel() : AbstractNumberModel(){
}

NumberModel::NumberModel(const shared_ptr<DictionaryModel>& dictModelPtrVal) 
    : AbstractNumberModel(dictModelPtrVal){
    }

shared_ptr<AbstractNumber> NumberModel::parseColumnStr(const string& columnStr){
    Number number = Number::parse(columnStr);
    return shared_ptr<AbstractNumber>(new Number(number));
}

string NumberModel::toPlainStr(unsigned int num, const string& before, const string& after) const{
    return Number(num, before, after).to_plain_str();
}
    
string NumberModel::getModelName() const{
    return "Number";
}
