/** ***************************************************************************
 * @remark CS 460: Definition of Parser class
 *
 * @author Luis Galvez
 *         Xander
 *         Christian Gonzalez
 *         Anthony Manese
 *
 * @file  Parser.hpp
 * @date  Fall 2024
 **************************************************************************** */

#ifndef PROJECT1_PHASE2_PARSER_HPP
#define PROJECT1_PHASE2_PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include "Token.hpp"
#include "CST.hpp"
#include <vector>

class Parser {
public:
    //default constructor, on declaration we need a vector of tokens, then we definie the token as blank, and create a new cst object.
    Parser(std::vector<Token>& incommingVector) : tokenVector(incommingVector),currentToken(1,1),cst(new CST()){};

    //function to get get our cst tree with out tokens.
    CST* parse();
private:

    //current token.
    Token currentToken;

    //possition in our vector which is passed in.
    int vectorPosition = 0;

    //vector passed in on declaration
    std::vector<Token> tokenVector;

    //concrete syntax tree object 
    CST* cst;
};


#endif //PROJECT1_PHASE2_PARSER_HPP
