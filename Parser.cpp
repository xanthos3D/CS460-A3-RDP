#include <iostream>
#include "Parser.hpp"
#include "CST.hpp"
#include "CSTNode.hpp"
#include "stack"

/** **************************************************************************************
parse function which creates our CST
@pre:vector of tokens from declaration
@post: returns a concrete syntax tree.
 *****************************************************************************************/
CST* Parser::parse() {

    //set our class token to the first token in our token vector
    currentToken = tokenVector[vectorPosition];

    //do we want desicion making here? what if the token doesnt form a proper statewment?

    //note it seems we want to go down a node for every time \n is found. luck for uss when 
    //we tokenize we keep track of the line number, so we can use that to tell when we should
    // go down a node

    //make the root node of the program in our cst
    cst->setRoot(new CSTNode(currentToken));

    //loop through the vector of tokens until EOF token
    while (!currentToken.isEOF()) {

        //recursively decend and make a CST

        //Declaration(cst->root);
    }
    return cst;
}
/** **************************************************************************************
grabbs a entire variable declaration
@pre: 
@post:
 *****************************************************************************************/
void Declaration(){
}

/** **************************************************************************************
grabs a entire expression
@pre: 
@post:
 *****************************************************************************************/
void Expression(){

}

/** **************************************************************************************
grabs function definition
@pre: 
@post:
 *****************************************************************************************/
void functionDefinition(CSTNode* parent) {

}

/** **************************************************************************************
grabbs veriable declaration
@pre: 
@post:
 *****************************************************************************************/
void variableDeclaration(CSTNode* parent) {
}

/** **************************************************************************************
grabbs statement
@pre: 
@post:
 *****************************************************************************************/
void statement(CSTNode* parent) {}