/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//canonical_huffman_encoder.cpp

#include <algorithm>
#include "canonical_huffman_encoder.h"
#include "helper.h"

string SymbolLen::dump() const{
    return symbol + " " + int2Str(codeLen);
}

SymbolLen SymbolLen::parse(const string& dumpStr){
    size_t pos = dumpStr.find_last_of(' ');
    string symbolVal = dumpStr.substr(0, pos);
    int codeLenVal = atoi(dumpStr.substr(pos + 1).c_str());
    return SymbolLen(symbolVal, codeLenVal);
}

bool SymbolLenGreaterThan(const SymbolLen& left, const SymbolLen& right) { 
    if(left.codeLen > right.codeLen)
        return true;
    if(left.codeLen < right.codeLen)
        return false;
    // compare symbol otherwise
    return left.symbol.compare(right.symbol) < 0;
}

CanonicalHuffmanEncoder::CanonicalHuffmanEncoder(){
    maxLength = 0;
    encoderPtr.reset(new HuffmanEncoder());
}

CanonicalHuffmanEncoder::~CanonicalHuffmanEncoder(){
}

void CanonicalHuffmanEncoder::buildCoder(const unordered_map<string, int>& wordFreqDict){
    encoderPtr->buildCoder(wordFreqDict);
    encoderPtr->buildSymoblLenVec(symbolLenVec);
    std::sort(symbolLenVec.begin(), symbolLenVec.end(), SymbolLenGreaterThan);
    buildCanonicalCoder();
}

void CanonicalHuffmanEncoder::calculateMaxLength(){
    for(auto it = symbolLenVec.begin(); it != symbolLenVec.end(); it++){
        if(it->codeLen > maxLength)
            maxLength = it->codeLen;
    }
}

void CanonicalHuffmanEncoder::countNuml(){
    numl = vector<int>(maxLength, 0);
    for(auto it = symbolLenVec.begin(); it != symbolLenVec.end(); it++){
        numl[it->codeLen - 1] ++;
    }

    numBeforeLength = vector<int>(maxLength, 0);
    int sum = 0;
    for(int len = maxLength - 1; len > 0; len--){
        sum += numl[len];
        numBeforeLength[len - 1] = sum;
    }
}

void CanonicalHuffmanEncoder::calculateFirstcode(){
    firstcode = vector<int>(maxLength, 0);
    firstcode[maxLength - 1] = 0;
    for(int len = maxLength - 1; len > 0; len--){
        firstcode[len - 1] = (firstcode[len] + numl[len]) / 2;
    }
}

void CanonicalHuffmanEncoder::calculateSymbolCode(){
    vector<int> nextcode(firstcode);
    for(auto it = symbolLenVec.begin(); it != symbolLenVec.end(); it++){
        BitArray code = getRightmostBits(nextcode[it->codeLen - 1], it->codeLen);
        std::pair<string, BitArray> symbolCodePair(it->symbol, code);
        word2CodeDict.insert(symbolCodePair);
        nextcode[it->codeLen - 1] ++;
    }
}

void CanonicalHuffmanEncoder::buildCanonicalCoder(){
    calculateMaxLength();
    countNuml();
    calculateFirstcode();
    calculateSymbolCode();
}

bool CanonicalHuffmanEncoder::isFresh(const string& word) const{
    return word2CodeDict.find(word) == word2CodeDict.end();
}

BitArray CanonicalHuffmanEncoder::encode(const string& word) const{
    unordered_map<string, BitArray>::const_iterator it = word2CodeDict.find(word);
    if(it == word2CodeDict.end()){
        //TODO shall not happen
        return BitArray();
    }
    return it->second;
}

int CanonicalHuffmanEncoder::calculateOffset(int length, int v) const{
    return numBeforeLength[length - 1] + (v - firstcode[length - 1]);
}

int CanonicalHuffmanEncoder::decode(BinaryCode& code, string& word) const{
    if(code.finish())
        return -1;
    int length = 1;
    int v = code.getFirstBitMoveToNext();
    while(v < firstcode[length - 1]){
        ++length;
        if(code.finish())
            return -1;
        v = v * 2 + code.getFirstBitMoveToNext();
    }
    int offset = calculateOffset(length, v);
    word = symbolLenVec[offset].symbol;
    return 0;
}
    
void CanonicalHuffmanEncoder::parse(string& dumpStr){
    vector<string> lines;
    split(dumpStr, '\n', lines);
    for(auto it = lines.begin(); it != lines.end(); it++)
        symbolLenVec.push_back(SymbolLen::parse(*it));
    buildCanonicalCoder();
}

string CanonicalHuffmanEncoder::dump() const{
    string ret = "";
    for(auto it = symbolLenVec.begin(); it != symbolLenVec.end(); it++){
        ret += it->dump() + "\n";
    }
    return ret;
}
