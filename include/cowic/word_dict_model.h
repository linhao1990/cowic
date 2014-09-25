//word_dict_model.h

#ifndef _WORD_DICT_MODEL_H_
#define _WORD_DICT_MODEL_H_

#include "dictionary_model.h"

class WordDictModel : public DictionaryModel{
public:
    WordDictModel();
    void updateColumn(const string& columnStr, int column);
    BitArray compressColumn(const string& columnStr, int column); 
    string getModelName() const;
};

#endif
