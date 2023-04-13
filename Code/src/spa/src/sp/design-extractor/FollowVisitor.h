#pragma once

#include <memory>
#include "Visitor.h"
#include "ast/StmtNode.h"
#include "ast/ProcedureNode.h"
#include "pkb/Facade/SPFacade.h"

class FollowVisitor : public Visitor {
private:
    std::shared_ptr<SPFacade> spFacade;
public:
    explicit FollowVisitor(std::shared_ptr<SPFacade> spf);

    void visit(ProcedureNode *node);
    void visit(IfNode *node);
    void visit(WhileNode *node);
};
