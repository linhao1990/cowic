/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//phrase_dict_model.h

#ifndef _PHRASE_DICT_MODEL_H_
#define _PHRASE_DICT_MODEL_H_

#include "dictionary_model.h"

class PhraseDictModel : public DictionaryModel{
public:
    PhraseDictModel();
    bool isWordInDict(const string& word) const;
    bool isWordInFreshEncoder(const string& word) const;
    void updateColumn(const string& columnStr, int column);
    BitArray compressColumn(const string& columnStr, int column); 
    string getModelName() const;
private:
    void findLongestMatch(const vector<string>& words, vector<string>::const_iterator& wordIt, string& match, bool (PhraseDictModel::*checkWordExistFunc)(const string&) const);
};

#endif
