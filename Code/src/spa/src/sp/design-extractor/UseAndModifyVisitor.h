#pragma once

#include <string>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "Visitor.h"
#include "ast/TNode.h"
#include "ast/StmtNode.h"
#include "ast/ExpressionNode.h"
#include "pkb/Facade/SPFacade.h"
#include "pkb/Facade/QPSFacade.h"

class UseAndModifyVisitor : public Visitor {
private:
    std::shared_ptr<SPFacade> spFacade;

    std::string currProcedure;
    // the following is used for adding indirect uses and modifies
    std::unordered_map<std::string, std::unordered_set<std::string>> procUseSets;
    std::unordered_map<std::string, std::unordered_set<std::string>> procModifySets;

    void addModifies(NumberedNode *node, std::string varName);
    void addUses(NumberedNode *node, std::string varName);
    void addAncestorModifies(TNode *node, std::string varName);
    void addAncestorUses(TNode *node, std::string varName);

public:
    explicit UseAndModifyVisitor(std::shared_ptr<SPFacade> spf);

    void visit(ProcedureNode *node) override;
    void visit(ReadNode *node) override;
    void visit(PrintNode *node) override;
    void visit(CallNode *node) override;
    void visit(AssignNode *node) override;
    void visit(VariableNode *node) override;
};
