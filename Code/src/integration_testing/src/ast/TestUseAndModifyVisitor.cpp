#include "ast/TNode.h"
#include "sp/design-extractor/UseAndModifyVisitor.h"
#include "sp/design-extractor/TopoSortVisitor.h"
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

std::string testUseAndModifyPath = "../../../../Tests09/integration_test/sp/parser/design-extraction/";


TEST_CASE("it should save correct stmt-modfies/uses-var and var-used/modifed-by-stmt relationship") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testUseAndModifyPath
            + "use_modify_visitor_1.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> lineNos;
    vector<string> vars;

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();

    auto visitor = std::make_shared<UseAndModifyVisitor>(spf);
    root->accept(visitor.get());

    vector<int> d;
    vector<string> c;


    // stmt modifies var
    c =  pkb.getQPSFacade().getModifies(1);
    REQUIRE(c == vector<string> {"lo"});
    c =  pkb.getQPSFacade().getModifies(2);
    REQUIRE(c == vector<string> {"hi"});
    c =  pkb.getQPSFacade().getModifies(3);
    REQUIRE(c == vector<string> { "arrMid", "lo", "hi" });
    c =  pkb.getQPSFacade().getModifies(4);
    REQUIRE(c == vector<string> { "arrMid"});
    c =  pkb.getQPSFacade().getModifies(5);
    REQUIRE(c == vector<string> { "lo", "hi"});

    // var used by stmt
    d =  pkb.getQPSFacade().getModifies("lo");
    REQUIRE(d == vector<int> { 1, 6, 5, 3 });
    d =  pkb.getQPSFacade().getModifies("hi");
    REQUIRE(d == vector<int> { 2, 7, 5, 3 });
    d =  pkb.getQPSFacade().getModifies("arrMid");
    REQUIRE(d == vector<int> { 4, 3 });
    d =  pkb.getQPSFacade().getModifies("x");
    REQUIRE(d == vector<int> { 9, 8 });

    // stmt uses var
    c = pkb.getQPSFacade().getUses(1);
    REQUIRE(c == vector<string> { });
    c = pkb.getQPSFacade().getUses(2);
    REQUIRE(c == vector<string> { "n" });
    c = pkb.getQPSFacade().getUses(3);
    sort(c.begin(), c.end());
    REQUIRE(c == vector<string> { "arrMid", "hi", "lo", "mid", "target" });

    c = pkb.getQPSFacade().getUses(4);
    REQUIRE(c == vector<string> { "lo", "hi" });
    c = pkb.getQPSFacade().getUses(5);
    REQUIRE(c == vector<string> { "target", "arrMid", "mid" });
    c = pkb.getQPSFacade().getUses(8);
    REQUIRE(c == vector<string> { "lo", "y"});

    // var used by stmt
    d = pkb.getQPSFacade().getUses("lo");
    sort(d.begin(), d.end());
    REQUIRE(d == vector<int> { 3, 4, 8 });
    d = pkb.getQPSFacade().getUses("hi");
    REQUIRE(d == vector<int> { 3, 4 });
    d = pkb.getQPSFacade().getUses("n");
    REQUIRE(d == vector<int> { 2 });
    d = pkb.getQPSFacade().getUses("target");
    REQUIRE(d == vector<int> { 5, 3 });
    d = pkb.getQPSFacade().getUses("arrMid");
    REQUIRE(d == vector<int> { 5, 3 });
    d = pkb.getQPSFacade().getUses("mid");
    REQUIRE(d == vector<int> { 6, 5, 3, 7 });
    d = pkb.getQPSFacade().getUses("x");
    REQUIRE(d == vector<int> { });
    d = pkb.getQPSFacade().getUses("y");
    REQUIRE(d == vector<int> { 10, 8 });
}

TEST_CASE("it should save and return all modifies and uses relationship") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testUseAndModifyPath + "use_modify_visitor_2.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> lineNos;
    vector<string> vars;

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();

    auto visitor = std::make_shared<UseAndModifyVisitor>(spf);
    root->accept(visitor.get());

    // modifies
    auto c =  pkb.getQPSFacade().getModifies();
    sort(c.begin(), c.end());
    for (auto p: c) {
        lineNos.push_back(p.first);
        vars.push_back(p.second);
    }
    CHECK(lineNos == vector<int> { 1, 2, 3, 3, 4, 5, 6, 7, 8, 8, 9, 10 });
    CHECK(vars == vector<string> { "a", "b", "d", "e", "d", "e", "h", "h", "a", "happiness", "happiness", "a" });
    lineNos.clear();
    vars.clear();

    // uses
    auto d =  pkb.getQPSFacade().getUses();
    sort(d.begin(), d.end());
    for (auto p: d) {
        lineNos.push_back(p.first);
        vars.push_back(p.second);
    }
    CHECK(lineNos == vector<int> { 3, 6, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10 });
    CHECK(vars == vector<string> { "c", "f", "g", "achieve", "b", "c", "engage", "h", "i", "j", "k", "l", "m",
                                   "meaning", "pEmo", "rel", "achieve", "engage", "meaning", "pEmo", "rel", "b", "c" });
}

