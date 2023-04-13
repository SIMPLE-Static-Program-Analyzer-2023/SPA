#pragma once

#include <memory>
#include <string>
#include <vector>
#include <sstream>

#include "ast/RootNode.h"
#include "ast/ProcedureNode.h"
#include "ast/StmtNode.h"
#include "ast/ExpressionNode.h"
#include "Lexer.h"
#include "ExprParser.h"
#include "SpException.h"

class Parser {
private:
    std::shared_ptr<Lexer> lexer;
    int stmtNo = 1;
    ExprParser exprParser = ExprParser(lexer);

    Procedure parseProcedure(AstRoot parent);
    std::vector<Stmt>  parseStmtLst(Node parent);
    Stmt parseStmt(Node parent);
    Read parseRead(Node parent);
    Assign parseAssign(Node parent);
    Print parsePrint(Node parent);
    Call parseCall(Node parent);
    If parseIf(Node parent);
    While parseWhile(Node parent);
    Expression parseExpression(Node parent);
    CondExpr parseCondExpr(Node parent);
    RelExpr parseRelExpr(Node parent);
    Variable parseVariable(Node parent);

    void advanceStmt();
    bool isRelExpr();

public:
    explicit Parser(std::shared_ptr<Lexer> lexer);
    AstRoot parse(AstRoot root);
};
