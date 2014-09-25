/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//huffman_encoder.cpp

#include <queue>
#include <sstream>

#include "huffman_encoder.h"
#include "helper.h"

using std::priority_queue;

class TreeNodeCompare{
    public:
        bool operator()(const TreeNodePtr& lhs, const TreeNodePtr& rhs) const
        {
            return lhs->getFreq() > rhs->getFreq();
        }
};

HuffmanEncoder::HuffmanEncoder(){
    root = NULL;
}

typedef priority_queue<TreeNodePtr, std::vector<TreeNodePtr>, TreeNodeCompare> node_pq;

void transform2PriorityQueue(const unordered_map<string, int>& wordFreqDict, node_pq& nodes){
    for(auto it = wordFreqDict.begin(); it != wordFreqDict.end(); it++){
        TreeNodePtr node (new TreeNode(it->first, it->second));
        nodes.push(node);
    }
}

TreeNodePtr topWithPop(node_pq& nodes){
    TreeNodePtr topValue = nodes.top();
    nodes.pop();
    return topValue;
}

TreeNodePtr constructHuffmanTree(node_pq& nodes){
    while(nodes.size() > 1){
        TreeNodePtr left = topWithPop(nodes);
        TreeNodePtr right = topWithPop(nodes);
        TreeNodePtr parent = TreeNodePtr(new TreeNode(left, right));
        nodes.push(parent);
    }
    return topWithPop(nodes);
}

void HuffmanEncoder::assignHuffmanCode(TreeNodePtr node, string pathCode){
    // Both left, right shall exist or both of them are NULL
    if(node->getLeft() == NULL){
        node->setCode(pathCode);
        word2CodeDict.insert(std::make_pair<string, BitArray>(node->getWord(), node->getCode()));
        return;
    }
    assignHuffmanCode(node->getLeft(), pathCode + "0");
    assignHuffmanCode(node->getRight(), pathCode + "1");
}

void HuffmanEncoder::buildCoder(const unordered_map<string, int>& wordFreqDict){
    node_pq nodes;
    transform2PriorityQueue(wordFreqDict, nodes);
    root = constructHuffmanTree(nodes);
    assignHuffmanCode(root, "");
}

bool HuffmanEncoder::isFresh(const string& word) const{
    return word2CodeDict.find(word) == word2CodeDict.end();
}

BitArray HuffmanEncoder::encode(const string& word) const{
    unordered_map<string, BitArray>::const_iterator it = word2CodeDict.find(word);
    if(it == word2CodeDict.end()){
        //TODO shall not happen
        return BitArray();
    }
    return it->second;
}

int HuffmanEncoder::traverseTreeParseCode(const TreeNodePtr& node, BinaryCode& code, string& word) const{
    if(node->getLeft() == NULL){
        //This is an leaf node.
        word = node->getWord();
        return 0;
    }
    // decide the traverse path by the first bit
    if(code.finish())
        return -1;
    bool bit = code.getFirstBitMoveToNext();
    if(bit == false){
        return traverseTreeParseCode(node->getLeft(), code, word);
    } else{
        return traverseTreeParseCode(node->getRight(), code, word);
    }
}

int HuffmanEncoder::decode(BinaryCode& code, string& word) const{
    return traverseTreeParseCode(root, code, word);
}

void HuffmanEncoder::parse(string& dumpStr){
    unordered_map<string, int> wordFreqDict;
    parseDict(dumpStr, wordFreqDict);
    buildCoder(wordFreqDict);
}

void traverseTreeSaveWordFreq(const TreeNodePtr& node, string& wordFreqStr){
    if(node->getLeft() == NULL){
        // append info of leaf node
        std::stringstream sstream;
        sstream << node->getWord() << " " << node->getFreq() << "\n";
        wordFreqStr += sstream.str();
    } else{
        traverseTreeSaveWordFreq(node->getLeft(), wordFreqStr);
        traverseTreeSaveWordFreq(node->getRight(), wordFreqStr);
    }
}

string HuffmanEncoder::dump() const{
    string wordFreqStr = "";
    traverseTreeSaveWordFreq(root, wordFreqStr);
    return wordFreqStr;
}

void HuffmanEncoder::buildSymoblLenVec(vector<SymbolLen>& symbolLenVec) const{
    for(auto it = word2CodeDict.begin(); it != word2CodeDict.end(); it++){
        symbolLenVec.push_back(SymbolLen(it->first, it->second.size()));
    }
}

HuffmanEncoder::~HuffmanEncoder(){
}
