/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//number.cpp

#include <sstream>

#include "number.h"

const string decimal_str = "[1-9][0-9]*|0";
const Number Number::illegal(0, "", "");
const int cflags = REG_EXTENDED;
bool Number::regex_initialized = false;
regex_t Number::decimal_regex;

Number::Number(unsigned int numericVal, const string& beforeVal, const string& afterVal) 
    : AbstractNumber(numericVal, beforeVal, afterVal){
}

void Number::initRegex(){
    if(!Number::regex_initialized){
        Number::regex_initialized = true;
        regcomp(&Number::decimal_regex, decimal_str.c_str(), cflags);
    }
}

Number Number::parse(const string& numberStr){
    initRegex();
    
    regmatch_t pmatch[1];
    size_t nmatch = 1;
    int status = regexec(&Number::decimal_regex, numberStr.c_str(), nmatch, pmatch, 0);
    if(status == 0){
        unsigned int num = 0;
        string section = numberStr.substr(pmatch[0].rm_so, pmatch[0].rm_eo - pmatch[0].rm_so);
        std::stringstream sstream(section);
        sstream >> num;
        string before = numberStr.substr(0, pmatch[0].rm_so);
        string after = numberStr.substr(pmatch[0].rm_eo);
        return Number(num, before, after);
    }

    return Number::illegal;
}

bool Number::isIllegal() const{
    return this->operator==(Number::illegal); 
}
