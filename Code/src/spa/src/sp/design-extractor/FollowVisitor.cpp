#include "FollowVisitor.h"

FollowVisitor::FollowVisitor(std::shared_ptr<SPFacade> spf) : spFacade((spf)) {}

void FollowVisitor::visit(ProcedureNode *node) {
    auto stmtLst = node->getStmts();
    for (int i = 0; i < stmtLst.size(); i++) {
        if (i != stmtLst.size() - 1) {
            spFacade->addFollows(stmtLst.at(i)->getStmtNo(), stmtLst.at(i + 1)->getStmtNo());
        }
        stmtLst.at(i)->accept(this);
    }
}

void FollowVisitor::visit(IfNode *node) {
    auto thenStmtLst = node->getThenStmts();
    for (int i = 0; i < thenStmtLst.size(); i++) {
        if (i != thenStmtLst.size() - 1) {
            spFacade->addFollows(thenStmtLst.at(i)->getStmtNo(), thenStmtLst.at(i + 1)->getStmtNo());
        }
        spFacade->addIfThen(node->getStmtNo(), thenStmtLst.at(i)->getStmtNo());
        thenStmtLst.at(i)->accept(this);
    }

    auto elseStmtLst = node->getElseStmts();
    for (int i = 0; i < elseStmtLst.size(); i++) {
        if (i != elseStmtLst.size() - 1) {
            spFacade->addFollows(elseStmtLst.at(i)->getStmtNo(), elseStmtLst.at(i + 1)->getStmtNo());
        }
        spFacade->addIfElse(node->getStmtNo(), elseStmtLst.at(i)->getStmtNo());
        elseStmtLst.at(i)->accept(this);
    }
}

void FollowVisitor::visit(WhileNode *node) {
    auto stmtLst = node->getStmts();
    for (int i = 0; i < stmtLst.size(); i++) {
        if (i != stmtLst.size() - 1) {
            spFacade->addFollows(stmtLst.at(i)->getStmtNo(), stmtLst.at(i + 1)->getStmtNo());
        }
        stmtLst.at(i)->accept(this);
    }
}
