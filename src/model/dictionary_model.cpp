/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//dictionary_model.cpp

#include <sstream>

#include "dictionary_model.h"
#include "compressor_factory.h"
#include "helper.h"
#include "spliter.h"

const int DictionaryModel::Threshold = 2;

const unsigned char freshChar = 129;
const unsigned char auxiliaryChar = 130;
const string freshMark(1, freshChar);
const string auxiliaryMark(1, auxiliaryChar);

DictionaryModel::DictionaryModel() : Model(){
}

void DictionaryModel::updateWord(const string& word){
    word2FreqDict[word] += 1;
}

int filterLowFreqWord(const unordered_map<string, int>& word2FreqDict, unordered_map<string, int>& filteredDict){
    int lowFreqCount = 0;
    for(auto it = word2FreqDict.begin(); it != word2FreqDict.end(); it++){
        if(DictionaryModel::isLowFreqWord(it->second)){
            lowFreqCount++;
        } else{
            filteredDict[it->first] = it->second;
        }
    }
    return lowFreqCount;
}

inline void pushFreshWord(unordered_map<string, int>& word2FreqDict, int freshFreq){
    word2FreqDict[freshMark] = freshFreq;
    word2FreqDict[auxiliaryMark] = freshFreq;
}

string dumpEncoder(const shared_ptr<Encoder>& encoderPtr, const unordered_map<string, int>& word2FreqDict){
    if(encoderPtr == NULL){
        // happens in train stage
        unordered_map<string, int> filteredDict;
        int freshFreq = filterLowFreqWord(word2FreqDict, filteredDict);
        pushFreshWord(filteredDict, freshFreq);
        shared_ptr<Encoder> tmp = CompressorFactory::createDictEncoder();
        tmp->buildCoder(filteredDict);
        return tmp->dump();
    } 
    return encoderPtr->dump();
}

string dumpFreshEncoder(const shared_ptr<FreshEncoder>& freshEncoderPtr){
    if(freshEncoderPtr == NULL){
        shared_ptr<FreshEncoder> tmp = CompressorFactory::createFreshEncoder();
        return tmp->dump();
    }
    return freshEncoderPtr->dump();
}

string dumpDict(const unordered_map<string, int>& dict){
    string ret = "";
    for(auto it = dict.begin(); it != dict.end(); ++it){
        if(!DictionaryModel::isLowFreqWord(it->second)){
            std::stringstream sstream;
            sstream << it->first << " " << it->second << "\n";
            ret += sstream.str();
        }
    }
    return ret;
}

string DictionaryModel::dump() const{
    string dictDumpStr = dumpDict(word2FreqDict);
    string encoderDumpStr = dumpEncoder(encoderPtr, word2FreqDict);
    string freshEncoderDumpStr = dumpFreshEncoder(freshEncoderPtr);

    std::stringstream sstream;
    sstream << getModelName() << "\n" 
            << dictDumpStr.size() << "\n" 
            << dictDumpStr 
            << encoderDumpStr.size() << "\n"
            << encoderDumpStr
            << freshEncoderDumpStr.size() << "\n"
            << freshEncoderDumpStr;
    return sstream.str();
}

void parseSubStr2Dict(string& dumpStr, int dictByteLen, unordered_map<string, int>& dict){
    string dictStr = cutNByte(dumpStr, dictByteLen);
    parseDict(dictStr, dict);
}

void parseSubStr2Encoder(string& dumpStr, int encoderByteLen, shared_ptr<Encoder>& encoderPtr){
    string encoderStr = cutNByte(dumpStr, encoderByteLen);
    encoderPtr = CompressorFactory::createDictEncoder();
    encoderPtr->parse(encoderStr);
}

void parseSubStr2FreshEncoder(string& dumpStr, int freshEncoderByteLen, shared_ptr<FreshEncoder>& freshEncoderPtr){
    string encoderStr = cutNByte(dumpStr, freshEncoderByteLen);
    freshEncoderPtr = CompressorFactory::createFreshEncoder();
    freshEncoderPtr->parse(encoderStr);
}

void DictionaryModel::parse(string& dumpStr){
    int dictByteLen = parseFirstLine2Int(dumpStr);
    parseSubStr2Dict(dumpStr, dictByteLen, word2FreqDict);
    int encoderByteLen = parseFirstLine2Int(dumpStr);
    parseSubStr2Encoder(dumpStr, encoderByteLen, encoderPtr);
    // set fresh mark after parse huffman encoder
    BitArray freshMarkCode = encoderPtr->encode(freshMark);
    BitArray auxiliaryMarkCode = encoderPtr->encode(auxiliaryMark);
    int freshEncoderByteLen = parseFirstLine2Int(dumpStr);
    parseSubStr2FreshEncoder(dumpStr, freshEncoderByteLen, freshEncoderPtr);
    freshEncoderPtr->setFreshCode(freshMarkCode, auxiliaryMarkCode);
}

bool DictionaryModel::isWordInEncoder(const string& word) const{
    return !(encoderPtr->isFresh(word));
}

BitArray DictionaryModel::compressWord(const string& word){
    //updateWord(word);
    if(!isWordInEncoder(word)){
        //cout<<"<F:"<<word<<":"<<freshEncoderPtr->encode(word).size()<<">";
        return freshEncoderPtr->encode(word);
    }
    //cout<<"<M:"<<word<<":"<<encoderPtr->encode(word).size()<<">";
    return encoderPtr->encode(word); 
}

bool isAuxiliaryMark(const string& word){
    return word == auxiliaryMark;
}

bool isFreshMark(const string& word){
    return word == freshMark;
}

int DictionaryModel::decompressWord(BinaryCode& code, string& word) const{
    int ret = encoderPtr->decode(code, word);
    if (ret < 0)
        return ret;
    if(isAuxiliaryMark(word))
        return freshEncoderPtr->decodeFresh(code, word, true);
    else if(isFreshMark(word))
        return freshEncoderPtr->decodeFresh(code, word, false);
    else
        return ret;
}

int DictionaryModel::decompressColumn(BinaryCode& code, string& columnStr, int column) const{
    string word;
    size_t lastOpenPos = string::npos;
    while(!code.finish()){
        int state = decompressWord(code, word);
        if (state < 0)
            return state;
        columnStr += word;
        if (Spliter::isNewColumn(lastOpenPos, word)){
            return 0;
        }
    }
    //succ decode all code. the last column
    return 0;
}

int DictionaryModel::getFreq(const string& word) const{
    unordered_map<string, int>::const_iterator it = word2FreqDict.find(word);
    return it->second;
}
