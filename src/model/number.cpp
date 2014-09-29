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
template <typename T>
const Number<T> Number<T>::illegal(0, "", "");
const int cflags = REG_EXTENDED;
template <typename T>
bool Number<T>::regex_initialized = false;
template <typename T>
regex_t Number<T>::decimal_regex;

template <typename T>
Number<T>::Number(T numericVal, const string& beforeVal, const string& afterVal) 
    : AbstractNumber<T>(numericVal, beforeVal, afterVal){
}

template <typename T>
void Number<T>::initRegex(){
    if(!Number<T>::regex_initialized){
        Number<T>::regex_initialized = true;
        regcomp(&Number<T>::decimal_regex, decimal_str.c_str(), cflags);
    }
}

template <typename T>
Number<T> Number<T>::parse(const string& numberStr){
    initRegex();
    
    regmatch_t pmatch[1];
    size_t nmatch = 1;
    int status = regexec(&Number<T>::decimal_regex, numberStr.c_str(), nmatch, pmatch, 0);
    if(status == 0){
        unsigned int num = 0;
        string section = numberStr.substr(pmatch[0].rm_so, pmatch[0].rm_eo - pmatch[0].rm_so);
        std::stringstream sstream(section);
        sstream >> num;
        string before = numberStr.substr(0, pmatch[0].rm_so);
        string after = numberStr.substr(pmatch[0].rm_eo);
        return Number<T>(num, before, after);
    }

    return Number<T>::illegal;
}

template <typename T>
bool Number<T>::isIllegal() const{
    return this->operator==(Number<T>::illegal); 
}

template class Number<unsigned int>;
template class Number<unsigned long long>;
