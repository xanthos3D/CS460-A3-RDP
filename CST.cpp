#include "CST.hpp"
#include "CSTNode.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "Token.hpp"
#include <queue>

/** **************************************************************************************
adds the leftchild to the current cstnode in our cst tree
@pre:takes, cstnode to be added, and a token to declare that cstnode
@post: adds that node as the leftchild to our cst in its current position.
 *****************************************************************************************/
void CST::addChild(CSTNode *root, Token token) {

    //if the root is a empty pointer return
    if ( root == nullptr )
        return;

    //if our cstnode has both a left and right nullptr
    if (root->getLeft() == nullptr && root->getRight() == nullptr) {
        //create a new cstnode to be added as the leftchild
        CSTNode* newNode = new CSTNode(token);
        root->leftChild(newNode);
    
    //otherwise if the left is nullptr then
    }else if (root->getLeft() != nullptr) {
        //recurse to that node, adding the leftchild 
        addChild(root->getLeft(), token);

    //otherwise if the right is nullptr then
    //notes, wouldnt we want to call our rightsibling recursive function here?
    }else if (root->getRight() != nullptr) {
        //recurse to that node, adding the rightchild 
        addChild(root->getRight(), token);
        //addSibling(root->getRight(), token);
    }
    return;
}

/** **************************************************************************************
adds the rightSibling to the current cstnode in our cst tree
@pre:takes, cstnode to be added, and a token to declare that cstnode
@post: adds that node as the rightSibling to our cst in its current position.
 *****************************************************************************************/
void CST::addSibling(CSTNode *root, Token token) {
    //if the root is a empty pointer return
    if ( root == nullptr )
        return;

    //if our cstnode has both a left and right nullptr
    if ( root->getRight() == nullptr && root->getLeft() == nullptr ){

        //create a new cstnode to be added as the rightsibling
        CSTNode* newNode = new CSTNode(token);
        root->rightSibling( newNode );

    //otherwise if the left is nullptr then
    }else if ( root->getRight() != nullptr ){
        
        //otherwise if the right is not nullptr then
        //note, wouldnt we want to call our child function?
        addSibling( root->getRight(),token);
        
    //otherwise we we need to recurse down the left branch
    } else if ( root->getLeft() != nullptr ){
        //otherwise if the left is not nullptr then
        
        addSibling( root->getLeft(),token);
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

        if (current->getLeft() != nullptr ){
            q.push( current->getLeft() );
        }
        if ( current->getRight() != nullptr ){
            q.push( current->getRight() );
        }
    }
}