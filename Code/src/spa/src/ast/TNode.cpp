#include "TNode.h"

// ========================
// TNode
// ========================
TNode::TNode(Node parent) : parent(parent) {}

Node TNode::getParent() const { return parent; }

void TNode::setParent(Node parent) {
    this->parent = parent;
}

// ========================
// Numbered Node
// ========================
NumberedNode::NumberedNode(Node parent, int stmtNo) : TNode(parent), stmtNo(stmtNo) {}

int NumberedNode::getStmtNo() const {
    return stmtNo;
}
