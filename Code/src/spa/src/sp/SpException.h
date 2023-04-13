#pragma once

#include <stdexcept>
#include <string>

class LexerException : public std::runtime_error {
public:
    // fail to match tokens
    explicit LexerException(std::string msg, int line, std::string expectedToken, std::string receivedToken)
            : std::runtime_error("line " + std::to_string(line)
            + ": " + msg + "\n"
            + "expected: " + expectedToken
            + ", got: " + receivedToken) {}

    // invalid identifier/constant
    explicit LexerException(std::string msg, int line, std::string receivedToken)
            : std::runtime_error("line " + std::to_string(line)
            + ": " + msg + "\n"
            + "received: " + receivedToken) {}

    // unknown character
    explicit LexerException(std::string msg, int line, char c)
            : std::runtime_error("line " + std::to_string(line)
            + ": " + msg + "\n"
            + "received char: " + std::string(1, c)
            ) {}
};

class ParserException : public std::runtime_error {
private:
    std::string message;
public:
    explicit ParserException(std::string msg) : std::runtime_error(msg) {}
    explicit ParserException(std::string msg, int line) : std::runtime_error("line " + std::to_string(line)
            + ": " + msg) {}
    explicit ParserException(std::string msg, int line, std::string receivedToken)
    : std::runtime_error("line " + std::to_string(line)
    + ": " + msg + "\n"
    + "received: " + receivedToken) {}
};


// general exception when processing
class DesignExtractorException :public std::runtime_error {
public:
    explicit DesignExtractorException(std::string msg) : std::runtime_error(msg) {}
};
