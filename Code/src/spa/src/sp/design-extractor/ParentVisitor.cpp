#include "ParentVisitor.h"

ParentVisitor::ParentVisitor(std::shared_ptr<SPFacade> spf) : spFacade(spf) {}

void ParentVisitor::visit(IfNode *node) {
    for (auto s : node->getThenStmts()) {
        spFacade->addParentChild(node->getStmtNo(), s->getStmtNo());
        s->accept(this);
    }

    for (auto s : node->getElseStmts()) {
        spFacade->addParentChild(node->getStmtNo(), s->getStmtNo());
        s->accept(this);
    }
}

void ParentVisitor::visit(WhileNode *node) {
    for (auto s : node->getStmts()) {
        spFacade->addParentChild(node->getStmtNo(), s->getStmtNo());
        s->accept(this);
    }
}
