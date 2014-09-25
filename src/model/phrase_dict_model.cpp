/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//phrase_dict_model.cpp

#include "spliter.h"
#include "phrase_dict_model.h"

PhraseDictModel::PhraseDictModel() : DictionaryModel(){
}

bool PhraseDictModel::isWordInDict(const string& word) const{
    return !(word2FreqDict.find(word) == word2FreqDict.end());
}

bool PhraseDictModel::isWordInFreshEncoder(const string& word) const{
    return !(freshEncoderPtr->isFresh(word));
}

void PhraseDictModel::findLongestMatch(const vector<string>& words, vector<string>::const_iterator& wordIt, string& match, bool (PhraseDictModel::*checkWordExistFunc)(const string&) const){
    match = "";
    while(wordIt != words.end() && (this->*checkWordExistFunc)(match + (*wordIt))){
        match += *wordIt;
        ++wordIt;
    }
}

void PhraseDictModel::updateColumn(const string& columnStr, int column){
    vector<string> words;
    Spliter::splitColumn2Words(columnStr, words);
    vector<string>::const_iterator wordIt = words.begin();
    string match;
    while(wordIt != words.end()){
        findLongestMatch(words, wordIt, match, &PhraseDictModel::isWordInDict);
        if(match != ""){
            updateWord(match);
            if(wordIt != words.end())
                updateWord(match + (*wordIt));
        } else{
            updateWord(*wordIt);
            wordIt++;
        }
    }
}

BitArray PhraseDictModel::compressColumn(const string& columnStr, int column){
    BitArray ret;
    vector<string> words;
    Spliter::splitColumn2Words(columnStr, words);
    vector<string>::const_iterator wordIt = words.begin();
    string match = "";
    while(wordIt != words.end()){
        findLongestMatch(words, wordIt, match, &PhraseDictModel::isWordInEncoder);
        if(!match.empty()){
            //cout<<"<M:"<<match<<":"<<code.size()<<">";
            ret += compressWord(match); 
        } else{
            // try to find in fresh encoder
            findLongestMatch(words, wordIt, match, &PhraseDictModel::isWordInFreshEncoder);
            if(match.empty()){
                match = *wordIt;
                ++wordIt;
            } else{
                // update
                if(wordIt != words.end())
                    freshEncoderPtr->updateWord(match + *wordIt);
            }
            //cout<<"<F:"<<match<<":"<<code.size()<<">";
            ret += freshEncoderPtr->encode(match);
        }
    }
    return ret;
}

string PhraseDictModel::getModelName() const{
    return "PhraseDict";
}
