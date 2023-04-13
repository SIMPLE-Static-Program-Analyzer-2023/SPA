#include "qps/Clause.h"
#include "pkb/PKB.h"
#include <string>
#include <iostream>
#include "catch.hpp"

TEST_CASE("General Clause") {
	SECTION("Getters") {
		std::string noSynLHS = "1";
		std::string noSynRHS = "\"x\"";
		Clause clause = Clause(noSynLHS, noSynRHS);
		REQUIRE(clause.getLHS() == "1");
		REQUIRE(clause.getRHS() == "\"x\"");
	}
	SECTION("isSynonym") {
		std::string noSynLHS = "s";
		std::string noSynRHS = "\"x\"";
		Clause clause = Clause(noSynLHS, noSynRHS);
		REQUIRE(clause.isSynonym(clause.getLHS()) == true);
		REQUIRE(clause.isSynonym(clause.getRHS()) == false);
	}
}

TEST_CASE("FollowsClause Evaluate") {
	std::vector<int> ifs = { 1,2, 3 };
	ifs.erase(ifs.begin() + 2);
	REQUIRE(ifs[0] == 1);
	REQUIRE((!(std::find(ifs.begin(), ifs.end(), 3) != ifs.end())));

	PKB pkb;
	std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
	QPSFacade qpsFacade = pkb.getQPSFacade();
	std::string target = "dummy";
	SECTION("0 synonym") {
		spFacade->addFollows(1, 2);
		std::string noSynLHS = "1";
		std::string noSynRHS = "2";
		FollowsClause followsClause = FollowsClause(noSynLHS, noSynRHS);
		ClauseResult clauseResult = ClauseResultBuilder::createBooleanResult(true);
		//REQUIRE(followsClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("LHS synonym") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		std::string synLHS = "s";
		std::string noSynRHS = "2";
		FollowsClause followsClause = FollowsClause(synLHS, noSynRHS);
		std::vector<std::string> list;
		list.push_back("1");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synLHS, list);
		//REQUIRE(followsClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("RHS synonym") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		std::string noSynLHS = "2";
		std::string synRHS = "s";
		FollowsClause followsClause = FollowsClause(noSynLHS, synRHS);
		std::vector<std::string> list;
		list.push_back("3");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synRHS, list);
		//REQUIRE(followsClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("2 synonyms") {
		// to be implemented
		//REQUIRE(0 == 0);
	}
}

TEST_CASE("FollowsTClause Evaluate") {
	PKB pkb;
	std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
	QPSFacade qpsFacade = pkb.getQPSFacade();
	std::string target = "dummy";
	SECTION("0 synonym") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		std::string noSynLHS = "1";
		std::string noSynRHS = "3";
		FollowsTClause followsTClause = FollowsTClause(noSynLHS, noSynRHS);
		ClauseResult clauseResult = ClauseResultBuilder::createBooleanResult(true);
		//REQUIRE(followsTClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("LHS synonym") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		std::string synLHS = "s";
		std::string noSynRHS = "3";
		FollowsTClause followsTClause = FollowsTClause(synLHS, noSynRHS);
		std::vector<std::string> list;
		list.push_back("1");
		list.push_back("2");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synLHS, list);
		//REQUIRE(followsTClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("RHS synonym") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		spFacade->addFollows(3, 4);
		std::string noSynLHS = "2";
		std::string synRHS = "s";
		FollowsTClause followsTClause = FollowsTClause(noSynLHS, synRHS);
		std::vector<std::string> list;
		list.push_back("3");
		list.push_back("4");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synRHS, list);
		//REQUIRE(followsTClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("2 synonyms") {
		spFacade->addFollows(1, 2);
		spFacade->addFollows(2, 3);
		std::string synLHS = "s1";
		std::string synRHS = "s2";
		FollowsTClause followsTClause = FollowsTClause(synLHS, synRHS);
		std::vector<std::string> synList;
		synList.push_back(synLHS);
		synList.push_back(synRHS);
		std::vector<std::string> list1;
		list1.push_back("1");
		list1.push_back("2");
		std::vector<std::string> list2;
		list2.push_back("2");
		list2.push_back("3");
		std::vector<std::string> list3;
		list3.push_back("1");
		list3.push_back("3");
		std::vector<std::vector<std::string>> tupleList;
		tupleList.push_back(list1);
		tupleList.push_back(list2);
		tupleList.push_back(list3);
		ClauseResult clauseResult = ClauseResultBuilder::createTupleResult(synList, tupleList);
		//REQUIRE(followsTClause.evaluate(qpsFacade, target) == clauseResult);
	}
}

