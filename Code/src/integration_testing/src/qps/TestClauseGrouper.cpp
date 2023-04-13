#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "catch.hpp"
#include "qps/Clause.h"
#include "qps/ClauseGroup/ClauseGrouper.h"
#include "qps/PriorityEvaluator/PriorityEvaluator.h"
#include "pkb/PKB.h"

using ClausePtr = std::shared_ptr<Clause>;

std::unordered_map<int, std::vector<ClausePtr>> populatePKB(std::shared_ptr<SPFacade>& spFacade) {
    spFacade->addStatement(1, StatementType::StatementAssign);      // y = x + 1
    spFacade->addVariable("x");
    spFacade->addVariable("y");
    spFacade->addStmtModifies(1, "y");
    spFacade->addStmtUses(1, "x");
    spFacade->addConstant("1");
    spFacade->addProcChild("procedure", 1);

    spFacade->addStatement(2, StatementType::StatementRead);        // read z
    spFacade->addFollows(1, 2);
    spFacade->addVariable("z");
    spFacade->addStmtModifies(2, "z");
    spFacade->addProcChild("procedure", 2);

    spFacade->addStatement(3, StatementType::StatementCall);      // call procedure1
    spFacade->addFollows(2, 3);
    spFacade->addProcedure("procedure");
    spFacade->addProcedure("procedure1");
    spFacade->addCalls("procedure", "procedure1");
    spFacade->addCallStmt(3, "procedure1");
    spFacade->addProcChild("procedure", 3);

    spFacade->addStatement(4, StatementType::StatementAssign);    // z = x + y + z
    spFacade->addProcChild("procedure1", 4);
    spFacade->addStmtUses(4, "x");
    spFacade->addStmtUses(4, "y");
    spFacade->addStmtUses(4, "z");
    spFacade->addStmtModifies(4, "z");

    std::unordered_map<int, std::vector<ClausePtr>> clauses;
    clauses[0] = std::vector<ClausePtr>();
    clauses[1] = std::vector<ClausePtr>();
    clauses[2] = std::vector<ClausePtr>();

    // Populate 0-synonym Clauses
    std::shared_ptr<Clause> clausePtr = std::make_shared<UsesClause>(UsesClause("1", "\"x\""));  // 0-True
    clauses[0].push_back(clausePtr);
    clausePtr = std::make_shared<ModifiesClause>(ModifiesClause("2", "\"z\""));  // 1-True
    clauses[0].push_back(clausePtr);
    clausePtr = std::make_shared<ModifiesClause>(ModifiesClause("4", "\"z\""));  // 2-True
    clauses[0].push_back(clausePtr);
    clausePtr = std::make_shared<UsesClause>(UsesClause("4", "\"x\""));  // 3-True
    clauses[0].push_back(clausePtr);
    clausePtr = std::make_shared<CallsClause>(CallsClause("\"procedure\"", "\"procedure1\""));  // 4-True
    clauses[0].push_back(clausePtr);
    clausePtr = std::make_shared<UsesClause>(UsesClause("3", "\"x\""));  // 5-False
    clauses[0].push_back(clausePtr);
    clausePtr = std::make_shared<UsesClause>(UsesClause("4", "\"w\""));  // 6-False
    clauses[0].push_back(clausePtr);
    clausePtr = std::make_shared<CallsClause>(CallsClause("\"procedure1\"", "\"procedure\""));  // 7-False
    clauses[0].push_back(clausePtr);

    // Populate 1-synonym Clauses
    // stmt s
    clausePtr = std::make_shared<UsesClause>(UsesClause("s", "\"x\""));  // 0, result = { 1, 4 }
    clauses[1].push_back(clausePtr);
    clausePtr = std::make_shared<ModifiesClause>(ModifiesClause("s", "\"z\"")); // 1, result = { 2, 4 }
    clauses[1].push_back(clausePtr);
    clausePtr = std::make_shared<UsesClause>(UsesClause("s", "\"w\""));  // 2-False
    clauses[1].push_back(clausePtr);
    // stmt s2
    clausePtr = std::make_shared<ModifiesClause>(ModifiesClause("s2", "\"y\""));  // 3, result = { 1 }
    clauses[1].push_back(clausePtr);
    clausePtr = std::make_shared<ModifiesClause>(ModifiesClause("s2", "\"x\""));  // 4-False
    clauses[1].push_back(clausePtr);
    // variable v
    clausePtr = std::make_shared<UsesClause>(UsesClause("1", "v"));  // 5
    clauses[1].push_back(clausePtr);
    clausePtr = std::make_shared<UsesClause>(UsesClause("2", "v"));  // 6-False
    clauses[1].push_back(clausePtr);
    // variable v2
    clausePtr = std::make_shared<UsesClause>(UsesClause("\"procedure1\"", "v2"));  // 7
    clauses[1].push_back(clausePtr);
    clausePtr = std::make_shared<UsesClause>(UsesClause("3", "v2"));  // 8-False
    clauses[1].push_back(clausePtr);
    // procedure p
    clausePtr = std::make_shared<CallsClause>(CallsClause("p", "\"procedure1\""));  // 9
    clauses[1].push_back(clausePtr);
    clausePtr = std::make_shared<UsesClause>(UsesClause("p", "\"x\""));  // 10
    clauses[1].push_back(clausePtr);
    clausePtr = std::make_shared<CallsClause>(CallsClause("p", "\"procedure\""));  // 11-False
    clauses[1].push_back(clausePtr);
    // procedure p2
    clausePtr = std::make_shared<CallsClause>(CallsClause("\"procedure\"", "p2"));  // 12
    clauses[1].push_back(clausePtr);
    clausePtr = std::make_shared<CallsClause>(CallsClause("\"procedure1\"", "p2"));  // 13-False
    clauses[1].push_back(clausePtr);

    // Populate 2-synonym Clauses
    // stmt s, stmt s2
    clausePtr = std::make_shared<FollowsClause>(FollowsClause("s", "s2"));  // 0, result = {<1, 2>, <2, 3>}
    clauses[2].push_back(clausePtr);
    clausePtr = std::make_shared<FollowsTClause>(FollowsTClause("s2", "s"));  // 1
    clauses[2].push_back(clausePtr);
    clausePtr = std::make_shared<ParentClause>(ParentClause("s2", "s"));  // 2-False
    clauses[2].push_back(clausePtr);
    // stmt s, variable v
    clausePtr = std::make_shared<UsesClause>(UsesClause("s", "v"));  // 3
    clauses[2].push_back(clausePtr);
    // stmt s, variable v2
    clausePtr = std::make_shared<ModifiesClause>(ModifiesClause("s", "v2"));  // 4
    clauses[2].push_back(clausePtr);
    // procedure p, variable v2
    clausePtr = std::make_shared<UsesClause>(UsesClause("p", "v2"));  // 5
    clauses[2].push_back(clausePtr);
    clausePtr = std::make_shared<ModifiesClause>(ModifiesClause("p", "v2"));  // 6
    clauses[2].push_back(clausePtr);
    // procedure p, variable v
    clausePtr = std::make_shared<UsesClause>(UsesClause("p", "v"));  // 7
    clauses[2].push_back(clausePtr);
    clausePtr = std::make_shared<ModifiesClause>(ModifiesClause("p", "v"));  // 8
    clauses[2].push_back(clausePtr);
    // procedure p, procedure p2
    clausePtr = std::make_shared<CallsClause>(CallsClause("p", "p2"));  // 9
    clauses[2].push_back(clausePtr);

    for (auto& clauseGroup : clauses) {
        for (auto& clause : clauseGroup.second) {
            PriorityEvaluator pe = PriorityEvaluator(clause);
            clause->setPriorityScore(pe.evaluatePriority());
        }
    }

    return clauses;

}

