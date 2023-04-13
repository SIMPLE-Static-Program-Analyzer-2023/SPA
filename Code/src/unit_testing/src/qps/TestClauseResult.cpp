#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "catch.hpp"

#include "pkb/PKB.h"
#include "pkb/Facade/QPSFacade.h"
#include "pkb/Facade/SPFacade.h"
#include "qps/QueryEvaluator/ClauseResultBuilder.h"
#include "qps/QueryEvaluator/ClauseResult.h"

void populatePkb(std::shared_ptr<SPFacade> &spf) {
	spf->addConstant("5");
	spf->addConstant("10");
	spf->addConstant("15");
	spf->addConstant("124");
	spf->addVariable("x");
	spf->addVariable("y");
	spf->addVariable("z");
	spf->addVariable("abs");
	spf->addStatement(1, StatementType::StatementAssign);
	spf->addStatement(2, StatementType::StatementCall);
	spf->addStatement(3, StatementType::StatementAssign);
	spf->addStatement(4, StatementType::StatementWhile);
	spf->addStatement(5, StatementType::StatementAssign);
	spf->addStatement(6, StatementType::StatementRead);
	spf->addStatement(7, StatementType::StatementIf);
}

TEST_CASE("Boolean ClauseResult methods") {
	
	std::vector<std::vector<std::string>> emptyTupleList;
	std::vector<std::string> emptyStringList;
	PKB pkb;
	QPSFacade qpsf = pkb.getQPSFacade();

	SECTION("False") {
		ClauseResult falseResult = ClauseResultBuilder::createBooleanResult(false);
		REQUIRE(falseResult.getTable() == emptyTupleList);
		REQUIRE(falseResult.isBoolean());
		REQUIRE(falseResult.size() == 0);
		REQUIRE(falseResult.extractQueryData(qpsf, "test", "testType") == emptyStringList);
	}
	
	SECTION("True") {
		ClauseResult trueResult = ClauseResultBuilder::createBooleanResult(true);
		REQUIRE(trueResult.getTable() == emptyTupleList);
		REQUIRE(trueResult.isBoolean());
		REQUIRE(trueResult.size() == 0);
		REQUIRE(trueResult.extractQueryData(qpsf, "test", "variable") == emptyStringList);
		REQUIRE_THROWS(trueResult.extractQueryData(qpsf, "test", "nonvalid-type") == emptyStringList);

        std::shared_ptr<SPFacade> spf = pkb.getSPFacade();
		populatePkb(spf);
    
		std::vector<std::string> varList = { "abs", "x", "y", "z" };
		auto resultList1 = trueResult.extractQueryData(qpsf, "test", "variable");
		std::sort(resultList1.begin(), resultList1.end());
		REQUIRE(resultList1 == varList);

		std::vector<std::string> stmtList = { "1", "2", "3", "4", "5", "6" , "7"};
		auto resultList2 = trueResult.extractQueryData(qpsf, "anySynonymName", "stmt");
		std::sort(resultList2.begin(), resultList2.end());
		REQUIRE(resultList2 == stmtList);

		std::vector<std::string> constantList = { "10", "124", "15", "5" };
		auto resultList3 = trueResult.extractQueryData(qpsf, "", "constant");
		std::sort(resultList3.begin(), resultList3.end());
		REQUIRE(resultList3 == constantList);
	}
}

