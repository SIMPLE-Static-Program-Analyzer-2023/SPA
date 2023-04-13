#include "ProcedureNode.h"
#include "sp/design-extractor/Visitor.h"

ProcedureNode::ProcedureNode(Node parent, std::string name) : TNode(parent), name(name) {}

void ProcedureNode::setStmts(std::vector<Stmt> stmtLst) {
    this->stmtLst = stmtLst;
}

std::vector<Stmt> ProcedureNode::getStmts() const {
    return stmtLst;
}

std::string ProcedureNode::getProcName() const {
    return name;
}

void ProcedureNode::accept(Visitor *v) {
    v->visit(this);
}

std::string ProcedureNode::toString() {
    std::string res = "======================== PROCEDURE ";
    res += this->name;
    res += " ========================\n";
    for (auto stmt : stmtLst) {
        res += stmt->toString();
    }
    return res + "#### end procedure ####\n";
}

bool ProcedureNode::operator==(const TNode &other) const {
    if (const ProcedureNode* o = dynamic_cast<const ProcedureNode*>(&other)) {
        if (name == o->name && stmtLst == o->stmtLst) {
            return true;
        }
    }
    return false;
}
