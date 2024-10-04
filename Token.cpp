//
// Created by xmani on 9/4/2024.
//

#include <iostream>
#include <iomanip>
#include "Token.hpp"

//default constructor, want to give it line data and set up some default vairables apart of the object.
Token::Token(int line, int pos)
        : _lineNumber(line), _charPos(pos),
          //Misc
          _identifier(false), _doubleQuote(false), _singleQuote(false),
          _semicolon(false), _comma(false), _eof(false),
          //Brackets & Parenthesis
          _LParen(false), _RParen(false), _LBrace(false), _RBrace(false),
          _LBracket(false), _RBracket(false),
          //Operators
          _assignmentOperator(false), _modulo(false),
          _plus(false), _minus(false), _asterisk(false), _divide(false), _caret(false),
          //Data Types
          _int(false), _double(false), _char(false), _string(false), _escChar(false),
          //Boolean Ops
          _boolE(false), _boolNE(false), _boolGT(false), _boolLT(false),
          _boolGTE(false), _boolLTE(false), _boolAnd(false), _boolOr(false),
          _boolTrue(false), _boolFalse(false),
          _tokenString("") {}


//set of return functions to tell if a token is if a given type.
bool& Token::isIdentifier(){ return _identifier;}
bool& Token::isDoubleQuote(){return _doubleQuote;}
bool& Token::isSingleQuote(){return _singleQuote;}
bool& Token::isSemicolon(){return _semicolon;}
bool& Token::isComma(){return _comma;}
bool& Token::isEOF(){return _eof;}

bool& Token::isLParen(){return _LParen;}
bool& Token::isRParen(){return _RParen;}
bool& Token::isLBrace(){return _LBrace;}
bool& Token::isRBrace(){return _RBrace;}
bool& Token::isLBracket(){return _LBracket;}
bool& Token::isRBracket(){return _RBracket;}

bool& Token::isAssignmentOperator(){return _assignmentOperator;}
bool& Token::isModulo(){return _modulo;}
bool& Token::isPlus(){return _plus;}
bool& Token::isMinus(){return _minus;}
bool& Token::isAsterisk(){return _asterisk;}
bool& Token::isDivide(){return _divide;}
bool& Token::isCarot(){return _caret;}

bool& Token::isInt(){return _int;}
bool& Token::isDouble(){return _double;}
bool& Token::isChar(){return _char;}
bool& Token::isEscChar() {return _escChar;}

bool& Token::isString(){return _string;}

bool& Token::isBoolE(){return _boolE;}
bool& Token::isBoolNot(){return _boolNot;}
bool& Token::isBoolNE(){return _boolNE;}
bool& Token::isBoolGT(){return _boolGT;}
bool& Token::isBoolLT(){return _boolLT;}
bool& Token::isBoolGTE(){return _boolGTE;}
bool& Token::isBoolLTE(){return _boolLTE;}
bool& Token::isBoolAnd(){return _boolAnd;}
bool& Token::isBoolOr(){return _boolOr;}
bool& Token::isBoolTrue(){return _boolTrue;}
bool& Token::isBoolFalse(){return _boolFalse;}



//create setter functions
void Token::setIdentifier(std::string input) { 
    _identifier = true;
    _tokenString = input;
 }
void Token::setDoubleQuote(){ _doubleQuote = true; }
void Token::setSingleQuote(){ _singleQuote = true; }
void Token::setSemicolon(){ _semicolon = true; }
void Token::setComma(){ _comma = true; }
void Token::setEndOfFile(){ _eof = true; }

void Token::setLParen(){ _LParen = true; }
void Token::setRParen(){ _RParen = true; }
void Token::setLBrace(){ _LBrace = true; }
void Token::setRBrace(){ _RBrace= true; }
void Token::setLBracket(){ _LBracket = true; }
void Token::setRBracket(){ _RBracket = true; }

