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
    program();
    return cst;
}





/** **************************************************************************************
Checks program meets BNF requirements
@pre:
@post:
 *****************************************************************************************/
void Parser::program() {
    if ( tokenVector[index].getTokenString() == "procedure" && tokenVector[index++].getTokenString() == "main") {
        main_procedure();
    } else if (tokenVector[index].getTokenString()== "function") {
        function_declaration();
    } else if (tokenVector[index].getTokenString() == "procedure") {
        procedure_declaration();
    } else if (peek() == datatype_specifer()) {
        declaration_statement();
    } else{
        //something wrong
        throw;
    }
}





/** **************************************************************************************
Checks main-procedure meets BNF requirements
@pre:
@post:
 *****************************************************************************************/
void Parser::main_procedure() {
    // Parse the main procedure based on your grammar rules
    expect("procedure");
    expect("main");
    expect("(");
    expect("void");
    expect(")");
    block_statement();

}




/** **************************************************************************************
Checks function_declaration meets BNF requirements
@pre:
@post:
 *****************************************************************************************/
void Parser::function_declaration(){

    expect("function");
    datatype_specifer();
    identifier();
    expect( "(");

    if (peek() == "void") {
        expect("void");
    } else {
        parameter_list();
    }

    expect ( ')')
    expect( '{' );
    compound_statement();
    expect( '}' );

}





/** **************************************************************************************
Checks procedure_declaration meets BNF requirements
@pre:
@post:
 *****************************************************************************************/
void Parser::procedure_declaration();




/** **************************************************************************************
Checks declaration_statement meets BNF requirements
@pre:
@post:
 *****************************************************************************************/
void Parser::declaration_statement();
void Parser::parameter_list();

void Parser::block_statement(){
    expect("{")
    compound_statement();
    expect("{")
}

void Parser::compound_statement() {
    if (statement()) {

    } else if (compound_statement()) {
        statement();
    }else{
        throw
    }
}


void Parser::statement();
void Parser::return_statement();
void Parser::declaration_statement();
void Parser::user_defined_function();
void Parser::getchar_function();
void Parser::printf_statement();
void Parser::assignment_statement();
void Parser::iteration_statement();
void Parser::selection_statement();
void Parser::expression();
void Parser::initialization_statement();
void Parser::boolean_expression();
void Parser::numerical_expression();
void Parser::relational_expression();
void Parser::equality_expression();
void Parser::boolean_operator();
void Parser::numerical_operator();
void Parser::numerical_operand();

std::string Parser::datatype_specifer(){

    Token token = peek();
    if (token.getTokenString() == "int") {
        expect("int");
        return "int";
    }

    if (token.getTokenString() == "char") {
        expect("char");
        return "char";
    }

    if (token.getTokenString() == "bool") {
        expect(" bool");
        return "bool";
    }
    throw;

}

void Parser::identifier_and_identifier_array_list();
void Parser::identifier_array_list();
void Parser::identifier_list();
void Parser::identifier(){

    expect( currentToken );
    identifier_tail();
}
void Parser::identifier_tail();




/** **************************************************************************************
Takes current token and creates a CSTNode from it, then moves onto next token in vector
@pre:
@post:
 *****************************************************************************************/
Token Parser::consume() {
    if (current < tokens.size()) {
        if ( currentToken.getLineNum() != tokenVector[index].getLineNum() ){
            cst->addChild( Token );
        } else {
            cst->addSibling();
        }
        return tokens[current++];
    }
    throw std::runtime_error("Unexpected end of input");
}





/** **************************************************************************************
compares ecxpected value with token type and make sure it follows BNF requirements
@pre:
@post:
 *****************************************************************************************/
void Parser::expect(const std::string& expected_value) {
    Token token = consume();
    if (token.value != expected_value) {
        throw std::runtime_error("Expected '" + expected_value + "' but got '" + token.value + "'");
    }
}





/** **************************************************************************************
Checks nect Token without consuming it
@pre:
@post:
 *****************************************************************************************/

Token Parser::peek(){
    return tokens[current];
}