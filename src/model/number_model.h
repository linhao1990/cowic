/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//number_model.h

#ifndef _NUMBER_MODEL_H_
#define _NUMBER_MODEL_H_

#include "abstract_number_model.h"

template <typename T>
class NumberModel : public AbstractNumberModel<T>{
public:
    NumberModel();
    NumberModel(const shared_ptr<DictionaryModel>& dictModelPtrVal);
    shared_ptr<AbstractNumber<T> > parseColumnStr(const string& columnStr);
    string toPlainStr(T num, const string& before, const string& after) const;
    string getModelName() const;
private:
    static const string name;
};

#endif
