#pragma once

#include <memory>
#include <vector>
#include <string>

#include "TNode.h"
#include "StmtNode.h"

class Visitor;

class ProcedureNode;
using Procedure = std::shared_ptr<ProcedureNode>;

class ProcedureNode : public TNode {
private:
    std::vector<Stmt> stmtLst;
    std::string name;
public:
    explicit ProcedureNode(Node parent, std::string name);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    void setStmts(std::vector<Stmt> stmtLst);
    std::vector<Stmt> getStmts() const;
    std::string getProcName() const;
};
