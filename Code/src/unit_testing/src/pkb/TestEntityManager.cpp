#include "catch.hpp"
#include "pkb/Manager/EntityManager.h"

using namespace std;

TEST_CASE("Entity Manager successfully initialises empty statement tables") {

    EntityManager em;
    // Statement table is empty, querying should throw Out-of-range exception
    REQUIRE_THROWS_AS(em.getStatementType(0), std::out_of_range);
    REQUIRE_THROWS_AS(em.getStatementType(1), std::out_of_range);

    SECTION("Statement tables successfully initialises and are empty") {
        CHECK_FALSE(em.isStatementOfType(1, StatementType::StatementRead));
        CHECK_FALSE(em.isStatementOfType(1, StatementType::StatementPrint));
        CHECK_FALSE(em.isStatementOfType(1, StatementType::StatementAssign));
        CHECK_FALSE(em.isStatementOfType(1, StatementType::StatementCall));
        CHECK_FALSE(em.isStatementOfType(1, StatementType::StatementWhile));
        REQUIRE_FALSE(em.isStatementOfType(1, StatementType::StatementIf));
    }
}

TEST_CASE("a statement can be added and queried correctly") {
    EntityManager em;
    em.addStatement(1, StatementType::StatementIf);
    REQUIRE(em.getStatementType(1) == StatementType::StatementIf);

    /*SECTION("adding a statement of the same statement number should throw an error ") {
        CHECK_THROWS_AS(em.addStatement(1, StatementType::StatementAssign), std::invalid_argument);
        REQUIRE_THROWS_AS(em.addStatement(1, StatementType::StatementIf), std::invalid_argument);
    }*/

    SECTION("an added statement is also added to the correct statementTypeTable only") {
        CHECK(em.isStatementOfType(1, StatementType::StatementIf));
        CHECK_FALSE(em.isStatementOfType(1, StatementType::StatementWhile));
        CHECK_FALSE(em.isStatementOfType(1, StatementType::StatementCall));
        CHECK_FALSE(em.isStatementOfType(1, StatementType::StatementAssign));
        CHECK_FALSE(em.isStatementOfType(1, StatementType::StatementRead));
        CHECK_FALSE(em.isStatementOfType(1, StatementType::StatementPrint));
        REQUIRE_FALSE(em.isStatementOfType(2, StatementType::StatementIf));
    }

    //adding a statement does not add a statement of a different number
    REQUIRE_THROWS_AS(em.getStatementType(2), std::out_of_range);

    SECTION("a second statement can be successfully added and queried") {
        em.addStatement(2, StatementType::StatementAssign);
        REQUIRE(em.getStatementType(2) == StatementType::StatementAssign);
    }
    
}   

TEST_CASE("adding an invalid statement type should throw an error") {
    EntityManager em;
    StatementType invalid_st = static_cast<StatementType>(100);
    REQUIRE_THROWS_AS(em.addStatement(100, invalid_st), std::invalid_argument);
}

// Querying a type for a variable that isnt there should return false or throw error?

//// Adding variables
// Adding 2 variables of the same name, regardless of value, should throw an error

//// Adding constants
// Adding 1 constant
// Adding 2 constants should not conflict

TEST_CASE("Getting all statements returns correct vector array") {
    EntityManager em;

    REQUIRE(em.getStatements().size() == 0);

    em.addStatement(1, StatementType::StatementAssign);
    vector<int> statementList = em.getStatements();
    REQUIRE(statementList.size() == 1);
    REQUIRE(statementList[0] == 1);

    em.addStatement(20, StatementType::StatementCall);
    statementList = em.getStatements();
    REQUIRE(statementList.size() == 2);
    REQUIRE(statementList[0] == 1);
    REQUIRE(statementList[1] == 20);
}

//// Getting all statements of a statement type
// Getting when table is empty should give an empty vector 
// Getting when table is empty when another isn't should give an empty vector
// Getting when table has 1 value should return a vector of length 1
// Getting when table has multiple values should return corresponding vector

//// Getting all variables
// Getting when table is empty should give an empty vector
// Getting when table has 1 value should return a vector of length 1
// Getting when table has multiple values should return corresponding vector
TEST_CASE("Getting all variables") {
    EntityManager em;

    REQUIRE(em.getVariables().size() == 0);

    em.addVariable("x");
    em.addVariable("y");
    std::vector<std::string>exp{ "x", "y" };
    REQUIRE(em.getVariables() == exp);
}

//// Getting all constants
// Getting when table is empty should give an empty vector
// Getting when table has 1 value should return a vector of length 1
// Getting when table has multiple values should return corresponding vector