TEST_CASE("UsesClause Evaluate") {
	PKB pkb;
    std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
	QPSFacade qpsFacade = pkb.getQPSFacade();
	std::string target = "dummy";
	SECTION("0 synonym") {
		spFacade->addStmtUses(1, "\"x\"");
		std::string noSynLHS = "1";
		std::string noSynRHS = "\"x\"";
		UsesClause usesClause = UsesClause(noSynLHS, noSynRHS);
		ClauseResult clauseResult = ClauseResultBuilder::createBooleanResult(true);
		//REQUIRE(usesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("LHS synonym") {
		spFacade->addStmtUses(1, "\"x\"");
		spFacade->addStmtUses(3, "\"x\"");
		std::string synLHS = "s";
		std::string noSynRHS = "\"x\"";
		UsesClause usesClause = UsesClause(synLHS, noSynRHS);
		std::vector<std::string> list;
		list.push_back("1");
		list.push_back("3");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synLHS, list);
		//REQUIRE(usesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("RHS synonym") {
		spFacade->addStmtUses(1, "\"x\"");
		spFacade->addStmtUses(1, "\"y\"");
		std::string noSynLHS = "1";
		std::string synRHS = "v";
		UsesClause usesClause = UsesClause(noSynLHS, synRHS);
		std::vector<std::string> list;
		list.push_back("\"x\"");
		list.push_back("\"y\"");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synRHS, list);
		//REQUIRE(usesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("2 synonyms") {
		spFacade->addStmtUses(1, "\"x\"");
		spFacade->addStmtUses(3, "\"y\"");
		std::string synLHS = "s";
		std::string synRHS = "v";
		UsesClause usesClause = UsesClause(synLHS, synRHS);
		std::vector<std::string> synList;
		synList.push_back(synLHS);
		synList.push_back(synRHS);
		std::vector<std::string> list1;
		list1.push_back("1");
		list1.push_back("\"x\"");
		std::vector<std::string> list2;
		list2.push_back("3");
		list2.push_back("\"y\"");
		std::vector<std::vector<std::string>> tupleList;
		tupleList.push_back(list1);
		tupleList.push_back(list2);
		ClauseResult clauseResult = ClauseResultBuilder::createTupleResult(synList, tupleList);
		//REQUIRE(usesClause.evaluate(qpsFacade, target) == clauseResult);
	}
    SECTION("Procedures (strings instead of stmtRef)") {
        spFacade->addProcedure("main");
        spFacade->addProcedure("second");
        spFacade->addProcedure("third");
        spFacade->addVariable("x");
        spFacade->addVariable("y");
        spFacade->addVariable("z");
        spFacade->addCallStmt(1, "main");
        spFacade->addCallStmt(2, "second");
        spFacade->addCallStmt(3, "third");
        spFacade->addCalls("main", "second");
        spFacade->addCalls("second", "third");
        spFacade->addStmtUses(1, "x");
        spFacade->addStmtUses(2, "y");
        spFacade->addProcUses("main", "x");
        spFacade->addProcUses("second", "x"); // second uses both x and y
        spFacade->addProcUses("second", "y");
        spFacade->addProcUses("third", "z"); // third uses z but third is not called
        std::unordered_map<std::string, std::string> synonymTypes;
        synonymTypes["proc"] = "procedure";
        synonymTypes["v"] = "variable";
        synonymTypes["c"] = "call";

        // main uses x, y and z (since main calls second and second calls third)
        // second uses x and y and z (since second calls third)
        // third uses z

        SECTION("0 synonym") {
            UsesClause testProcStmt1 = UsesClause("main", "x");
            UsesClause testProcStmt2 = UsesClause("second", "x");
            UsesClause testProcStmt3 = UsesClause("second", "y");
            UsesClause testProcStmt4 = UsesClause("main", "y");
            UsesClause testProcStmt5 = UsesClause("third", "x");

            ClauseResult trueResult = ClauseResultBuilder::createBooleanResult(true);
            ClauseResult falseResult = ClauseResultBuilder::createBooleanResult(false);

            auto result = testProcStmt1.evaluate(qpsFacade, synonymTypes);

//            REQUIRE(testProcStmt1.evaluate(qpsFacade, target, synonymTypes) == trueResult);
//            REQUIRE(testProcStmt2.evaluate(qpsFacade, target, synonymTypes) == trueResult);
//            REQUIRE(testProcStmt3.evaluate(qpsFacade, target, synonymTypes) == trueResult);
//            REQUIRE(testProcStmt4.evaluate(qpsFacade, target, synonymTypes) == trueResult);
//            REQUIRE(testProcStmt5.evaluate(qpsFacade, target, synonymTypes) == falseResult);
        }

        SECTION("LHS synonym") {
            UsesClause testProcStmt1 = UsesClause("proc", "x");
            UsesClause testProcStmt2 = UsesClause("proc", "y");
            UsesClause testProcStmt3 = UsesClause("proc", "z");

            std::vector<std::string> list1;
            list1.push_back("main");
            list1.push_back("second");
            ClauseResult result1 = ClauseResultBuilder::createListResult("proc", list1);
            std::vector<std::string> list2;
            list2.push_back("main");
            list2.push_back("second");
            ClauseResult result2 = ClauseResultBuilder::createListResult("proc", list2);
            std::vector<std::string> list3;
            list3.push_back("third");
            ClauseResult result3 = ClauseResultBuilder::createListResult("proc", list3);

//            REQUIRE(testProcStmt1.evaluate(qpsFacade, target, synonymTypes) == result1);
//            REQUIRE(testProcStmt2.evaluate(qpsFacade, target, synonymTypes) == result2);
//            REQUIRE(testProcStmt3.evaluate(qpsFacade, target, synonymTypes) == result3);
        }

        SECTION("RHS Synonym") {
            UsesClause testProcStmt1 = UsesClause("main", "v");  // True
            UsesClause testProcStmt2 = UsesClause("second", "v");  // True
            UsesClause testProcStmt3 = UsesClause("third", "v");  // True

            std::vector<std::string> list1;
            list1.push_back("x");
            list1.push_back("y");
            list1.push_back("z");
            ClauseResult result1 = ClauseResultBuilder::createListResult("v", list1);
            std::vector<std::string> list2;
            list2.push_back("z");
            ClauseResult result2 = ClauseResultBuilder::createListResult("v", list2);

//            REQUIRE(testProcStmt1.evaluate(qpsFacade, target, synonymTypes) == result1);
//            REQUIRE(testProcStmt2.evaluate(qpsFacade, target, synonymTypes) == result1);
//            REQUIRE(testProcStmt3.evaluate(qpsFacade, target, synonymTypes) == result2);
        }

        SECTION("2 synonyms") {
            UsesClause testProcStmt1 = UsesClause("proc", "v");

            std::vector<std::vector<std::string>> list1;
            list1.push_back({"main", "x"});
            list1.push_back({"main", "y"});
            list1.push_back({"main", "z"});
            list1.push_back({"second", "x"});
            list1.push_back({"second", "y"});
            list1.push_back({"second", "z"});
            list1.push_back({"third", "z"});
            std::vector<std::string> synonymList = {"proc", "v"};
            ClauseResult result1 = ClauseResultBuilder::createTupleResult(synonymList, list1);

//            REQUIRE(testProcStmt1.evaluate(qpsFacade, target, synonymTypes) == result1);
        }
    }
}

