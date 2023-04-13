#include "qps/QueryProcessor.h"
#include "pkb/PKB.h"
#include <string>
#include <iostream>
#include "catch.hpp"

TEST_CASE("Test QueryProcessor") {
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

	SECTION("Invalid Query") {
		std::vector<std::string> expected = { "g" };
		//REQUIRE(qps.process("invalid") == expected);
	}
	SECTION("Empty Result") {
		std::vector<std::string> expected = { "dummyVal" };
		REQUIRE(qps.process("stmt s; variable v; Select s such that Uses(s, v)") == expected);
		//REQUIRE(1 == 1);
	}
	SECTION("No Clause") {
		std::vector<std::string> expected = { "a", "c", "e"};
		//REQUIRE(qps.process("stmt s; Select s such that Uses (s, \"a\")") == expected);
	}
	SECTION("Different Select") {
		std::vector<std::string> expected = { "patternTest" };
		//REQUIRE(qps.process("variable v; stmt s; Select s such that Uses(1, v)") == expected);
		REQUIRE(qps.process("assign a; variable v; Select a pattern a (\"varX\", _\"123\"_)") == expected);
	}
}

TEST_CASE("Test Uses and Modifies for procedures") {
    PKB pkb;
    std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
    QPSFacade qpsFacade = pkb.getQPSFacade();
    auto qps = QueryProcessor(qpsFacade);
    spFacade->addStmtUses(1, "a");
    spFacade->addStmtUses(1, "c");
    spFacade->addStmtUses(2, "e");
    spFacade->addStmtModifies(3, "a");
    spFacade->addStmtModifies(5, "c");
    spFacade->addVariable("a");
    spFacade->addVariable("c");
    spFacade->addVariable("e");
    spFacade->addProcedure("proc");
    spFacade->addProcUses("proc", "a");
    spFacade->addProcUses("proc", "c");
    spFacade->addProcUses("proc", "e");
    spFacade->addProcModifies("proc", "a");
    spFacade->addProcModifies("proc", "c");

    SECTION("2 synonym Uses") {
        std::vector<std::string> expected = { "proc" };
        REQUIRE(qps.process("procedure a; variable v; Select a such that Uses (a, v)") == expected);
        std::vector<std::string> expectedVariables = { "a", "c", "e" };
        REQUIRE(qps.process("procedure a; variable v; Select v such that Uses (a, v)") == expectedVariables);
    }

    SECTION("2 synonym Modifies") {
        std::vector<std::string> expected = { "proc" };
        REQUIRE(qps.process("procedure a; variable v; Select a such that Modifies (a, v)") == expected);
        std::vector<std::string> expectedVariables = { "a", "c" };
        REQUIRE(qps.process("procedure a; variable v; Select v such that Modifies (a, v)") == expectedVariables);
    }

    SECTION("1 synonym Uses") {
        std::vector<std::string> expected = { "proc" };
        REQUIRE(qps.process("procedure p; Select p such that Uses(p, \"a\")") == expected);
        std::vector<std::string> expectedVariables = { "a", "c", "e" };
        REQUIRE(qps.process("variable v; Select v such that Uses(\"proc\", v)") == expectedVariables);
    }

    SECTION("1 synonym Modifies") {
        std::vector<std::string> expected = { "proc" };
        REQUIRE(qps.process("procedure p; Select p such that Modifies(p, \"a\")") == expected);
        std::vector<std::string> expectedVariables = { "a", "c" };
        REQUIRE(qps.process("variable v; Select v such that Modifies(\"proc\", v)") == expectedVariables);
    }

//    SECTION("0 synonym Uses") {
//        std::vector<std::string> expected = { "true" };
//        REQUIRE(qps.process("Select BOOLEAN such that Uses(\"proc\", \"a\")") == expected);
//    }
//
//    SECTION("0 synonym Modifies") {
//        std::vector<std::string> expected = { "true" };
//        std::vector<std::string> expectedFalse = { "false" };
//        REQUIRE(qps.process("Select BOOLEAN such that Modifies(\"proc\", \"a\")") == expected);
//        REQUIRE(qps.process("Select BOOLEAN such that Modifies(\"proc\", \"e\")") == expected);
//    }

}

TEST_CASE("Test Uses and Modifies for calls") {
    PKB pkb;
    std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
    QPSFacade qpsFacade = pkb.getQPSFacade();
    auto qps = QueryProcessor(qpsFacade);

    spFacade->addProcedure("proc");
    spFacade->addProcedure("proc2");
    spFacade->addCalls("proc", "proc2");

    spFacade->addVariable("a");
    spFacade->addVariable("c");
    spFacade->addVariable("e");

    spFacade->addStmtUses(1, "a");
    spFacade->addStmtUses(1, "c");
    spFacade->addStmtModifies(2, "a");
    spFacade->addCallStmt(3, "proc2");
    spFacade->addStmtModifies(5, "c");
    spFacade->addStmtUses(5, "e");
    spFacade->addStmtUses(5, "c");

    spFacade->addProcUses("proc", "a");
    spFacade->addProcUses("proc", "c");
    spFacade->addProcUses("proc2", "c");
    spFacade->addProcUses("proc2", "e");
    spFacade->addProcModifies("proc", "a");
    spFacade->addProcModifies("proc2", "c");

    SECTION("2 synonym Uses") {
        std::vector<std::string> expected = { "3" };
        REQUIRE(qps.process("call a; variable v; Select a such that Uses (a, v)") == expected);
        std::vector<std::string> expectedVariables = { "c", "e" };
        REQUIRE(qps.process("call a; variable v; Select v such that Uses (a, v)") == expectedVariables);
    }

    SECTION("2 synonym Modifies") {
        std::vector<std::string> expected = { "3" };
        REQUIRE(qps.process("call a; variable v; Select a such that Modifies (a, v)") == expected);
        std::vector<std::string> expectedVariables = { "a", "c" };
        REQUIRE(qps.process("call a; variable v; Select v such that Modifies (a, v)") == expectedVariables);
    }

    SECTION("1 synonym Uses") {
        std::vector<std::string> expected = { "3" };
        REQUIRE(qps.process("call c; Select c such that Uses(c, \"c\")") == expected);
        std::vector<std::string> expectedVariables = { "c", "e" };
        REQUIRE(qps.process("variable v; Select v such that Uses(3, v)") == expectedVariables);
    }

    SECTION("1 synonym Modifies") {
        std::vector<std::string> expected = { "3" };
        REQUIRE(qps.process("call c; Select c such that Modifies(c, \"c\")") == expected);
        std::vector<std::string> expectedVariables = { "a", "c" };
        REQUIRE(qps.process("variable v; Select v such that Modifies(3, v)") == expectedVariables);
    }

//    SECTION("0 synonym Uses") {
//        std::vector<std::string> expected = { "true" };
//        std::vector<std::string> expectedFalse = { "false" };
//        REQUIRE(qps.process("Select BOOLEAN such that Uses(3, \"c\")") == expected);
//        REQUIRE(qps.process("Select BOOLEAN such that Uses(3, \"a\")") == expectedFalse);
//    }
//
//    SECTION("0 synonym Modifies") {
//        std::vector<std::string> expected = { "true" };
//        std::vector<std::string> expectedFalse = { "false" };
//        REQUIRE(qps.process("Select BOOLEAN such that Modifies(3, \"a\")") == expected);
//        REQUIRE(qps.process("Select BOOLEAN such that Modifies(3, \"e\")") == expectedFalse);
//    }
}