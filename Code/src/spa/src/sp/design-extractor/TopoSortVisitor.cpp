#include "TopoSortVisitor.h"
#include <iostream>

TopoSortVisitor::TopoSortVisitor() {}

void TopoSortVisitor::visit(RootNode *node) {
    for (auto proc : node->getProcedures()) {
        procedureNameMap.insert({proc->getProcName(), proc});
        graph.insert({proc, std::unordered_set<Procedure>{}});
        inDegrees.insert({proc, 0});
    }

    for (auto proc : node->getProcedures()) {
        currProcedure = proc;
        proc->accept(this);
    }

    topoSort();
    std::reverse(topoOrder.begin(), topoOrder.end());
    node->setProcedures(topoOrder);
}

void TopoSortVisitor::visit(CallNode *node) {
    if (currProcedure->getProcName() == node->getProcName()) {
        throw DesignExtractorException("recursive call found");
    }
    std::string calleeName = node->getProcName();
    if (procedureNameMap.find(calleeName) == procedureNameMap.end()) {
        throw DesignExtractorException("unknown procedure called: " + calleeName);
    }

    Procedure callee = procedureNameMap.at(node->getProcName());
    // use alias to modify original
    std::unordered_set<Procedure> &allCallees = graph.at(currProcedure);

    // do nothing if we have alr added this edge
    if (allCallees.find(callee) != allCallees.end()) {
        return;
    }

    allCallees.insert(callee);
    inDegrees.at(callee) += 1;
}

void TopoSortVisitor::topoSort() {
    for (auto p : graph) {
        Procedure proc = p.first;
        if (inDegrees.at(proc) == 0) {
            procQueue.push(proc);
        }
    }

    while (procQueue.size() > 0) {
        Procedure curr = procQueue.front();
        procQueue.pop();
        topoOrder.push_back(curr);
        auto neighbors = graph.at(curr);
        for (Procedure nbr : neighbors) {
            inDegrees.at(nbr) -= 1;
            if (inDegrees.at(nbr) == 0) {
                procQueue.push(nbr);
            }
        }
    }

    if (graph.size() != topoOrder.size()) {
        throw DesignExtractorException("cyclic call found");
    }
}