TEST_CASE("ModifiesClause Evaluate") {
	PKB pkb;
	std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
	QPSFacade qpsFacade = pkb.getQPSFacade();
	std::string target = "dummy";
	SECTION("0 synonym") {
		spFacade->addStmtModifies(1, "\"x\"");
		std::string noSynLHS = "1";
		std::string noSynRHS = "\"x\"";
		ModifiesClause modifiesClause = ModifiesClause(noSynLHS, noSynRHS);
		ClauseResult clauseResult = ClauseResultBuilder::createBooleanResult(true);
		//REQUIRE(modifiesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("LHS synonym") {
		spFacade->addStmtModifies(1, "\"x\"");
		spFacade->addStmtModifies(3, "\"x\"");
		std::string synLHS = "s";
		std::string noSynRHS = "\"x\"";
		ModifiesClause modifiesClause = ModifiesClause(synLHS, noSynRHS);
		std::vector<std::string> list;
		list.push_back("1");
		list.push_back("3");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synLHS, list);
		//REQUIRE(modifiesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("RHS synonym") {
		spFacade->addStmtModifies(1, "\"x\"");
		spFacade->addStmtModifies(2, "\"y\"");
		std::string noSynLHS = "1";
		std::string synRHS = "v";
		ModifiesClause modifiesClause = ModifiesClause(noSynLHS, synRHS);
		std::vector<std::string> list;
		list.push_back("\"x\"");
		ClauseResult clauseResult = ClauseResultBuilder::createListResult(synRHS, list);
		//REQUIRE(modifiesClause.evaluate(qpsFacade, target) == clauseResult);
	}
	SECTION("2 synonyms") {
		spFacade->addStmtModifies(1, "\"x\"");
		spFacade->addStmtModifies(3, "\"y\"");
		spFacade->addStmtModifies(5, "\"x\"");
		std::string synLHS = "s";
		std::string synRHS = "v";
		ModifiesClause modifiesClause = ModifiesClause(synLHS, synRHS);
		std::vector<std::string> synList;
		synList.push_back(synLHS);
		synList.push_back(synRHS);
		std::vector<std::string> list1;
		list1.push_back("1");
		list1.push_back("\"x\"");
		std::vector<std::string> list2;
		list2.push_back("3");
		list2.push_back("\"y\"");
		std::vector<std::string> list3;
		list3.push_back("5");
		list3.push_back("\"x\"");
		std::vector<std::vector<std::string>> tupleList;
		tupleList.push_back(list1);
		tupleList.push_back(list2);
		tupleList.push_back(list3);
		ClauseResult clauseResult = ClauseResultBuilder::createTupleResult(synList, tupleList);
		//REQUIRE(modifiesClause.evaluate(qpsFacade, target) == clauseResult);
	}
}