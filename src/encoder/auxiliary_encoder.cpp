/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//auxiliary_encoder.cpp

#include <sstream>

#include "helper.h"
#include "auxiliary_encoder.h"

bool AuxiliaryEncoder::isFresh(const string& word) const{
    return wordlistEncoderPtr->isFresh(word);
}

void AuxiliaryEncoder::updateWord(const string& word){
    wordlistEncoderPtr->updateThenCheckExist(word);
}

void AuxiliaryEncoder::setFreshCode(const BitArray& freshCodeValue, const BitArray& auxiliaryCodeValue){
    auxiliaryCode = auxiliaryCodeValue;
    plainEncoderPtr.reset(new PlainEncoder());
    plainEncoderPtr->setFreshCode(freshCodeValue, auxiliaryCodeValue);
}

BitArray AuxiliaryEncoder::encode(const string& word) const{
    bool exist = wordlistEncoderPtr->updateThenCheckExist(word);
    if(exist){
        BitArray indexCode = wordlistEncoderPtr->encode(word);
        return auxiliaryCode + indexCode;
    } else{
        return plainEncoderPtr->encode(word);
    }
}

int AuxiliaryEncoder::decode(BinaryCode& code, string& word) const{
    // shall not reach here.
    return -1;
}

int AuxiliaryEncoder::decodeFresh(BinaryCode& code, string& word, bool isAuxiliary) const{
    if(isAuxiliary){
        return wordlistEncoderPtr->decode(code, word);
    } else{
        return plainEncoderPtr->decodeFresh(code, word);
    }
}

void parseSubStr2Auxiliary(string& dumpStr, int encoderByteLen, shared_ptr<WordListEncoder>& encoderPtr){
    string filename = cutNByte(dumpStr, encoderByteLen);
    encoderPtr.reset(new WordListEncoder(filename));
}

void AuxiliaryEncoder::parse(string& dumpStr){
    int auxiliaryByteLen = parseFirstLine2Int(dumpStr);
    parseSubStr2Auxiliary(dumpStr, auxiliaryByteLen, wordlistEncoderPtr);
}

string AuxiliaryEncoder::dump() const{
    string auxiliaryDumpStr = AuxiliaryFilename::getNextFilename();

    std::stringstream sstream;
    sstream << auxiliaryDumpStr.size() << "\n"
            << auxiliaryDumpStr;
    return sstream.str();
}
