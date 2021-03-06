//fix_precision_number.h

#ifndef _FIX_PRECISION_NUMBER_H_
#define _FIX_PRECISION_NUMBER_H_

#include <regex.h>

#include "abstract_number.h"

class FixPrecisionNumber : public AbstractNumber{
public:
    FixPrecisionNumber(unsigned int numericVal, const string& beforeVal, const string& afterVal, const unsigned int precisionVal);
    bool isIllegal() const;
    static FixPrecisionNumber parse(const string& ipStr, unsigned int precision);
    string to_plain_str() const;
    bool operator ==(const FixPrecisionNumber& other) const;
    bool operator !=(const FixPrecisionNumber& other) const;
    static const FixPrecisionNumber illegal;
private:
    unsigned int precision;

    static void initRegex(unsigned int precision);
    static int regex_initialized;
    static regex_t decimal_regex;
};

#endif
