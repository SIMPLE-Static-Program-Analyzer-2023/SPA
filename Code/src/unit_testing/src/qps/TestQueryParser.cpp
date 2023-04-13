//
// Created by Vanessa Khor on 9/2/23.
//
#include "qps/QueryProcessor.h"
#include "qps/QueryLexer.h"
#include "qps/QueryParser.h"
#include <string>
#include <sstream>
#include "catch.hpp"
#include <typeinfo>

using namespace std;

// valid queries
TEST_CASE("Parse simple select variable query") {
    std::string input = "variable v; Select v";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "v");
    }
    REQUIRE(testQuery.getClauseList().size() == 0);
    std::vector<std::string> synonyms;
    synonyms.push_back("v");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "variable");
    }
}

TEST_CASE("Parse simple select stmt query") {
    std::string input = "stmt ss; Select ss";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "ss");
    }

    REQUIRE(testQuery.getClauseList().size() == 0);
    std::vector<std::string> synonyms;
    synonyms.push_back("ss");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse simple uses query") {
    string input = "variable v; Select v such that Uses(1, v)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "v");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "1");
        REQUIRE(c->getRHS() == "v");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("v");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "variable");
    }
}

TEST_CASE("Parse simple Calls query") {
    string input = "procedure p; Select p such that Calls(\"Third\", p)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "\"Third\"");
        REQUIRE(c->getRHS() == "p");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("p");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "procedure");
    }
}

TEST_CASE("Parse simple uses query with IDENT and wildcard") {
    string input = "variable v; Select v such that Uses(2, \"x\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "v");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "2");
        REQUIRE(c->getRHS() == "\"x\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("v");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "variable");
    }
}

TEST_CASE("Parse simple uses query with string IDENT") {
    string input = "variable v; Select v such that Uses(\"proc\", v)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "v");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "\"proc\"");
        REQUIRE(c->getRHS() == "v");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("v");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "variable");
    }
}

TEST_CASE("Parse simple modifies query with ifs") {
    string input = "if ifs; Select ifs such that Modifies(ifs, _)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "ifs");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "ifs");
        REQUIRE(c->getRHS() == "_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("ifs");
    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "if");
    }
}

TEST_CASE("Parse simple modifies query with call declaration") {
    string input = "call c; variable v; Select c such that Modifies(c, v)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "c");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "c");
        REQUIRE(c->getRHS() == "v");
    }
    std::vector<std::string> synonyms1;
    synonyms1.push_back("c");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("v");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "call");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "variable");
        }
    }
}

TEST_CASE("Parse simple modifies query") {
    string input = "stmt s; variable v; Select s such that Modifies(s, v)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "s");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "s");
        REQUIRE(c->getRHS() == "v");
    }

    std::vector<std::string> synonyms1;
    synonyms1.push_back("s");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("v");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "stmt");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "variable");
        }
    }
}

TEST_CASE("Parse Follows* query") {
    string input = "stmt s1, s2; Select s1 such that Follows*(s1, s2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "s1");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "s1");
        REQUIRE(c->getRHS() == "s2");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("s1");
    synonyms.push_back("s2");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse Follows query") {
    string input = "while w1, w2; Select w1 such that Follows(w1, w2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "w1");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "w1");
        REQUIRE(c->getRHS() == "w2");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("w1");
    synonyms.push_back("w2");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "while");
    }
}

TEST_CASE("Parse Parent query") {
    string input = "stmt s1, s2; Select s1 such that Parent(s1, s2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "s1");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "s1");
        REQUIRE(c->getRHS() == "s2");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("s1");
    synonyms.push_back("s2");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse Parent* query with wildcard") {
    string input = "stmt s1; Select s1 such that Parent*(_, 1)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "s1");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "1");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("s1");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse simple pattern query") {
    string input = "assign a; Select a pattern a(_, _\"x\"_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "_\"x\"_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse pattern query with IDENT") {
    string input = "assign a; Select a pattern a(\"x\", _\"3\"_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "\"x\"");
        REQUIRE(c->getRHS() == "_\"3\"_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse double wildcard pattern query") {
    string input = "assign a; Select a pattern a(_, _)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse pattern with operators") {
    string input = "assign a; Select a pattern a(_, \"v+x*yy\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "\"v+x*yy\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse pattern with operators and wildcard") {
    string input = "assign a; Select a pattern a(_, _\"v+x*y\"_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "_\"v+x*y\"_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse pattern for exact matching") {
    string input = "assign a; Select a pattern a(_, \"v\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "\"v\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse pattern with multiple operators") {
    string input = "assign a; Select a pattern a(_, \"a + b * c - d / e % i\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "\"a+b*c-d/e%i\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse pattern with inner brackets") {
    string input = "assign a; Select a pattern a(_, _\"x + (a+b) + (c-d)\"_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "_\"x+(a+b)+(c-d)\"_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse single pattern with brackets") {
    string input = "assign a; Select a pattern a(_, _\"(var81)*(8)\"_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "_\"(var81)*(8)\"_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse pattern with multiple inner brackets") {
    string input = "assign a; Select a pattern a(_, \"((a + ((b) * (c))) - (((d) / (e)) % i))\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "_");
        REQUIRE(c->getRHS() == "\"((a+((b)*(c)))-(((d)/(e))%i))\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}


TEST_CASE("Parse pattern with while") {
    string input = "while w; Select w pattern w(\"v\", _)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "w");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "\"v\"");
        REQUIRE(c->getRHS() == "_");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("w");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "while");
    }
}

