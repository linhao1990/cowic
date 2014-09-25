/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//fix_precision_number.cpp

#include <sstream>
#include <iostream>

#include "fix_precision_number.h"
#include "helper.h"

const string decimal_str = "[0-9]+\\.[0-9]";
const FixPrecisionNumber FixPrecisionNumber::illegal(0, "", "", 0);
const int cflags = REG_EXTENDED;
int FixPrecisionNumber::regex_initialized = -1;
regex_t FixPrecisionNumber::decimal_regex;

FixPrecisionNumber::FixPrecisionNumber(unsigned int numericVal, const string& beforeVal, const string& afterVal, unsigned int precisionVal) 
    : AbstractNumber(numericVal, beforeVal, afterVal), precision(precisionVal){
}

void FixPrecisionNumber::initRegex(unsigned int precision){
    if(FixPrecisionNumber::regex_initialized != precision){
        FixPrecisionNumber::regex_initialized = precision;
        string precision_decimal_str = decimal_str + "{" + int2Str(precision) + "}";
        regcomp(&FixPrecisionNumber::decimal_regex, precision_decimal_str.c_str(), cflags);
    }
}

FixPrecisionNumber FixPrecisionNumber::parse(const string& numberStr, unsigned int precision){
    initRegex(precision);
    
    regmatch_t pmatch[1];
    size_t nmatch = 1;
    int status = regexec(&FixPrecisionNumber::decimal_regex, numberStr.c_str(), nmatch, pmatch, 0);
    if(status == 0){
        unsigned int num = 0;
        string section = numberStr.substr(pmatch[0].rm_so, pmatch[0].rm_eo - pmatch[0].rm_so);
        section = section.erase(section.find('.'), 1);
        std::stringstream sstream(section);
        sstream >> num;
        string before = numberStr.substr(0, pmatch[0].rm_so);
        string after = numberStr.substr(pmatch[0].rm_eo);
        return FixPrecisionNumber(num, before, after, precision);
    }

    return FixPrecisionNumber::illegal;
}

string FixPrecisionNumber::to_plain_str() const{
    std::stringstream sstream;
    sstream << getBefore();

    unsigned int num = getNumeric();
    unsigned int divisor = 1;
    for(int i = 0; i < precision; i++)
        divisor *= 10;
    unsigned int quotient = num / divisor;
    unsigned int remainder = num % divisor; 
    sstream << quotient;
    sstream << ".";

    while(divisor > 10 && divisor / 10 > remainder){
        sstream << "0";
        divisor /= 10;
    }
    sstream<<remainder;

    sstream << getAfter();
    return sstream.str();
}

bool FixPrecisionNumber::isIllegal() const{
    return this->operator==(FixPrecisionNumber::illegal); 
}

bool FixPrecisionNumber::operator ==(const FixPrecisionNumber& other) const{
    return getNumeric() == other.getNumeric() && precision == other.precision;
}

bool FixPrecisionNumber::operator !=(const FixPrecisionNumber& other) const{
    return !(this->operator== (other));
}
