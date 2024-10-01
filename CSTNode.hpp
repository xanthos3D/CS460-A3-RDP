#ifndef CS460_A2_TOKENIZER_CSTNODE_HPP
#define CS460_A2_TOKENIZER_CSTNODE_HPP
#include <iostream>
#include <fstream>
#include <string>
#include "Token.hpp"

class CSTNode {
public:
    CSTNode(const Token& token) : token(token), leftChild(nullptr), rightSibling(nullptr) {}
    CSTNode* getLeft() { return leftChild;}
    CSTNode* getRight() { return rightSibling;}
    void leftChild( CSTNode *newLeftChild ) { leftChild = newLeft; }
    void rightSibling(CSTNode *newRightSibling) { rightSibling = newRight; }
    Token getToken() { return token; }
private:
    Token token;
    CSTNode *leftChild, *rightSibling;
};



#endif //CS460_A2_TOKENIZER_CSTNODE_H