TEST_CASE("Parse pattern with if") {
    string input = "if ifs; Select ifs pattern ifs(\"x\", _, _)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "ifs");
    }

    for (auto c : testQuery.getClauseList()) {

        REQUIRE(c->getLHS() == "\"x\"");
        REQUIRE(c->getRHS() == "_");
        // check if ptr is pointing to a IfPatternClause object
        auto child_ptr = std::dynamic_pointer_cast<IfPatternClause>(c);
        if (child_ptr != nullptr) {
            REQUIRE(child_ptr->getThird() == "_");
        } else {
            REQUIRE(false);
        }
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("ifs");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "if");
    }
}

TEST_CASE("Parse PQL query with multiple clauses") {
    string input = "assign a; while w; variable v; Select a pattern a (v, _) such that Parent* (w, a)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    REQUIRE(testQuery.isSynonymPresent("a") == true);
    REQUIRE(testQuery.isSynonymPresent("w") == true);
    REQUIRE(testQuery.isSynonymPresent("x") == false);

    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    std::vector<std::shared_ptr<Clause>> clauses = testQuery.getClauseList();
    for(int i = 0; i < clauses.size(); i++) {
        if(i == 0) {
            REQUIRE(clauses[i]->getLHS() == "v");
            REQUIRE(clauses[i]->getRHS() == "_");
        } else {
            REQUIRE(clauses[i]->getLHS() == "w");
            REQUIRE(clauses[i]->getRHS() == "a");
        }
    }

    std::vector<std::string> synonyms1;
    synonyms1.push_back("a");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<std::string> synonyms3;
    synonyms3.push_back("v");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "assign");
        } else if (i == 1) {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms3);
            REQUIRE(dec[i].getType() == "variable");
        }
    }
}

TEST_CASE("Parse PQL query with multiple clauses 2") {

    string input = "assign a; while w; Select a such that Parent(w, a) pattern a (_,_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    std::vector<std::shared_ptr<Clause>> clauses = testQuery.getClauseList();
    for(int i = 0; i < clauses.size(); i++) {
        if(i == 0) {
            REQUIRE(clauses[i]->getLHS() == "w");
            REQUIRE(clauses[i]->getRHS() == "a");
        } else {
            REQUIRE(clauses[i]->getLHS() == "_");
            REQUIRE(clauses[i]->getRHS() == "_");
        }
    }

    std::vector<std::string> synonyms1;
    synonyms1.push_back("a");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "assign");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse PQL query with IDENT") {

    string input = "assign a; while w; Select a such that Parent(w, a) pattern a (_,_\"x\"_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    std::vector<std::shared_ptr<Clause>> clauses = testQuery.getClauseList();
    for(int i = 0; i < clauses.size(); i++) {
        if(i == 0) {
            REQUIRE(clauses[i]->getLHS() == "w");
            REQUIRE(clauses[i]->getRHS() == "a");
        } else {
            REQUIRE(clauses[i]->getLHS() == "_");
            REQUIRE(clauses[i]->getRHS() == "_\"x\"_");
        }
    }

    std::vector<std::string> synonyms1;
    synonyms1.push_back("a");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "assign");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse parent query") {

    string input = "procedure p; read rd; Select p such that Parent(rd, _)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p");
    }

    std::vector<std::shared_ptr<Clause>> clauses = testQuery.getClauseList();
    for(int i = 0; i < clauses.size(); i++) {
            REQUIRE(clauses[i]->getLHS() == "rd");
            REQUIRE(clauses[i]->getRHS() == "_");
    }

    std::vector<std::string> synonyms1;
    synonyms1.push_back("p");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("rd");

    std::vector<Declaration> dec = testQuery.getDeclarationList();

    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms1);
            REQUIRE(dec[i].getType() == "procedure");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "read");
        }
    }
}

