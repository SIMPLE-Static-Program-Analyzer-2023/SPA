#pragma once

#include <memory>
#include "Visitor.h"
#include "ast/StmtNode.h"
#include "pkb/Facade/SPFacade.h"

class ParentVisitor : public Visitor {
private:
    std::shared_ptr<SPFacade> spFacade;
public:
    explicit ParentVisitor(std::shared_ptr<SPFacade> spf);

    void visit(IfNode *node);
    void visit(WhileNode *node);
};
