#include <string>
#include <vector>

#include "catch.hpp"
#include "qps/QueryEvaluator/ClauseResult.h"
#include "qps/QueryEvaluator/ClauseResultBuilder.h"

TEST_CASE("Bool-bool pairs") {
	ClauseResult falseResult1 = ClauseResultBuilder::createBooleanResult(false);
	ClauseResult falseResult2 = ClauseResultBuilder::createBooleanResult(false);
	ClauseResult trueResult1 = ClauseResultBuilder::createBooleanResult(true);
	ClauseResult trueResult2 = ClauseResultBuilder::createBooleanResult(true);

	REQUIRE(ClauseResultEvaluator::evaluate(falseResult1, trueResult1) == falseResult1);
	REQUIRE(ClauseResultEvaluator::evaluate(falseResult1, trueResult1) == falseResult2);
	REQUIRE(ClauseResultEvaluator::evaluate(trueResult1, falseResult1) == falseResult2);
	REQUIRE(ClauseResultEvaluator::evaluate(trueResult2, falseResult1) == falseResult1);

	REQUIRE(ClauseResultEvaluator::evaluate(falseResult1, falseResult2) == falseResult1);
	REQUIRE(ClauseResultEvaluator::evaluate(trueResult1, trueResult2) == trueResult2);
}

TEST_CASE("Bool pairs") {
	ClauseResult falseResult = ClauseResultBuilder::createBooleanResult(false);
	ClauseResult trueResult = ClauseResultBuilder::createBooleanResult(true);

	SECTION("list") {
		std::vector<std::string> tupleList;
		tupleList.push_back("1");
		tupleList.push_back("3");
		tupleList.push_back("4");
		tupleList.push_back("7");
		ClauseResult listResult = ClauseResultBuilder::createListResult("x", tupleList);

		REQUIRE(ClauseResultEvaluator::evaluate(falseResult, listResult) == falseResult);
		REQUIRE(ClauseResultEvaluator::evaluate(listResult, falseResult) == falseResult);
		REQUIRE(ClauseResultEvaluator::evaluate(trueResult, listResult) == listResult);
		REQUIRE(ClauseResultEvaluator::evaluate(listResult, trueResult) == listResult);
	}

	SECTION("tuple") {
		std::vector < std::string > synonymList{ "x", "y" };
		std::vector<std::vector<std::string>> tupleList;
		tupleList.push_back({ "1", "2" });
		tupleList.push_back({ "3", "5" });
		tupleList.push_back({ "4", "7" });
		ClauseResult tupleResult = ClauseResultBuilder::createTupleResult(synonymList, tupleList);

		REQUIRE(ClauseResultEvaluator::evaluate(falseResult, tupleResult) == falseResult);
		REQUIRE(ClauseResultEvaluator::evaluate(tupleResult, falseResult) == falseResult);
		REQUIRE(ClauseResultEvaluator::evaluate(trueResult, tupleResult) == tupleResult);
		REQUIRE(ClauseResultEvaluator::evaluate(tupleResult, trueResult) == tupleResult);
	}
}

TEST_CASE("List pairs") {

	std::vector<std::string> list1;
	list1.push_back("1");
	list1.push_back("3");
	list1.push_back("4");
	list1.push_back("7");
	ClauseResult listResult1 = ClauseResultBuilder::createListResult("x", list1);
	ClauseResult differentListResult = ClauseResultBuilder::createListResult("y", list1);

	std::vector<std::string> list2;
	list2.push_back("1");
	list2.push_back("2");
	list2.push_back("3");
	list2.push_back("5");
	ClauseResult listResult2 = ClauseResultBuilder::createListResult("x", list2);

	SECTION("List to list gives correct table") {
		std::vector<std::string> ansList;
		ansList.push_back("1");
		ansList.push_back("3");
		ClauseResult ansResult = ClauseResultBuilder::createListResult("x", ansList);

		REQUIRE(ClauseResultEvaluator::evaluate(listResult1, listResult2) == ansResult);
		REQUIRE(ClauseResultEvaluator::evaluate(listResult1, listResult1) == listResult1);
	}

	SECTION("Other result has common synonyms but empty list, or no overlapping values") {
		std::vector<std::string> emptyList;
		ClauseResult emptyListResult = ClauseResultBuilder::createListResult("x", emptyList);
		ClauseResult falseResult = ClauseResultBuilder::createBooleanResult(false);
        std::vector<std::vector<std::string>> crossProductList;
        // cross between list1 and differentListResult
        crossProductList.push_back({ "1", "1" });
        crossProductList.push_back({ "1", "3" });
        crossProductList.push_back({ "1", "4" });
        crossProductList.push_back({ "1", "7" });
        crossProductList.push_back({ "3", "1" });
        crossProductList.push_back({ "3", "3" });
        crossProductList.push_back({ "3", "4" });
        crossProductList.push_back({ "3", "7" });
        crossProductList.push_back({ "4", "1" });
        crossProductList.push_back({ "4", "3" });
        crossProductList.push_back({ "4", "4" });
        crossProductList.push_back({ "4", "7" });
        crossProductList.push_back({ "7", "1" });
        crossProductList.push_back({ "7", "3" });
        crossProductList.push_back({ "7", "4" });
        crossProductList.push_back({ "7", "7" });
        std::vector<std::string> crossProductSynonyms{ "x", "y" };
        ClauseResult crossProductResult = ClauseResultBuilder::createTupleResult(crossProductSynonyms, crossProductList);

		REQUIRE(ClauseResultEvaluator::evaluate(listResult2, emptyListResult) == falseResult);
		REQUIRE(ClauseResultEvaluator::evaluate(listResult1, differentListResult) == crossProductResult);

		std::vector<std::string> list3;
		list3.push_back("100");
		list3.push_back("200");
		list3.push_back("300");
		list3.push_back("500");
		ClauseResult listResult3 = ClauseResultBuilder::createListResult("x", list3);
		REQUIRE(ClauseResultEvaluator::evaluate(listResult1, listResult3) == falseResult);
	}
}

