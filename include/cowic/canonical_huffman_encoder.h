//canonical_huffman_encoder.h

#ifndef _CANONICAL_HUFFMAN_ENCODER_H_
#define _CANONICAL_HUFFMAN_ENCODER_H_

#include "huffman_encoder.h"

#include <memory>

using std::shared_ptr;

class CanonicalHuffmanEncoder : public Encoder{
public:
    CanonicalHuffmanEncoder();
    void buildCoder(const unordered_map<string, int>& wordFreqDict);
    bool isFresh(const string& word) const;
    BitArray encode(const string& word) const;
    int decode(BinaryCode& code, string& word) const;
    void parse(string& dumpStr);
    string dump() const;
    ~CanonicalHuffmanEncoder();
private:
    vector<SymbolLen> symbolLenVec;
    int maxLength;
    vector<int> numl;
    vector<int> numBeforeLength;
    vector<int> firstcode;
    unordered_map<string, BitArray> word2CodeDict;
    shared_ptr<HuffmanEncoder> encoderPtr;

    // The SymbolLenVec is calculated and sorted, calculate code for each symbol
    void buildCanonicalCoder();
    void calculateMaxLength();
    void countNuml();
    void calculateSymbolCode();
    void calculateFirstcode();
    int calculateOffset(int length, int v) const;
};

#endif
