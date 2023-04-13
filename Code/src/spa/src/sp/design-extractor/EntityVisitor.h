#pragma once

#include <utility>
#include <memory>
#include <string>
#include "Visitor.h"
#include "ast/TNode.h"
#include "ast/StmtNode.h"
#include "ast/ExpressionNode.h"
#include "pkb/Facade/SPFacade.h"

class EntityVisitor : public Visitor {
private:
    std::shared_ptr<SPFacade> spFacade;
    std::string currProcedure;
public:
    explicit EntityVisitor(std::shared_ptr<SPFacade> spf);

    void visit(ProcedureNode *node);
    void visit(ReadNode *node);
    void visit(PrintNode *node);
    void visit(AssignNode *node);
    void visit(CallNode *node);
    void visit(IfNode *node);
    void visit(WhileNode *node);
    void visit(VariableNode *node);
    void visit(ConstantNode *node);
};
