/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//abstract_number.cpp

#include <sstream>

#include "abstract_number.h"

template <typename T>
AbstractNumber<T>::AbstractNumber(T numericVal, const string& beforeVal, const string& afterVal) 
    : numeric(numericVal), before(beforeVal), after(afterVal){
}

template <typename T>
T AbstractNumber<T>::getNumeric() const{
    return numeric;
}

template <typename T>
string AbstractNumber<T>::getBefore() const{
    return before;
}

template <typename T>
string AbstractNumber<T>::getAfter() const{
    return after;
}

template <typename T>
string AbstractNumber<T>::to_plain_str() const{
    std::stringstream sstream;
    sstream << getBefore();

    sstream << getNumeric();

    sstream << getAfter();
    return sstream.str();
}

template <typename T>
bool AbstractNumber<T>::operator ==(const AbstractNumber& other) const{
    return getNumeric() == other.getNumeric();
}

template <typename T>
bool AbstractNumber<T>::operator !=(const AbstractNumber& other) const{
    return !(this->operator== (other));
}

template class AbstractNumber<unsigned int>; 
template class AbstractNumber<unsigned long long>; 
