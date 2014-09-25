/* Cowic is a C++ library to compress formatted log like Apache access log.
 *
 * Cowic is released under the New BSD license (see LICENSE.txt). Go to the
 * project home page for more info:
 *
 * https://github.com/linhao1990/cowic.git
 */
//tree_node.h

#ifndef _TREE_NODE_H_
#define _TREE_NODE_H_

#include <memory>

#include "bit_array.h"

class TreeNode;
typedef std::shared_ptr<TreeNode> TreeNodePtr;

class TreeNode{
public:
    TreeNode(string wordValue, int freqValue);
    TreeNode(TreeNodePtr leftValue, TreeNodePtr rightValue);
    int getFreq() const;
    TreeNodePtr getLeft() const;
    TreeNodePtr getRight() const;
    string getWord() const;
    BitArray getCode() const;
    void setCode(const string& plainStr);
    ~TreeNode();
private:
    TreeNodePtr left;
    TreeNodePtr right;
    string word;
    int freq;
    BitArray code;
};

#endif