TEST_CASE("Parse simple calls query") {
    string input = "procedure p; Select p such that Calls(p, \"Third\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "p");
        REQUIRE(c->getRHS() == "\"Third\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("p");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "procedure");
    }
}

//TEST_CASE("TEST ST PARSER") {
//    string input = "procedure p; assign a; Select p such that pattern a (_, _)";
//    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
//    QueryLexer l1 = QueryLexer(iss);
//    Query testQuery = Query();
//    QueryParser p1 = QueryParser(l1, testQuery);
//    p1.parse();
//}

TEST_CASE("Parse simple callsT query") {
    string input = "procedure p; Select p such that Calls*(\"proc\", p)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "\"proc\"");
        REQUIRE(c->getRHS() == "p");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("p");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "procedure");
    }
}

TEST_CASE("Parse simple next query") {
    string input = "stmt s; Select s such that Next(2, s)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "s");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "2");
        REQUIRE(c->getRHS() == "s");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("s");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse simple nextT query") {
    string input = "stmt s; Select s such that Next*(s, 8)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "s");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "s");
        REQUIRE(c->getRHS() == "8");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("s");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse simple affects query") {
    string input = "assign a1, a2; Select a2 such that Affects(a1, a2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a2");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "a1");
        REQUIRE(c->getRHS() == "a2");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a1");
    synonyms.push_back("a2");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

TEST_CASE("Parse simple affectsT query") {
    string input = "assign a1; Select a1 such that Affects*(a1, 5)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a1");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "a1");
        REQUIRE(c->getRHS() == "5");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a1");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "assign");
    }
}

// test cases for query with attributes, tuple and boolean selection
TEST_CASE("Parse query with select boolean") {
    string input = "procedure p; Select BOOLEAN such that Calls(p, \"Third\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "_BOOLEAN");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "p");
        REQUIRE(c->getRHS() == "\"Third\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("p");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "procedure");
    }
}

TEST_CASE("Parse query with select single attribute") {
    string input = "procedure p; Select p.procName such that Calls(p, \"Third\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p.procName");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "p");
        REQUIRE(c->getRHS() == "\"Third\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("p");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "procedure");
    }
}

TEST_CASE("Parse query with select single attribute in tuple") {
    string input = "procedure p; Select <p.procName> such that Calls(p, \"Third\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p.procName");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "p");
        REQUIRE(c->getRHS() == "\"Third\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("p");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "procedure");
    }
}

TEST_CASE("Parse query with select single syn in tuple") {
    string input = "procedure p; Select <p> such that Calls(p, \"Third\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "p");
        REQUIRE(c->getRHS() == "\"Third\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("p");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "procedure");
    }
}

