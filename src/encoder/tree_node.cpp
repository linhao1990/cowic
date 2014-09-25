/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//tree_node.cpp

#include "tree_node.h"

TreeNode::TreeNode(string wordValue, int freqValue) : word(wordValue), freq(freqValue){
}

TreeNode::TreeNode(TreeNodePtr leftValue, TreeNodePtr rightValue) : left(leftValue), right(rightValue){
    freq = left->getFreq() + right->getFreq();
}

int TreeNode::getFreq() const{
    return freq;
}

TreeNodePtr TreeNode::getLeft() const{
    return left;
}

TreeNodePtr TreeNode::getRight() const{
    return right;
}

string TreeNode::getWord() const{
    return word;
}

BitArray TreeNode::getCode() const{
    return code;
}

void TreeNode::setCode(const string& plainStr){
    code = BitArray::parseFrom(plainStr);
}

TreeNode::~TreeNode(){
}

