#pragma once

#include <memory>

#include "ast/RootNode.h"
#include "EntityVisitor.h"
#include "UseAndModifyVisitor.h"
#include "FollowVisitor.h"
#include "ParentVisitor.h"
#include "TopoSortVisitor.h"
#include "pkb/Facade/SPFacade.h"

class DesignExtractor {
private:
    std::unique_ptr<EntityVisitor> entityVisitor;
    std::unique_ptr<UseAndModifyVisitor> useAndModifyVisitor;
    std::unique_ptr<ParentVisitor> parentVisitor;
    std::unique_ptr<FollowVisitor> followVisitor;
    std::unique_ptr<TopoSortVisitor> topoSortVisitor;
    AstRoot root;
public:
    DesignExtractor(std::shared_ptr<SPFacade> spFacade, AstRoot root);
    void extract();
};
