#ifndef CS460_A2_TOKENIZER_CST_HPP
#define CS460_A2_TOKENIZER_CST_HPP
#include <iostream>
#include <fstream>
#include <string>
#include "Token.hpp"
#include "CSTNode.hpp"

//Concrete syntax tree, this is the tree object which we want to return
class CST {
public:
    //default constructer which  sets our first cstnode to nullptr
    CST() : root(nullptr) {}

    //function to set the root node to a new cstnode
    void setRoot(CSTNode* node) { root = node; }

    //function to add cstnode to the leftchild
    void addChild(CSTNode *root, Token token);

    //funcxtion assd sctnode to be the right sibling
    void addSibling(CSTNode *root, Token token);
    //breadth first traversal to print out our cst
    void printTree();  

private:
    //object made of one cst node which connects to the other nodes apart of this data structure
    CSTNode* root;
};


#endif //CS460_A2_TOKENIZER_CST_HPP
