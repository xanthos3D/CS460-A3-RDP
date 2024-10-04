#ifndef CS460_A2_TOKENIZER_CSTNODE_HPP
#define CS460_A2_TOKENIZER_CSTNODE_HPP
#include <iostream>
#include <fstream>
#include <string>
#include "Token.hpp"

class CSTNode {
public:
    CSTNode(const Token& token) : token(token), _leftChild(nullptr), _rightSibling(nullptr) {}
    CSTNode* getLeft() { return _leftChild;}
    CSTNode* getRight() { return _rightSibling;}
    void leftChild( CSTNode *newLeftChild ) { _leftChild = newLeftChild; }
    void rightSibling(CSTNode *newRightSibling) { _rightSibling = newRightSibling; }
    Token getToken() { return token; }
private:
    Token token;
    CSTNode *_leftChild, *_rightSibling;
};



#endif //CS460_A2_TOKENIZER_CSTNODE_H
