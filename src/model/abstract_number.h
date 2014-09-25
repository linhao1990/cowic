/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * HOMEPAGE_URL
 */
//abstract_number.h

#ifndef _ABSTRACT_NUMBER_H_
#define _ABSTRACT_NUMBER_H_

#include <string>

using std::string;

class AbstractNumber{
public:
    AbstractNumber(unsigned int numericVal, const string& beforeVal, const string& afterVal);
    unsigned int getNumeric() const;
    string getBefore() const;
    string getAfter() const;
    bool operator ==(const AbstractNumber& other) const;
    bool operator !=(const AbstractNumber& other) const;
    virtual string to_plain_str() const;
    virtual bool isIllegal() const = 0;
private:
    unsigned int numeric;
    string before;
    string after;
};

#endif
