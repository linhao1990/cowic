//huffman_encoder.h
#ifndef _HUFFMAN_ENCODRE_H
#define _HUFFMAN_ENCODRE_H

#include "encoder.h"
#include "tree_node.h"

class SymbolLen{
public:
    SymbolLen(string symbolVal, int codeLenVal) : symbol(symbolVal), codeLen(codeLenVal){}
    string dump() const;
    static SymbolLen parse(const string& dumpStr);
    string symbol;
    int codeLen;
};

class HuffmanEncoder : public Encoder{
public:
    HuffmanEncoder();
    void buildCoder(const unordered_map<string, int>& wordFreqDict);
    bool isFresh(const string& word) const;
    /* The caller must gurantee word exists,
     * Otherwise the behavior is undefined.
     * */
    BitArray encode(const string& word) const;
    int decode(BinaryCode& code, string& word) const;
    void parse(string& dumpStr);
    /** Dump the huffman encoder into a string. So we can save it and rebuild it in future.
     *  We will write words and its frequncy, then we will rebuild the huffman tree.
     *  Format:
     *  <WORD1> <#FREQ1>
     *  ...
     *  <WORDN> <#FREQN>
     *  Explanation:
     *  Each line is a word and its frequncy split by a space. Note the word itself can be a space
     *  so we shall use the last space to split word and frequncy.
     */
    string dump() const;
    /* Used in canonical huffman encoder only
     * */
    void buildSymoblLenVec(vector<SymbolLen>& symbolLenVec) const;
    ~HuffmanEncoder();
private:
    TreeNodePtr root;
    // accelerate encode speed
    unordered_map<string, BitArray> word2CodeDict;
    /** Assign huffman code to each leave node, left branch with code 0, right branch with code 1
     */
    void assignHuffmanCode(TreeNodePtr node, string pathCode);
    int traverseTreeParseCode(const TreeNodePtr& node, BinaryCode& code, string& word) const;
};

#endif
