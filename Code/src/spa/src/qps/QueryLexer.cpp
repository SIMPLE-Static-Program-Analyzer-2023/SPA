#include <unordered_set>
#include <cctype>
#include "QueryLexer.h"
#include "QpsException.h"

static const std::unordered_set<char> operatorAndDelimiter({ '{', '}', '(', ')', '+', '-', '*', '/', '%',
                                                ';', '=', '>', '<', '&', '|', '!', ',', '\"', '_', '.', '#'});

std::unordered_set<std::string> exprCharSets = { "(", ")", "+", "-", "*", "/", "%" };

void QueryLexer::eatSpaceAndNewLine() {
    while (isspace(source->peek()) || source->peek() == '\n') {
        if (source->peek() == '\n') {
            lineNo++;
        }
        source->get();
    }
}

void QueryLexer::nextToken() {
    eatSpaceAndNewLine();
    if (source->eof()) {
        currentToken = "";
        currentTokenType = TokenType::END;
        return;
    }

    char nextChar = source->peek();
    std::string newToken = "";
    if (isalpha(nextChar)) {  // first char is a digit, found identifier token
        currentTokenType = TokenType::IDENT;
        while (isalnum(source->peek())) {
            newToken += std::string(1, source->get());
        }
    }
    else if (isdigit(nextChar)) {  // first char is a digit, found number token
        currentTokenType = TokenType::NUMBER;
        while (isdigit(source->peek())) {
            newToken += std::string(1, source->get());
        }
    }
    else if (operatorAndDelimiter.find(nextChar) != operatorAndDelimiter.end()) {
        currentTokenType = TokenType::OTHERS;
        char c = source->get();
        if (c == '&' && source->peek() == '&') {
            // there is no need to check for a single & or |, parser will discover this error
            newToken = "&&";
            source->get();
        }
        else if (c == '|' && source->peek() == '|') {
            newToken = "||";
            source->get();
        }
        else if (c == '=' && source->peek() == '=') {
            newToken = "==";
            source->get();
        }
        else if (c == '!' && source->peek() == '=') {
            newToken = "!=";
            source->get();
        }
        else if (c == '>' && source->peek() == '=') {
            newToken = ">=";
            source->get();
        }
        else if (c == '<' && source->peek() == '=') {
            newToken = "<=";
            source->get();
        }
        else {
            newToken = c;
        }
    }
    else {
        throw QpsSyntaxException("Unknown character found", lineNo, newToken);
    }
    currentToken = newToken;
}


bool QueryLexer::match(std::string s) {
    return currentToken == s;
}

bool QueryLexer::matchIdentifier() {
    return currentTokenType == TokenType::IDENT;
}

bool QueryLexer::matchConstant() {
    return currentTokenType == TokenType::NUMBER;
}

bool QueryLexer::matchExprChar() {
    return exprCharSets.find(currentToken) != exprCharSets.end();
}

void QueryLexer::eat(std::string str) {
    if (!match(str)) {
        throw QpsSyntaxException("current token does not match given string", lineNo, currentToken);
    }
    nextToken();
}

std::string QueryLexer::eatIdentifier() {
    if (!matchIdentifier()) {
        throw QpsSyntaxException("current token is not a valid identifier", lineNo, currentToken);
    }
    std::string curr = currentToken;
    nextToken();
    return curr;
}

std::string QueryLexer::eatConstant() {
    if (!matchConstant()) {
        throw QpsSyntaxException("current token is not a valid constant", lineNo, currentToken);
    }
    std::string curr = currentToken;
    nextToken();
    return curr;
}

std::string QueryLexer::eatExprChar() {
    if (!matchExprChar()) {
        throw QpsSyntaxException("current token is not a valid expression character",
            lineNo, currentToken);
    }
    std::string op = currentToken;
    nextToken();
    return op;
}

std::string QueryLexer::getCurrentToken() {
    return currentToken;
}

int QueryLexer::getLineNo() {
    return lineNo;
}

TokenType QueryLexer::getTokenType() {
    return currentTokenType;
}

char QueryLexer::peekChar() {
    eatSpaceAndNewLine();
    return source->peek();
}

bool QueryLexer::isEnd() {
    return source->eof();
}

QueryLexer::QueryLexer(std::shared_ptr<std::istream> source) : source(source) {
    nextToken();
}
