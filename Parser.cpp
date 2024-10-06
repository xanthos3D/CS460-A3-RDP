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
    //do we want desicion making here? what if the token doesnt form a proper statewment?
    currentToken = tokenVector[index];
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
    if ( tokenVector[state].getTokenString() == "procedure" && tokenVector[state++].getTokenString() == "main") {
        main_procedure();
    } else if (tokenVector[state].getTokenString()== "function") {
        function_declaration();
    } else if (tokenVector[state].getTokenString() == "procedure") {
        procedure_declaration();
    } else if ( datatype_specifier() ) {
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
void Parser::main_procedure(){
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

    if ( datatype_specifier() ) {
        expect(tokenVector[state].getTokenString());
    }
    if (tokenVector[state].isIdentifier() ) {
        expect(tokenVector[state].getTokenString());
    }
    expect( "(");

    if (tokenVector[state].getTokenString() == "void") {
        expect("void");
    } else {
        parameter_list();
    }
    expect( ')');
    expect( '{' );
    compound_statement();
}





/** **************************************************************************************
Checks procedure_declaration meets BNF requirements
@pre:
@post:
 *****************************************************************************************/
void Parser::procedure_declaration(){
     expect( "procedure");
    if ( tokenVector[state].isIdentifier() ) {
        expect(tokenVector[state].getTokenString());
    }
    expect( '(' );
    if (tokenVector[state].getTokenString() == "void") {
        expect("void");
    } else {
        parameter_list();
    }
    expect( ')' );
    expect( '{' );
    compound_statement();
    expect( '}' );
}



void Parser::parameter_list(){
    if( !tokenVector[state].isRParen()) {
        if ( datatype_specifier() ){
            expect(tokenVector[state].getTokenString() );
        }
        identifier();
        expect( ",");
        parameter_list();
    }
}


//DONE
void Parser::block_statement(){
    expect( "{");
    if( !tokenVector[state].isRParen()) {
        compound_statement();
    }
    expect( "}");
}

//DONE
void Parser::compound_statement() {
    if ( !tokenVector[state].isRBrace() ) {
        statement();
        compound_statement();
    }
}


void Parser::statement(){
    Token token = tokenVector[state];
    if (token.getTokenString() == "return"){
        return_statement();
    } else if ( token.getTokenString() == "if "){
        selection_statement();
    }else if (token.getTokenString() == "printf"){
        printf_statement();
    }else if (token.getTokenString() == "for"){
        iteration_statement();
    } else if ( datatype_specifier() || token.isIdentifier() ) {
        expect( token.getTokenString() );
    }

}



void Parser::return_statement(){
        expect("return");
        if (tokenVector[state].isSingleQuote() || tokenVector[state].isDoubleQuote()) {
            expect(tokenVector[state].getTokenString());
        }
        else {
            expression();
        }
        expect (';');
}


/** **************************************************************************************
Checks declaration_statement meets BNF requirements
@pre:
@post:
 *****************************************************************************************/
void Parser::declaration_statement(){
    if ( datatype_specifier() ){
        expect( tokenVector[state].getTokenString());
    }

    if (tokenVector[state].isIdentifier()) {
        expect(tokenVector[state].getTokenString());
    } else {

        identifier_and_identifier_array_list();
    }
    expect (';');
}

void Parser::user_defined_function(){
    if (tokenVector[state].isIdentifier()) {
        expect( tokenVector[state].getTokenString() );
    }
    expect("(");
    if (tokenVector[state].isIdentifier()) {
        identifier_and_identifier_array_list();
    } else{
        expression();
    }
    expect(")");

}



void Parser::getchar_function(){
    expect("getchar");
    expect("(");
    if (tokenVector[state].isIdentifier()) {
        expect( tokenVector[state].getTokenString() );
    }
    expect(")");

}

void Parser::printf_statement(){

    expect("getchar");
    expect("(");
    if ( tokenVector[state].isDoubleQuote() || tokenVector[state].isDoubleQuote() ){
        expect( tokenVector[state].getTokenString() );
        if ( tokenVector[state].isComma() ){
            identifier_and_identifier_array_list();
        }
    }
    expect(")");
    expect(";");

}

void Parser::assignment_statement(){

    if (tokenVector[state].isIdentifier()) {
        expect( tokenVector[state].getTokenString() );
    }
    if ( tokenVector[state].isAssignmentOperator() ){
        expect( tokenVector[state].getTokenString() );
    }
    if (tokenVector[state].isSingleQuote() || tokenVector[state].isDoubleQuote()) {
        expect(tokenVector[state].getTokenString());
    }
    else {
        expression();
    }
}
void Parser::iteration_statement() {

    if ( tokenVector[state].getTokenString() == "for") {
        expect(tokenVector[state].getTokenString());
        expect("(");
        initialization_statement();
        if (tokenVector[state].isSemicolon()) {
            expect(tokenVector[state].getTokenString());
        }
        boolean_expression();
        if (tokenVector[state].isSemicolon()) {
            expect(tokenVector[state].getTokenString());
        }
        expression();
        expect(")");
        if (tokenVector[state].isLBrace()) {
            block_statement();
        } else {
            statement();
        }
    } else if ( tokenVector[index++].getTokenString() == "while" ){
        expect(tokenVector[state].getTokenString());
        expect("(");
        boolean_expression();
        expect(")");
        if (tokenVector[state].isLBrace()) {
            block_statement();
        } else {
            statement();
        }
    }
}
void Parser::selection_statement(){
    if ( currentToken.getTokenString() == "if "){
        expect(tokenVector[state].getTokenString());
    }
    expect("(");
    boolean_expression();
    expect(")");
    if (tokenVector[state].isLBrace()) {
        block_statement();
        if ( currentToken.getTokenString() == "else "){
            expect(tokenVector[state].getTokenString());
            if (tokenVector[state].isLBrace()) {
                block_statement();
            } else {
                statement();
            }
        }
    } else {
        statement();
        if ( currentToken.getTokenString() == "else "){
            expect(tokenVector[state].getTokenString());
            if (tokenVector[state].isLBrace()) {
                block_statement();
            } else {
                statement();
            }
        }
    }

}


void Parser::expression(){
    //<BOOLEAN_EXPRESSION> |

    //<NUMERICAL_EXPRESSION>
}
void Parser::initialization_statement(){

    if (tokenVector[state].isIdentifier()) {
        expect( tokenVector[state].getTokenString() );
    }
    if ( tokenVector[state].isAssignmentOperator() ){
        expect( tokenVector[state].getTokenString() );
    }
    if (tokenVector[state].isSingleQuote() || tokenVector[state].isDoubleQuote()) {
        expect(tokenVector[state].getTokenString());
    }
}
void Parser::boolean_expression(){
    if (tokenVector[state].isBoolTrue()){
        expect( "TRUE");
    }else if (tokenVector[state].isBoolFalse()){
        expect( "FALSE");
    } else if ( tokenVector[state].isLParen() ){
        expect("(");
        if (tokenVector[state].isIdentifier()) {
            expect( tokenVector[state].getTokenString() );
            boolean_operator();
            boolean_expression();
        }
        expect(")");
    } else {
        numerical_expression();
        if (  tokenVector[state].isBoolE() || tokenVector[state].isBoolNE() ||
                tokenVector[state].isBoolLT() || tokenVector[state].isBoolGT() ||
                tokenVector[state].isBoolLTE() || tokenVector[state].isBoolGTE()){
            expect( tokenVector[state].getTokenString() );
        }
        numerical_expression();
    }
}





void Parser::numerical_expression(){
    if ( tokenVector[state].isLParen() ) {
        expect("(");
        numerical_operand();

    } else {
        numerical_operand();
        if (tokenVector[state].isRParen()) {
        }
        numerical_operator();
        {
        expect( tokenVector[state].getTokenString() );
        }
        }
    }

    /*<L_PAREN>
    <NUMERICAL_OPERAND> <R_PAREN> |


    <L_PAREN>
    <NUMERICAL_OPERAND> <NUMERICAL_OPERATOR> <NUMERICAL_EXPRESSION>
    <R_PAREN>

     <L_PAREN>
    <NUMERICAL_OPERAND> <NUMERICAL_OPERATOR> <NUMERICAL_EXPRESSION>
    <R_PAREN>

    <NUMERICAL_OPERAND> |


    <NUMERICAL_OPERAND>
    <NUMERICAL_OPERATOR> <NUMERICAL_EXPRESSION>

    <NUMERICAL_OPERAND> <NUMERICAL_OPERATOR> <L_PAREN>
    <NUMERICAL_EXPRESSION> <R_PAREN> <NUMERICAL_OPERAND>
    <NUMERICAL_OPERATOR> <NUMERICAL_EXPRESSION>

    <NUMERICAL_OPERAND> <NUMERICAL_OPERATOR> <L_PAREN>
    <NUMERICAL_EXPRESSION> <R_PAREN>*/

void Parser::relational_expression(){

}
void Parser::equality_expression(){

}

void Parser::boolean_operator(){
    /*if ( tokenVector[state] == "==" || tokenVector[state] == "&&"){
        expect(tokenVector[state].getTokenString());
    }*/
}
void Parser::numerical_operator(){

}

void Parser::numerical_operand(){
    /*
    <IDENTIFIER> |

    <INTEGER> |

    <GETCHAR_FUNCTION> |

    <USER_DEFINED_FUNCTION> |

    <SINGLE_QUOTE> <CHARACTER> <SINGLE_QUOTE> |

    <SINGLE_QUOTE> <ESCAPED_CHARACTER> <SINGLE_QUOTE> |

    <DOUBLE_QUOTE>
    <CHARACTER> <DOUBLE_QUOTE> |

    <DOUBLE_QUOTE> <ESCAPED_CHARACTER>
    <DOUBLE_QUOTE>*/
}

bool Parser::datatype_specifier(){
    if (tokenVector[state].getTokenString() == "int" || tokenVector[state].getTokenString() == "char" ||
                                                        tokenVector[state].getTokenString() == "bool"){
        return true;
    }                                    
        return false;
}

void Parser::identifier_and_identifier_array_list(){

}
void Parser::identifier_array_list(){

}
void Parser::identifier_list(){

}

std::string Parser::identifier(){
    if (!tokenVector[state].isIdentifier()){
        throw std::runtime_error("Expected an identifier, but got '" + tokenVector[state].getTokenString() + "'");
    }
    return tokenVector[state].getTokenString();
}
void Parser::identifier_tail(){}




/** **************************************************************************************
If the token
@pre:
@post:
 *****************************************************************************************/
void Parser::expect(const std::string& expected_value) {
    if (state < tokenVector.size()) {
        Token token = tokenVector[state];
        if (token.getTokenString() != expected_value) {
            throw std::runtime_error("Expected '" + expected_value + "' but got '" + tokenVector[state].getTokenString() + "'");
        }
        /*
        if ( tokenVector[state].isRBrace()   ){
            //use the cst root node and pass in the token.
            cst->addChild(  ,token);
        } else if ( tokenVector[state].isSemicolon() ){
            cst->addSibling( token );
            newStatement = true;
        }else if (newStatement){
            cst->addChild( token );
            newStatement = false;
        } else {
            cst->addSibling( token ) ;
        }*/
    }
    throw std::runtime_error("Unexpected end of input");
}

