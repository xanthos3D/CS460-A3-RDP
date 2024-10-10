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
#include "CSTNode.hpp"
#include <vector>

class Parser {
public:
    //default constructor, on declaration we need a vector of tokens, then we definie the token as blank, and create a new cst object.
    Parser(std::vector<Token>& incommingVector) : tokenVector(incommingVector) ,cst(new CST()), newStatement(false){};

    //a
    void expect(const std::string& expected_value);

    //functions to check BNF standards
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
    bool relational_expression();
    bool boolean_operator();
    bool numerical_operator();
    bool numerical_operand();
    bool datatype_specifier();
    void identifier_and_identifier_array_list();
    void identifier_array_list();
    void identifier_list();
    void identifier();
    void single_quoted_string();
    void double_quoted_string();
    void printTree() { cst->printTree(); }

    //function to get get our cst tree with out tokens.
    CST* parse();
private:

    //possition in our vector which is passed in, and current state
    int state = 0;
    bool newStatement = false;
    //vector passed in on declaration
    std::vector<Token> tokenVector;
    //concrete syntax tree object
    CST* cst;
};


#endif //PROJECT1_PHASE2_PARSER_HPP