TEST_CASE("Parse query with select tuple with 2 elements") {
    string input = "assign a; while w; Select <a, w> such that Calls (\"Second\", \"Third\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for (int i = 0; i < targetList.size(); i++) {
        if (i == 0) {
            REQUIRE(targetList[i] == "a");
        } else {
            REQUIRE(targetList[i] == "w");
        }
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "\"Second\"");
        REQUIRE(c->getRHS() == "\"Third\"");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("a");

    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dl = testQuery.getDeclarationList();
    for (int i = 0; i < dl.size(); i++) {
        if (i == 0) {
            REQUIRE(dl[i].getSynonymList() == synonyms);
            REQUIRE(dl[i].getType() == "assign");
        } else {
            REQUIRE(dl[i].getSynonymList() == synonyms2);
            REQUIRE(dl[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse query with select tuple with 3 elements") {
    string input = "while w1, w2, w3; Select <w1, w2, w3> such that Parent* (w1, w2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for (int i = 0; i < targetList.size(); i++) {
        if (i == 0) {
            REQUIRE(targetList[i] == "w1");
        } else if (i == 1) {
            REQUIRE(targetList[i] == "w2");
        } else {
            REQUIRE(targetList[i] == "w3");
        }
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "w1");
        REQUIRE(c->getRHS() == "w2");
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("w1");
    synonyms.push_back("w2");
    synonyms.push_back("w3");

    std::vector<Declaration> dl = testQuery.getDeclarationList();
    for (int i = 0; i < dl.size(); i++) {
        REQUIRE(dl[i].getSynonymList() == synonyms);
        REQUIRE(dl[i].getType() == "while");
    }
}

TEST_CASE("Parse query with select tuple with single syn element") {
    string input = "assign a1, a2; Select <a2> such that Affects (a1, a2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for (int i = 0; i < targetList.size(); i++) {
        REQUIRE(targetList[i] == "a2");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "a1");
        REQUIRE(c->getRHS() == "a2");
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("a1");
    synonyms.push_back("a2");

    std::vector<Declaration> dl = testQuery.getDeclarationList();
    for (int i = 0; i < dl.size(); i++) {
        REQUIRE(dl[i].getSynonymList() == synonyms);
        REQUIRE(dl[i].getType() == "assign");
    }
}

TEST_CASE("Parse query with select tuple with single attribute element") {
    string input = "assign a1, a2; Select <a1.stmt#> such that Affects (a1, a2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for (int i = 0; i < targetList.size(); i++) {
        REQUIRE(targetList[i] == "a1.stmt#");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "a1");
        REQUIRE(c->getRHS() == "a2");
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("a1");
    synonyms.push_back("a2");

    std::vector<Declaration> dl = testQuery.getDeclarationList();
    for (int i = 0; i < dl.size(); i++) {
        REQUIRE(dl[i].getSynonymList() == synonyms);
        REQUIRE(dl[i].getType() == "assign");
    }
}

TEST_CASE("Parse query with select tuple with attributes") {
    string input = "assign a1, a2; Select <a1.stmt#, a2> such that Affects (a1, a2)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for (int i = 0; i < targetList.size(); i++) {
        if (i == 0) {
            REQUIRE(targetList[i] == "a1.stmt#");
        } else {
            REQUIRE(targetList[i] == "a2");
        }
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "a1");
        REQUIRE(c->getRHS() == "a2");
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("a1");
    synonyms.push_back("a2");

    std::vector<Declaration> dl = testQuery.getDeclarationList();
    for (int i = 0; i < dl.size(); i++) {
        REQUIRE(dl[i].getSynonymList() == synonyms);
        REQUIRE(dl[i].getType() == "assign");
    }
}

// test cases for query with attributes and with clause
TEST_CASE("Parse simple with query with attribute") {
    string input = "procedure p, q; Select p.procName such that Calls (p, q)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p.procName");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "p");
        REQUIRE(c->getRHS() == "q");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("p");
    synonyms.push_back("q");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "procedure");
    }
}

TEST_CASE("Parse simple with query with stmt# attribute") {
    string input = "stmt p, q; Select p.stmt# such that Follows (p, q)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p.stmt#");
    }

    for(std::shared_ptr<Clause> c : testQuery.getClauseList()) {
        REQUIRE(c->getLHS() == "p");
        REQUIRE(c->getRHS() == "q");
    }
    std::vector<std::string> synonyms;
    synonyms.push_back("p");
    synonyms.push_back("q");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "stmt");
    }
}

TEST_CASE("Parse simple with query with attribute and with clause") {
    string input = "procedure p, q; Select p such that Calls (p, q) with q.procName = \"Third\" such that Modifies (p, \"i\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p");
    }

    std::vector<std::shared_ptr<Clause>> cl = testQuery.getClauseList();
    for(int i = 0; i < cl.size(); i++) {
        if(i == 0) {
            REQUIRE(cl[i]->getLHS() == "p");
            REQUIRE(cl[i]->getRHS() == "q");
        } else if (i == 1) {
            REQUIRE(cl[i]->getLHS() == "q");
            REQUIRE(cl[i]->getRHS() == "\"Third\"");
        } else {
            REQUIRE(cl[i]->getLHS() == "p");
            REQUIRE(cl[i]->getRHS() == "\"i\"");
        }
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("p");
    synonyms.push_back("q");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "procedure");
    }
}

