#include "UseAndModifyVisitor.h"

UseAndModifyVisitor::UseAndModifyVisitor(std::shared_ptr<SPFacade> spf)
    : spFacade(spf)
{}

void UseAndModifyVisitor::addModifies(NumberedNode *node, std::string varName) {
    spFacade->addStmtModifies(node->getStmtNo(), varName);
    addAncestorModifies(node, varName);
    procModifySets.at(currProcedure).insert(varName);
}

void UseAndModifyVisitor::addUses(NumberedNode *node, std::string varName) {
    spFacade->addStmtUses(node->getStmtNo(), varName);
    addAncestorUses(node, varName);
    procUseSets.at(currProcedure).insert(varName);
}

void UseAndModifyVisitor::visit(ProcedureNode *node) {
    std::string procName = node->getProcName();
    currProcedure = procName;
    procUseSets.insert({procName, std::unordered_set<std::string> {}});
    procModifySets.insert({procName, std::unordered_set<std::string> {}});
    Visitor::visit(node);
}

void UseAndModifyVisitor::visit(ReadNode *node) {
    addModifies(node, node->getVar()->getVarName());
}

void UseAndModifyVisitor::visit(PrintNode *node) {
    node->getVar()->accept(this);
}

void UseAndModifyVisitor::visit(CallNode *node) {
    std::string callee = node->getProcName();
    for (auto modifiedVar : procModifySets.at(callee)) {
        addModifies(node, modifiedVar);
    }
    for (auto usedVar : procUseSets.at(callee)) {
        addUses(node, usedVar);
    }
}

void UseAndModifyVisitor::visit(AssignNode *node) {
    addModifies(node, node->getLhs()->getVarName());
    node->getRhs()->accept(this);
}

void UseAndModifyVisitor::visit(VariableNode *node) {
    // all modifies relationship are added while visiting assign and read nodes
    // all uses relationship are added while visiting variable node
    addUses(node, node->getVarName());
}

void UseAndModifyVisitor::addAncestorModifies(TNode *node, std::string varName) {
    Node parent = node->getParent();
    while (parent != nullptr) {
        if (const WhileNode* ancestor = dynamic_cast<const WhileNode*>(parent.get())) {
            spFacade->addStmtModifies(ancestor->getStmtNo(), varName);
        } else if (const IfNode* ancestor = dynamic_cast<const IfNode*>(parent.get())) {
            spFacade->addStmtModifies(ancestor->getStmtNo(), varName);
        } else if (const ProcedureNode* ancestor = dynamic_cast<const ProcedureNode*>(parent.get())) {
            spFacade->addProcModifies(ancestor->getProcName(), varName);
        }
        parent = parent->getParent();
    }
}

void UseAndModifyVisitor::addAncestorUses(TNode *node, std::string varName) {
    Node parent = node->getParent();
    while (parent != nullptr) {
        if (const WhileNode* ancestor = dynamic_cast<const WhileNode*>(parent.get())) {
            spFacade->addStmtUses(ancestor->getStmtNo(), varName);
        } else if (const IfNode* ancestor = dynamic_cast<const IfNode*>(parent.get())) {
            spFacade->addStmtUses(ancestor->getStmtNo(), varName);
        } else if (const ProcedureNode* ancestor = dynamic_cast<const ProcedureNode*>(parent.get())) {
            spFacade->addProcUses(ancestor->getProcName(), varName);
        }
        parent = parent->getParent();
    }
}