TEST_CASE("Table ClauseResult methods") {
	PKB pkb;
	QPSFacade qpsf = pkb.getQPSFacade();
	std::vector<std::vector<std::string>> emptyTupleList;
	std::vector<std::string> emptyStringList;
    auto spf = pkb.getSPFacade();
    spf->addVariable("x");
    spf->addVariable("y");
    spf->addVariable("z");

	SECTION("List") {
		std::vector<std::string> list = { "x", "y", "z" };
		ClauseResult listResult = ClauseResultBuilder::createListResult("v", list);
		REQUIRE_FALSE(listResult.isBoolean());
		REQUIRE(listResult.size() == 1);
		REQUIRE(listResult.extractQueryData(qpsf, "v", "variable") == list);
		REQUIRE(listResult.extractQueryData(qpsf, "c", "constant") == emptyStringList);
	}

	SECTION("Tuple") {
        spf->addConstant("1");
        spf->addConstant("2");
        spf->addConstant("3");
		std::vector<std::string> list1 = { "x", "1" };
		std::vector<std::string> list2 = { "y", "2" };
		std::vector<std::string> list3 = { "z", "3" };
		std::vector<std::vector<std::string>> variableList = { list1, list2, list3 };
		std::vector<std::string> synonymList = { "synonym1", "synonym2"};
		ClauseResult tupleResult = ClauseResultBuilder::createTupleResult(synonymList, variableList);
		REQUIRE_FALSE(tupleResult.isBoolean());
		REQUIRE(tupleResult.size() == 2);
		std::vector<std::string> synonym1List = { "x", "y", "z" };
		std::vector<std::string> synonym2List = { "1", "2", "3" };
		REQUIRE(tupleResult.extractQueryData(qpsf, "synonym1", "variable") == synonym1List);
		REQUIRE(tupleResult.extractQueryData(qpsf, "synonym2", "constant") == synonym2List);
		REQUIRE(tupleResult.extractQueryData(qpsf, "invalidSynonym", "stmt") == emptyStringList);
	}
}