TEST_CASE("Tuple pairs") {
	std::vector < std::string > synonymList{ "x", "y" };
	std::vector<std::vector<std::string>> tupleList;
	tupleList.push_back({ "1", "2" });
	tupleList.push_back({ "3", "5" });
	tupleList.push_back({ "4", "7" });
	ClauseResult tupleResult1 = ClauseResultBuilder::createTupleResult(synonymList, tupleList);

	SECTION("Tuple-tuple pair gives TableResult with 3 synonyms") {
		std::vector < std::string > synonymList2{ "x", "zee" };
		std::vector<std::vector<std::string>> tupleList2;
		tupleList2.push_back({ "1", "10" });
		tupleList2.push_back({ "3", "string" });
		tupleList2.push_back({ "5", "no join on this row" });
		ClauseResult tupleResult2 = ClauseResultBuilder::createTupleResult(synonymList2, tupleList2);

		std::vector<std::string> synonymListAns({ "y", "zee", "x" });
		std::vector<std::vector<std::string>> ansList;
		ansList.push_back({ "2", "10", "1" });
		ansList.push_back({ "5", "string", "3" });
		ClauseResult ansResult = ClauseResultBuilder::createTableResult(synonymListAns, ansList, 3);

		REQUIRE(ClauseResultEvaluator::evaluate(tupleResult1, tupleResult2) == ansResult);
	}

	SECTION("Tuple-list pair gives TableResult with 2 synonyms") {
		std::vector<std::string> list1;
		list1.push_back("1");
		list1.push_back("5");
		list1.push_back("4");
		list1.push_back("7");
		ClauseResult listResult1 = ClauseResultBuilder::createListResult("x", list1);

		std::vector<std::string> synonymListAns({ "x", "y" });
		std::vector<std::vector<std::string>> ansList;
		ansList.push_back({ "1", "2" });
		ansList.push_back({ "4", "7" });
		ClauseResult ansResult = ClauseResultBuilder::createTableResult(synonymListAns, ansList, 2);

		REQUIRE(ClauseResultEvaluator::evaluate(tupleResult1, listResult1) == ansResult);
	}

	SECTION("Tuple-tuple pair with no common synonym gives BooleanFalse") {
		std::vector < std::string > synonymList2{ "z", "w" };
		std::vector<std::vector<std::string>> tupleList2;
		tupleList2.push_back({ "1", "10" });
		tupleList2.push_back({ "variable", "string" });
		tupleList2.push_back({ "5", "example" });
		ClauseResult tupleResult2 = ClauseResultBuilder::createTupleResult(synonymList2, tupleList2);

        // Cross-product between tupleResult1 and tupleResult2
		std::vector<std::vector<std::string>> crossProductList;
        crossProductList.push_back({ "1", "2", "1", "10" });
        crossProductList.push_back({ "1", "2", "variable", "string" });
        crossProductList.push_back({ "1", "2", "5", "example" });
        crossProductList.push_back({ "3", "5", "1", "10" });
        crossProductList.push_back({ "3", "5", "variable", "string" });
        crossProductList.push_back({ "3", "5", "5", "example" });
        crossProductList.push_back({ "4", "7", "1", "10" });
        crossProductList.push_back({ "4", "7", "variable", "string" });
        crossProductList.push_back({ "4", "7", "5", "example" });
        std::vector<std::string> crossProductSynonyms{ "x", "y", "z", "w" };
        ClauseResult crossProductResult = ClauseResultBuilder::createTableResult(crossProductSynonyms, crossProductList, 4);

		REQUIRE(ClauseResultEvaluator::evaluate(tupleResult1, tupleResult2) == crossProductResult);
	}
}

TEST_CASE("Table-table pairs") {
	std::vector < std::string > synonymList{ "x", "y", "z" };
	std::vector<std::vector<std::string>> tupleList;
	tupleList.push_back({ "1", "2", "3"});
	tupleList.push_back({ "4", "5", "6"});
	tupleList.push_back({ "7", "8", "9"});
	ClauseResult tupleResult1 = ClauseResultBuilder::createTableResult(synonymList, tupleList, 3);

	SECTION("Table-table pair gives TableResult with 4 synonyms") {
		std::vector < std::string > synonymList2{ "x", "z", "w"};
		std::vector<std::vector<std::string>> tupleList2;
		tupleList2.push_back({ "1", "3", "5"});
		tupleList2.push_back({ "7", "9", "11"});
		tupleList2.push_back({ "13", "15", "17"});
		ClauseResult tupleResult2 = ClauseResultBuilder::createTableResult(synonymList2, tupleList2, 3);

		std::vector<std::string> synonymListAns({ "x", "y", "z", "w"});
		std::vector<std::vector<std::string>> ansList;
		ansList.push_back({ "1", "2", "3", "5"});
		ansList.push_back({ "7", "8", "9", "11"});
		ClauseResult ansResult = ClauseResultBuilder::createTableResult(synonymListAns, ansList, 4);

		auto ans = ClauseResultEvaluator::evaluate(tupleResult1, tupleResult2);
		REQUIRE(ans == ansResult);
	}

	//SECTION("Table-table pair gives TableResult with 5 synonyms") {
	//}

	//SECTION("Table-table pair gives TableResult with 3 synonyms") {
	//}
}