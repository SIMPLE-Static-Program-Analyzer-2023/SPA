#include "Lexer.h"

const std::unordered_set<char> Lexer::operatorAndDelimiters({'{', '}', '(', ')', '+', '-', '*', '/', '%',
                                                      ';', '=', '>', '<', '&', '|', '!'});

const std::unordered_set<std::string> Lexer::doubleCharTokens = {"||", "&&", "==", "!=", ">=", "<="};

const std::unordered_set<std::string> Lexer::relOperators({">", "<", ">=", "<=", "==", "!="});

// "!" is deliberately omitted here because it is matched and eaten directly using Lexer::match
const std::unordered_set<std::string> Lexer::condOperators = {"&&", "||"};

const std::unordered_set<std::string> Lexer::exprCharSet = {"(", ")", "+", "-", "*", "/", "%"};

void Lexer::eatSpaceAndNewLine() {
    while (isspace(source->peek()) || source->peek() == '\n') {
        if (source->peek() == '\n') {
            lineNo++;
        }
        source->get();
    }
}

void Lexer::advanceLexerToken() {
    if (tokenBufferQueue.empty()) {
        advanceSourceToken();
        return;
    }
    tokenBufferQueue.pop();
    tokenTypeQueue.pop();
}

void Lexer::advanceSourceToken() {
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
    } else if (isdigit(nextChar)) {  // first char is a digit, found number token
        currentTokenType = TokenType::NUMBER;
        while (isdigit(source->peek())) {
            newToken += std::string(1, source->get());
        }
    } else if (operatorAndDelimiters.find(nextChar) != operatorAndDelimiters.end()) {
        currentTokenType = TokenType::OTHERS;
        char c = source->get();
        bool isDoubleCharToken = false;

        for (std::string doubleCharToken : doubleCharTokens) {
            if (c == doubleCharToken.at(0) && source->peek() == doubleCharToken.at(1)) {
                newToken = doubleCharToken;
                source->get();
                isDoubleCharToken = true;
                break;
            }
        }

        if (!isDoubleCharToken) {
            newToken = c;
        }
    } else {
        throw LexerException("Unknown character found", lineNo, nextChar);
    }
    currentToken = newToken;
}


bool Lexer::match(std::string s) {
    return getCurrentToken() == s;
}

bool Lexer::matchIdentifier() {
    return getTokenType() == TokenType::IDENT;
}

bool Lexer::matchConstant() {
    return getTokenType() == TokenType::NUMBER;
}

bool Lexer::matchExprChar() {
    return exprCharSet.find(getCurrentToken()) != exprCharSet.end();
}

bool Lexer::matchRelOperator() {
    return relOperators.find(getCurrentToken()) != relOperators.end();
}

bool Lexer::matchCondOperator() {
    return condOperators.find(getCurrentToken()) != condOperators.end();
}

bool Lexer::isBufferEmpty() const {
    return tokenBufferQueue.empty();
}

std::string Lexer::getCondOpFromBuffer() {
    int unmatched = 0;
    bool isEndOfFirstBracket = false;
    std::queue<std::string> bufferCopy = tokenBufferQueue;

    while (!isEndOfFirstBracket) {
        if (bufferCopy.front() == "(") {
            unmatched += 1;
        } else if (bufferCopy.front() == ")") {
            if (unmatched == 1) {
                isEndOfFirstBracket = true;
            } else {
                unmatched -= 1;
            }
        }
        bufferCopy.pop();
    }

    std::string currToken = bufferCopy.front();
    return currToken;
}

std::string Lexer::getCondOpFromStream() {
    int unmatched = 0;
    bool isEndOfFirstBracket = false;

    while (!isEndOfFirstBracket) {
        if (currentToken == "(") {
            unmatched += 1;
        } else if (currentToken == ")") {
            if (unmatched == 1) {
                isEndOfFirstBracket = true;
            } else {
                unmatched -= 1;
            }
        }

        tokenBufferQueue.push(currentToken);
        tokenTypeQueue.push(currentTokenType);
        advanceSourceToken();
    }

    return currentToken;
}

void Lexer::eat(std::string str) {
    if (!match(str)) {
        throw LexerException("current token does not match given string", lineNo, str, currentToken);
    }
    advanceLexerToken();
}

std::string Lexer::eatIdentifier() {
    if (!matchIdentifier()) {
        throw LexerException("current token is not a valid identifier", lineNo, currentToken);
    }
    std::string curr = getCurrentToken();
    advanceLexerToken();
    return curr;
}

std::string Lexer::eatConstant() {
    if (!matchConstant()) {
        throw LexerException("current token is not a valid constant", lineNo, currentToken);
    }
    std::string curr = getCurrentToken();
    advanceLexerToken();
    return curr;
}

std::string Lexer::eatExprChar() {
    if (!matchExprChar()) {
        throw LexerException("current token is not a valid expression character",
                             lineNo, currentToken);
    }
    std::string op = getCurrentToken();
    advanceLexerToken();
    return op;
}

std::string Lexer::eatRelOperator() {
    if (!matchRelOperator()) {
        throw LexerException("current token is not a valid relational operator",
                             lineNo, currentToken);
    }
    std::string op = getCurrentToken();
    advanceLexerToken();
    return op;
}

std::string Lexer::eatCondOperator() {
    if (!matchCondOperator()) {
        throw LexerException("current token is not '&&' or '||' ",
                             lineNo, currentToken);
    }
    std::string op = getCurrentToken();
    advanceLexerToken();
    return op;
}

std::string Lexer::getCurrentToken() {
    if (tokenBufferQueue.empty()) {
        return currentToken;
    }
    return tokenBufferQueue.front();
}

int Lexer::getLineNo() {
    return lineNo;
}

TokenType Lexer::getTokenType() {
    if (tokenTypeQueue.empty()) {
        return currentTokenType;
    }
    return tokenTypeQueue.front();
}

char Lexer::peekChar() {
    eatSpaceAndNewLine();
    return source->peek();
}

bool Lexer::isEnd() {
    return source->eof();
}

Lexer::Lexer(std::shared_ptr<std::istream> source) : source(source) {
    advanceSourceToken();
}