TEST_CASE("Extract Query data") {
    PKB pkb;
    QPSFacade qpsf = pkb.getQPSFacade();
    auto spf = pkb.getSPFacade();
    populatePkb(spf);

    std::vector<std::vector<std::string>> emptyTupleList;
    std::vector<std::string> emptyStringList;
    std::vector<std::string> constantList = { "5", "10", "15", "124" };
    std::vector<std::vector<std::string>> constantTupleList = { constantList };
    std::vector<std::string> stmtList = { "1", "2", "3", "4", "5", "6", "7" };
    std::vector<std::vector<std::string>> stmtTupleList = { stmtList };

    SECTION("Boolean") {
        ClauseResult booleanResult = ClauseResultBuilder::createBooleanResult(true);
        ClauseResult falseBooleanResult = ClauseResultBuilder::createBooleanResult(false);

        SECTION("Select BOOLEAN") {
            REQUIRE(booleanResult.extractQueryBoolean());
            REQUIRE_FALSE(falseBooleanResult.extractQueryBoolean());
        }

        SECTION("Select synonym") {
            std::vector<std::string> res = { "x", "y", "z", "abs" };
            REQUIRE(booleanResult.extractQueryData(qpsf, "test", "variable") == res);
            REQUIRE(falseBooleanResult.extractQueryData(qpsf, "test", "variable") == emptyStringList);
        }

        SECTION("Select <tuple>") {
            std::unordered_map<std::string, std::string> synonymTypes;
            synonymTypes["test1"] = "variable";
            synonymTypes["test2"] = "constant";
            // False boolean should return empty tuple list
            REQUIRE(falseBooleanResult.extractQueryTuple({"test1", "test2"}, synonymTypes, qpsf) == emptyTupleList);

            // True boolean should return cross-product of synonym types
            std::vector<std::vector<std::string>> resultList = {
                {"x", "5"}, {"x", "10"}, {"x", "15"}, {"x", "124"},
                {"y", "5"}, {"y", "10"}, {"y", "15"}, {"y", "124"},
                {"z", "5"}, {"z", "10"}, {"z", "15"}, {"z", "124"},
                {"abs", "5"}, {"abs", "10"}, {"abs", "15"}, {"abs", "124"}
            };
            REQUIRE(booleanResult.extractQueryTuple({"test1", "test2"}, synonymTypes, qpsf) == resultList);
        }
    }

    SECTION("List") {
        std::vector<std::string> list = { "x", "y", "z" };
        ClauseResult listResult = ClauseResultBuilder::createListResult("v", list);
        CHECK_FALSE(listResult.isBoolean());
        CHECK(listResult.size() == 1);

        REQUIRE(listResult.extractQueryBoolean());

        REQUIRE(listResult.extractQueryData(qpsf, "v", "variable") == list);
        REQUIRE(listResult.extractQueryData(qpsf, "c", "constant") == constantList);

        std::unordered_map<std::string, std::string> synonymTypes;
        synonymTypes["v"] = "variable";
        synonymTypes["c"] = "constant";
        synonymTypes["dummyVariable"] = "variable";

        std::vector<std::vector<std::string>> resList = {{"x"}, {"y"}, {"z"}};
        REQUIRE(listResult.extractQueryTuple({"v"}, synonymTypes, qpsf) == resList);

        resList = {{ "x" }, { "y" }, { "z" }, { "abs" }};
        REQUIRE(listResult.extractQueryTuple({"dummyVariable"}, synonymTypes, qpsf) == resList);

        resList = {{"5"}, {"10"}, {"15"}, {"124"}};
        REQUIRE(listResult.extractQueryTuple({"c"}, synonymTypes, qpsf) == resList);

        resList = {{"x", "5"}, {"x", "10"}, {"x", "15"}, {"x", "124"},
                   {"y", "5"}, {"y", "10"}, {"y", "15"}, {"y", "124"},
                   {"z", "5"}, {"z", "10"}, {"z", "15"}, {"z", "124"}};
        REQUIRE(listResult.extractQueryTuple({"v", "c"}, synonymTypes, qpsf) == resList);

        resList = {{"x", "5"}, {"x", "10"}, {"x", "15"}, {"x", "124"},
                   {"y", "5"}, {"y", "10"}, {"y", "15"}, {"y", "124"},
                   {"z", "5"}, {"z", "10"}, {"z", "15"}, {"z", "124"},
                   {"abs", "5"}, {"abs", "10"}, {"abs", "15"}, {"abs", "124"}};
        REQUIRE(listResult.extractQueryTuple({"dummyVariable", "c"}, synonymTypes, qpsf) == resList);
    }

    SECTION("Tuple") {
        std::vector<std::string> list1 = { "x", "5" };
        std::vector<std::string> list2 = { "y", "10" };
        std::vector<std::string> list3 = { "z", "15" };
        std::vector<std::vector<std::string>> variableList = { list1, list2, list3 };
        std::vector<std::string> synonymList = { "synonym1", "synonym2"};
        ClauseResult tupleResult = ClauseResultBuilder::createTupleResult(synonymList, variableList);

        std::vector<std::string> synonym1List = { "x", "y", "z" };
        std::vector<std::string> synonym2List = { "10", "15", "5" };

        CHECK_FALSE(tupleResult.isBoolean());
        CHECK(tupleResult.size() == 2);

        REQUIRE(tupleResult.extractQueryBoolean());

        REQUIRE(tupleResult.extractQueryData(qpsf, "synonym1", "variable") == synonym1List);
        REQUIRE(tupleResult.extractQueryData(qpsf, "synonym2", "constant") == synonym2List);
        REQUIRE(tupleResult.extractQueryData(qpsf, "otherSynonym", "stmt") == stmtList);

        std::unordered_map<std::string, std::string> synonymTypes;
        synonymTypes["synonym1"] = "variable";
        synonymTypes["synonym2"] = "constant";
        synonymTypes["otherSynonym"] = "stmt";  // Dummy Statement
        synonymTypes["dummyConstant"] = "constant";
        synonymTypes["dummyVariable"] = "variable";

        std::vector<std::vector<std::string>> resList = {{"x"}, {"y"}, {"z"}};
        REQUIRE(tupleResult.extractQueryTuple({"synonym1"}, synonymTypes, qpsf) == resList);
        REQUIRE(tupleResult.extractQueryTuple({"synonym1", "synonym2"}, synonymTypes, qpsf) == variableList);

        resList = {{"x", "5", "1"}, {"x", "5", "2"}, {"x", "5", "3"}, {"x", "5", "4"}, {"x", "5", "5"},
                   {"x", "5", "6"}, {"x", "5", "7"},
                   {"y", "10", "1"}, {"y", "10", "2"}, {"y", "10", "3"}, {"y", "10", "4"}, {"y", "10", "5"},
                   {"y", "10", "6"}, {"y", "10", "7"},
                   {"z", "15", "1"}, {"z", "15", "2"}, {"z", "15", "3"}, {"z", "15", "4"}, {"z", "15", "5"},
                   {"z", "15", "6"}, {"z", "15", "7"} };
        REQUIRE(tupleResult.extractQueryTuple({"synonym1", "synonym2", "otherSynonym"}, synonymTypes, qpsf) == resList);

        resList = {{"x", "x", "1"}, {"x", "x", "2"}, {"x", "x", "3"}, {"x", "x", "4"}, {"x", "x", "5"},
                   {"x", "x", "6"}, {"x", "x", "7"},
                   {"y", "x", "1"}, {"y", "x", "2"}, {"y", "x", "3"}, {"y", "x", "4"}, {"y", "x", "5"},
                   {"y", "x", "6"}, {"y", "x", "7"},
                   {"z", "x", "1"}, {"z", "x", "2"}, {"z", "x", "3"}, {"z", "x", "4"}, {"z", "x", "5"},
                   {"z", "x", "6"}, {"z", "x", "7"},
                   {"abs", "x", "1"}, {"abs", "x", "2"}, {"abs", "x", "3"}, {"abs", "x", "4"}, {"abs", "x", "5"},
                   {"abs", "x", "6"}, {"abs", "x", "7"},
                   {"x", "y", "1"}, {"x", "y", "2"}, {"x", "y", "3"}, {"x", "y", "4"}, {"x", "y", "5"},
                   {"x", "y", "6"}, {"x", "y", "7"},
                   {"y", "y", "1"}, {"y", "y", "2"}, {"y", "y", "3"}, {"y", "y", "4"}, {"y", "y", "5"},
                   {"y", "y", "6"}, {"y", "y", "7"},
                   {"z", "y", "1"}, {"z", "y", "2"}, {"z", "y", "3"}, {"z", "y", "4"}, {"z", "y", "5"},
                   {"z", "y", "6"}, {"z", "y", "7"},
                   {"abs", "y", "1"}, {"abs", "y", "2"}, {"abs", "y", "3"}, {"abs", "y", "4"}, {"abs", "y", "5"},
                   {"abs", "y", "6"}, {"abs", "y", "7"},
                   {"x", "z", "1"}, {"x", "z", "2"}, {"x", "z", "3"}, {"x", "z", "4"}, {"x", "z", "5"},
                   {"x", "z", "6"}, {"x", "z", "7"},
                   {"y", "z", "1"}, {"y", "z", "2"}, {"y", "z", "3"}, {"y", "z", "4"}, {"y", "z", "5"},
                   {"y", "z", "6"}, {"y", "z", "7"},
                   {"z", "z", "1"}, {"z", "z", "2"}, {"z", "z", "3"}, {"z", "z", "4"}, {"z", "z", "5"},
                   {"z", "z", "6"}, {"z", "z", "7"},
                   {"abs", "z", "1"}, {"abs", "z", "2"}, {"abs", "z", "3"}, {"abs", "z", "4"}, {"abs", "z", "5"},
                   {"abs", "z", "6"}, {"abs", "z", "7"},};
        REQUIRE(tupleResult.extractQueryTuple({"dummyVariable", "synonym1", "otherSynonym"}, synonymTypes, qpsf) == resList);

        resList = { {"x", "x", "5"}, {"x", "y", "5"}, {"x", "z", "5"}, {"x", "abs", "5"},
                    {"y", "x", "10"}, {"y", "y", "10"}, {"y", "z", "10"}, {"y", "abs", "10"},
                    {"z", "x", "15"}, {"z", "y", "15"}, {"z", "z", "15"}, {"z", "abs", "15"} };
        REQUIRE(tupleResult.extractQueryTuple({"synonym1", "dummyVariable", "synonym2"}, synonymTypes, qpsf) == resList);
    }
}