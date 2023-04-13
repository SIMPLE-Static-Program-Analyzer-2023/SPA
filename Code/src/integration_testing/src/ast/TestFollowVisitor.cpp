#include "ast/TNode.h"
#include "sp/design-extractor/FollowVisitor.h"
#include "pkb/Facade/SPFacade.h"
#include "pkb/Facade/QPSFacade.h"
#include "pkb/PKB.h"
#include "sp/Lexer.h"
#include "sp/Parser.h"
#include <memory>
#include <fstream>

#include "catch.hpp"
using namespace std;

std::string testFollowVisitorPath = "../../../../Tests09/integration_test/sp/parser/design-extraction/";


TEST_CASE("test follows visitor 1") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testFollowVisitorPath + "follow_visitor_1.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> before;
    vector<int> after;

    PKB pkb;
    auto spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();

    auto visitor = std::make_shared<FollowVisitor>(spf);
    root->accept(visitor.get());

    auto a =  pkb.getQPSFacade().getFollowsStarForward(1);
    REQUIRE(a == vector<int>{ 2, 3, 10, 11 });
    auto b =  pkb.getQPSFacade().getFollowsStarForward(4);
    REQUIRE(b == vector<int>{ 5, 6});
    auto c =  pkb.getQPSFacade().getFollowsStarForward(7);
    REQUIRE(c == vector<int>{ 8, 9});
}


