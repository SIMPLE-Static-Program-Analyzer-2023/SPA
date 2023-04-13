#pragma once

#include <string>
#include <list>
#include <istream>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <cctype>
#include <queue>
#include "SpException.h"

enum class TokenType {
    IDENT,
    NUMBER,
    OTHERS,
    END
};

class Lexer {
private:
    int lineNo = 1;
    std::string currentToken;
    TokenType currentTokenType;
    std::shared_ptr<std::istream> source;
    std::queue<std::string> tokenBufferQueue;
    std::queue<TokenType> tokenTypeQueue;

    static const std::unordered_set<char> operatorAndDelimiters;
    static const std::unordered_set<std::string> doubleCharTokens;
    static const std::unordered_set<std::string> relOperators;
    static const std::unordered_set<std::string> exprCharSet;

    void eatSpaceAndNewLine();

public:
    explicit Lexer(std::shared_ptr<std::istream> source);

    static const std::unordered_set<std::string> condOperators;
    std::string getCondOpFromBuffer();
    std::string getCondOpFromStream();
    bool isBufferEmpty() const;

    void advanceSourceToken();
    void advanceLexerToken();

    bool match(std::string);
    bool matchIdentifier();
    bool matchConstant();
    bool matchRelOperator();
    bool matchCondOperator();
    bool matchExprChar();

    void eat(std::string);
    std::string eatIdentifier();
    std::string eatConstant();
    std::string eatRelOperator();
    std::string eatCondOperator();
    std::string eatExprChar();

    std::string getCurrentToken();
    TokenType getTokenType();
    int getLineNo();
    char peekChar();
    bool isEnd();
};
