#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(std::shared_ptr<SPFacade> spFacade, AstRoot root) {
    this->topoSortVisitor = std::make_unique<TopoSortVisitor>();
    this->entityVisitor = std::make_unique<EntityVisitor>(spFacade);
    this->useAndModifyVisitor = std::make_unique<UseAndModifyVisitor>(spFacade);
    this->parentVisitor = std::make_unique<ParentVisitor>(spFacade);
    this->followVisitor = std::make_unique<FollowVisitor>(spFacade);
    this->root = root;
}

void DesignExtractor::extract() {
    this->root->accept(topoSortVisitor.get());
    this->root->accept(entityVisitor.get());
    this->root->accept(useAndModifyVisitor.get());
    this->root->accept(parentVisitor.get());
    this->root->accept(followVisitor.get());
}


