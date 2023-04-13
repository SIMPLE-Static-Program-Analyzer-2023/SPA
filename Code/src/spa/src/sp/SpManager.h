#pragma once

#include <istream>
#include <memory>

#include "design-extractor/DesignExtractor.h"
#include "ast/RootNode.h"
#include "pkb/PKB.h"
#include "Parser.h"

class SpManager {
public:
    AstRoot processSIMPLE(std::shared_ptr<std::istream> src);
    void extractAndPopulate(AstRoot root, PKB pkb);
};
