#include "ast/TNode.h"
#include "sp/design-extractor/EntityVisitor.h"
#include "pkb/Facade/SPFacade.h"
#include "pkb/Facade/QPSFacade.h"
#include "pkb/PKB.h"
#include "sp/Lexer.h"
#include "sp/Parser.h"
#include <memory>
#include <fstream>
#include <unordered_set>

#include "catch.hpp"

using namespace std;

std::string testEntityVisitorPath = "../../../../Tests09/integration_test/sp/parser/design-extraction/";

TEST_CASE("it should visit and save all statements, variables, and constants and patterns") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testEntityVisitorPath + "entity_visitor_1.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();

    auto visitor = std::make_shared<EntityVisitor>(spf);
    root->accept(visitor.get());

    auto a =  pkb.getQPSFacade().getStatements();
    REQUIRE(a == vector<int>  { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 });

    auto b =  pkb.getQPSFacade().getConstants();
    REQUIRE(b == vector<string> { "12", "11", "2", "9", "6", "7", "1", "5", "10", "8", "4", "3" });

    auto c =  pkb.getQPSFacade().getVariables();
    REQUIRE(c == vector<string> { "achieve", "rel", "meaning", "engage", "pEmo", "happiness", "l", "k", "h", "m", "g",
                                  "f", "c", "d", "e", "j", "b", "i", "a" });
}

TEST_CASE("it should visit and save all statements, variables, and constants and patterns 2") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testEntityVisitorPath + "entity_visitor_2.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();

    auto visitor = std::make_shared<EntityVisitor>(spf);
    root->accept(visitor.get());

    auto a =  pkb.getQPSFacade().getStatements();
    REQUIRE(a == vector<int>  { 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 });

    auto b =  pkb.getQPSFacade().getConstants();
    REQUIRE(b == vector<string> { "99", "4", "3" });

    auto c =  pkb.getQPSFacade().getVariables();
    REQUIRE(c == vector<string> { "d", "c", "b", "a", "test", "while", "if", "world", "k", "hello", "v", "p", "y",
                                  "asd", "q", "x" });
}

TEST_CASE("it should visit and save all procedure names") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testEntityVisitorPath
            + "entity_visitor_3_multiple_procedures.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();

    auto visitor = std::make_shared<EntityVisitor>(spf);
    root->accept(visitor.get());

    auto a =  pkb.getQPSFacade().getStatements();
    unordered_set<int> a_set(a.begin(), a.end());
    auto a_expected = unordered_set<int>  { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    REQUIRE(a.size() == a_expected.size());
    REQUIRE(a_set == a_expected);


    auto b =  pkb.getQPSFacade().getConstants();
    unordered_set<string> b_set(b.begin(), b.end());
    auto b_expected = unordered_set<string>  { "4", "3" };
    REQUIRE(b_set == b_expected);
    REQUIRE(b.size() == b_expected.size());

    auto c =  pkb.getQPSFacade().getVariables();
    unordered_set<string> c_set(c.begin(), c.end());
    auto c_expected = unordered_set<string>  { "x", "y", "z", "c", "e", "q", "w" };
    REQUIRE(c_set == c_expected);
    REQUIRE(c.size() == c_expected.size());

    auto d =  pkb.getQPSFacade().getProcedures();
    unordered_set<string> d_set(d.begin(), d.end());
    auto d_expected = unordered_set<string>  { "aar", "bar", "ear", "car", "dar", "far", "gar" };
    REQUIRE(d_set == d_expected);
    REQUIRE(d.size() == d_expected.size());
}

TEST_CASE("it should visit call node and save all call-related info") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testEntityVisitorPath
                                                                    + "entity_visitor_4_call_info.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();

    auto visitor = std::make_shared<EntityVisitor>(spf);
    root->accept(visitor.get());

    auto qpsf = pkb.getQPSFacade();

    // all call statements
    auto a = qpsf.getCallStatements();
    std::sort(a.begin(), a.end());
    REQUIRE(a == vector<int> { 2, 7, 9, 10, 12, 14, 16 });

    // all calls
    auto b = qpsf.getCalls();
    std::sort(b.begin(), b.end());
    vector<string> b1;
    vector<string> b2;
    for (auto p: b) {
        b1.push_back(p.first);
        b2.push_back(p.second);
    }

    CHECK(b1 == vector<string> { "aar", "bar", "car", "car", "dar", "ear", "far" });
    CHECK(b2 == vector<string> { "bar", "car", "dar", "gar", "ear", "far", "gar" });

    // all call star
    auto c = qpsf.getCallsStar();
    std::sort(c.begin(), c.end());
    vector<string> c1;
    vector<string> c2;
    for (auto p: b) {
        c1.push_back(p.first);
        c2.push_back(p.second);
    }

    CHECK(c1 == vector<string> { "aar", "bar", "car", "car", "dar", "ear", "far" });
    CHECK(c2 == vector<string> { "bar", "car", "dar", "gar", "ear", "far", "gar" });

    // individual's callee(*), caller(*)
    auto aar_callee_star= qpsf.getCalleeStar("aar");
    CHECK(aar_callee_star == vector<string> { "far", "gar", "dar", "car", "ear", "bar" });

    auto car_callee_star= qpsf.getCalleeStar("car");
    REQUIRE(car_callee_star == vector<string> { "far", "ear", "gar", "dar" });


    auto gar_caller_star = qpsf.getCallerStar("gar");
    REQUIRE(gar_caller_star == vector<string> { "aar", "bar", "ear", "far", "dar", "car" });
    auto gar_caller = qpsf.getCaller("gar");
    REQUIRE(gar_caller == vector<string> { "car", "far" });

    auto far_caller_star = qpsf.getCallerStar("far");
    std::sort(far_caller_star.begin(), far_caller_star.end());
    REQUIRE(far_caller_star == vector<string> { "aar", "bar", "car", "dar", "ear" });
    auto far_caller = qpsf.getCaller("far");
    std::sort(far_caller.begin(), far_caller.end());
    REQUIRE(far_caller == vector<string> { "ear"});
}