TEST_CASE("it should save proc uses and modifies relationship") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testUseAndModifyPath
            + "use_modify_visitor_3.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    // temp data structure for testing
    vector<int> lineNos;
    vector<string> vars;

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();

    auto visitor = std::make_shared<UseAndModifyVisitor>(spf);
    root->accept(visitor.get());

    auto a = qpsf.getUsesPProc("foo");
    std::sort(a.begin(), a.end());
    REQUIRE(a == vector<string> { "foo1", "foo213", "h" });

    auto a2 = qpsf.getModifiesPProc("foo");
    std::sort(a2.begin(), a2.end());
    REQUIRE(a2 == vector<string> { "f", "g", "i", "j" });


    auto b = qpsf.getUsesPProc("bar");
    std::sort(b.begin(), b.end());
    REQUIRE(b == vector<string> { "bar231", "c" });

    auto b2 = qpsf.getModifiesPProc("bar");
    std::sort(b2.begin(), b2.end());
    REQUIRE(b2 == vector<string> { "a", "b", "d", "e" });

    auto c = qpsf.getUsesPProc("car");
    std::sort(c.begin(), c.end());
    REQUIRE(c == vector<string> { "carHonda", "m" });

    auto c2 = qpsf.getModifiesPProc("car");
    std::sort(c2.begin(), c2.end());
    REQUIRE(c2 == vector<string> { "k", "l", "n", "o" });
}


TEST_CASE("it should save indirect proc uses and modifies relationship") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testUseAndModifyPath
                                                                    + "use_modify_visitor_indirect.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();

    auto topoVisitor = std::make_shared<TopoSortVisitor>();
    root->accept(topoVisitor.get());
    auto procs = root->getProcedures();
    REQUIRE(procs.at(0)->getProcName() == "C");
    auto visitor = std::make_shared<UseAndModifyVisitor>(spf);
    root->accept(visitor.get());

    auto modifiedA = qpsf.getModifiesPProc("A");
    std::sort(modifiedA.begin(), modifiedA.end());
    REQUIRE(modifiedA == vector<string> { "x", "y", "z" });

    auto modifiedB = qpsf.getModifiesPProc("B");
    std::sort(modifiedB.begin(), modifiedB.end());
    REQUIRE(modifiedB == vector<string> { "y", "z" });

    auto modifiedC = qpsf.getModifiesPProc("C");
    std::sort(modifiedC.begin(), modifiedC.end());
    REQUIRE(modifiedC == vector<string> { "z" });

    // uses
    auto usedA = qpsf.getUsesPProc("A");
    std::sort(usedA.begin(), usedA.end());
    REQUIRE(usedA == vector<string> { "l", "m", "n", "o", "p", "q" });

    auto usedB = qpsf.getUsesPProc("B");
    std::sort(usedB.begin(), usedB.end());
    REQUIRE(usedB == vector<string> {  "m", "n", "p", "q" });

    auto usedC = qpsf.getUsesPProc("C");
    std::sort(usedC.begin(), usedC.end());
    REQUIRE(usedC == vector<string> { "n", "q" });
}

