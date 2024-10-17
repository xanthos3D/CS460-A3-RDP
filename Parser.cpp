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

        //std::cout<<"+++block start token++++++++++++++++++++++++++++++++++++++++++ "<<std::endl;
        //tokenVector[state].print();
        //std::cout<<"+++block start token++++++++++++++++++++++++++++++++++++++++++ "<<std::endl;
        if (tokenVector[state].getTokenString()== "function") {
            //std::cout<<" found function declaration" <<std::endl;
            function_declaration();
        } else if (tokenVector[state].getTokenString() == "procedure") {
            //std::cout<<" found  procedure declaration " <<std::endl;
            procedure_declaration();
        } else if ( datatype_specifier() ) {
            //std::cout<<" found  declaration statement " <<std::endl;
            declaration_statement();
        }else if ( tokenVector[state].getTokenString() == "procedure") { //needs  to be fixed to distinguish between procedure and procedure main
            //std::cout<<" found main procedure" <<std::endl;
            main_procedure();
        }else{
            //something wrong
            //std::cout<<"looked for declaration of function or procedure, but none found?"<<std::endl;
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

    //then we expect an identifier/ function name
    if (tokenVector[state].isIdentifier() ) {
        if (datatype_specifier() || reserved_word()){
            throw std::runtime_error("Syntax error on line " + std::to_string(tokenVector[state].getLineNum()) + ": reserved word \"" +
                                     tokenVector[state].getTokenString() + "\" cannot be used for the name of a function.");
        }
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

    //then an array, can be valid syntax for a param list. so if we find a bracket
    if(tokenVector[state].isLBracket()){
        //call our function to handle an array [i]
        identifier_and_identifier_array_list();
    }

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

    //make a temp token?
    Token token = tokenVector[state];

    //std::cout<<"statement called test: ("<< token.getTokenString()<<")"<<std::endl;

    //cases to recurse through a variety of different statements.
    if (token.getTokenString() == "return"){
        //std::cout<<"return found"<<std::endl;
        return_statement();
    }else if ( token.getTokenString() == "if"){
        //std::cout<<"if found"<<std::endl;
        selection_statement();
    }else if (token.getTokenString() == "else"){
        //std::cout<<"else found"<<std::endl;
        selection_statement();
    }else if (token.getTokenString() == "printf"){
        //std::cout<<"printf found"<<std::endl;
        printf_statement();
    }else if (token.getTokenString() == "for" || token.getTokenString() == "while"){
        //std::cout<<"for/while found"<<std::endl;
        iteration_statement();
    } else if ((token.isIdentifier() && tokenVector[state+1].isAssignmentOperator()) ||
               (token.isIdentifier() && tokenVector[state+1].isLBracket()) ||
               (token.isIdentifier() && tokenVector[state+1].isLParen())){
        if (tokenVector[state+1].isAssignmentOperator() || token.isIdentifier() && tokenVector[state+1].isLBracket()) {
            //std::cout << "assignment found" << std::endl;
            assignment_statement();
        }else if (tokenVector[state+1].isLParen()){
            //std::cout << "user defined func found" << std::endl;
            user_defined_function();
            expect(";");
        }

        //case which handles variable declarizations and assignments.
    } else if ( datatype_specifier() || token.isIdentifier() ) {
        //std::cout<<"declaration found: "<< token.getTokenString()<<std::endl;
        declaration_statement();
    }else{
        std::cout<<"error?"<<std::endl;
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
    //std::cout<<"in declaration_statement"<<std::endl;
    //if we recieve a specifier(data type)
    if ( datatype_specifier() ){
        //add that token to cst
        expect( tokenVector[state].getTokenString() );

        //then we expect an identifier after it.
        if (tokenVector[state].isIdentifier()) {
            identifier();
            //expect(tokenVector[state].getTokenString());
        } else {
            throw;
        }
    }

    //if we see a left bracket we are declaring an array
    if ( tokenVector[state].isLBracket()){
        identifier_and_identifier_array_list();
    }

    //if we see a comma after ward, that means that multiple things are being declared
    if ( tokenVector[state].isComma()){
        expect(",");
        identifier_and_identifier_array_list();
    }
    //expect statement to end with a semicolon.
    if(tokenVector[state].isRParen()){
        return;
    }else{
        expect(";");
    }
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
        double_quoted_string();
        //if there is a comma, variables may be involved, so acount for those here
        if ( tokenVector[state].isComma() ){
            expect(",");
            identifier_and_identifier_array_list();
        }
    }else if (tokenVector[state].isSingleQuote()){
        single_quoted_string();
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
    if (tokenVector[state].isLBracket()){
        expect("[");
        expect( tokenVector[state].getTokenString() );
        expect("]");
    }
    expect("=");

    if (tokenVector[state].isSingleQuote()){
        single_quoted_string();
    }else if (tokenVector[state].isDoubleQuote()) {
        double_quoted_string();
    }else {
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

    //if identifier is a for loop then
    if ( tokenVector[state].getTokenString() == "for") {

        //set for loop to true, so that we handle semicolons correctly within for statement
        inForLoop = true;
        expect(tokenVector[state].getTokenString());
        expect("(");
        //std::cout<<"entering initilization statement for for loop"<<std::endl;
        initialization_statement();
        if (tokenVector[state].isSemicolon()) {
            expect(tokenVector[state].getTokenString());
        }
        //std::cout<<"entering boolean statement for for loop"<<std::endl;
        boolean_expression();
        if (tokenVector[state].isSemicolon()) {
            expect(tokenVector[state].getTokenString());
        }
        //std::cout<<"entering increment expression statement for for loop"<<std::endl;
        expression();
        expect(")");
        if (tokenVector[state].isLBrace()) {
            block_statement();
        } else {
            statement();
        }
        //set for loop to false, so we handle semicolons normally after this.
        inForLoop = false;
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
        if ( tokenVector[state].getTokenString() == "else"){
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
    //std::cout<<"in expression"<<std::endl;
    //if we find a token to be true or false boolean then
    if ( tokenVector[state].isBoolTrue() || tokenVector[state].isBoolFalse() ) {
        //call our boolean expression code
        boolean_expression();
        //if we find a int
    } else if (tokenVector[state].isInt()) {
        //call numeric expression
        numerical_expression();
        //if we find a identifier and a left paren
    } else if ( tokenVector[state].isIdentifier() && ( ( tokenVector[state+1].isLParen()) )) {
        //std::cout<<"in expression, expecting function with paren"<<std::endl;
        //then we found a function and want to encase the next expression in parenthesis
        expect( tokenVector[state].getTokenString() );
        expect("(");
        expression();
        expect(")");
        return;
        //if we find a identifier and therei s a demicolon agter it
    }else if ( tokenVector[state].isIdentifier() && ( ( tokenVector[state+1].isSemicolon() ) )) {
        //std::cout<<"in expression, expecting function with paren"<<std::endl;
        //expect the identifier and then return
        expect( tokenVector[state].getTokenString() );
        return;
        //if we find a identifier and a bracket after it.
    }else if ( tokenVector[state].isIdentifier() && ( ( tokenVector[state+1].isLBracket()))) {
        //then we found an array and expect it to close properly
        //std::cout<<"in expression, expecting function with bracket so array"<<std::endl;
        //eat the identifier
        expect( tokenVector[state].getTokenString() );

        //then handle array with out declaration statement code.
        declaration_statement();
        return;
    }else if (tokenVector[state].isIdentifier()) {
        //std::cout<<"in expression, expecting variable"<<std::endl;

        //if we see a assignment operator after a identifier
        if(tokenVector[state+1].isAssignmentOperator()){
            //eat identifier and then assignment operator
            expect(tokenVector[state].getTokenString());
            expect("=");

            //then call our numerical expression function
            numerical_expression();

        }else if ( relational_expression() || relational_expression() ) {
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
            single_quoted_string();
        } else if ( tokenVector[state].isDoubleQuote() ){
            double_quoted_string();
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

    //std::cout<<"in boolean_expression"<<std::endl;
    // if we find keyword true or false
    if (tokenVector[state].isBoolTrue()){
        expect( "TRUE");

        // then check to see if a operator follows it.
        if(relational_expression()){
            //then another expresion should follow it
            boolean_expression();
        }
    }else if (tokenVector[state].isBoolFalse()){
        expect( "FALSE");

        // then check to see if a operator follows it.
        if(relational_expression()){
            //then another expresion should follow it
            boolean_expression();
        }

        //if we find a identifier
    }else if(tokenVector[state].isBoolNot()){

        //eat !
        expect("!");

        //recursively call our boolean expression
        boolean_expression();

        //if we find a open paren
    }else if(tokenVector[state].isIdentifier()){

        //could be a variable so eat up input
        expect(tokenVector[state].getTokenString());

        //could be an array refrence or a function call
        if(tokenVector[state].isLBracket()||tokenVector[state].isLParen()){
            identifier_and_identifier_array_list();
        }

        //after we figure out what we are working with then check if next symbol is a bool operator
        if(relational_expression()){
            //then another expresion should follow it
            boolean_expression();
            //it could also be a numerical operator which in that case
        }else if(numerical_operator()){
            //then call numericla expression after we eat the numerical operator.
            numerical_expression();
        }

        //if we find a open paren
    }else if(tokenVector[state].isDoubleQuote()){

        // note, do we need to acount for ' (char) ' as well?
        //eat ""
        expect("\"");
        //we expect a string after the first quote
        if(tokenVector[state].isString()){
            //eat string, then quote
            expect(tokenVector[state].getTokenString());
            expect("\"");
        }else{
            std::cout<<"in boolean expression expected a string after a quote but recieved: "<<tokenVector[state].getTokenString()<< std::endl;
        }

        //recursively call our boolean expression
        boolean_expression();

        //if we find a open paren
    }else if (tokenVector[state].isLParen() ){
        //eat paren
        expect("(");

        if(tokenVector[state].isBoolNot()){
            expect("!");

            //recursively call our boolean expression
            boolean_expression();
            //if we find a open paren
        }

        // if we find a identifier
        if (tokenVector[state].isIdentifier()) {
            //could just receive an identifier by its self so recursive call so it's caught by our
            //case above where the identifier could be a array for function call.
            boolean_expression();

        }
        expect(")");

        if(relational_expression()){
            //then another expression should follow it
            boolean_expression();
            //it could also be a numerical operator which in that case
        }


    }else if (tokenVector[state].isSingleQuote() ){
        //eat paren
        single_quoted_string();

        boolean_expression();

    }else{

        //std::cout<<"in else case!"<<std::endl;
        numerical_expression();
        if (  relational_expression() ){
            expect( tokenVector[state].getTokenString() );
        }
        numerical_expression();

        //after we figure out what we are working with then check if next symbol is a operator
        if(relational_expression()){
            //then another expression should follow it
            boolean_expression();
        }
    }
    //std::cout<<"end boolean_expression"<<std::endl;
}



/** **************************************************************************************
checks that the follwoing expression is a numerical expression that follows BNF rules
@pre: is called when we find a neumeric expression for a bool operation
@post:
 *****************************************************************************************/
void Parser::numerical_expression() {
    //std::cout<<"in numerical_expression"<<std::endl;

    bool eat = true;
    while(eat){

        //loop is empty, but the heavy lifting is done in the bool checks
        if(tokenVector[state].isLParen()){
            //std::cout<<"begin paren inside of numerical expression"<<std::endl;
            //eat up the parens
            expect("(");
            //we then need to recurse into a new numeric expression call.
            numerical_expression();
            //then we expect a close paren
            expect(")");

            //std::cout<<"end paren inside of numerical expression"<<std::endl;

        }else{
            eat = numerical_operand();
            //attempt to eat, but not an operand

            if(eat == false && tokenVector[state].isIdentifier()){
                //call expression function to make function call?
                expression();
            }


        }

        //then if there is a operator afterward, need to eat that before going to next loop
        eat = numerical_operator();

        if(relational_expression()){
            boolean_expression();
        }

    }


}


/** **************************************************************************************
checks if token is a relational_expression
@pre:
@post:
 *****************************************************************************************/
bool Parser::relational_expression(){
    //std::cout<<"in relational_expression/boolean_operator"<<std::endl;
    if (tokenVector[state].isBoolE()){
        expect("==");
        return true;
    }else if (tokenVector[state].isBoolNE()){
        expect("!=");
        return true;
    }else if (tokenVector[state].isBoolGT()){
        expect(">");
        return true;
    }else if (tokenVector[state].isBoolLT()){
        expect("<");
        return true;
    }else if (tokenVector[state].isBoolGTE()){
        expect(">=");
        return true;
    }else if (tokenVector[state].isBoolLTE()){
        expect("<=");
        return true;
    }else if (tokenVector[state].isBoolOr()){
        expect("||");
        return true;
    }else if (tokenVector[state].isBoolAnd()){
        expect("&&");
        return true;
    }else{
        //std::cout<<"token is not currently a boolean operator"<<std::endl;
        return false;
    }
}





/** **************************************************************************************
checks if token is a boolean operator
@pre:
@post:
 *****************************************************************************************/
bool Parser::numerical_operator(){
    //std::cout<<"in numerical_operator"<<std::endl;
    if (tokenVector[state].isPlus()){
        expect("+");
        return true;
    }else if (tokenVector[state].isMinus()){
        expect("-");
        return true;
    }else if (tokenVector[state].isAsterisk()){
        expect("*");
        return true;
    }else if (tokenVector[state].isDivide()){
        expect("/");
        return true;
    }else if (tokenVector[state].isModulo()){
        expect("%");
        return true;
    }else if (tokenVector[state].isCarot()){
        expect("^");
        return true;
    }else{
        //std::cout<<"token is not currently a numerical operator"<<std::endl;
        return false;
    }
}

/** **************************************************************************************
checks if numerical_operand follows BNF
@pre:
@post:
 *****************************************************************************************/
bool Parser::numerical_operand(){
    //std::cout<<"in numerical_operand"<<std::endl;
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
        }else if (tokenVector[state].isString()){
            expect( tokenVector[state].getTokenString());
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
returns if token is a datatype specifier
@pre:
@post:
 *****************************************************************************************/
bool Parser::reserved_word(){
    return (tokenVector[state].getTokenString() == "if" || tokenVector[state].getTokenString() == "else" ||
            tokenVector[state].getTokenString() == "void" || tokenVector[state].getTokenString() == "main" ||
            tokenVector[state].getTokenString() == "procedure" || tokenVector[state].getTokenString() == "for" ||
            tokenVector[state].getTokenString() == "while" || tokenVector[state].getTokenString() == "printf" ||
            tokenVector[state].getTokenString() == "getchar" || tokenVector[state].getTokenString() == "return" ||
            tokenVector[state].getTokenString() == "function");
}





/** **************************************************************************************
checks if token is followed by identifier list or array lsit
@pre:
@post:
 *****************************************************************************************/
void Parser::identifier_and_identifier_array_list() {
    //std::cout<<"in identifier_and_identifier_array_list"<<std::endl;
    if (tokenVector[state].isInt()){
        state -= 2;
    }

    //special case, if we end up finding a identifier here then
    if(tokenVector[state].isIdentifier()){
        expect(tokenVector[state].getTokenString());
    }

    //after that identifier is there was one, or if we dont hav to worry about one
    //do we have a bracket, if so then we have a array, go to that function logic
    if ( tokenVector[state].isLBracket()) {
        identifier_array_list();

        //otherwise if its a open paren then we have a function call
    }else if (tokenVector[state].isLParen()) {
        expect("(");
        identifier_list();
        expect(")");

        //if we see a comma by its self then call identifier list. mostelikely called after identifier list was finished with a function call or array[a]
    }else if (tokenVector[state].isComma()) {
        expect(",");

        identifier_list();
    } else if (tokenVector[state].isIdentifier() /*&& tokenVector[state + 1].isSemicolon()*/){
        identifier();

    }else{
        //throw std::runtime_error("Expected an identifier, but got '" + tokenVector[state].getTokenString() + "'");
    }

    //std::cout<<"end identifier_and_identifier_array_list"<<std::endl;
}





/** **************************************************************************************
checks identifier  list follows BNF rules
@pre:
@post:
 *****************************************************************************************/
void Parser::identifier_list() {
    //std::cout<<"in identifier list"<<std::endl;

    //if we find, a identifier, or a data type
    if(tokenVector[state].isIdentifier()){
        //eat that value
        expect(tokenVector[state].getTokenString());

        //then if we find a comma after that we recurse on this function
        if(tokenVector[state].isComma()){
            //std::cout<<"recursively calling identifier list"<<std::endl;
            //eat the comma
            expect(",");
            //recurse to get the next identifier in our list
            identifier_list();
            //found a function call or a array in identifier list we call our identifier_and_identifier_array_list() function
        }else if(tokenVector[state].isLParen() || tokenVector[state].isLBracket()){
            identifier_and_identifier_array_list();
        }
    }else{
        //throw error, potentially called after we found a comma but no proper identifier or data type was found.
    }

}





/** **************************************************************************************
checks identifier array list follows BNF rules
@pre:
@post:
 *****************************************************************************************/
void Parser::identifier_array_list() {
    //std::cout<<" in identifier_array_list"<<std::endl;
    //std::cout<<"token is: "<<tokenVector[state].getTokenString()<<std::endl;

    // ok so we found an array call, we expect to be at the open lbracket
    expect( "[" );

    if (tokenVector[state].getTokenString().find("-") != std::string::npos){
        throw std::runtime_error("Syntax error on line " + std::to_string(tokenVector[state].getLineNum()) + ": array declaration must be a positive integer.");
    }
        //then we expect either an int which we eat
    else if (tokenVector[state].isInt()) {
        expect(tokenVector[state].getTokenString());
        //or an identifier that we eat
    }else if(tokenVector[state].isIdentifier()){
        expect(tokenVector[state].getTokenString());
    }
    //then we expect a closing bracket
    expect( "]" );

    //recursively call our array if there is an extra demention to our array.
    if ( tokenVector[state].isComma() ) {
        identifier_array_list();
    }
    //std::cout<<" ending identifier_array_list"<<std::endl;
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
    if (datatype_specifier() || reserved_word()){
        throw std::runtime_error("Syntax error on line " + std::to_string(tokenVector[state].getLineNum()) + ": reserved word \"" +
                                 tokenVector[state].getTokenString() + "\" cannot be used for the name of a variable.");
    }
    expect( tokenVector[state].getTokenString() );
}


/** **************************************************************************************
checks if single quoted string
@pre:
@post:
 *****************************************************************************************/
void Parser::single_quoted_string(){
    std::cout<<"in single quote"<<std::endl;
    if (!tokenVector[state].isSingleQuote()){
        throw std::runtime_error("Expected a single quote, but got '" + tokenVector[state].getTokenString() + "'");
    }
    expect( "\'");
    expect(tokenVector[state].getTokenString());
    expect("\'");
}

/** **************************************************************************************
checks if double quoted string
@pre:
@post:
 *****************************************************************************************/
void Parser::double_quoted_string(){
    if (!tokenVector[state].isDoubleQuote()){
        throw std::runtime_error("Expected a double quote, but got '" + tokenVector[state].getTokenString() + "'");
    }
    expect( "\"");
    expect(tokenVector[state].getTokenString());
    if (tokenVector[state].isEOF()){
        throw std::runtime_error("Syntax error on line " + std::to_string(tokenVector[state].getLineNum()) + ": unterminated string quote.");
    }
    expect("\"");
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
        //tokenVector[state].print();        //Use this to debug and know which token youre at
        if (token.getTokenString() != expected_value) {
            throw std::runtime_error("Expected '" + expected_value + "' but got: '" + tokenVector[state].getTokenString()  + "' on line " + std::to_string(tokenVector[state].getLineNum()));
        }

        //if state is zero, then make new root
        if (state == 0){
            cst->setRoot(new CSTNode(tokenVector[state]));
            //if we find a left or right brace then make a right child of that token
        }else if (token.isLBrace()|| token.isRBrace() ){
            newStatement = true;
            cst->addChild( cst->getRoot(), token );
            //sibling for semicolon, but only if we arnt in a for loop
        }else if ( token.isSemicolon() && inForLoop == false){
            newStatement = true;
            cst->addSibling( cst->getRoot(), token );

            //sibling if we are in a for loop then we havent entered a new statement.
        }else if ( token.isSemicolon() && inForLoop == true){
            newStatement = false;
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




