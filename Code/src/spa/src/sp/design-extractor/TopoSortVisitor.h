#pragma once

#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <string>
#include <vector>

#include "Visitor.h"
#include "ast/TNode.h"
#include "ast/StmtNode.h"
#include "ast/ExpressionNode.h"
#include "sp/SpException.h"

class TopoSortVisitor : public Visitor {
private:
    Procedure currProcedure;

    // string to procedure node map
    std::unordered_map<std::string, Procedure> procedureNameMap;

    // graph representation
    std::unordered_map<Procedure, std::unordered_set<Procedure>> graph;

    std::unordered_map<Procedure, int> inDegrees;

    std::queue<Procedure> procQueue;

    std::vector<Procedure> topoOrder;

    void topoSort();

public:
    TopoSortVisitor();

    virtual void visit(RootNode *node);
    virtual void visit(CallNode *node);
};
