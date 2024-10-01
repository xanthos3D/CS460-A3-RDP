#include "CST.hpp"
#include "CSTNode.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "Token.hpp"


void CST::addChild(CSTNode *root, Token token) {

    if ( root == nullptr )
        return;

    if (root->getLeft() == nullptr && root->getRight() == nullptr) {
        CSTNode* newNode = new CSTNode(token);
        root->leftChild(newNode);
    }else if (root->getLeft() != nullptr) {
        addChild(root->getLeft(), token);
    }else if (root->getRight() != nullptr) {
        addChild(root->getRight(), token);
    }
    return;
}

void CST::addSibling(CSTNode *root, Token token) {

    if ( root == nullptr )
        return;

    if ( root->getRight() == nullptr && root->getLeft() == nullptr ){
        CSTNode* newNode = new CSTNode( Token );
        root->rightSibling( newNode );
    }else if ( root->getRight() != nullptr ){
        addSibling( root->getRight() );
    } else if ( root->getLeft() != nullptr ){
        addSibling( root->getLeft() );
    }
    return;
}

void CST::printTree(){
    if ( root == nullptr ) {
        return;
    }
    std::queue<CSTNode*> q;
    q.push( root );

    while ( !q.empty() ) {

        CSTNode* current = q.front();
        std::cout << current->getToken().getTokenString() << std::endl;
        q.pop();

        if ( current->leftChild() != nullptr ){
            q.push( current->leftSubtree() );
        }
        if ( current->rightSibling() != nullptr ){
            q.push( current->rightSubtree() );
        }
    }
}