TEST_CASE("Parse simple with query with attribute and multiple with clause") {
    string input = "procedure p, q; Select p such that Calls (p, q) with q.procName = \"Third\" and p.procName = q.procName";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "p");
    }

    std::vector<std::shared_ptr<Clause>> cl = testQuery.getClauseList();
    for(int i = 0; i < cl.size(); i++) {
        if(i == 0) {
            REQUIRE(cl[i]->getLHS() == "p");
            REQUIRE(cl[i]->getRHS() == "q");
        } else if (i == 1) {
            REQUIRE(cl[i]->getLHS() == "q");
            REQUIRE(cl[i]->getRHS() == "\"Third\"");
        } else {
            REQUIRE(cl[i]->getLHS() == "p");
            REQUIRE(cl[i]->getRHS() == "q");
        }
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("p");
    synonyms.push_back("q");

    for(Declaration d : testQuery.getDeclarationList()) {
        REQUIRE(d.getSynonymList() == synonyms);
        REQUIRE(d.getType() == "procedure");
    }
}

// test cases below consist of multi-clause unit tests

TEST_CASE("Parse multi clause with Parent* and Modifies and Next*") {
    string input = "assign a; while w; Select a such that Modifies (a, \"x\") and Parent* (w, a) and Next* (1, a)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    std::vector<shared_ptr<Clause>> cl = testQuery.getClauseList();
    for(int k = 0; k < cl.size(); k++) {
        if(k == 0) {
            REQUIRE(cl[k]->getLHS() == "a");
            REQUIRE(cl[k]->getRHS() == "\"x\"");
        } else if (k == 1) {
            REQUIRE(cl[k]->getLHS() == "w");
            REQUIRE(cl[k]->getRHS() == "a");
        } else {
            REQUIRE(cl[k]->getLHS() == "1");
            REQUIRE(cl[k]->getRHS() == "a");
        }
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("a");
    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dec = testQuery.getDeclarationList();
    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms);
            REQUIRE(dec[i].getType() == "assign");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse multi clause with Parent* and Modifies such that Next*") {
    string input = "assign a; while w; Select a such that Modifies (a, \"x\") and Parent* (w, a) such that Next* (1, a)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    std::vector<shared_ptr<Clause>> cl = testQuery.getClauseList();
    for(int k = 0; k < cl.size(); k++) {
        if(k == 0) {
            REQUIRE(cl[k]->getLHS() == "a");
            REQUIRE(cl[k]->getRHS() == "\"x\"");
        } else if (k == 1) {
            REQUIRE(cl[k]->getLHS() == "w");
            REQUIRE(cl[k]->getRHS() == "a");
        } else {
            REQUIRE(cl[k]->getLHS() == "1");
            REQUIRE(cl[k]->getRHS() == "a");
        }
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("a");
    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dec = testQuery.getDeclarationList();
    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms);
            REQUIRE(dec[i].getType() == "assign");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse multi clause with all such that") {
    string input = "assign a; while w; Select a such that Modifies (a, \"x\") such that Parent* (w, a) such that Next* (1, a)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    std::vector<shared_ptr<Clause>> cl = testQuery.getClauseList();
    for(int k = 0; k < cl.size(); k++) {
        if(k == 0) {
            REQUIRE(cl[k]->getLHS() == "a");
            REQUIRE(cl[k]->getRHS() == "\"x\"");
        } else if (k == 1) {
            REQUIRE(cl[k]->getLHS() == "w");
            REQUIRE(cl[k]->getRHS() == "a");
        } else {
            REQUIRE(cl[k]->getLHS() == "1");
            REQUIRE(cl[k]->getRHS() == "a");
        }
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("a");
    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dec = testQuery.getDeclarationList();
    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms);
            REQUIRE(dec[i].getType() == "assign");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse multi clause with Parent* and Modifies and pattern") {
    string input = "assign a; while w; Select a such that Modifies (a, \"x\") and Parent* (w, a) pattern a (_, \"x\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    std::vector<shared_ptr<Clause>> cl = testQuery.getClauseList();
    for(int k = 0; k < cl.size(); k++) {
        if(k == 0) {
            REQUIRE(cl[k]->getLHS() == "a");
            REQUIRE(cl[k]->getRHS() == "\"x\"");
        } else if (k == 1) {
            REQUIRE(cl[k]->getLHS() == "w");
            REQUIRE(cl[k]->getRHS() == "a");
        } else {
            REQUIRE(cl[k]->getLHS() == "_");
            REQUIRE(cl[k]->getRHS() == "\"x\"");
        }
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("a");
    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dec = testQuery.getDeclarationList();
    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms);
            REQUIRE(dec[i].getType() == "assign");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse multi clause with multiple pattern clauses") {
    string input = "assign a; while w; Select a pattern a (_, \"xx\") and w (_, _) and a (_, \"x + 2\")";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    std::vector<shared_ptr<Clause>> cl = testQuery.getClauseList();
    for(int k = 0; k < cl.size(); k++) {
        if(k == 0) {
            REQUIRE(cl[k]->getLHS() == "_");
            REQUIRE(cl[k]->getRHS() == "\"xx\"");
        } else if (k == 1) {
            REQUIRE(cl[k]->getLHS() == "_");
            REQUIRE(cl[k]->getRHS() == "_");
        } else {
            REQUIRE(cl[k]->getLHS() == "_");
            REQUIRE(cl[k]->getRHS() == "\"x+2\"");
        }
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("a");
    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dec = testQuery.getDeclarationList();
    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms);
            REQUIRE(dec[i].getType() == "assign");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse multi clause with such that -> pattern -> such that") {
    string input = "assign a; while w; Select a such that Modifies (a, \"x\") pattern a (_,_) such that Parent* (w, a)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a");
    }

    std::vector<shared_ptr<Clause>> cl = testQuery.getClauseList();
    for(int k = 0; k < cl.size(); k++) {
        if(k == 0) {
            REQUIRE(cl[k]->getLHS() == "a");
            REQUIRE(cl[k]->getRHS() == "\"x\"");
        } else if (k == 1) {
            REQUIRE(cl[k]->getLHS() == "_");
            REQUIRE(cl[k]->getRHS() == "_");
        } else {
            REQUIRE(cl[k]->getLHS() == "w");
            REQUIRE(cl[k]->getRHS() == "a");
        }
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("a");
    std::vector<std::string> synonyms2;
    synonyms2.push_back("w");

    std::vector<Declaration> dec = testQuery.getDeclarationList();
    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            REQUIRE(dec[i].getSynonymList() == synonyms);
            REQUIRE(dec[i].getType() == "assign");
        } else {
            REQUIRE(dec[i].getSynonymList() == synonyms2);
            REQUIRE(dec[i].getType() == "while");
        }
    }
}

