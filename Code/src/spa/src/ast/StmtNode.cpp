#include "StmtNode.h"
#include "sp/design-extractor/Visitor.h"

// ========================
// Stmt Node
// ========================
StmtNode::StmtNode(Node parent, int stmtNo) : NumberedNode(parent, stmtNo) {}

// ========================
// Read Node
// ========================
ReadNode::ReadNode(Node parent, int StmtNo) : StmtNode(parent, StmtNo) {}

Variable ReadNode::getVar() {
    return var;
}

std::string ReadNode::toString() {
    return "read " + var->toString() + "\n";
}

void ReadNode::setVar(Variable var) {
    this->var = var;
}

void ReadNode::accept(Visitor *v) {
    v->visit(this);
}

bool ReadNode::operator==(const TNode &other) const {
    if (const ReadNode* o = dynamic_cast<const ReadNode*>(&other)) {
        return var == o->var  && stmtNo == o->stmtNo;
    }
    return false;
}

// ========================
// Print Node
// ========================
PrintNode::PrintNode(Node parent, int stmtNo) : StmtNode(parent, stmtNo) {}

std::string PrintNode::toString() {
    return "print " + var->toString() + "\n";
}

void PrintNode::setVar(Variable var) {
    this->var = var;
}

Variable PrintNode::getVar() {
    return var;
}

void PrintNode::accept(Visitor *v) {
    v->visit(this);
}

bool PrintNode::operator==(const TNode &other) const {
    if (const PrintNode* o = dynamic_cast<const PrintNode*>(&other)) {
        return var == o->var  && stmtNo == o->stmtNo;
    }
    return false;
}

// ========================
// Assign Node
// ========================
AssignNode::AssignNode(Node parent, int stmtNo) : StmtNode(parent, stmtNo) {}

void AssignNode::setAssign(Variable lhs, Expression rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

Variable AssignNode::getLhs() {
    return lhs;
}

Expression AssignNode::getRhs() {
    return rhs;
}

void AssignNode::accept(Visitor *v) {
    v->visit(this);
}

std::string AssignNode::toString() {
    return lhs->toString() + ":=" + rhs->toString() + "\n";
}

bool AssignNode::operator==(const TNode &other) const {
    if (const AssignNode* o = dynamic_cast<const AssignNode*>(&other)) {
        return *lhs == *(o->lhs) && *rhs == *(o->rhs) && stmtNo == o->stmtNo;
    }
    return false;
}

// ========================
// Call Node
// ========================
CallNode::CallNode(Node parent, int stmtNo, std::string procName) : StmtNode(parent, stmtNo), procName(procName) {}

bool CallNode::operator==(const TNode &other) const {
    if (const CallNode* o = dynamic_cast<const CallNode*>(&other)) {
        return procName == o->procName && stmtNo == o->stmtNo;
    }
    return false;
}

std::string CallNode::toString() {
    return "call " + procName + "\n";
}

void CallNode::accept(Visitor *v) {
    v->visit(this);
}

std::string CallNode::getProcName() const {
    return procName;
}

// ========================
// If Node
// ========================
IfNode::IfNode(Node parent, int stmtNo) : StmtNode(parent, stmtNo) {}

bool IfNode::operator==(const TNode &other) const {
    if (const IfNode* o = dynamic_cast<const IfNode*>(&other)) {
        return stmtNo == o->stmtNo && condExpr == o->condExpr
               && thenStmtLst == o->thenStmtLst && elseStmtLst == o->elseStmtLst;
    }
    return false;
}

std::string IfNode::toString() {
    std::string res = "if: ";
    res += condExpr->toString() + "\n";
    for (auto stmt : thenStmtLst) {
        res += stmt->toString();
    }
    res += "else:\n";
    for (auto stmt : elseStmtLst) {
        res += stmt->toString();
    }
    return res + "#### end if ####\n";
}

void IfNode::accept(Visitor *v) {
    v->visit(this);
}

void IfNode::setIf(CondExpr condExpr, std::vector<Stmt> thenStmtLst, std::vector<Stmt> elseStmtLst) {
    this->condExpr = condExpr;
    this->thenStmtLst = thenStmtLst;
    this->elseStmtLst = elseStmtLst;
}

CondExpr IfNode::getCondExpr() const {
    return condExpr;
}

std::vector<Stmt> IfNode::getElseStmts() const {
    return elseStmtLst;
}

std::vector<Stmt> IfNode::getThenStmts() const {
    return thenStmtLst;
}

// ========================
// While Node
// ========================
WhileNode::WhileNode(Node parent, int stmtNo) : StmtNode(parent, stmtNo) {}

bool WhileNode::operator==(const TNode &other) const {
    if (const WhileNode* o = dynamic_cast<const WhileNode*>(&other)) {
        return stmtNo == o->stmtNo && condExpr == o->condExpr
               && stmtLst == o->stmtLst;
    }
    return false;
}

std::string WhileNode::toString() {
    std::string res = "while: ";
    res += condExpr->toString() + "\n";
    for (auto stmt : stmtLst) {
        res += stmt->toString();
    }
    return res + "#### end while ####\n";
}

void WhileNode::accept(Visitor *v) {
    v->visit(this);
}

void WhileNode::setWhile(CondExpr condExpr, std::vector<Stmt> stmts) {
    this->condExpr = condExpr;
    this->stmtLst = stmts;
}

CondExpr WhileNode::getCondExpr() const { return condExpr; }

std::vector<Stmt> WhileNode::getStmts() const { return stmtLst; }