void Token::setAssignmentOperator() { _assignmentOperator = true; }
void Token::setModulo(){ _modulo = true; }
void Token::setPlus(){ _plus = true; }
void Token::setMinus(){ _minus = true; }
void Token::setAsterisk(){ _asterisk = true; }
void Token::setDivide(){ _divide = true; }
void Token::setCarot(){ _caret = true; }

void Token::setInt(std::string input) { 
    _int = true;
    _tokenString = input;
}
void Token::setDouble(std::string input){
     _double = true; 
     _tokenString = input;
}
void Token::setChar(std::string input){
     _char = true;
     _tokenString = input;
}

void Token::setEscChar(std::string input){
    _escChar = true;
    _tokenString = input;

}

void Token::setString(std::string input){
     _string = true;
     _tokenString = input;
}

void Token::setBoolE(){ _boolE= true; }
void Token::setBoolNot(){ _boolNot= true; }
void Token::setBoolNE(){ _boolNE= true; }
void Token::setBoolGT(){ _boolGT= true; }
void Token::setBoolLT(){ _boolLT= true; }
void Token::setBoolGTE(){ _boolGTE= true; }
void Token::setBoolLTE(){ _boolLTE= true; }
void Token::setBoolAnd(){ _boolAnd= true; }
void Token::setBoolOr(){ _boolOr= true; }
void Token::setBoolTrue(){ _boolTrue= true; }
void Token::setBoolFalse(){ _boolFalse= true; }

//or we want to return the token string.
std::string Token::getTokenString() { return _tokenString; }

void Token::setTokenString(std::string dataString) {
    _tokenString = dataString;
}

