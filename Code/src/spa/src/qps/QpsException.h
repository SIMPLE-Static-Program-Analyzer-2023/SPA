//
// Created by Vanessa Khor on 13/2/23.
//
#pragma once
#include <stdexcept>
#include <string>

/**
 * Referred to SpException
 */
class QpsSyntaxException : public std::runtime_error {
public:
    std::string msg = "SyntaxError";

    // invalid syntax (e.g. case sensitive errors)
    explicit QpsSyntaxException(std::string msg, int line, std::string expectedToken, std::string receivedToken)
            : std::runtime_error("line " + std::to_string(line)
                                 + ": " + msg + "\n"
                                 + "expected: " + expectedToken
                                 + ", got: " + receivedToken) {};

    // unknown character
    explicit QpsSyntaxException(std::string msg, int line, std::string receivedToken)
            : std::runtime_error("line " + std::to_string(line)
                                 + ": " + msg + "\n"
                                 + "received: " + receivedToken
    ) {};

    explicit QpsSyntaxException(std::string msg)
            : std::runtime_error(msg + "\n") {};
};

class QpsSemanticException : public std::runtime_error {
public:
    std::string msg = "SemanticError";

    explicit QpsSemanticException(std::string msg, int line, std::string expectedToken, std::string receivedToken)
    : std::runtime_error("line " + std::to_string(line)
    + ": " + msg + "\n"
    + "expected: " + expectedToken
    + ", got: " + receivedToken) {};

    explicit QpsSemanticException(std::string msg, int line, std::string receivedToken)
            : std::runtime_error("line " + std::to_string(line)
                                 + ": " + msg + "\n"
                                 + ", received: " + receivedToken) {};


    explicit QpsSemanticException(std::string msg) : std::runtime_error(msg) {};


    explicit QpsSemanticException(std::string msg, int line) : std::runtime_error("line " + std::to_string(line)
                                                                                  + ": " + msg) {};
};
