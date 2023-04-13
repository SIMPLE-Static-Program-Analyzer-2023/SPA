#include "Parser.h"

Parser::Parser(std::shared_ptr<Lexer> lexer) : lexer(lexer)  {}

// ========================
// Helpers
// ========================

void Parser::advanceStmt() {
    stmtNo++;
}

bool Parser::isRelExpr() {
    std::string currToken = lexer->getCurrentToken();
    if (currToken == "!") {
        return false;
    }
    if (currToken != "(") {
        return true;
    }

    std::string op;

    if (lexer->isBufferEmpty()) {
        op = lexer->getCondOpFromStream();
    } else {
        op = lexer->getCondOpFromBuffer();
    }

    return Lexer::condOperators.find(op) == Lexer::condOperators.end();
}

// ========================
// Parse functions
// ========================

AstRoot Parser::parse(AstRoot root) {
    std::vector<Procedure> procedures;

    while (!lexer->isEnd()) {
        auto proc = parseProcedure(root);
        procedures.push_back(proc);
    }

    if (procedures.size() == 0) {
        throw ParserException("No procedures are found");
    }

    root->setProcedures(procedures);

    return root;
}

Procedure Parser::parseProcedure(AstRoot parent) {
    lexer->eat("procedure");

    std::string procName = lexer->eatIdentifier();
    Procedure procNode = std::make_shared<ProcedureNode>(parent, procName);

    lexer->eat("{");
    std::vector<Stmt> stmts = parseStmtLst(procNode);
    procNode->setStmts(stmts);
    lexer->eat("}");

    return procNode;
}

std::vector<Stmt> Parser::parseStmtLst(Node parent) {
    std::vector<Stmt> stmts;
    while (!lexer->match("}")) {
        Stmt stmt = parseStmt(parent);
        stmts.push_back(stmt);
    }
    return stmts;
}

Stmt Parser::parseStmt(Node parent) {
    Stmt s;
    std::string keyword = lexer->getCurrentToken();

    // must check assign first due to edge case e.g. read = read + 1;
    if (lexer->matchIdentifier() && lexer->peekChar() == '=') {
        s = parseAssign(parent);
    } else if (keyword == "read") {
        s = parseRead(parent);
    } else if (keyword == "print") {
        s = parsePrint(parent);
    } else if (keyword == "call") {
        s = parseCall(parent);
    } else if (keyword == "if") {
        s = parseIf(parent);
    } else if (keyword == "while") {
        s = parseWhile(parent);
    } else {
        throw ParserException("Invalid statement found", lexer->getLineNo());
    }

    return s;
}

Read Parser::parseRead(Node parent) {
    lexer->eat("read");
    Read readNode = std::make_shared<ReadNode>(parent, stmtNo);

    Variable v = parseVariable(readNode);
    readNode->setVar(v);

    lexer->eat(";");
    advanceStmt();
    return readNode;
}

Print Parser::parsePrint(Node parent) {
    lexer->eat("print");
    Print printNode = std::make_shared<PrintNode>(parent, stmtNo);

    Variable v = parseVariable(printNode);
    printNode->setVar(v);

    lexer->eat(";");
    advanceStmt();
    return printNode;
}

Assign Parser::parseAssign(Node parent) {
    Assign assignNode = std::make_shared<AssignNode>(parent, stmtNo);
    Variable lhs = parseVariable(parent);
    lexer->eat("=");
    Expression rhs = parseExpression(assignNode);
    assignNode->setAssign(lhs, rhs);

    lexer->eat(";");
    advanceStmt();
    return assignNode;
}

Call Parser::parseCall(Node parent) {
    lexer->eat("call");
    Call callNode = std::make_shared<CallNode>(parent, stmtNo, lexer->eatIdentifier());

    lexer->eat(";");
    advanceStmt();
    return callNode;
}

If Parser::parseIf(Node parent) {
    lexer->eat("if");
    If ifNode = std::make_shared<IfNode>(parent, stmtNo);
    // it is the responsibility of parseIf to eat the outer brackets
    lexer->eat("(");
    CondExpr condExpr = parseCondExpr(ifNode);
    lexer->eat(")");

    // then
    lexer->eat("then");
    lexer->eat("{");
    advanceStmt();

    std::vector<Stmt> thenStmtLst = parseStmtLst(ifNode);
    lexer->eat("}");

    // else
    lexer->eat("else");
    lexer->eat("{");
    std::vector<Stmt> elseStmtLst = parseStmtLst(ifNode);
    lexer->eat("}");

    ifNode->setIf(condExpr, thenStmtLst, elseStmtLst);
    return ifNode;
}

While Parser::parseWhile(Node parent) {
    lexer->eat("while");
    While whileNode = std::make_shared<WhileNode>(parent, stmtNo);

    lexer->eat("(");
    CondExpr condExpr = parseCondExpr(whileNode);
    lexer->eat(")");

    // stmts
    lexer->eat("{");
    advanceStmt();
    std::vector<Stmt> stmts = parseStmtLst(whileNode);
    lexer->eat("}");

    whileNode->setWhile(condExpr, stmts);
    return whileNode;
}

Expression Parser::parseExpression(Node parent) {
    return exprParser.parseExpression(parent, stmtNo);
}

CondExpr Parser::parseCondExpr(Node parent) {
    CondExpr condExpr;
    if (lexer->match("!")) {
        condExpr = std::make_shared<CondExprNode>(parent, stmtNo, CondExprType::NOT);

        lexer->eat("!");
        lexer->eat("(");

        CondExpr innerCondExpr = parseCondExpr(condExpr);
        condExpr->setNot(innerCondExpr);

        lexer->eat(")");
    } else if (isRelExpr()) {
        condExpr = std::make_shared<CondExprNode>(parent, stmtNo, CondExprType::REL_EXPR);
        RelExpr relExpr = parseRelExpr(condExpr);
        condExpr->setRelExpr(relExpr);
    } else {
        condExpr = std::make_shared<CondExprNode>(parent, stmtNo, CondExprType::AND_OR);

        lexer->eat("(");
        CondExpr lhs = parseCondExpr(condExpr);
        lexer->eat(")");

        std::string condOp = lexer->eatCondOperator();

        lexer->eat("(");
        CondExpr rhs = parseCondExpr(condExpr);
        lexer->eat(")");

        condExpr->setAndOr(lhs, rhs, condOp);
    }
    return condExpr;
}

RelExpr Parser::parseRelExpr(Node parent) {
    RelExpr relExpr = std::make_shared<RelExprNode>(parent, stmtNo);

    Expression lhs = parseExpression(relExpr);
    std::string op = lexer->eatRelOperator();
    Expression rhs = parseExpression(relExpr);

    relExpr->setRelExpr(lhs, rhs, op);
    return relExpr;
}

Variable Parser::parseVariable(Node parent) {
    std::string varName = lexer->eatIdentifier();
    Variable v = std::make_shared<VariableNode>(parent, stmtNo, varName);
    return v;
}
