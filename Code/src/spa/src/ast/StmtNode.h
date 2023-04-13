#pragma once

#include <memory>
#include <vector>
#include <string>

#include "TNode.h"
#include "ExpressionNode.h"

class Visitor;

class StmtNode;     // abstract
class ReadNode;
class PrintNode;
class AssignNode;
class CallNode;
class IfNode;
class WhileNode;

using Stmt = std::shared_ptr<StmtNode>;
using Read = std::shared_ptr<ReadNode>;
using Print = std::shared_ptr<PrintNode>;
using Assign = std::shared_ptr<AssignNode>;
using Call = std::shared_ptr<CallNode>;
using If = std::shared_ptr<IfNode>;
using While = std::shared_ptr<WhileNode>;

class StmtNode : public NumberedNode {
public:
    explicit StmtNode(Node parent, int StmtNo);
    virtual std::string toString() = 0;
    virtual void accept(Visitor *v) = 0;
};

class ReadNode : public StmtNode {
private:
    Variable var;
public:
    explicit ReadNode(Node parent, int stmtNo);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    void setVar(Variable var);
    Variable getVar();
};

class PrintNode : public StmtNode {
private:
    Variable var;
public:
    explicit PrintNode(Node parent, int stmtNo);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    void setVar(Variable var);
    Variable getVar();
};

class CallNode : public StmtNode {
private:
    std::string procName;
public:
    explicit CallNode(Node parent, int stmtNo, std::string procName);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    std::string getProcName() const;
};

class AssignNode : public StmtNode {
private:
    Variable lhs;
    Expression rhs;
public:
    explicit AssignNode(Node parent, int stmtNo);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    void setAssign(Variable lhs, Expression rhs);
    Variable getLhs();
    Expression getRhs();
};

class IfNode : public StmtNode {
private:
    CondExpr condExpr;
    std::vector<Stmt> thenStmtLst;
    std::vector<Stmt> elseStmtLst;
public:
    explicit IfNode(Node parent, int stmtNo);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    void setIf(CondExpr condExpr, std::vector<Stmt> thenStmtLst, std::vector<Stmt> elseStmtLst);
    CondExpr getCondExpr() const;
    std::vector<Stmt> getThenStmts() const;
    std::vector<Stmt> getElseStmts() const;
};

class WhileNode : public StmtNode {
private:
    CondExpr condExpr;
    std::vector<Stmt> stmtLst;
public:
    explicit WhileNode(Node parent, int stmtNo);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    void setWhile(CondExpr condExpr, std::vector<Stmt> stmts);
    CondExpr getCondExpr() const;
    std::vector<Stmt> getStmts() const;
};
