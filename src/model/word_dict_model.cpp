/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//word_dict_model.cpp

#include "spliter.h"
#include "word_dict_model.h"

WordDictModel::WordDictModel() : DictionaryModel(){
}

void WordDictModel::updateColumn(const string& columnStr, int column){
    vector<string> words;
    Spliter::splitColumn2Words(columnStr, words);
    for(vector<string>::const_iterator wordIt = words.begin(); wordIt != words.end(); wordIt++){
        updateWord(*wordIt);
    }
}

BitArray WordDictModel::compressColumn(const string& columnStr, int column){
    BitArray ret;
    vector<string> words;
    Spliter::splitColumn2Words(columnStr, words);
    for(vector<string>::const_iterator wordIt = words.begin(); wordIt != words.end(); wordIt++){
        BitArray wordCode = compressWord(*wordIt);
        ret += wordCode;
    }
    return ret;
}

string WordDictModel::getModelName() const{
    return "WordDict";
}
