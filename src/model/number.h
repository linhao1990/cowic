/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * HOMEPAGE_URL
 */
//number.h

#ifndef _NUMBER_H_
#define _NUMBER_H_

#include <regex.h>

#include "abstract_number.h"

class Number : public AbstractNumber{
public:
    Number(unsigned int numericVal, const string& beforeVal, const string& afterVal);
    bool isIllegal() const;
    static Number parse(const string& ipStr);
    static const Number illegal;
private:
    static void initRegex();
    static bool regex_initialized;
    static regex_t decimal_regex;
};

#endif
