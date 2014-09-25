/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * HOMEPAGE_URL
 */
//abstract_number.cpp

#include <sstream>

#include "abstract_number.h"

AbstractNumber::AbstractNumber(unsigned int numericVal, const string& beforeVal, const string& afterVal) 
    : numeric(numericVal), before(beforeVal), after(afterVal){
}

unsigned int AbstractNumber::getNumeric() const{
    return numeric;
}

string AbstractNumber::getBefore() const{
    return before;
}

string AbstractNumber::getAfter() const{
    return after;
}

string AbstractNumber::to_plain_str() const{
    std::stringstream sstream;
    sstream << getBefore();

    sstream << getNumeric();

    sstream << getAfter();
    return sstream.str();
}

bool AbstractNumber::operator ==(const AbstractNumber& other) const{
    return getNumeric() == other.getNumeric();
}

bool AbstractNumber::operator !=(const AbstractNumber& other) const{
    return !(this->operator== (other));
}
