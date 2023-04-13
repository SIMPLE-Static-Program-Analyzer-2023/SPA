#include "EntityVisitor.h"

EntityVisitor::EntityVisitor(std::shared_ptr<SPFacade> spf) : spFacade(std::move(spf)) {}


void EntityVisitor::visit(ProcedureNode *node) {
    spFacade->addProcedure(node->getProcName());
    this->currProcedure = node->getProcName();
    Visitor::visit(node);
}

void EntityVisitor::visit(ReadNode *node) {
    spFacade->addStatement(node->getStmtNo(), StatementType::StatementRead);
    spFacade->addProcChild(currProcedure, node->getStmtNo());
    Visitor::visit(node);
}

void EntityVisitor::visit(PrintNode *node) {
    spFacade->addStatement(node->getStmtNo(), StatementType::StatementPrint);
    spFacade->addProcChild(currProcedure, node->getStmtNo());
    Visitor::visit(node);
}

void EntityVisitor::visit(AssignNode *node) {
    spFacade->addStatement(node->getStmtNo(), StatementType::StatementAssign);
    spFacade->addProcChild(currProcedure, node->getStmtNo());
    // out of place here, but it would a waste to go through the ast again just to add pattern?
    spFacade->addPattern(node->getStmtNo(), node);
    Visitor::visit(node);
}

void EntityVisitor::visit(CallNode *node) {
    auto callee = node->getProcName();
    int stmtNo = node->getStmtNo();

    spFacade->addStatement(stmtNo, StatementType::StatementCall);
    spFacade->addCallStmt(stmtNo, callee);
    spFacade->addCalls(currProcedure, callee);
    spFacade->addProcChild(currProcedure, node->getStmtNo());
}

void EntityVisitor::visit(IfNode *node) {
    spFacade->addStatement(node->getStmtNo(), StatementType::StatementIf);
    spFacade->addIfPattern(node->getStmtNo(), node->getCondExpr());
    spFacade->addProcChild(currProcedure, node->getStmtNo());
    Visitor::visit(node);
}

void EntityVisitor::visit(WhileNode *node) {
    spFacade->addStatement(node->getStmtNo(), StatementType::StatementWhile);
    spFacade->addWhilePattern(node->getStmtNo(), node->getCondExpr());
    spFacade->addProcChild(currProcedure, node->getStmtNo());
    Visitor::visit(node);
}

void EntityVisitor::visit(VariableNode *node) {
    spFacade->addVariable(node->getVarName());
}

void EntityVisitor::visit(ConstantNode *node) {
    spFacade->addConstant(node->getConstant());
}
