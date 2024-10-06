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
    Parser(std::vector<Token>& incommingVector) : tokenVector(incommingVector),currentToken(1,1) ,cst(new CST()){};

    void expect(const char& expected_value);
    void expect(const std::string& expected_value);
    void program();
    void main_procedure();
    void function_declaration();
    void procedure_declaration();
    void parameter_list();
    void block_statement();
    void compound_statement();
    void statement();
    void return_statement();
    void declaration_statement();
    void user_defined_function();
    void getchar_function();
    void printf_statement();
    void assignment_statement();
    void iteration_statement();
    void selection_statement();
    void expression();
    void initialization_statement();
    void boolean_expression();
    void numerical_expression();
    void relational_expression();
    void equality_expression();
    void boolean_operator();
    void numerical_operator();
    void numerical_operand();
    bool datatype_specifier();
    void identifier_and_identifier_array_list();
    void identifier_array_list();
    void identifier_list();
    std::string identifier();
    void identifier_tail();

    //function to get get our cst tree with out tokens.
    CST* parse();
private:

    //possition in our vector which is passed in, and current state
    int state = 0;
    bool newStatement;
    //current token.
    Token currentToken;

    //possition in our vector which is passed in.
    int index = 0;

    //vector passed in on declaration
    std::vector<Token> tokenVector;
    //concrete syntax tree object 
    CST* cst;
};


#endif //PROJECT1_PHASE2_PARSER_HPP
