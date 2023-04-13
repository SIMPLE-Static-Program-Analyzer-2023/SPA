#include "SpManager.h"

AstRoot SpManager::processSIMPLE(std::shared_ptr<std::istream> src) {
    std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>(src);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    Parser parser(lexer);
    return parser.parse(root);
}

void SpManager::extractAndPopulate(AstRoot root, PKB pkb) {
    DesignExtractor designExtractor(pkb.getSPFacade(), root);
    designExtractor.extract();
}
