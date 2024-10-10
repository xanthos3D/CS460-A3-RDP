//
// Created by xmani on 2/2/2023.
//
//
// Created by Ali Kooshesh on 1/22/23.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"
//simple function that puts the fstream into the object
Tokenizer::Tokenizer(std::string name) : lineNumber{ 1 },
                                         charPosition{ 1 },
                                         inputFileName{ name } {
    inputStream.open(inputFileName, std::ios::in);  // open the input file. We will make sure that it is open in getToken.
}

//used to determine if a character is important, aka is a token we care about.
// for the case of our tokenizer there will be a lot.
bool Tokenizer::charOfInterest(char c) {

    //fill out the list of character we want to look out for when looping
    //if char is a number or a alpha character then we found a craf of interest
    if(state == 0){

        //if we find a alpha character then we are likely working with a variable
        // note variables cant start with a number.
        if (isalnum(c)) {
            return true;
        }else if(c == '*') {
            return true;
        }else if(c == '-'){
            return true;
        }else if(c == '+'){
            return true;
        }else if(c == '/'){
            return true;
        }else if(c == '%'){
            return true;
        }else if(c == '^'){
            return true;
        }else if(c == '='){
            return true;
        }else if(c == '('){
            return true;
        }else if(c == ')'){
            return true;
        }else if(c == '\"'){
            return true;
        }else if(c == '\''){
            return true;
        }else if(c == ';'){
            return true;
        }else if(c == ','){
            return true;
        }else if(c == '['){
            return true;
        }else if(c == ']'){
            return true;
        }else if(c == '{'){
            return true;
        }else if(c == '}'){
            return true;
        }else if(c == '>'){
            return true;
        }else if(c == '<'){
            return true;
        }else if(c == '&'){
            return true;
        }else if(c == '|'){
            return true;
        }
    }else if(state == 1){

    }


    return false;
}

