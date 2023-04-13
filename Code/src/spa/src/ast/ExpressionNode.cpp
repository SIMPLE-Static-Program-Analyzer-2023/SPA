#include "ExpressionNode.h"
#include "sp/design-extractor/Visitor.h"

// ========================
// Expression Node
// ========================
ExpressionNode::ExpressionNode(Node parent, int stmtNo) : NumberedNode(parent, stmtNo) {}

bool ExpressionNode::operator==(const TNode &other) const {
    const BinaryOpNode* thisAsBinOp = dynamic_cast<const BinaryOpNode*>(this);
    const BinaryOpNode* otherASBinOp = dynamic_cast<const BinaryOpNode*>(&other);
    if (thisAsBinOp != nullptr && otherASBinOp != nullptr) {
        return thisAsBinOp == otherASBinOp;
    }

    const RefNode* thisAsRef = dynamic_cast<const RefNode*>(this);
    const RefNode* otherAsRef = dynamic_cast<const RefNode*>(&other);
    if (thisAsRef != nullptr && otherAsRef != nullptr) {
        return thisAsRef == otherAsRef;
    }
    return false;
}

bool ExpressionNode::contains(Expression other) {
    BinaryOpNode* thisAsBinOp = dynamic_cast<BinaryOpNode*>(this);
    RefNode *thisAsRef = dynamic_cast<RefNode*>(this);
    std::shared_ptr<BinaryOpNode> otherAsBinOp = std::dynamic_pointer_cast<BinaryOpNode>(other);
    std::shared_ptr<RefNode> otherAsRef = std::dynamic_pointer_cast<RefNode>(other);

    if (thisAsRef != nullptr && otherAsBinOp != nullptr) {
        return false;
    }
    if (thisAsRef != nullptr && otherAsRef != nullptr) {
        return *thisAsRef == *otherAsRef;
    }
    if (thisAsBinOp != nullptr && otherAsRef != nullptr) {
        return thisAsBinOp->getLhs()->contains(other) || thisAsBinOp->getRhs()->contains(other);
    }
    if (thisAsBinOp != nullptr && otherAsBinOp != nullptr) {
        return *thisAsBinOp == *otherAsBinOp
            || thisAsBinOp->getLhs()->contains(other)
            || thisAsBinOp->getRhs()->contains(other);
    }
    return false;
}

// ========================
// BinaryOp Node
// ========================
BinaryOpNode::BinaryOpNode(Node parent, int stmtNo, Expression lhs, Expression rhs, std::string op)
        : ExpressionNode(parent, stmtNo), lhs(lhs), rhs(rhs), op(op) {}

BinaryOpNode::BinaryOpNode(Node parent, int stmtNo, Expression lhs, Expression rhs)
        : ExpressionNode(parent, stmtNo), lhs(lhs), rhs(rhs) {}

bool BinaryOpNode::operator==(const TNode &other) const {
    const BinaryOpNode* o = dynamic_cast<const BinaryOpNode*>(&other);
    if (o != nullptr) {
        bool sameOp = op == o->op;
        bool sameLhs = *lhs == *(o->lhs);
        bool sameRhs = *rhs == *(o->rhs);
        return sameOp && sameLhs && sameRhs;
    }
    return false;
}

std::string BinaryOpNode::toString() {
    return lhs->toString() + " " + op + " " + rhs->toString();
}

