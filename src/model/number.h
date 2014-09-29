/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//number.h

#ifndef _NUMBER_H_
#define _NUMBER_H_

#include <regex.h>

#include "abstract_number.h"

template <typename T>
class Number : public AbstractNumber<T>{
public:
    Number(T numericVal, const string& beforeVal, const string& afterVal);
    bool isIllegal() const;
    static Number parse(const string& ipStr);
    static const Number illegal;
private:
    static void initRegex();
    static bool regex_initialized;
    static regex_t decimal_regex;
};

#endif