TEST_CASE("Parse query with 6 clauses") {
    string input = "assign a1, a2; while w1, w2; Select a2 pattern a1 (\"x\", _) and a2 (\"x\", _\"x\"_) such that Affects (a1, a2) and Parent* (w2, a2) and Parent* (w1, w2) pattern a1 (_,_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l1 = QueryLexer(iss);
    Query testQuery = Query();
    QueryParser p1 = QueryParser(l1, testQuery);
    p1.parse();

    // comparison
    std::vector<std::string> targetList = testQuery.getTargetLst();
    for(std::string s : targetList) {
        REQUIRE(s == "a2");
    }

    std::vector<shared_ptr<Clause>> cl = testQuery.getClauseList();
    for(int k = 0; k < cl.size(); k++) {
        if(k == 0) {
            REQUIRE(cl[k]->getLHS() == "\"x\"");
            REQUIRE(cl[k]->getRHS() == "_");
        } else if (k == 1) {
            REQUIRE(cl[k]->getLHS() == "\"x\"");
            REQUIRE(cl[k]->getRHS() == "_\"x\"_");
        } else if (k == 2) {
            REQUIRE(cl[k]->getLHS() == "a1");
            REQUIRE(cl[k]->getRHS() == "a2");
        } else if (k == 3) {
            REQUIRE(cl[k]->getLHS() == "w2");
            REQUIRE(cl[k]->getRHS() == "a2");
        } else if (k == 4) {
            REQUIRE(cl[k]->getLHS() == "w1");
            REQUIRE(cl[k]->getRHS() == "w2");
        } else {
            REQUIRE(cl[k]->getLHS() == "_");
            REQUIRE(cl[k]->getRHS() == "_");
        }
    }

    std::vector<std::string> synonyms;
    synonyms.push_back("a1");
    synonyms.push_back("a2");
    std::vector<std::string> synonyms2;
    synonyms2.push_back("w1");
    synonyms2.push_back("w2");

    std::vector<Declaration> dec = testQuery.getDeclarationList();
    for(int i = 0; i < dec.size(); i++) {
        if(i == 0) {
            std::vector<std::string> s1 = dec[i].getSynonymList();
            for(int k = 0; k < 2; k++) {
                REQUIRE(s1[k] == synonyms[k]);
                REQUIRE(dec[i].getType() == "assign");
            }
        } else {
            std::vector<std::string> s2 = dec[i].getSynonymList();
            for(int k = 0; k < 2; k++) {
                REQUIRE(s2[k] == synonyms2[k]);
                REQUIRE(dec[i].getType() == "while");
            }
        }
    }

}
