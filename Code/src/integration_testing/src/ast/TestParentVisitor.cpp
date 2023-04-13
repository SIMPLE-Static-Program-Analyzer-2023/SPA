#include "ast/TNode.h"
#include "sp/design-extractor/ParentVisitor.h"
#include "pkb/Facade/SPFacade.h"
#include "pkb/Facade/QPSFacade.h"
#include "pkb/PKB.h"
#include "sp/Lexer.h"
#include "sp/Parser.h"
#include <memory>
#include <fstream>

#include "catch.hpp"
using namespace std;

std::string testParentVisitorPath = "../../../../Tests09/integration_test/sp/parser/design-extraction/";


TEST_CASE("test parents 1") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testParentVisitorPath + "parent_visitor1.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> parents;
    vector<int> children;

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();

    auto visitor = std::make_shared<ParentVisitor>(spf);
    root->accept(visitor.get());

    auto a =  pkb.getQPSFacade().getAllParentStar();
    for (auto p: a) {
        parents.push_back(p.first);
        children.push_back(p.second);
    }
    CHECK(parents == vector<int> { 6, 1, 1, 1, 1 });
    CHECK(children == vector<int> { 7, 2, 3, 4, 5 });
}

TEST_CASE("test parents 2 nested loops") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testParentVisitorPath
            + "parent_visitor2_nested.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> parents;
    vector<int> children;

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();

    auto visitor = std::make_shared<ParentVisitor>(spf);
    root->accept(visitor.get());

    auto a =  pkb.getQPSFacade().getAllParentStar();
    for (auto p: a) {
        parents.push_back(p.first);
        children.push_back(p.second);
    }
    CHECK(parents == vector<int> { 4, 7, 7, 2, 6, 6, 6, 1, 1, 1, 1 });
    CHECK(children == vector<int> { 5, 8, 9, 3, 7, 8, 9, 2, 4, 3, 5 });
}


TEST_CASE("test parents 2 tripled nested loops") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testParentVisitorPath
                                                                    + "parent_visitor3_triple_nested.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> parents;
    vector<int> children;

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();

    auto visitor = std::make_shared<ParentVisitor>(spf);
    root->accept(visitor.get());

    auto a =  pkb.getQPSFacade().getAllParentStar();
    for (auto p: a) {
        parents.push_back(p.first);
        children.push_back(p.second);
    }
    CHECK(parents == vector<int> { 3, 2, 2, 1, 1, 1, 1 });
    CHECK(children == vector<int> { 4, 3, 4, 2, 5, 3, 4 });
}

