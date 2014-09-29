/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//abstract_model.cpp

#include <sstream>

#include "helper.h"
#include "abstract_number_model.h"
#include "compressor_factory.h"

template <typename T>
const int AbstractNumberModel<T>::legalBit = 0x1;
template <typename T>
const int AbstractNumberModel<T>::beforeBit = 0x2;
template <typename T>
const int AbstractNumberModel<T>::afterBit = 0x4;

template <typename T>
AbstractNumberModel<T>::AbstractNumberModel() : Model(), dictModelPtr(){
    initFlag2FreqDict();
}

template <typename T>
AbstractNumberModel<T>::AbstractNumberModel(const shared_ptr<DictionaryModel>& dictModelPtrVal) 
    : Model(), dictModelPtr(dictModelPtrVal){
    initFlag2FreqDict();
}

template <typename T>
void AbstractNumberModel<T>::initFlag2FreqDict(){
    for(int i = 0; i < 8; i++)
        flag2FreqDict[i] = 1;
}

template <typename T>
string AbstractNumberModel<T>::dump() const{
    std::stringstream sstream;
    string modelDumpStr = dictModelPtr->dump();
    string encoderDumpStr = dumpFlagEncoder();
    sstream << getModelName() << "\n" 
            << encoderDumpStr.size() << "\n"
            << encoderDumpStr
            << modelDumpStr.size() << "\n"
            << modelDumpStr;
    return sstream.str();
}

template <typename T>
void AbstractNumberModel<T>::parse(string& dumpStr){
    int encoderByteLen = parseFirstLine2Int(dumpStr);
    string encoderDumpStr = cutNByte(dumpStr, encoderByteLen);
    //TODO  parse huffman encoder name 
    //string encoderName = cutFirstLine(encoderDumpStr);
    flagEncoderPtr = CompressorFactory::createDictEncoder();
    flagEncoderPtr->parse(encoderDumpStr);

    int modelByteLen = parseFirstLine2Int(dumpStr);
    string modelDumpStr = cutNByte(dumpStr, modelByteLen);
    string dictModelName = cutFirstLine(modelDumpStr);
    dictModelPtr = CompressorFactory::createDictColumnModel(dictModelName);
    dictModelPtr->parse(modelDumpStr);
} 

template <typename T>
BitArray AbstractNumberModel<T>::compressBeforeAfter(const string& beforeAfter){
    BitArray markCode;
    if(beforeAfter.size() > 0){
        markCode.push_back(true);
        return markCode + dictModelPtr->compressWord(beforeAfter);
    } else{
        markCode.push_back(false);
        return markCode;
    }
}

template <typename T>
int AbstractNumberModel<T>::decompressBeforeAfter(BinaryCode& code, string& beforeAfter) const{
    if(code.finish())
        return -1;
    bool mark = code.getFirstBitMoveToNext();
    if(mark){
        return dictModelPtr->decompressWord(code, beforeAfter);
    } else{
        beforeAfter = "";
        return 0;
    }
}

template <typename T>
void AbstractNumberModel<T>::updateColumn(const string& columnStr, int column){
    // Try to parse ip 
    shared_ptr<AbstractNumber<T> > ptr = parseColumnStr(columnStr);
    int flag = 0;
    // Dispatch to dictModelPtr when fail
    if(ptr->isIllegal()){
        dictModelPtr->updateColumn(columnStr, column);
    } else{
        flag |= legalBit;
        // update before 
        if(ptr->getBefore().size() > 0){
            dictModelPtr->updateWord(ptr->getBefore());
            flag |= beforeBit;
        }
        if(ptr->getAfter().size() > 0){
            dictModelPtr->updateWord(ptr->getAfter());
            flag |= afterBit;
        }
    }
    flag2FreqDict[flag] += 1;
}

template <typename T>
BitArray AbstractNumberModel<T>::compressNumber(unsigned int num){
    return BitArray(num);
}

//TODO: if compress column by dictionaryModel is shorter than ip,
// e.g. the ip appears in the train stage. then we can treat it as illegal
// and compress it by dictionaryModel
template <typename T>
BitArray AbstractNumberModel<T>::compressColumn(const string& columnStr, int column){
    // Try to parse number, 
    shared_ptr<AbstractNumber<T> > ptr = parseColumnStr(columnStr);
    int flag = 0;
    if(ptr->isIllegal()){
        // Dispatch to dictModelPtr when fail
        BitArray flagCode = flagEncoderPtr->encode(int2Str(flag));
        return flagCode + dictModelPtr->compressWord(columnStr);
    } else{
        flag |= legalBit;
        BitArray numCode = compressNumber(ptr->getNumeric());
        BitArray beforeCode;
        if(!ptr->getBefore().empty()){
            flag |= beforeBit;
            beforeCode = dictModelPtr->compressWord(ptr->getBefore());
        }
        BitArray afterCode;
        if(!ptr->getAfter().empty()){
            flag |= afterBit;
            afterCode = dictModelPtr->compressWord(ptr->getAfter());
        }
        BitArray flagCode = flagEncoderPtr->encode(int2Str(flag));
        return  flagCode + numCode + beforeCode + afterCode;
    }
}

template <typename T>
int AbstractNumberModel<T>::decompressNumber(BinaryCode& code, unsigned int& num) const{
    int ret = decodeInt(code, num);
    if(ret < 0)
        return ret;
    return 0;
}

template <typename T>
int AbstractNumberModel<T>::decompressColumn(BinaryCode& code, string& columnStr, int column) const{
    string flagStr;
    int ret = flagEncoderPtr->decode(code, flagStr);
    if(ret < -1)
        return ret;
    int flag = atoi(flagStr.c_str());

    if(flag & legalBit){
        unsigned int num;
        decompressNumber(code, num);

        string before;
        if(flag & beforeBit)
            dictModelPtr->decompressWord(code, before);

        string after;
        int ret = 0;
        if(flag & afterBit)
            ret = dictModelPtr->decompressWord(code, after);

        columnStr = toPlainStr(num, before, after);
        return ret;
    } else{
        // illegal, 
        return dictModelPtr->decompressColumn(code, columnStr, column);
    }
}

template <typename T>
string AbstractNumberModel<T>::dumpFlagEncoder() const{
    if(flagEncoderPtr == NULL){
        // in train stage
        unordered_map<string, int> dict;
        for(auto it = flag2FreqDict.begin(); it != flag2FreqDict.end(); it++)
            dict[int2Str(it->first)] = it->second;
        shared_ptr<Encoder> tmp = CompressorFactory::createDictEncoder();
        tmp->buildCoder(dict);
        return tmp->dump();
    }
    return flagEncoderPtr->dump();
}

template class AbstractNumberModel<unsigned int>; 
template class AbstractNumberModel<unsigned long long>; 
