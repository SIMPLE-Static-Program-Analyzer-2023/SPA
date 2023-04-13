#include "ast/TNode.h"
#include "sp/design-extractor/TopoSortVisitor.h"
#include "pkb/Facade/SPFacade.h"
#include "pkb/PKB.h"
#include "sp/Lexer.h"
#include "sp/Parser.h"
#include <memory>
#include <fstream>

#include "catch.hpp"

using namespace std;

std::string testTopologicalSortVisitorPath = "../../../../Tests09/integration_test/sp/parser/design-extraction/";


TEST_CASE("it should give the correct topological order") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testTopologicalSortVisitorPath
                                                                    + "topo_visitor_1.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> lineNos;
    vector<string> vars;

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();

    auto visitor = std::make_shared<TopoSortVisitor>();
    root->accept(visitor.get());
    vector<string> a;
    for (auto p: root->getProcedures()) {
        a.push_back(p->getProcName());
    }
    REQUIRE(a == vector<string> { "gar", "far", "ear", "dar", "car", "bar", "aar" });

}

TEST_CASE("it should raise exception when there is recursive call") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testTopologicalSortVisitorPath
                                                                    + "topo_visitor_2_recursive.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> lineNos;
    vector<string> vars;

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();

    auto visitor = std::make_shared<TopoSortVisitor>();

    REQUIRE_THROWS_WITH(root->accept(visitor.get()), Catch::Matchers::Contains("recursive call found"));
}

TEST_CASE("it should raise exception when there is cyclic call") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testTopologicalSortVisitorPath
                                                                    + "topo_visitor_3_cyclic.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();

    auto visitor = std::make_shared<TopoSortVisitor>();

    REQUIRE_THROWS_WITH(root->accept(visitor.get()), Catch::Matchers::Contains("cyclic call found"));
}

TEST_CASE("it should throw cyclic error when there are multiple calls to same callee") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testTopologicalSortVisitorPath
                                                                    + "topo_visitor_3_cyclic_2.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();

    auto visitor = std::make_shared<TopoSortVisitor>();


    REQUIRE_NOTHROW(root->accept(visitor.get()));
}
