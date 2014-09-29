/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//abstract_number.h

#ifndef _ABSTRACT_NUMBER_H_
#define _ABSTRACT_NUMBER_H_

#include <string>

using std::string;

template <typename T>
class AbstractNumber{
public:
    AbstractNumber(T numericVal, const string& beforeVal, const string& afterVal);
    T getNumeric() const;
    string getBefore() const;
    string getAfter() const;
    bool operator ==(const AbstractNumber& other) const;
    bool operator !=(const AbstractNumber& other) const;
    virtual string to_plain_str() const;
    virtual bool isIllegal() const = 0;
private:
    T numeric;
    string before;
    string after;
};

#endif