//and ofc a function to print out tokens.
std::string Token::print() {

    //spits out line number and position of token.
    //std::cout << "[" << std::setw(2) << _lineNumber << ", " << std::setw(3) << _charPos << "] ";

    std::string output = "";
    //its a + opperator just print out + dont store anything in the string for that token
    if (isEOF()) {

    }else{
        std::cout<<std::endl;
        std::cout<< "Token type: ";
        output+= "Token type: ";

        if(isIdentifier()){
            std::cout<< "IDENTIFIER" <<std::endl;
            std::cout<<"Token:      "<<_tokenString<<std::endl;

            output+= "IDENTIFIER\n";
            output+= "Token:      ";
            output+= _tokenString;
            output+= "\n";
        }else if(isDoubleQuote()){
            std::cout<< "DOUBLE_QUOTE" <<std::endl;
            std::cout<<"Token:      "<<"\""<<std::endl;

            output+= "DOUBLE_QUOTE\n";
            output+= "Token:      ";
            output+= "\"";
            output+= "\n";
        }else if(isSingleQuote()){
            std::cout<< "SINGLE_QUOTE" <<std::endl;
            std::cout<<"Token:      "<<"'"<<std::endl;

            output+= "SINGLE_QUOTE\n";
            output+= "Token:      ";
            output+= "'";
            output+= "\n";
        }else if(isSemicolon()){
            std::cout<< "SEMICOLON" <<std::endl;
            std::cout<<"Token:      "<<";"<<std::endl;

            output+= "SEMICOLON\n";
            output+= "Token:      ";
            output+= ";";
            output+= "\n";
        }else if(isComma()){
            std::cout<< "COMMA" <<std::endl;
            std::cout<<"Token:      "<<","<<std::endl;

            output+= "COMMA\n";
            output+= "Token:      ";
            output+= ",";
            output+= "\n";
        }else if(isLParen()){
            std::cout<< "LEFT_PARENTHESIS" <<std::endl;
            std::cout<<"Token:      "<<"("<<std::endl;

            output+= "LEFT_PARENTHESIS\n";
            output+= "Token:      ";
            output+= "(";
            output+= "\n";
        }else if(isRParen()){
            std::cout<< "RIGHT_PARENTHESIS" <<std::endl;
            std::cout<<"Token:      "<<")"<<std::endl;

            output+= "RIGHT_PARENTHESIS\n";
            output+= "Token:      ";
            output+= ")";
            output+= "\n";
        }else if(isLBrace()){
            std::cout<< "LEFT_BRACE" <<std::endl;
            std::cout<<"Token:      "<<"{"<<std::endl;

            output+= "LEFT_BRACE\n";
            output+= "Token:      ";
            output+= "{";
            output+= "\n";
        }else if(isRBrace()){
            std::cout<< "RIGHT_BRACE" <<std::endl;
            std::cout<<"Token:      "<<"}"<<std::endl;

            output+= "Right_BRACE\n";
            output+= "Token:      ";
            output+= "}";
            output+= "\n";
        }else if(isLBracket()){
            std::cout<< "LEFT_BRACKET" <<std::endl;
            std::cout<<"Token:      "<<"["<<std::endl;

            output+= "LEFT_BRACKET\n";
            output+= "Token:      ";
            output+= "[";
            output+= "\n";
        }else if(isRBracket()){
            std::cout<< "RIGHT_BRACKET" <<std::endl;
            std::cout<<"Token:      "<<"]"<<std::endl;

            output+= "RIGHT_BRACKET\n";
            output+= "Token:      ";
            output+= "]";
            output+= "\n";
        }else if(isAssignmentOperator()){
            std::cout<< "ASSIGNMENT_OPERATOR" <<std::endl;
            std::cout<<"Token:      "<<"="<<std::endl;

            output+= "ASSIGNMENT_OPERATOR\n";
            output+= "Token:      ";
            output+= "=";
            output+= "\n";
        }else if(isModulo()){
            std::cout<< "MODULUS" <<std::endl;
            std::cout<<"Token:      "<<"%"<<std::endl;

            output+= "MODULUS\n";
            output+= "Token:      ";
            output+= "%";
            output+= "\n";
        }else if(isPlus()){
            std::cout<< "PLUS" <<std::endl;
            std::cout<<"Token:      "<<"+"<<std::endl;

            output+= "PLUS\n";
            output+= "Token:      ";
            output+= "+";
            output+= "\n";
        }else if(isMinus()){
            std::cout<< "MINUS" <<std::endl;
            std::cout<<"Token:      "<<"-"<<std::endl;

            output+= "MINUS\n";
            output+= "Token:      ";
            output+= "-";
            output+= "\n";
        }else if(isAsterisk()){
            std::cout<< "ASTERISK" <<std::endl;
            std::cout<<"Token:      "<<"*"<<std::endl;

            output+= "ASTERISK\n";
            output+= "Token:      ";
            output+= "*";
            output+= "\n";
        }else if(isDivide()){
            std::cout<< "DIVISION" <<std::endl;
            std::cout<<"Token:      "<<"/"<<std::endl;

            output+= "DIVISION\n";
            output+= "Token:      ";
            output+= "/";
            output+= "\n";
        }else if(isCarot()){
            std::cout<< "CARET" <<std::endl;
            std::cout<<"Token:      "<<"^"<<std::endl;

            output+= "CARET\n";
            output+= "Token:      ";
            output+= "^";
            output+= "\n";

        }else if(isInt()){
            std::cout<< "INTEGER" <<std::endl;
            std::cout<<"Token:      "<<_tokenString<<std::endl;

            output+= "INTEGER\n";
            output+= "Token:      ";
            output+= _tokenString;
            output+= "\n";

        }else if(isDouble()){
            std::cout<< "DOUBLE" <<std::endl;
            std::cout<<"Token:      "<<_tokenString<<std::endl;

            output+= "DOUBLE\n";
            output+= "Token:      ";
            output+= _tokenString;
            output+= "\n";
        }else if(isChar()){
            std::cout<< "CHAR" <<std::endl;
            std::cout<<"Token:      "<<_tokenString<<std::endl;

            output+= "CHAR\n";
            output+= "Token:      ";
            output+= _tokenString;
            output+= "\n";
        }else if(isEscChar()){
            std::cout<< "ESC CHAR" <<std::endl;
            std::cout<<"Token:      "<<_tokenString<<std::endl;

            output+= "ESC CHAR\n";
            output+= "Token:      ";
            output+= _tokenString;
            output+= "\n";
        }else if(isString()){
            std::cout<< "STRING" <<std::endl;
            std::cout<<"Token:      "<<_tokenString<<std::endl;

            output+= "STRING\n";
            output+= "Token:      ";
            output+= _tokenString;
            output+= "\n";
        }else if(isBoolE()){
            std::cout<< "BOOLEAN_EQUAL" <<std::endl;
            std::cout<<"Token:      "<<"=="<<std::endl;

            output+= "BOOLEAN_EQUAL\n";
            output+= "Token:      ";
            output+= "==";
            output+= "\n";
        }else if(isBoolNot()){
            std::cout<< "BOOLEAN_EQUAL" <<std::endl;
            std::cout<<"Token:      "<<"!"<<std::endl;

            output+= "BOOLEAN_EQUAL\n";
            output+= "Token:      ";
            output+= "!";
            output+= "\n";
        }else if(isBoolNE()){
            std::cout<< "BOOLEAN_NOT_EQUAL" <<std::endl;
            std::cout<<"Token:      "<<"!="<<std::endl;

            output+= "BOOLEAN_NOT_EQUAL\n";
            output+= "Token:      ";
            output+= "==";
            output+= "\n";
        }else if(isBoolGT()){
            std::cout<< "BOOLEAN_GREATER_THAN" <<std::endl;
            std::cout<<"Token:      "<<">"<<std::endl;

            output+= "BOOLEAN_GREATER_THAN\n";
            output+= "Token:      ";
            output+= ">";
            output+= "\n";
        }else if(isBoolLT()){
            std::cout<< "BOOLEAN_LESS_THAN" <<std::endl;
            std::cout<<"Token:      "<<"<"<<std::endl;

            output+= "BOOLEAN_LESS_THAN\n";
            output+= "Token:      ";
            output+= "<";
            output+= "\n";
        }else if(isBoolGTE()){
            std::cout<< "BOOLEAN_GREATER_THAN_OR EQUAL" <<std::endl;
            std::cout<<"Token:      "<<">="<<std::endl;

            output+= "BOOLEAN_GREATER_THAN_OR EQUAL\n";
            output+= "Token:      ";
            output+= ">=";
            output+= "\n";
        }else if(isBoolLTE()){
            std::cout<< "BOOLEAN_LESS_THAN_OR EQUAL" <<std::endl;
            std::cout<<"Token:      "<<"<="<<std::endl;

            output+= "BOOLEAN_LESS_THAN_OR EQUAL\n";
            output+= "Token:      ";
            output+= "<=";
            output+= "\n";
        }else if(isBoolAnd()){
            std::cout<< "BOOLEAN_AND" <<std::endl;
            std::cout<<"Token:      "<<"&&"<<std::endl;

            output+= "BOOLEAN_AND\n";
            output+= "Token:      ";
            output+= "&&";
            output+= "\n";
        }else if(isBoolOr()){
            std::cout<< "BOOLEAN_OR" <<std::endl;
            std::cout<<"Token:      "<<"||"<<std::endl;

            output+= "BOOLEAN_OR\n";
            output+= "Token:      ";
            output+= "||";
            output+= "\n";
        }else if(isBoolTrue()){
            std::cout<< "BOOLEAN_TRUE" <<std::endl;
            std::cout<<"Token:      "<<"True"<<std::endl;

            output+= "BOOLEAN_GREATER_THAN_OR EQUAL\n";
            output+= "Token:      ";
            output+= ">=";
            output+= "\n";
        }else if(isBoolFalse()){
            std::cout<< "BOOLEAN_FALSE" <<std::endl;
            std::cout<<"Token:      "<<"False"<<std::endl;
        }else{
        std::cout << "Token::print -- unknown token.\n" << std::endl;
        exit(3);
        }
       output+= "\n";
    }
    return output;

}
