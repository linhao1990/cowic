/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//ip.cpp

#include <stdio.h>

#include "ip.h"

const string decimal_str = "([1-2]?[0-9]?[0-9])";
const string dot_str = "\\.";
const string dot_decimal_str = decimal_str + dot_str + decimal_str + dot_str + decimal_str + dot_str + decimal_str;
const IP IP::illegal(0, "", "");
const int cflags = REG_EXTENDED;
bool IP::regex_initialized = false;
regex_t IP::dot_decimal_regex;

IP::IP(unsigned int numericVal, const string& beforeVal, const string& afterVal) 
    : AbstractNumber(numericVal, beforeVal, afterVal){
}

static char buf[32];
static int part[4];
string IP::to_plain_str() const{
    unsigned int temp = getNumeric();
    for(int i = 0; i < 3; i++){
        part[i] = ((temp & 0xFF000000) >> 24);
        temp = temp << 8;
    }
    part[3] = ((temp & 0xFF000000) >> 24);
    sprintf(buf, "%s%d.%d.%d.%d%s", getBefore().c_str(), part[0], part[1], part[2], part[3], getAfter().c_str());
    return string(buf);
}

void IP::initRegex(){
    if(!IP::regex_initialized){
        IP::regex_initialized = true;
        regcomp(&IP::dot_decimal_regex, dot_decimal_str.c_str(), cflags);
    }
}

IP IP::parse(const string& ipStr){
    initRegex();
    
    regmatch_t pmatch[5];
    size_t nmatch = 5;
    int status = regexec(&IP::dot_decimal_regex, ipStr.c_str(), nmatch, pmatch, 0);
    if(status == 0){
        unsigned int num = 0;
        for(int i = 1 ; i < 5; i++){
            string section = ipStr.substr(pmatch[i].rm_so, pmatch[i].rm_eo - pmatch[i].rm_so);
            if(section.length() > 1 && section[0] == '0')
                return IP::illegal;
            num = (num << 8) + atoi(section.c_str());
        }
        string before = ipStr.substr(0, pmatch[0].rm_so);
        string after = ipStr.substr(pmatch[0].rm_eo);
        return IP(num, before, after);
    }

    return IP::illegal;
}

bool IP::isIllegal() const{
    return this->operator==(IP::illegal); 
}
