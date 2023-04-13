#include "Visitor.h"

void Visitor::visit(RootNode *node) {
    std::vector<Procedure> procs = node->getProcedures();
    for (auto proc : procs) {
        proc->accept(this);
    }
}

void Visitor::visit(ProcedureNode *node) {
    auto stmtLst = node->getStmts();
    for (auto stmt : stmtLst) {
        stmt->accept(this);
    }
}

void Visitor::visit(ReadNode *node) {
    Variable var = node->getVar();
    var->accept(this);
}

void Visitor::visit(PrintNode *node) {
    Variable var = node->getVar();
    var->accept(this);
}

void Visitor::visit(AssignNode *node) {
    Variable lhs = node->getLhs();
    Expression rhs = node->getRhs();
    lhs->accept(this);
    rhs->accept(this);
}

void Visitor::visit(CallNode *node) {
    // default implementation: does nothing
    // not all visitors need to visit constant
}

void Visitor::visit(IfNode *node) {
    node->getCondExpr()->accept(this);
    for (std::shared_ptr<StmtNode> stmt : node->getThenStmts()) {
        stmt->accept(this);
    }
    for (std::shared_ptr<StmtNode>  stmt : node->getElseStmts()) {
        stmt->accept(this);
    }
}

void Visitor::visit(WhileNode *node) {
    node->getCondExpr()->accept(this);
    for (std::shared_ptr<StmtNode> stmt : node->getStmts()) {
        stmt->accept(this);
    }
}

void Visitor::visit(BinaryOpNode *node) {
    node->getLhs()->accept(this);
    node->getRhs()->accept(this);
}

void Visitor::visit(CondExprNode *node) {
    if (node->getType() == CondExprType::AND_OR) {
        node->getLhs()->accept(this);
        node->getRhs()->accept(this);
    } else if (node->getType() == CondExprType::NOT) {
        node->getLhs()->accept(this);
    } else {
        node->getRelExpr()->accept(this);
    }
}

void Visitor::visit(RelExprNode *node) {
    node->getLhs()->accept(this);
    node->getRhs()->accept(this);
}

void Visitor::visit(ConstantNode *node) {
    // default implementation: does nothing
    // not all visitors need to visit constant
}

void Visitor::visit(VariableNode *node) {
    // default implementation: does nothing
    // not all visitors need to visit variable
}