// function that makes the token.
Token Tokenizer::getToken() {
    // temp char to store input comming from the fstream
    char c;
    //temp string variable for storing tag.
    std::string tempText = "";

    // check to see if file is readable, if not printer error message.
    if (!inputStream.is_open()) {
        std::cout << "Tokenizer::getToken() called with a stream that is not open." << std::endl;
        std::cout << "Make sure that " << inputFileName << " exists and is readable. Terminating.";
        exit(2);
    }

    // makes a token with the given line number and char position.
    Token token(lineNumber, charPosition);

    //state 0 of our dfa, havent seen any input or returning from previus states. grabs identifiers
    if(state == 0){

        //std::cout<<"looping through on state 0"<<std::endl;
        //this eats up all non esential input we dont care about, such as spaces not inside a string token.
        while (inputStream.get(c) && !charOfInterest(c)) {
            charPosition++;
            if(c == '\n'){
                lineNumber++;
            }
        }

        if (inputStream.eof()) {
            token.setEndOfFile();
            return token;
            //found a identifier
        }else if(isalpha(c)){
            //std::cout<<"found identifier"<<std::endl;
            //grab all the rest of the characters in the identifier
            tempText += c;
            while (inputStream.get(c) && c != ' ' && c != '(' && c != ')'&& c != '[' && c != ']'&& c != '{' && c != '}' && c != ';'&& c != ',') {
                tempText += c;
                charPosition++;
            }

            inputStream.putback(c);
            token.setIdentifier(tempText);            //set token and return it
            return token;

        }if (isdigit(c)){
            tempText += c;

            while(inputStream.get(c) && isdigit(c)){
                tempText += c;
                charPosition++;
            }
            token.setInt(tempText);
            //error state for digit exclude all input but valid input.
            //man need more statemens in if to correctly protect agianst all cases where a digit is broken by a symbol
            if(isalpha(c)){

                std::cout<<"Syntax error on line "<<lineNumber<<": invalid integer"<<std::endl;
                exit(1);
            }

            inputStream.putback(c);
            return token;

        }else if(c == '('){
            token.setLParen();
            token.setTokenString( "(");
            return token;
        }else if(c == ')'){
            token.setRParen();
            token.setTokenString( ")");
            return token;
        }else if(c == '{'){
            token.setLBrace();
            token.setTokenString( "{");
            return token;
        }else if(c == '}'){
            token.setRBrace();
            token.setTokenString( "}");
            return token;
        }else if(c == '['){
            token.setLBracket();
            token.setTokenString("[");
            return token;
        }else if(c == ']'){
            token.setRBracket();
            token.setTokenString("]");
            return token;
        }else if(c == ';'){
            token.setSemicolon();
            token.setTokenString( ";");
            return token;
        }else if(c == '='){
            //need a special case where if there is a equal after this then
            if(inputStream.peek() == '='){
                //eat up next input
                inputStream.get(c);
                token.setBoolE();
            }else{
                token.setAssignmentOperator();
            }
            token.setTokenString( "=");
            return token;
        }else if(c == '-'){
            tempText = '-';
            inputStream.get(c);
            if (isdigit(c)){
                tempText += c;

                while(inputStream.get(c) && isdigit(c)){
                    tempText += c;
                    charPosition++;
                }
                token.setInt(tempText);
            }else{
                token.setMinus();
                token.setTokenString("-");
            }
            inputStream.putback(c);
            return token;
        }else if(c == '\"'){
            token.setDoubleQuote();
            token.setTokenString("\"");
            state = 1;
            return token;
        }else if(c == '\''){
            token.setSingleQuote();
            token.setTokenString("\'");
            state = 3;
            return token;
        }else if(c == ','){
            token.setComma();
            token.setTokenString( ",");
            return token;
        }else if(c == '%'){
            token.setModulo();
            token.setTokenString("%");
            return token;
        }else if(c == '*'){
            token.setAsterisk();
            token.setTokenString("*");
            return token;
        }else if(c == '+'){
            tempText = '+';
            inputStream.get(c);
            if (isdigit(c)){
                tempText += c;

                while(inputStream.get(c) && isdigit(c)){
                    tempText += c;
                    charPosition++;
                }
                token.setInt(tempText);
            }else{
                token.setPlus();
                token.setTokenString("+");
            }
            inputStream.putback(c);
            return token;
        }else if(c == '>'){
            std::cout<<"found > "<<std::endl;
            if(inputStream.peek() == '='){
                //eat up next input
                inputStream.get(c);
                token.setBoolGTE();
                token.setTokenString(">=");
            }else{
                token.setBoolGT();
                token.setTokenString(">");
            }

            return token;
        }else if(c == '<'){
            if(inputStream.peek() == '='){
                //eat up next input
                inputStream.get(c);
                token.setBoolLTE();
                token.setTokenString("<=");
            }else{
                token.setBoolLT();
                token.setTokenString("<");
            }
            return token;
        }else if(c == '&'){
            if(inputStream.peek() == '&'){
                //eat up next input
                inputStream.get(c);
                token.setBoolAnd();
                token.setTokenString("&&");
            }else{
                std::cout<< "malformed && on line: "<<lineNumber<<" position: "<<charPosition<<std::endl;
                exit(1);
            }

            return token;
        }else if(c == '|'){
            if(inputStream.peek() == '|'){
                //eat up next input
                inputStream.get(c);
                token.setBoolOr();
                token.setTokenString("||");
            }else{
                std::cout<< "malformed || on line: "<<lineNumber<<" position: "<<charPosition<<std::endl;
                exit(1);
            }

            return token;
        }else if(c == '/'){
            token.setDivide();
            token.setTokenString("/");
            return token;
            // additional case for ! and !=
        }else if(c == '!'){
            std::cout<<"found > "<<std::endl;
            if(inputStream.peek() == '='){
                //eat up next input
                inputStream.get(c);
                token.setBoolNE();
                token.setTokenString("!=");
            }else{
                token.setBoolNot();
                token.setTokenString("!");
            }

            return token;
            //this is null at the end of string - in the case of an unclosed string (missing ending double quote)
        }else if (c == '\000' || c == '\0'){
            std::cout << "End of String" << std::endl;
            token.setEndOfFile();

            return token;
        }

        //state if we find a double quote chane the way we take in tokens to treat everything after the first quote
        //and change state once we find another double quote token.
    }else if(state == 1){
        while (inputStream.get(c) && c != '\"') {
            tempText += c;
            charPosition++;
            if (c == '\\'){
                inputStream.get(c);
                tempText += c;
                charPosition++;
            }
        }

        inputStream.putback(c);
        //We found an end double quote, so we can't return to state 0 yet because that looks for opening double quote
        //state 2 takes on a closing double quote
        state = 2;
        //set token and return it
        token.setString(tempText);

        return token;
    }else if(state == 2){
        inputStream.get(c);
        token.setDoubleQuote();
        token.setTokenString("\"");
        state = 0;
        return token;

        //state to handle interior of single quotes.
    }else if(state == 3){
        while (inputStream.get(c) && c != '\'') {
            tempText += c;
            charPosition++;
        }

        inputStream.putback(c);
        //We found an end double quote, so we can't return to state 0 yet because that looks for opening double quote
        //state 2 takes on a closing double quote
        state = 4;
        //set token and return it
        token.setString(tempText);

        return token;
        //state to handle ending '
    }else if(state == 4){
        inputStream.get(c);
        token.setSingleQuote();
        state = 0;
        return token;
    }


    // after this point is where our DFA will be defined. we want various state to acount for scenarious like string tokens,
    // this tokenizer class has a int named state which we should use to keep track of the state as we tokenize.
    std::cout<< state << std::endl;

    //if we have reached the end of the file then we set the bool of current token to reflect that then we return token.
    if (inputStream.eof()) {
        token.setEndOfFile();
        return token;
    }else{
        // The condition of one of the above "if ... else if" statements should
        // evaluate to programming_assignment_3-test_file_4.c because function charOfInterest return programming_assignment_3-test_file_4.c when it
        // finds a character that is the start of some token and that the
        // "if ... else if" has one case for each character that starts a token.
        // Therefore, the control should never get here. However, it is always
        // a good idea to have an "else" when you have a sequence of "if ... else if"
        // statements so that you do not end up with a case that has not been handled.
        // For example, you forgot to account for one of the cases.
        std::cout << "There is a logic error in function Tokenizer::getToken.\n";
        std::cout << "The function doesn't know how to process this character: ->" << c << "<-\n";
        exit(1);
    }

    //we should never return a token using this statement.
    return token;
}





