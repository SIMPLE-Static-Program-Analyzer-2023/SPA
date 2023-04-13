#include "sp/Lexer.h"

#include "catch.hpp"
//#include <istream>
#include <sstream>

using namespace std;

TEST_CASE("basic test with spaces and new line") {
    string input = string("hello       world23     \n        2332wzasd   \n  asd");
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    Lexer l = Lexer(iss);
    REQUIRE(l.getCurrentToken() == "hello");
    REQUIRE(l.getTokenType() == TokenType::IDENT);
    REQUIRE(l.getLineNo() == 1);
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "world23");
    REQUIRE(l.getTokenType() == TokenType::IDENT);
    REQUIRE(l.getLineNo() == 1);
    l.advanceSourceToken();

    // tokenizing a constant 2332 out of "2332wzasd" is intended
    // no problem due to how parser uses lexer to check syntax
    REQUIRE(l.getCurrentToken() == "2332");
    REQUIRE(l.getTokenType() == TokenType::NUMBER);
    REQUIRE(l.getLineNo() == 2);
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "wzasd");
    REQUIRE(l.getTokenType() == TokenType::IDENT);
    REQUIRE(l.getLineNo() == 2);
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "asd");
    REQUIRE(l.getTokenType() == TokenType::IDENT);
    REQUIRE(l.getLineNo() == 3);
}

TEST_CASE("delimiters = {} () ; ") {
    string input = string("procedure proc { x = (y); \n read x; \n  x = 333; }");
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    Lexer l = Lexer(iss);
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "{");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "=");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "(");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "y");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == ")");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == ";");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "read");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == ";");
    l.advanceSourceToken();
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "333");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "}");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.isEnd());
}

TEST_CASE("minified") {
    string input = string("procedure proc{x=(y);read x;x=333;}");
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    Lexer l = Lexer(iss);
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "{");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "=");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "(");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "y");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == ")");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == ";");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "read");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == ";");
    l.advanceSourceToken();
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "333");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "}");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.isEnd());
}

TEST_CASE("operators: + - * / % > < >= <= == && ||") {
    string input = string(" > < >=  \n <=  3 +  3 - 3  / 3 %  3     ==  && \n ||");
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    Lexer l = Lexer(iss);
    REQUIRE(l.getCurrentToken() == ">");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "<");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == ">=");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "<=");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "+");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "-");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "/");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "%");
    l.advanceSourceToken();
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "==");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "&&");
    l.advanceSourceToken();
    REQUIRE(l.getCurrentToken() == "||");
    l.advanceSourceToken();
    REQUIRE(l.isEnd());
}

TEST_CASE("eat and match methods") {
    string input = string("procedure proc { x = y;  \n  x = 333; \n read  = 1; }");
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    Lexer l = Lexer(iss);
    l.match("procedure");
    REQUIRE(l.getTokenType() == TokenType::IDENT);
    l.eat("procedure");
    REQUIRE(l.matchIdentifier());
    REQUIRE(l.getTokenType() == TokenType::IDENT);
    REQUIRE(l.eatIdentifier() == "proc");
    REQUIRE(l.getTokenType() == TokenType::OTHERS);
    l.eat("{");
    REQUIRE(l.matchIdentifier());
    REQUIRE(l.getTokenType() == TokenType::IDENT);
    REQUIRE(l.eatIdentifier() == "x");
    REQUIRE(l.getTokenType() == TokenType::OTHERS);
    l.eat("=");
    l.eatIdentifier();
    REQUIRE(l.getTokenType() == TokenType::OTHERS);
    l.eat(";");
    l.eatIdentifier();
    REQUIRE(l.getTokenType() == TokenType::OTHERS);
    l.eat("=");
    REQUIRE(l.getTokenType() == TokenType::NUMBER);
    l.matchConstant();
    REQUIRE(l.eatConstant() == "333");
    l.eat(";");
    REQUIRE(l.eatIdentifier() == "read");
    l.eat("=");
    REQUIRE(l.getTokenType() == TokenType::NUMBER);
    l.matchConstant();
    REQUIRE(l.eatConstant() == "1");
}

TEST_CASE("peekChar") {
    string input = string(" read     \n  = read +1; ");
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    Lexer l = Lexer(iss);
    REQUIRE(l.peekChar() == '=');

    string input2 = string(" read      \n              = read +1; ");
    shared_ptr<istringstream> iss2 = make_shared<istringstream>(input2);
    Lexer l2 = Lexer(iss2);
    REQUIRE(l2.peekChar() == '=');
}

TEST_CASE("conditional expressions") {
    string input = string("if( ( ! (3 > 4)) || (3 > 4) )");
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    Lexer l = Lexer(iss);
    l.eat("if");
    l.eat("(");
    l.eat("(");
    l.eat("!");
    l.eat("(");
    REQUIRE(l.eatConstant() == "3");
    l.eat(">");
    REQUIRE(l.eatConstant() == "4");
    l.eat(")");
    l.eat(")");
    l.eat("||");
    l.eat("(");
    REQUIRE(l.eatConstant() == "3");
    l.eat(">");
    REQUIRE(l.eatConstant() == "4");
    l.eat(")");
    l.eat(")");
    REQUIRE(l.isEnd());
}

TEST_CASE("conditional expressions minified") {
    string input = string("if((!(3 > 4))||(3>4))");
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    Lexer l = Lexer(iss);
    l.eat("if");
    l.eat("(");
    l.eat("(");
    l.eat("!");
    l.eat("(");
    REQUIRE(l.eatConstant() == "3");
    l.eat(">");
    REQUIRE(l.eatConstant() == "4");
    l.eat(")");
    l.eat(")");
    l.eat("||");
    l.eat("(");
    REQUIRE(l.eatConstant() == "3");
    l.eat(">");
    REQUIRE(l.eatConstant() == "4");
    l.eat(")");
    l.eat(")");
    REQUIRE(l.isEnd());
}
