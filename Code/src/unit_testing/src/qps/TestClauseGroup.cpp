#include "catch.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include "qps/ClauseGroup/ClauseGroup.h"
#include "qps/Clause.h"
#include "pkb/PKB.h"

std::shared_ptr<Clause> createZeroSynonymClause() {
    std::string noSynLHS = "1";
    std::string noSynRHS = "\"x\"";
    UsesClause clause = UsesClause(noSynLHS, noSynRHS);
    std::shared_ptr<Clause> clausePtr = std::make_shared<UsesClause>(clause);
    return clausePtr;
}

std::shared_ptr<Clause> createOneSynonymClause() {
    std::string noSynLHS = "s";
    std::string noSynRHS = "\"x\"";
    // 1-synonym clause, LHS
    UsesClause clause = UsesClause(noSynLHS, noSynRHS);
    std::shared_ptr<Clause> clausePtr = std::make_shared<UsesClause>(clause);
    return clausePtr;
}

ClauseResult populatePKB(std::shared_ptr<SPFacade>& spFacade) {
    spFacade->addFollows(1, 2);
    spFacade->addFollows(2, 3);
    std::string synLHS = "s";
    std::string synRHS = "s2";
    std::vector<std::string> synList;
    synList.push_back(synLHS);
    synList.push_back(synRHS);
    std::vector<std::string> list1;
    list1.emplace_back("1");
    list1.emplace_back("2");
    std::vector<std::string> list2;
    list2.emplace_back("2");
    list2.emplace_back("3");
    std::vector<std::string> list3;
    list3.emplace_back("1");
    list3.emplace_back("3");
    std::vector<std::vector<std::string>> tupleList;
    tupleList.push_back(list1);
    tupleList.push_back(list2);
    tupleList.push_back(list3);
    ClauseResult clauseResult = ClauseResultBuilder::createTupleResult(synList, tupleList);
    return clauseResult;
}

TEST_CASE("SynonymClauseGroup") {
    PKB pkb;
    std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
    ClauseResult clauseResult = populatePKB(spFacade);
    QPSFacade qpsFacade = pkb.getQPSFacade();

    auto oneSynonymClause = createOneSynonymClause();
    SynonymClauseGroup clauseGroup;
    REQUIRE(clauseGroup.getNumClauses() == 0);

    clauseGroup.addClause(oneSynonymClause);
    REQUIRE(clauseGroup.getNumClauses() == 1);

    SECTION("Adding to SynonymClauseGroup should update the common synonyms") {
        auto commonSynonyms = clauseGroup.getCommonSynonyms();
        REQUIRE(commonSynonyms.size() == 1);
        REQUIRE(commonSynonyms.find("s") != commonSynonyms.end());

        clauseGroup.addClause(oneSynonymClause);
        REQUIRE(clauseGroup.getNumClauses() == 2);
        commonSynonyms = clauseGroup.getCommonSynonyms();
        REQUIRE(commonSynonyms.size() == 1);
        REQUIRE(commonSynonyms.find("s") != commonSynonyms.end());
    }

    SECTION("Reducing the one-Clause should create a False ClauseResult") {
        std::unordered_map<std::string, std::string> synonymTypes;
        synonymTypes["s"] = "stmt";
        auto reducedClauseResult = clauseGroup.reduce(qpsFacade, synonymTypes);
        REQUIRE(reducedClauseResult.isBoolean());
        REQUIRE_FALSE(reducedClauseResult.getSynonymList().size());
        REQUIRE_FALSE(reducedClauseResult.extractQueryBoolean());
    }

    spFacade->addStmtUses(1, "x");

    SECTION("Reducing the one-Clause with the correct information should create a list ClauseResult") {
        std::unordered_map<std::string, std::string> synonymTypes;
        synonymTypes["s"] = "stmt";
        auto reducedClauseResult = clauseGroup.reduce(qpsFacade, synonymTypes);
        REQUIRE_FALSE(reducedClauseResult.isBoolean());
        REQUIRE(reducedClauseResult.getSynonymList().size() == 1);
        REQUIRE(reducedClauseResult.getSynonymList()[0] == "s");
    }

    // Add 2-synonym clause
    FollowsTClause followsTClause = FollowsTClause("s", "s2");
    std::shared_ptr<Clause> followsTClausePtr = std::make_shared<FollowsTClause>(followsTClause);
    clauseGroup.addClause(followsTClausePtr);

    SECTION("Reduce") {
        std::unordered_map<std::string, std::string> synonymTypes;
        synonymTypes["s"] = "stmt";
        synonymTypes["s2"] = "stmt";
        auto reducedClauseResult = clauseGroup.reduce(qpsFacade, synonymTypes);
        REQUIRE_FALSE(reducedClauseResult.isBoolean());
        REQUIRE(reducedClauseResult.getSynonymList().size() == 2);
        CHECK(reducedClauseResult.getSynonymList()[0] == "s2");
        CHECK(reducedClauseResult.getSynonymList()[1] == "s");
        REQUIRE(reducedClauseResult.extractQueryBoolean());
        REQUIRE(reducedClauseResult.getTable()[0].size() == 2);

        std::vector<std::string> tuple1 = { "2", "1" };
        std::vector<std::string> tuple2 = { "3", "1" };
        REQUIRE(reducedClauseResult.getTable()[0] == tuple1);
        REQUIRE(reducedClauseResult.getTable()[1] == tuple2);
    }
}

TEST_CASE("NonSynonymClauseGroup") {
    auto zeroSynonymClause = createZeroSynonymClause();
    NonSynonymClauseGroup nonSynonymClauseGroup;
    REQUIRE(nonSynonymClauseGroup.getNumClauses() == 0);

    nonSynonymClauseGroup.addClause(zeroSynonymClause);
    REQUIRE(nonSynonymClauseGroup.getNumClauses() == 1);

    nonSynonymClauseGroup.addClause(zeroSynonymClause);
    REQUIRE(nonSynonymClauseGroup.getNumClauses() == 2);

    PKB pkb;
    std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
    ClauseResult clauseResult = populatePKB(spFacade);
    QPSFacade qpsFacade = pkb.getQPSFacade();
    std::unordered_map<std::string, std::string> synonymTypes;
    synonymTypes["s"] = "stmt";

    SECTION("Reduce gives false result") {
        auto reducedClauseResult = nonSynonymClauseGroup.reduce(qpsFacade, synonymTypes);
        REQUIRE(reducedClauseResult.isBoolean());
        REQUIRE_FALSE(reducedClauseResult.extractQueryBoolean());

    }

    SECTION("Reduce gives true result") {
        spFacade->addStmtUses(1, "x");
        auto reducedClauseResult = nonSynonymClauseGroup.reduce(qpsFacade, synonymTypes);
        REQUIRE(reducedClauseResult.isBoolean());
        REQUIRE(reducedClauseResult.extractQueryBoolean());
    }
}