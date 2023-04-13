#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "TNode.h"

class Visitor;

class ExpressionNode;   // abstract
class BinaryOpNode;
class CondExprNode;
class RelExprNode;
class RefNode;  // abstract
class VariableNode;
class ConstantNode;

using Expression = std::shared_ptr<ExpressionNode>;
using BinaryOp = std::shared_ptr<BinaryOpNode>;
using RelExpr = std::shared_ptr<RelExprNode>;
using CondExpr = std::shared_ptr<CondExprNode>;
using Ref = std::shared_ptr<RefNode>;
using Variable = std::shared_ptr<VariableNode>;
using Constant = std::shared_ptr<ConstantNode>;

class ExpressionNode : public NumberedNode {
public:
    explicit ExpressionNode(Node parent, int stmtNo);
    bool contains(Expression other);
    bool operator==(const TNode& other) const override;
};

class BinaryOpNode : public ExpressionNode {
protected:
    std::string op;
    Expression lhs;
    Expression rhs;
public:
    BinaryOpNode(Node parent, int stmtNo, Expression lhs, Expression rhs);
    BinaryOpNode(Node parent, int stmtNo, Expression lhs, Expression rhs, std::string op);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    void setBinaryOp(std::string op, Expression lhs, Expression rhs);
    Expression getLhs() const;
    Expression getRhs() const;
    std::string getOp();
};


enum class CondExprType {
    NOT,
    AND_OR,
    REL_EXPR
};

class CondExprNode : public NumberedNode {
private:
    CondExprType type;
    RelExpr relExpr;
    std::string op;
    // for NOT node, the lhs contains the inner conditional expression, the rhs is empty
    CondExpr lhs;
    CondExpr rhs;
public:
    explicit CondExprNode(Node parent, int stmtNo, CondExprType type);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    void setAndOr(CondExpr lhs, CondExpr rhs, std::string op);
    void setNot(CondExpr lhs);
    void setRelExpr(RelExpr relExpr);
    CondExprType getType() const;
    CondExpr getLhs() const;
    CondExpr getRhs() const;
    RelExpr getRelExpr() const;
};

class RelExprNode : public NumberedNode {
private:
    Expression lhs;
    Expression rhs;
    std::string op;
public:
    static const std::unordered_set<std::string> relExprOperators;
    explicit RelExprNode(Node parent, int stmtNo);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    void setRelExpr(Expression lhs, Expression rhs, std::string op);
    Expression getLhs();
    Expression getRhs();
};

class RefNode: public ExpressionNode {
public:
    explicit RefNode(Node parent, int stmtNo);
};

class VariableNode : public RefNode {
private:
    std::string varName;
public:
    explicit VariableNode(Node parent, int stmtNo, std::string varName);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    std::string getVarName() const;
};

class ConstantNode : public RefNode {
private:
    std::string number;
public:
    explicit ConstantNode(Node parent, int stmtNo, std::string number);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    std::string getConstant() const;
};
