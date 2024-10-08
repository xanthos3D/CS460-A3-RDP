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

    //calls our over arching function to recurse through our token vector
    program();
    return cst;
}





/** **************************************************************************************
Checks program meets BNF requirements
@pre:
@post: recusively calls one of our functions based on the given token
 *****************************************************************************************/
 void Parser::program() {
    while(!tokenVector[state].isEOF()){

        std::cout<<"+++block start token++++++++++++++++++++++++++++++++++++++++++ "<<std::endl;
        tokenVector[state].print();
        std::cout<<"+++block start token++++++++++++++++++++++++++++++++++++++++++ "<<std::endl;
        if (tokenVector[state].getTokenString()== "function") {
            std::cout<<" found function declaration" <<std::endl;
            function_declaration();
        } else if (tokenVector[state].getTokenString() == "procedure") {
            std::cout<<" found  procedure declaration " <<std::endl;
            procedure_declaration();
        } else if ( datatype_specifier() ) {
            std::cout<<" found  declaration statement " <<std::endl;
            declaration_statement();
        }else if ( tokenVector[state].getTokenString() == "procedure") {
            std::cout<<" found main procedure" <<std::endl;
            main_procedure();
        }else{
            //something wrong
            std::cout<<"looked for declaration of function or procedure, but none found?"<<std::endl;
            std::cout<<"offending token:"<<std::endl;
            tokenVector[state].print();
            throw;
        }
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
@pre: is called when we find a function declaration
@post:
 *****************************************************************************************/
void Parser::function_declaration(){
    //so when we find a function we expect the token to be a identifier with the string function
    expect("function");

    //then we expect some data type specifier, and if it is then
    if ( datatype_specifier() ) {
        //get that datatype name and call our expect function
        expect(tokenVector[state].getTokenString());
    }

    //then we expect a identifier/ function name
    if (tokenVector[state].isIdentifier() ) {
        expect(tokenVector[state].getTokenString());
    }

    // then a open paren
    expect( "(");
    //may need a loop to handle, multiple instances of parameters being declared.
    //if we see the keyword void, in our current token.
    if (tokenVector[state].getTokenString() == "void") {
        //expect the void token.
        expect("void");
    } else {
        //otherwise we call our paramlist, until we reach a )
        parameter_list();
    }
    expect( ")");
    expect( "{" );
    // calls compound statement to start the interior of our function block.
    compound_statement();
    expect( "}" );
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
    expect( "(" );
    if (tokenVector[state].getTokenString() == "void") {
        expect("void");
    } else {
        parameter_list();
    }
    expect( ")" );
    expect( "{" );
    compound_statement();
    expect( "}" );
}





/** **************************************************************************************
checks that the parameter lsit  follows BNF rules
@pre: is called when we find parameter for a function
@post:generates cst for parameters
 *****************************************************************************************/
void Parser::parameter_list(){
        //if we find a data type
        if ( datatype_specifier() ){
            //call expect with that data type
            expect(tokenVector[state].getTokenString() );
        }
        //then expect variable name
        identifier();

        // then we check to see if a comma is present
        if ( tokenVector[state].isComma() ) {
            //if it is then add it
            expect(",");
            // and recuse until we are out of parameters to add.
            parameter_list();
        }
}





/** **************************************************************************************
checks that the follwoing statement is a block statement that follows BNF rules
@pre:
@post:
 *****************************************************************************************/
 void Parser::block_statement(){
    expect( "{");
    if( !tokenVector[state].isRParen()) {
        compound_statement();
    }
    expect( "}");
}





/** **************************************************************************************
checks that the follwoing statement is a compound statementthat follows BNF rules
@pre: called at the beginning of a scope block
@post:recurses through the scope of that block
 *****************************************************************************************/
void Parser::compound_statement() {

    //unless we find a closing brace
    if ( !tokenVector[state].isRBrace() ) {
        //check for statements and recurse this function.
        statement();
        compound_statement();
    }
}





/** **************************************************************************************
checks that the follwoing statement makes sure its a statemtn defined in BNF
@pre:is called to find a statement
@post: recurses though tokens to create a statement.
 *****************************************************************************************/
void Parser::statement(){

    std::cout<<"statement called"<<std::endl;

    //make a temp token? 
    Token token = tokenVector[state];

    //cases to recurse through a variety of different statements.
    if (token.getTokenString() == "return"){
        return_statement();
    }else if ( token.getTokenString() == "if"){
        std::cout<<"if found"<<std::endl;
        selection_statement();
    }else if (token.getTokenString() == "printf"){
        printf_statement();
    }else if (token.getTokenString() == "for"){
        iteration_statement();
    } else if (token.isIdentifier() && tokenVector[state+1].isAssignmentOperator() ){
        assignment_statement();
    
    //case which handles variable declarizations and assignments.
    } else if ( datatype_specifier() || token.isIdentifier() ) {
        declaration_statement();
    }
}





/** **************************************************************************************
checks that the follwoing statement is a return statement that follows BNF rules
@pre:
@post:
 *****************************************************************************************/
void Parser::return_statement(){
        expect("return");
        if (tokenVector[state].isSingleQuote() || tokenVector[state].isDoubleQuote()) {
            expect(tokenVector[state].getTokenString());
        }
        else {
            expression();
        }
    expect(";");
}






/** **************************************************************************************
Checks declaration_statement meets BNF requirements
@pre:
@post:
 *****************************************************************************************/
void Parser::declaration_statement(){
    //std::cout<<"declaration_statement called"<<std::endl;
    //if we recieve a specifier(data type)
    if ( datatype_specifier() ){
        //add that token to cst
        expect( tokenVector[state].getTokenString() );

        //then we expect a identifier after it.
        if (tokenVector[state].isIdentifier()) {
            expect(tokenVector[state].getTokenString());
        } else {
            throw;
        }
    }

    //if we see a comma after ward, that means that multiple things are being declared
    if ( tokenVector[state].isComma()){
        expect(",");
        identifier_and_identifier_array_list();
    }
    //expect statement to end with a semicolon.
    expect(";");
}





/** **************************************************************************************
checks that the follwoing function is a user_defined_function  that follows BNF rules
@pre:
@post:
 *****************************************************************************************/
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





/** **************************************************************************************
checks that the follwoing function is a getchar function that follows BNF rules
@pre:
@post:
 *****************************************************************************************/
void Parser::getchar_function(){
    expect("getchar");
    expect("(");
    if (tokenVector[state].isIdentifier()) {
        expect( tokenVector[state].getTokenString() );
    }
    expect(")");

}





/** **************************************************************************************
checks that the follwoing statement is a printf statement that follows BNF rules
@pre:
@post:
 *****************************************************************************************/
void Parser::printf_statement(){

    expect("printf");
    expect("(");
    //after the first double quote
    if (tokenVector[state].isDoubleQuote()){
        //eat the string
        expect("");
        expect(tokenVector[state].getTokenString());
        expect("");
        //if there is a comma, variables may be involved, so acount for those here
        if ( tokenVector[state].isComma() ){
            expect(",");
            identifier_and_identifier_array_list();
        }



    }
    expect(")");
    expect(";");

}





/** **************************************************************************************
checks that the follwoing statement is a assignment statement that follows BNF rules
@pre:
@post:
 *****************************************************************************************/
void Parser::assignment_statement(){
    expect( tokenVector[state].getTokenString() );
    expect( tokenVector[state].getTokenString() );

    if (tokenVector[state].isSingleQuote() || tokenVector[state].isDoubleQuote()) {
        expect(tokenVector[state].getTokenString());
    }
    else {
        expression();
    }
    expect(";");
}






/** **************************************************************************************
checks that the follwoing statement is an iteration statement that follows BNF rules
@pre:
@post:
 *****************************************************************************************/
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
    } else if ( tokenVector[state].getTokenString() == "while" ){
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





/** **************************************************************************************
checks that the follwoing statement is a selection statement that follows BNF rules
@pre: when token starts a if
@post: generate if cst statement
 *****************************************************************************************/
void Parser::selection_statement(){

    //first token should be a if statement.
    if ( tokenVector[state].getTokenString() == "if"){
        expect(tokenVector[state].getTokenString());
    }

    //then a open paren
    expect("(");
    //bool expression
    boolean_expression();
    //closed paren
    expect(")");
    // then a block statement
    if (tokenVector[state].isLBrace()) {
        //recurse througho ur block function.
        block_statement();
        if ( tokenVector[state].getTokenString() == "else "){
            expect(tokenVector[state].getTokenString());
            if (tokenVector[state].isLBrace()) {
                block_statement();
            } else {
                statement();
            }
        }
    } else {
        statement();
        if ( tokenVector[state].getTokenString() == "else "){
            expect(tokenVector[state].getTokenString());
            if (tokenVector[state].isLBrace()) {
                block_statement();
            } else {
                statement();
            }
        }
    }

}





/** **************************************************************************************
checks that the follwoing statement is a initialization expression that follows BNF rules
@pre:
@post:
 *****************************************************************************************/
 void Parser::expression(){
    std::cout<<"in expression"<<std::endl;
    if ( tokenVector[state].isBoolTrue() || tokenVector[state].isBoolFalse() ) {
        boolean_expression();
    } else if (tokenVector[state].isInt()) {
        numerical_expression();
    } else if ( tokenVector[state].isIdentifier() && ( ( tokenVector[state+1].isLParen()) )) {
        std::cout<<"in expression, expecting function with paren"<<std::endl;
        expect( tokenVector[state].getTokenString() );
        expect("(");
        expression();
        expect(")");
        return;
    }else if ( tokenVector[state].isIdentifier() && ( ( tokenVector[state+1].isSemicolon() ) )) {
        std::cout<<"in expression, expecting function with paren"<<std::endl;
        expect( tokenVector[state].getTokenString() );
        return;
    }else if (tokenVector[state].isIdentifier()) {
         std::cout<<"in expression, expecting variable"<<std::endl;
            if ( boolean_operator() || relational_expression() ) {
                boolean_expression();
            }
            numerical_expression();
            }
        if ( tokenVector[state].isLParen() ) {
            expect("(");
            expression();
            expect(")");
        }
 }





/** **************************************************************************************
checks that the follwoing statement is a initialization expression that follows BNF rules
@pre:
@post:
 *****************************************************************************************/
void Parser::initialization_statement(){

    if (tokenVector[state].isIdentifier()  && ( tokenVector[state+1].isAssignmentOperator() ) ) {
        expect( tokenVector[state].getTokenString() );
        expect( tokenVector[state].getTokenString() );
        if  (tokenVector[state].isSingleQuote() ){
            // single_quote_string();
        } else if ( tokenVector[state].isDoubleQuote() ){
            // double_quote_string();
        } else {
            expression();
        }
    }
}







/** **************************************************************************************
checks that the follwoing expression is a boolean expression that follows BNF rules
@pre:recieves start to boolean expression
@post: creates cst expression for boolean
 *****************************************************************************************/
void Parser::boolean_expression(){
    std::cout<<"in boolean_expression"<<std::endl;
    // if we find keyword true or false
    if (tokenVector[state].isBoolTrue()){
        expect( "TRUE");
    }else if (tokenVector[state].isBoolFalse()){
        expect( "FALSE");
    //if we find a open paren
    }else if ( tokenVector[state].isLParen() ){
        expect("(");
        // if we find a identifier
        if (tokenVector[state].isIdentifier()) {
            expect( tokenVector[state].getTokenString() );
            boolean_operator();
            boolean_expression();
        }
        expect(")");
    }else{

        numerical_expression();
        if (  relational_expression() ){
            expect( tokenVector[state].getTokenString() );
        }
        numerical_expression();
    }
    std::cout<<"end boolean_expression"<<std::endl;
}



/** **************************************************************************************
checks that the follwoing expression is a numerical expression that follows BNF rules
@pre: is called when we find a neumeric expression for a bool operation
@post:
 *****************************************************************************************/
void Parser::numerical_expression() {
     std::cout<<"in numerical_expression"<<std::endl;

        bool eat = true;
        while(eat){

            //loop is empty, but the heavy lifting is done in the bool checks
            if(tokenVector[state].isLParen()){
                std::cout<<"begin paren inside of neumerical expression"<<std::endl;
                //eat up the parens
                expect("(");
                //we then need to recurse into a new neumeric expression call.
                numerical_expression();
                //then we expect a close paren
                expect(")");

                std::cout<<"end paren inside of neumerical expression"<<std::endl;

            }else{
                eat = numerical_operand();
                //attempt to eat, but not a operand

                if(eat = false && tokenVector[state].isIdentifier()){
                    //call expression function to make function call?
                    expression();
                }


            }

            //then if there is a operator afterward, need to eat that before going to next loop
            eat = numerical_operator();

        }
              
    
}


/** **************************************************************************************
checks if token is a relational_expression
@pre:
@post:
 *****************************************************************************************/
bool Parser::relational_expression(){
    std::cout<<"in relational_expression"<<std::endl;
    return tokenVector[state].isBoolE() || tokenVector[state].isBoolNE() ||
       tokenVector[state].isBoolLT() || tokenVector[state].isBoolGT() ||
       tokenVector[state].isBoolLTE() || tokenVector[state].isBoolGTE();
}





/** **************************************************************************************
checks if token is a boolean operator
@pre:
@post:
 *****************************************************************************************/
bool Parser::boolean_operator(){
    std::cout<<"in boolean_operator"<<std::endl;
    return (tokenVector[state].isBoolOr() || tokenVector[state].isBoolAnd());
}





/** **************************************************************************************
checks  if token is a numerical_operator
@pre:
@post:
 *****************************************************************************************/
bool Parser::numerical_operator(){
    std::cout<<"in numerical_operator"<<std::endl;
    if (tokenVector[state].isPlus()){
        expect("");
        return true;
    }else if (tokenVector[state].isMinus()){
        expect("");
        return true;
    }else if (tokenVector[state].isAsterisk()){
        expect("");
        return true;
    }else if (tokenVector[state].isDivide()){
        expect("");
        return true;
    }else if (tokenVector[state].isModulo()){
        expect("");
        return true;
    }else if (tokenVector[state].isCarot()){
        expect("");
        return true;
    }else{
        std::cout<<"token is not currently a neumerical operator"<<std::endl;
        return false;
    }
}





/** **************************************************************************************
checks if numerical_operand follows BNF
@pre:
@post:
 *****************************************************************************************/
bool Parser::numerical_operand(){
    std::cout<<"in numerical_operand"<<std::endl;
    //if we find a int
    if (tokenVector[state].isIdentifier()) {
        if (tokenVector[state + 1].isLParen()) {
            user_defined_function();
        } else {
            expect( tokenVector[state].getTokenString() );
        }
        return true;
    // if we find a identifier
    } else if (tokenVector[state].isInt()){
        expect( tokenVector[state].getTokenString() );

        return true;
    //if we find a char
    }else if (tokenVector[state].getTokenString() == "getchar"){
        getchar_function();

        return true;
    //if we find a single quote
    }else if (tokenVector[state].isSingleQuote()){
        expect("'");
        if(tokenVector[state].isChar()){
            expect( tokenVector[state].getTokenString() );
        }else if (tokenVector[state].isEscChar()){
            expect( tokenVector[state].getTokenString() );
        }else{
            //ERROR ERROR ERROR ERROR ERROR ERROR
        }
        expect("'");

        return true;
    //if we find a double quote
    }else if (tokenVector[state].isDoubleQuote()){
        expect("\"");
        if(tokenVector[state].isChar()){
            expect( tokenVector[state].getTokenString() );
        }else if (tokenVector[state].isEscChar()){
            expect( tokenVector[state].getTokenString() );
        }else{
            //ERROR ERROR ERROR ERROR ERROR ERROR
        }
        expect("\"");
        return true;
    } else {
        //ERROR ERROR ERROR ERROR ERROR ERROR ERROR
        return false;
    }
}






/** **************************************************************************************
returns if token is a datatype specifier
@pre:
@post:
 *****************************************************************************************/
bool Parser::datatype_specifier(){
    return (tokenVector[state].getTokenString() == "int" || tokenVector[state].getTokenString() == "char" ||
                                                        tokenVector[state].getTokenString() == "bool");
}





/** **************************************************************************************
checks if token is followed by identifier list or array lsit
@pre:
@post:
 *****************************************************************************************/
void Parser::identifier_and_identifier_array_list() {
    std::cout<<"in identifier_and_identifier_array_list"<<std::endl;
    if (tokenVector[state].isIdentifier() && tokenVector[state + 1].isLBracket()) {
        identifier_array_list();
    } else if (tokenVector[state].isIdentifier() && tokenVector[state + 1].isComma()) {
        identifier_list();
        
    } else if (tokenVector[state].isIdentifier() && tokenVector[state + 1].isSemicolon()){
        identifier();
        return;
    }else {
        throw std::runtime_error("Expected an identifier, but got '" + tokenVector[state].getTokenString() + "'");
    }
}





/** **************************************************************************************
checks identifier  list follows BNF rules
@pre:
@post:
 *****************************************************************************************/
void Parser::identifier_list() {

    if ( tokenVector[state].isIdentifier() && tokenVector[state + 1].isComma() ) {
        expect(tokenVector[state].getTokenString());
        expect(",");
        identifier_list();
    }else if( tokenVector[state].isIdentifier()){
        expect(tokenVector[state].getTokenString());
    }
}





/** **************************************************************************************
checks identifier array list follows BNF rules
@pre:
@post:
 *****************************************************************************************/
void Parser::identifier_array_list() {
    expect( tokenVector[state].getTokenString() );
    expect( "[" );
    if (tokenVector[state].isInt()) {
        expect(tokenVector[state].getTokenString());
    }
    expect( "]" );
    if ( tokenVector[state].isComma() ) {
        identifier_array_list();
    }
}





/** **************************************************************************************
checks if token is identifier
@pre:
@post:
 *****************************************************************************************/
void Parser::identifier(){
    if (!tokenVector[state].isIdentifier()){
        throw std::runtime_error("Expected an identifier, but got '" + tokenVector[state].getTokenString() + "'");
    }
    expect( tokenVector[state].getTokenString() );
}






/** **************************************************************************************
If the token string matches expected_value then it adds it to the CST tree
@pre:
@post:
 *****************************************************************************************/
void Parser::expect(const std::string& expected_value) {
    //protective if to ensure we are within out vector of tokens.
    if (state < tokenVector.size()) {
        //make a token defined with the current token in our vector at our state position.
        Token token = tokenVector[state];
        tokenVector[state].print();        //Use this to debug and know which token youre at
        if (token.getTokenString() != expected_value) {
            throw std::runtime_error("Expected '" + expected_value + "' but got: '" + tokenVector[state].getTokenString()  + "'");
        }

        if (state == 0){
            cst->setRoot(new CSTNode(tokenVector[state]));
        }else if (token.isLBrace() ){
            newStatement = true;
            cst->addChild( cst->getRoot(), token );
        }else if ( token.isSemicolon() ){
            newStatement = true;
            cst->addSibling( cst->getRoot(), token );
        }else if (newStatement){
            cst->addChild( cst->getRoot(), token );
            newStatement = false;
        } else {
            cst->addSibling( cst->getRoot(), token ) ;
        }
        state++;
    } else {
        throw std::runtime_error("Unexpected end of input");
    }
}




