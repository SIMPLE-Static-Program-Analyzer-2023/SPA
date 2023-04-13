#include "qps/QueryProcessor.h"
#include <string>
#include <iostream>
#include "catch.hpp"
#include "pkb/Facade/QPSFacade.h"

TEST_CASE("Test query process flow") {
    std::string input = "variable v; Select v such that Uses(1, v)";
    PKB pkb;
    std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
    QPSFacade qpsFacade = pkb.getQPSFacade();
    QueryProcessor qps = QueryProcessor(qpsFacade);
    spFacade->addStmtUses(1, "a");
    spFacade->addStmtUses(1, "c");
    spFacade->addStmtUses(2, "e");
    spFacade->addStatement(1, StatementType::StatementAssign);
    spFacade->addStatement(2, StatementType::StatementAssign);
    spFacade->addVariable("a");
    spFacade->addVariable("c");
    spFacade->addVariable("e");

    SECTION("Invalid Query") {
        std::vector<std::string> expected = { "g" };
        //  REQUIRE(qps.process("invalid") == expected);
    }
    SECTION("Empty Result") {
        std::vector<std::string> expected = { "1", "2" };
        //  REQUIRE(qps.process("stmt s; Select s such that Uses(1, \"a\")") == expected);
        std::vector<std::string> expectedEmpty;
        //  REQUIRE(qps.process("assign a; variable v; Select a pattern a (_, _\"123\"_)") == expectedEmpty);
    }

    SECTION("No Clause") {
        std::vector<std::string> expected = { "a", "c", "e"};
        //  REQUIRE(qps.process("variable v; Select v") == expected);
    }
    SECTION("Different Select") {
        std::vector<std::string> expected = { "1", "2" };
        //  REQUIRE(qps.process("stmt s; variable v; Select s such that Uses(1, v)") == expected);
    }
}

TEST_CASE("Test syntax exception") {
    PKB pkb;
    std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
    QPSFacade qpsFacade = pkb.getQPSFacade();
    QueryProcessor qps = QueryProcessor(qpsFacade);
    spFacade->addStmtUses(1, "a");
    spFacade->addStmtUses(1, "c");
    spFacade->addStmtUses(2, "e");
    //spFacade->addStatement(1, StatementType::StatementAssign);
    spFacade->addStatement(2, StatementType::StatementAssign);
    spFacade->addVariable("a");
    spFacade->addVariable("ab");
    spFacade->addVariable("abc");

    std::vector<std::string> expected = { "SyntaxError" };

    REQUIRE(qps.process("procedure p; Select 1") == expected);
    REQUIRE(qps.process("Select") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Follows(, 2)") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Follows(1,)") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Follows(,)") == expected);

    REQUIRE(qps.process("procedure p; Select p such that Uses(p, 2)") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Modifies(p, 2)") == expected);
    REQUIRE(qps.process("procedure p; assign a; Select p pattern a (_, p)") == expected);

    // queries with abrupt ending
    REQUIRE(qps.process("if ifs; Select ifs pattern") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Modifies") == expected);
    REQUIRE(qps.process("procedure p; assign a; Select") == expected);
    REQUIRE(qps.process("procedure p; assign a; Select p such that pattern a (_, _)") == expected); // such that keyword before pattern
    REQUIRE(qps.process("procedure p; Select p such that Follows(1, 2) and") == expected);
    REQUIRE(qps.process("procedure p; assign a; Select p such that") == expected);

    // invalid attributes
    REQUIRE(qps.process("assign a, a1; Select a1 with a.stmt = 12") == expected);
    REQUIRE(qps.process("assign a, a1; Select a1 with a.number = 12") == expected);
}

TEST_CASE("Test semantic exception") {
    PKB pkb;
    std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
    QPSFacade qpsFacade = pkb.getQPSFacade();
    QueryProcessor qps = QueryProcessor(qpsFacade);
    spFacade->addStmtUses(1, "a");
    spFacade->addStmtUses(1, "c");
    spFacade->addStmtUses(2, "e");
    //spFacade->addStatement(1, StatementType::StatementAssign);
    spFacade->addStatement(2, StatementType::StatementAssign);
    spFacade->addVariable("a");
    spFacade->addVariable("ab");
    spFacade->addVariable("abc");

    std::vector<std::string> expected = { "SemanticError" };
    REQUIRE(qps.process("Select p") == expected);
    REQUIRE(qps.process("variable v; Select v such that Uses(_, v)") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Follows (p, _)") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Follows*(_ ,p)") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Parent(_, p)") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Parent*(_, p)") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Parent (\"a\", _)") == expected);
    REQUIRE(qps.process("procedure p; assign a; Select p pattern a (p, _)") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Parent(_, p)") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Parent*(p, _)") == expected);
    REQUIRE(qps.process("procedure p; constant const; Select p such that Follows(const, _)") == expected);

    REQUIRE(qps.process("procedure p; read rd; constant const; Select p such that Uses(rd, _)") == expected);
    REQUIRE(qps.process("procedure p; print pn; constant const; Select p such that Modifies(pn, _)") == expected);
    REQUIRE(qps.process("procedure p; read rd; constant const; Select p such that Uses(const, _)") == expected);
    REQUIRE(qps.process("procedure p; read rd; constant const; Select p such that Modifies(const, _)") == expected);
    REQUIRE(qps.process("procedure p; variable v; constant const; Select p such that Modifies(v, _)") == expected);
    REQUIRE(qps.process("procedure p; variable v; constant const; Select p such that Modifies(p, const)") == expected);

    // undeclared variables
    REQUIRE(qps.process("procedure p; Select p pattern a (_, _)") == expected);
    REQUIRE(qps.process("procedure p; Select v") == expected);
    REQUIRE(qps.process("procedure p; Select p such that Follows(w, 2)") == expected);

    // attributes mismatch
    REQUIRE(qps.process("assign a, a1; Select a1 with a.varName = \"12\"") == expected);
    REQUIRE(qps.process("assign a, a1; Select a1 with a.procName = \"12\"") == expected);
    REQUIRE(qps.process("assign a, a1; Select a1 with a.stmt# = \"12\"") == expected);
    REQUIRE(qps.process("procedure p; variable v; Select p with p.procName = 12") == expected);
    REQUIRE(qps.process("procedure p; variable v; Select p with p.procName = v.value") == expected);
    REQUIRE(qps.process("procedure p; call c; Select p with p.procName = c.stmt#") == expected);
    REQUIRE(qps.process("assign a, a1; Select a1 with \"12\" = a.procName") == expected);
    REQUIRE(qps.process("assign a, a1; Select a1 with \"12\" = a.stmt#") == expected);
    REQUIRE(qps.process("assign a, a1; Select a1 with \"12\" = 12") == expected);
    REQUIRE(qps.process("assign a, a1; Select a1 with \"12\" = \"test\"") == expected);
}