TEST_CASE("it should save indirect proc uses and modifies relationship - 2") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testUseAndModifyPath
                                                                    + "use_modify_visitor_indirect_2.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();

    auto topoVisitor = std::make_shared<TopoSortVisitor>();
    root->accept(topoVisitor.get());

    vector<string> procs;
    for (auto p:root->getProcedures()) {
        procs.push_back(p->getProcName());
    }
    REQUIRE(procs == vector<string> { "D", "C", "B", "A" });

    auto visitor = std::make_shared<UseAndModifyVisitor>(spf);
    root->accept(visitor.get());

    auto modifiedA = qpsf.getModifiesPProc("A");
    std::sort(modifiedA.begin(), modifiedA.end());
    REQUIRE(modifiedA == vector<string> { "hello", "y", "z" });

    auto modifiedB = qpsf.getModifiesPProc("B");
    std::sort(modifiedB.begin(), modifiedB.end());
    REQUIRE(modifiedB == vector<string> { "hello", "y", "z" });

    auto modifiedC = qpsf.getModifiesPProc("C");
    std::sort(modifiedC.begin(), modifiedC.end());
    REQUIRE(modifiedC == vector<string> { "z" });

    // uses
    auto usedA = qpsf.getUsesPProc("A");
    std::sort(usedA.begin(), usedA.end());
    REQUIRE(usedA == vector<string> { "m", "n", "p", "q", "world" });

    auto usedB = qpsf.getUsesPProc("B");
    std::sort(usedB.begin(), usedB.end());
    REQUIRE(usedB == vector<string> { "m", "n", "p", "q", "world" });

    auto usedC = qpsf.getUsesPProc("C");
    std::sort(usedC.begin(), usedC.end());
    REQUIRE(usedC == vector<string> { "n", "q" });

    // A uses
    vector<string> placeholder = qpsf.getUses(1);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "m", "n", "p", "q", "world" });

    placeholder = qpsf.getUses(2);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "m", "n", "p", "q", "world" });

    // A modifies
    placeholder= qpsf.getModifies(1);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "hello", "y", "z" });

    placeholder= qpsf.getModifies(2);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "hello", "y", "z" });

    // B uses
    placeholder = qpsf.getUses(4);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "m", "n", "p", "q", "world" });

    placeholder = qpsf.getUses(5);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "n", "q" });

    placeholder = qpsf.getUses(6);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> {  "world" });

    // B modifies
    placeholder= qpsf.getModifies(4);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "hello",  "z" });

    placeholder= qpsf.getModifies(5);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "z" });

    placeholder= qpsf.getModifies(6);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "hello"});
}

TEST_CASE("it should save indirect proc uses and modifies relationship - 3 lecture example") {
    std::shared_ptr<std::ifstream> src = make_shared<std::ifstream>(testUseAndModifyPath
                                                                    + "use_modify_visitor_indirect_3.txt");
    std::shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    PKB pkb;
    std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();

    auto topoVisitor = std::make_shared<TopoSortVisitor>();
    root->accept(topoVisitor.get());


    auto visitor = std::make_shared<UseAndModifyVisitor>(spf);
    root->accept(visitor.get());

    auto modifiedA = qpsf.getModifiesPProc("computeCentroid");
    std::sort(modifiedA.begin(), modifiedA.end());
    REQUIRE(modifiedA == vector<string> { "cenX", "cenY", "count", "flag", "normSq", "x", "y" });

    auto modifiedB = qpsf.getModifiesPProc("main");
    std::sort(modifiedB.begin(), modifiedB.end());
    REQUIRE(modifiedB == vector<string> { "cenX", "cenY", "count", "flag", "normSq", "x", "y" });


    // uses
    auto usedA = qpsf.getUsesPProc("computeCentroid");
    std::sort(usedA.begin(), usedA.end());
    REQUIRE(usedA == vector<string> { "cenX", "cenY", "count", "x", "y" });

    auto usedB = qpsf.getUsesPProc("main");
    std::sort(usedB.begin(), usedB.end());
    REQUIRE(usedB == vector<string>  { "cenX", "cenY", "count", "flag", "normSq", "x", "y" });


    vector<string> placeholder;
    vector<string> placeholder2;

    // 5
    placeholder= qpsf.getModifies(5);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "cenX", "cenY", "count", "x", "y" });

    placeholder = qpsf.getUses(5);
    std::sort(placeholder.begin(), placeholder.end());
    REQUIRE(placeholder == vector<string> { "cenX", "cenY", "count", "x", "y" });

    //readpoint starts at 15

    // 22
    placeholder= qpsf.getModifies(22);
    std::sort(placeholder.begin(), placeholder.end());
    placeholder2= qpsf.getModifiesPProc("computeCentroid");
    std::sort(placeholder2.begin(), placeholder2.end());
    REQUIRE(placeholder == placeholder2);

    placeholder = qpsf.getUses(22);
    std::sort(placeholder.begin(), placeholder.end());
    placeholder2= qpsf.getUsesPProc("computeCentroid");
    std::sort(placeholder2.begin(), placeholder2.end());
    REQUIRE(placeholder == placeholder2);
}

