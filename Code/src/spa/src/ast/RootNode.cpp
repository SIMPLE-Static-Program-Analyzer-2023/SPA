#include "RootNode.h"
#include "sp/design-extractor/Visitor.h"

RootNode::RootNode(Node parent) : TNode(parent) {}

void RootNode::setProcedures(std::vector<Procedure> procs) {
    this->procedures = procs;
}
std::vector<Procedure> RootNode::getProcedures() {
    return procedures;
}

void RootNode::accept(Visitor *v) {
    v->visit(this);
}

std::string RootNode::toString() {
    std::string  res("");
    if (procedures.size() == 0) {
        return "[empty_root]";
    }

    for (auto p : procedures) {
        res += p->toString();
    }

    return res;
}

bool RootNode::operator==(const TNode &other) const {
    if (const RootNode* o = dynamic_cast<const RootNode*>(&other)) {
        if (procedures == o->procedures) {
            return true;
        }
    }
    return false;
}
