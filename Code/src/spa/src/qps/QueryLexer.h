#pragma once

#include <string>
#include <list>
#include <istream>
#include <memory>
#include "../sp/Lexer.h"

// enum class TokenType {
//     IDENT,
//     NUMBER,
//     OTHERS,
//     END
// };

class QueryLexer {
private:
    int lineNo = 1;

    std::string currentToken;

    TokenType currentTokenType;

    std::shared_ptr<std::istream> source;

    void eatSpaceAndNewLine();

public:
    void nextToken();

    explicit QueryLexer(std::shared_ptr<std::istream> source);

    bool match(std::string);

    bool matchIdentifier();

    bool matchConstant();

    bool matchExprChar();

    void eat(std::string);

    std::string eatIdentifier();

    std::string eatConstant();

    std::string eatExprChar();

    std::string getCurrentToken();

    int getLineNo();

    TokenType getTokenType();

    char peekChar();

    bool isEnd();
};
