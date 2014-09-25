//ip_model.h

#ifndef _IP_MODEL_H_
#define _IP_MODEL_H_

#include "abstract_number_model.h"

class IPModel : public AbstractNumberModel{
public:
    IPModel();
    IPModel(const shared_ptr<DictionaryModel>& dictModelPtrVal);
    shared_ptr<AbstractNumber> parseColumnStr(const string& columnStr);
    string toPlainStr(unsigned int num, const string& before, const string& after) const;
    string getModelName() const;
private:
};

#endif
