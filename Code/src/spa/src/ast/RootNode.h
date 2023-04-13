#pragma once

#include <memory>
#include <vector>
#include <string>

#include "TNode.h"
#include "ProcedureNode.h"

class Visitor;

class RootNode;
using AstRoot = std::shared_ptr<RootNode>;

class RootNode: public TNode {
private:
    std::vector<Procedure> procedures;
public:
    explicit RootNode(Node parent);
    bool operator==(const TNode& other) const override;
    std::string toString() override;
    void accept(Visitor *v) override;
    void setProcedures(std::vector<Procedure> procs);
    std::vector<Procedure> getProcedures();
};
