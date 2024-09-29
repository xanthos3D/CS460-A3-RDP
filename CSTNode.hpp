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
    Token getToken() { return token; }
private:
    Token token;
    CSTNode* leftChild;
    CSTNode* rightSibling;

void addChild(CSTNode* child) {
    if (!leftChild) {
        leftChild = child;  // If no children, set the first child
    } else {
        // Otherwise, find the rightmost sibling and add the child there
        CSTNode* sibling = leftChild;
        while (sibling->rightSibling) {
            sibling = sibling->rightSibling;
        }
        sibling->rightSibling = child;  // Add as a right sibling
    }
}
};



#endif //CS460_A2_TOKENIZER_CSTNODE_H
