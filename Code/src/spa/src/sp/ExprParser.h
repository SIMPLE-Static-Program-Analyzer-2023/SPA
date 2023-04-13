#pragma once

#include <vector>
#include <string>
#include <stack>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

#include "ast/ExpressionNode.h"
#include "Lexer.h"
#include "SpException.h"

class ExprParser {
private:
    std::shared_ptr<Lexer> lexer;
    int unmatchedBrackets = 0;
    std::vector<std::string> tokens;
    std::vector<TokenType> types;
    std::vector<std::string> postfix;
    std::vector<TokenType> postfixTypes;

    void reset();
    bool isEndOfExpr();
    void infixToPostfix();
    void extractExprTokens();
    friend class TestExprParser;

public:
    explicit ExprParser(std::shared_ptr<Lexer> lexer);
    Expression parseExpression(Node parent, int stmtNo);
    static Expression generateExpression(std::string expressionString);
    static bool isValidExpression(std::string expressionString);
};
