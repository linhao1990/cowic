/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//ip.h

#ifndef _IP_H_
#define _IP_H_

#include <regex>
#include <stdlib.h>
#include <regex.h>

#include "abstract_number.h"

class IP : public AbstractNumber<unsigned int>{
public:
    IP(unsigned int numericVal, const string& beforeVal, const string& afterVal);
    string to_plain_str() const;
    bool isIllegal() const;
    static IP parse(const string& ipStr);
    static const IP illegal;
private:
    static void initRegex();
    static bool regex_initialized;
    static regex_t dot_decimal_regex;
};

#endif