void BinaryOpNode::setBinaryOp(std::string op, Expression lhs, Expression rhs) {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

void BinaryOpNode::accept(Visitor *v) {
    v->visit(this);
}

Expression BinaryOpNode::getLhs() const {
    return lhs;
}

Expression BinaryOpNode::getRhs() const {
    return rhs;
}

std::string BinaryOpNode::getOp() {
    return op;
}

// ========================
// CondExpr Node
// ========================
CondExprNode::CondExprNode(Node parent, int stmtNo, CondExprType type) : NumberedNode(parent, stmtNo), type(type) {}

bool CondExprNode::operator==(const TNode &other) const {
    if (const CondExprNode* o = dynamic_cast<const CondExprNode*>(&other)) {
        return  stmtNo == o->stmtNo && type == o->type && relExpr == o->relExpr
                && lhs == o->lhs && rhs == o->rhs && op == o->op;
    }
    return false;
}

std::string CondExprNode::toString() {
    std::string res = "";
    if (type == CondExprType::AND_OR) {
        res += "(" + lhs->toString() + " " + op + " " + rhs->toString() + ")";
    } else if (type == CondExprType::NOT) {
        res += "!(" + lhs->toString() + ")";
    } else {
        res += relExpr->toString();
    }
    return res;
}

void CondExprNode::accept(Visitor *v) {
    v->visit(this);
}

void CondExprNode::setRelExpr(RelExpr relExpr) {
    this->relExpr = std::move(relExpr);
}

void CondExprNode::setAndOr(CondExpr lhs, CondExpr rhs, std::string op) {
    this->lhs = std::move(lhs);
    this->rhs = std::move(rhs);
    this->op = op;
}

void CondExprNode::setNot(CondExpr lhs) {
    this->lhs = std::move(lhs);
}

CondExprType CondExprNode::getType() const {
    return type;
}

CondExpr CondExprNode::getLhs() const { return lhs; }

CondExpr CondExprNode::getRhs() const { return rhs; }

RelExpr CondExprNode::getRelExpr() const { return relExpr; }

// ========================
// RelExpr Node
// ========================
static const std::unordered_set<std::string> relExprOperators({">", "<", ">=", "<=", "==", "!="});

RelExprNode::RelExprNode(Node parent, int stmtNo) : NumberedNode(parent, stmtNo) {}

void RelExprNode::setRelExpr(Expression lhs, Expression rhs, std::string op) {
    this->lhs = std::move(lhs);
    this->rhs = std::move(rhs);
    this->op = op;
}

bool RelExprNode::operator==(const TNode &other) const {
    if (const RelExprNode* o = dynamic_cast<const RelExprNode*>(&other)) {
        return  stmtNo == o->stmtNo && lhs == o->lhs && rhs == o->rhs && op == o->op;
    }
    return false;
}

std::string RelExprNode::toString() {
    return lhs->toString() + " " + op + " " + rhs->toString();
}

void RelExprNode::accept(Visitor *v) {
    v->visit(this);
}

Expression RelExprNode::getLhs() {
    return lhs;
}

Expression RelExprNode::getRhs() {
    return rhs;
}

// ========================
// Ref Node
// ========================
RefNode::RefNode(Node parent, int stmtNo) : ExpressionNode(parent, stmtNo) {}

// ========================
// Constant Node
// ========================
ConstantNode::ConstantNode(Node parent, int stmtNo, std::string number) : RefNode(parent, stmtNo), number(number) {}

void ConstantNode::accept(Visitor *v) {
    v->visit(this);
}

std::string ConstantNode::toString() {
    return number;
}

bool ConstantNode::operator==(const TNode &other) const {
    if (const ConstantNode* o = dynamic_cast<const ConstantNode*>(&other)) {
        return number == o->number;
    }
    return false;
}

std::string ConstantNode::getConstant() const {
    return number;
}

// ========================
// Variable Node
// ========================
VariableNode::VariableNode(Node parent, int stmtNo, std::string varName) : RefNode(parent, stmtNo), varName(varName) {}

void VariableNode::accept(Visitor *v) {
    v->visit(this);
}

std::string VariableNode::toString() {
    return varName;
}

bool VariableNode::operator==(const TNode &other) const {
    if (const VariableNode* o = dynamic_cast<const VariableNode*>(&other)) {
        return varName == o->varName;
    }
    return false;
}

std::string VariableNode::getVarName() const {
    return varName;
}