TEST_CASE("Initialise ClauseGrouper") {
    ClauseGrouper cg;
    REQUIRE(cg.getClauseGroups().empty());
    REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
}

TEST_CASE("ClauseGroup addClause, ClauseGroup reduce") {
    ClauseGrouper cg;
    PKB pkb;
    auto spFacade = pkb.getSPFacade();
    auto qpsFacade = pkb.getQPSFacade();
    auto clauses = populatePKB(spFacade);
    std::unordered_map<std::string, std::string> synonymTypes;
    synonymTypes["s"] = "stmt";
    synonymTypes["s2"] = "stmt";
    synonymTypes["v"] = "variable";
    synonymTypes["v2"] = "variable";
    synonymTypes["p"] = "procedure";
    synonymTypes["p2"] = "procedure";
    std::string target = "s";

    SECTION("0-synonym Clauses should be added and reduced to a boolean CR") {
        SECTION("1 synonym reduced to true") {
            cg.addClause(clauses[0][0]);
            REQUIRE(cg.getClauseGroups().empty());
            REQUIRE(cg.getNonSynonymGroup().getNumClauses() == 1);
            auto reducedGroup = cg.getNonSynonymGroup().reduce(qpsFacade, synonymTypes);
            REQUIRE(reducedGroup.isBoolean());
            REQUIRE(reducedGroup.extractQueryBoolean());
        }

        SECTION("1 synonym reduced to false") {
            cg.addClause(clauses[0][5]);
            REQUIRE(cg.getClauseGroups().empty());
            REQUIRE(cg.getNonSynonymGroup().getNumClauses() == 1);
            auto reducedGroup = cg.getNonSynonymGroup().reduce(qpsFacade, synonymTypes);
            REQUIRE(reducedGroup.isBoolean());
            REQUIRE_FALSE(reducedGroup.extractQueryBoolean());
        }

        SECTION("2 synonyms reduced to true") {
            cg.addClause(clauses[0][0]);
            cg.addClause(clauses[0][1]);
            REQUIRE(cg.getClauseGroups().empty());
            REQUIRE(cg.getNonSynonymGroup().getNumClauses() == 2);
            auto reducedGroup = cg.getNonSynonymGroup().reduce(qpsFacade, synonymTypes);
            REQUIRE(reducedGroup.isBoolean());
            REQUIRE(reducedGroup.extractQueryBoolean());
        }

        SECTION("2 synonyms reduced to false") {
            cg.addClause(clauses[0][0]);
            cg.addClause(clauses[0][5]);
            REQUIRE(cg.getClauseGroups().empty());
            REQUIRE(cg.getNonSynonymGroup().getNumClauses() == 2);
            auto reducedGroup = cg.getNonSynonymGroup().reduce(qpsFacade, synonymTypes);
            REQUIRE(reducedGroup.isBoolean());
            REQUIRE_FALSE(reducedGroup.extractQueryBoolean());
        }
    }

    SECTION("1-synonym Clauses should be added and reduced to a list CR") {
        SECTION("1 reduced to list") {
            cg.addClause(clauses[1][0]);
            REQUIRE(cg.getClauseGroups().size() == 1);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 1);
            auto group = cg.getClauseGroups()[0];
            auto reducedGroup = group.reduce(qpsFacade, synonymTypes);
            REQUIRE_FALSE(reducedGroup.isBoolean());
            auto result = reducedGroup.extractQueryData(qpsFacade, target, synonymTypes[target]);
            // { 1, 4 }
            REQUIRE(result.size() == 2);
            REQUIRE(result[0] == "1");
            REQUIRE(result[1] == "4");
        }

        SECTION("1 reduced to false boolean (empty list)") {
            cg.addClause(clauses[1][6]);
            REQUIRE(cg.getClauseGroups().size() == 1);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 1);
            auto group = cg.getClauseGroups()[0];
            auto reducedGroup = group.reduce(qpsFacade, synonymTypes);
            REQUIRE(reducedGroup.isBoolean());
            REQUIRE_FALSE(reducedGroup.extractQueryBoolean());
        }

        SECTION("2 with different synonyms") {
            cg.addClause(clauses[1][0]);
            cg.addClause(clauses[1][6]);
            REQUIRE(cg.getClauseGroups().size() == 2);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());

            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 1);
            auto firstGroupSynonym = cg.getClauseGroups()[0].getCommonSynonyms();
            REQUIRE(firstGroupSynonym.find("s") != firstGroupSynonym.end());

            REQUIRE(cg.getClauseGroups()[1].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[1].getCommonSynonyms().size() == 1);
            auto secondGroupSynonym = cg.getClauseGroups()[1].getCommonSynonyms();
            REQUIRE(secondGroupSynonym.find("v") != secondGroupSynonym.end());
        }

        SECTION("2 with same synonym") {
            SECTION("reduced to list") {
                cg.addClause(clauses[1][0]);
                cg.addClause(clauses[1][1]);
                REQUIRE(cg.getClauseGroups().size() == 1);
                REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
                REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 2);
                REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 1);
                auto group = cg.getClauseGroups()[0];
                auto reducedGroup = group.reduce(qpsFacade, synonymTypes);
                REQUIRE_FALSE(reducedGroup.isBoolean());
                auto result = reducedGroup.extractQueryData(qpsFacade,
                                                            target,
                                                            synonymTypes[target]);
                // { 1, 4 } and { 2, 4 } = { 4 }
                REQUIRE(result.size() == 1);
                REQUIRE(result[0] == "4");
            }

            SECTION("reduced to false boolean") {
                cg.addClause(clauses[1][5]);
                cg.addClause(clauses[1][6]);
                REQUIRE(cg.getClauseGroups().size() == 1);
                REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
                REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 2);
                REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 1);
                auto group = cg.getClauseGroups()[0];
                auto reducedGroup = group.reduce(qpsFacade, synonymTypes);
                REQUIRE(reducedGroup.isBoolean());
                REQUIRE_FALSE(reducedGroup.extractQueryBoolean());
            }
        }

        SECTION("Multiple in multiple groups, fixed order") {
            // stmt s - reduces to list
            cg.addClause(clauses[1][0]);
            cg.addClause(clauses[1][1]);
            // stmt s2 - reduces to false
            cg.addClause(clauses[1][3]);
            cg.addClause(clauses[1][4]);
            // variable v - reduces to list
            cg.addClause(clauses[1][5]);
            // procedure p2 - reduces to false
            cg.addClause(clauses[1][13]);

            REQUIRE(cg.getClauseGroups().size() == 4);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 2);

            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 2);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 1);
            auto firstGroupSynonym = cg.getClauseGroups()[0].getCommonSynonyms();
            REQUIRE(firstGroupSynonym.find("s") != firstGroupSynonym.end());

            REQUIRE(cg.getClauseGroups()[1].getNumClauses() == 2);
            REQUIRE(cg.getClauseGroups()[1].getCommonSynonyms().size() == 1);
            auto secondGroupSynonym = cg.getClauseGroups()[1].getCommonSynonyms();
            REQUIRE(secondGroupSynonym.find("s2") != secondGroupSynonym.end());

            REQUIRE(cg.getClauseGroups()[2].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[2].getCommonSynonyms().size() == 1);
            auto thirdGroupSynonym = cg.getClauseGroups()[2].getCommonSynonyms();
            REQUIRE(thirdGroupSynonym.find("v") != thirdGroupSynonym.end());

            REQUIRE(cg.getClauseGroups()[3].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[3].getCommonSynonyms().size() == 1);
            auto fourthGroupSynonym = cg.getClauseGroups()[3].getCommonSynonyms();
            REQUIRE(fourthGroupSynonym.find("p2") != fourthGroupSynonym.end());
        }

        SECTION("Multiple in multiple groups, arbitrary order") {
            // stmt s - reduces to list: 0, 1
            // stmt s2 - reduces to false: 3, 4
            // variable v - reduces to list: 5
            // procedure p2 - reduces to false: 13
            cg.addClause(clauses[1][0]);
            cg.addClause(clauses[1][13]);
            cg.addClause(clauses[1][3]);
            cg.addClause(clauses[1][1]);
            cg.addClause(clauses[1][5]);
            cg.addClause(clauses[1][4]);

            REQUIRE(cg.getClauseGroups().size() == 4);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 2);

            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 2);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 1);
            auto firstGroupSynonym = cg.getClauseGroups()[0].getCommonSynonyms();
            REQUIRE(firstGroupSynonym.find("s") != firstGroupSynonym.end());

            REQUIRE(cg.getClauseGroups()[1].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[1].getCommonSynonyms().size() == 1);
            auto secondGroupSynonym = cg.getClauseGroups()[1].getCommonSynonyms();
            REQUIRE(secondGroupSynonym.find("p2") != secondGroupSynonym.end());

            REQUIRE(cg.getClauseGroups()[2].getNumClauses() == 2);
            REQUIRE(cg.getClauseGroups()[2].getCommonSynonyms().size() == 1);
            auto thirdGroupSynonym = cg.getClauseGroups()[2].getCommonSynonyms();
            REQUIRE(thirdGroupSynonym.find("s2") != thirdGroupSynonym.end());

            REQUIRE(cg.getClauseGroups()[3].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[3].getCommonSynonyms().size() == 1);
            auto fourthGroupSynonym = cg.getClauseGroups()[3].getCommonSynonyms();
            REQUIRE(fourthGroupSynonym.find("v") != fourthGroupSynonym.end());
        }
    }

    SECTION("2-synonym and 1-synonym Clauses") {
        SECTION("2-synonym added to 1 CG, then 1-synonym") {
            cg.addClause(clauses[2][0]);  // s, s2
            REQUIRE(cg.getClauseGroups().size() == 1);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 2);

            cg.addClause(clauses[1][3]);  // s2
            REQUIRE(cg.getClauseGroups().size() == 1);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 2);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 2);

            // Reduce result
            auto result = cg.getClauseGroups()[0].reduce(qpsFacade, synonymTypes);
            REQUIRE(result.isBoolean());
            REQUIRE_FALSE(result.extractQueryBoolean());
        }

        SECTION("1-synonym added to CG, then 2-synonym") {
            cg.addClause(clauses[1][1]);  // s
            REQUIRE(cg.getClauseGroups().size() == 1);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 1);

            cg.addClause(clauses[2][0]);  // s, s2
            REQUIRE(cg.getClauseGroups().size() == 1);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 2);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 2);

            // Reduce, returns a <2, 3> tuple
            auto result = cg.getClauseGroups()[0].reduce(qpsFacade, synonymTypes);
            REQUIRE_FALSE(result.isBoolean());
            REQUIRE(result.extractQueryBoolean());
            REQUIRE(result.getSynonymList().size() == 2);
            REQUIRE(result.getSynonymList()[1] == "s");
            REQUIRE(result.getSynonymList()[0] == "s2");
            REQUIRE(result.getTable().size() == 1);
            std::vector<std::string> sResult = { "2" };
            std::vector<std::string> s2Result = { "3" };
            REQUIRE(result.extractQueryData(qpsFacade, "s", synonymTypes["s"]) == sResult);
            REQUIRE(result.extractQueryData(qpsFacade, "s2", synonymTypes["s2"]) == s2Result);
        }

        SECTION("2 2-synonym added to same CG with 1 common synonym") {
            cg.addClause(clauses[2][0]);  // s, s2
            REQUIRE(cg.getClauseGroups().size() == 1);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 2);

            cg.addClause(clauses[2][3]);  // s, v
            REQUIRE(cg.getClauseGroups().size() == 1);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 2);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 3);

            // Reduce
        }

        SECTION("2-synonyms added to 2 separate CGs") {
            cg.addClause(clauses[2][3]);  // s, v
            REQUIRE(cg.getClauseGroups().size() == 1);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 2);

            cg.addClause(clauses[2][5]);  // p, v2
            REQUIRE(cg.getClauseGroups().size() == 2);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 2);
            REQUIRE(cg.getClauseGroups()[1].getNumClauses() == 1);
            REQUIRE(cg.getClauseGroups()[1].getCommonSynonyms().size() == 2);

            auto commonSynonyms = cg.getClauseGroups()[0].getCommonSynonyms();
            REQUIRE(commonSynonyms.find("s") != commonSynonyms.end());
            REQUIRE(commonSynonyms.find("v") != commonSynonyms.end());

            commonSynonyms = cg.getClauseGroups()[1].getCommonSynonyms();
            REQUIRE(commonSynonyms.find("p") != commonSynonyms.end());
            REQUIRE(commonSynonyms.find("v2") != commonSynonyms.end());

            SECTION("Test combine: Add another 2-synonym to combine both CGs") {
                cg.addClause(clauses[2][4]);  // s, v2
                REQUIRE(cg.getClauseGroups().size() == 2);  // Should still have 2 CGs, but 1 is tombstone
                // In this case, index 0 is the tombstone, index 1 is the combined CG
                REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
                REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 0);
                REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 0);
                REQUIRE(cg.getClauseGroups()[1].getNumClauses() == 3);
                REQUIRE(cg.getClauseGroups()[1].getCommonSynonyms().size() == 4);
            }
        }

        SECTION("3 2-synonyms added to same CG") {
            cg.addClause(clauses[2][0]);  // s, s2
            cg.addClause(clauses[2][4]);  // s, v2
            cg.addClause(clauses[2][5]);  // p, v2

            // Should have 1 CG with 3 clauses and 4 common synonyms.
            // Also, worth noting that there are no tombstone CGs.
            REQUIRE(cg.getClauseGroups().size() == 1);
            REQUIRE_FALSE(cg.getNonSynonymGroup().getNumClauses());
            REQUIRE(cg.getClauseGroups()[0].getNumClauses() == 3);
            REQUIRE(cg.getClauseGroups()[0].getCommonSynonyms().size() == 4);
        }
    }
}
