#pragma once

#include <vector>
#include <memory>
#include "ast/RootNode.h"
#include "ast/ProcedureNode.h"
#include "ast/StmtNode.h"
#include "ast/ExpressionNode.h"

class Visitor {
public:
    virtual void visit(RootNode *node);
    virtual void visit(ProcedureNode *node);
    virtual void visit(ReadNode *node);
    virtual void visit(PrintNode *node);
    virtual void visit(AssignNode *node);
    virtual void visit(CallNode *node);
    virtual void visit(IfNode *node);
    virtual void visit(WhileNode *node);
    virtual void visit(BinaryOpNode *node);
    virtual void visit(CondExprNode *node);
    virtual void visit(RelExprNode *node);
    virtual void visit(VariableNode *node);
    virtual void visit(ConstantNode *node);
};
