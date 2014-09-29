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

template <typename T>
const string NumberModel<T>::name = "Number";

template <>
const string NumberModel<unsigned long long>::name = "LongNumber";

template <typename T>
NumberModel<T>::NumberModel() : AbstractNumberModel<T>(){
}

template <typename T>
NumberModel<T>::NumberModel(const shared_ptr<DictionaryModel>& dictModelPtrVal) 
    : AbstractNumberModel<T>(dictModelPtrVal){
    }

template <typename T>
shared_ptr<AbstractNumber<T> > NumberModel<T>::parseColumnStr(const string& columnStr){
    Number<T> number = Number<T>::parse(columnStr);
    return shared_ptr<AbstractNumber<T> >(new Number<T>(number));
}

template <typename T>
string NumberModel<T>::toPlainStr(T num, const string& before, const string& after) const{
    return Number<T>(num, before, after).to_plain_str();
}
    
template <typename T>
string NumberModel<T>::getModelName() const{
    return NumberModel<T>::name;
}

template class NumberModel<unsigned int>;
template class NumberModel<unsigned long long>;
