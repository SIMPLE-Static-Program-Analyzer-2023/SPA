//
// Created by Vanessa Khor on 17/2/23.
//
#include "qps/QueryLexer.h"
#include <string>
#include <sstream>
#include "catch.hpp"
#include <iostream>

using namespace std;

TEST_CASE("basic lexer get current token test") {
    std::string input = "variable v; Select v such that Uses(1, v) pattern a (_, _)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l = QueryLexer(iss);
    REQUIRE(l.getCurrentToken() == "variable");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "v");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == ";");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "Select");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "v");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "such");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "that");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "Uses");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "(");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "1");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == ",");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "v");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == ")");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "pattern");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "a");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "(");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "_");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == ",");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "_");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == ")");
}

TEST_CASE("lexer get current token test with string") {
    std::string input = "variable v; Select v such that Uses(1, v) pattern a (_,_\"x\"_)";
    shared_ptr<istringstream> iss = make_shared<istringstream>(input);
    QueryLexer l = QueryLexer(iss);
    REQUIRE(l.getCurrentToken() == "variable");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "v");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == ";");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "Select");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "v");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "such");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "that");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "Uses");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "(");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "1");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == ",");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "v");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == ")");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "pattern");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "a");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "(");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "_");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == ",");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "_");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "\"");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "x");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "\"");
    l.nextToken();
    REQUIRE(l.getCurrentToken() == "_");
}

TEST_CASE("varying whitespace") {
    SECTION("2 syn") {
        std::list<std::string> queryList;
        std::string queryNoSpace = "Uses(x,y)";
        queryList.push_back(queryNoSpace);
        std::string queryLhsSpace1 = "Uses( x,y)";
        std::string queryLhsSpace2 = "Uses(x ,y)";
        std::string queryLhsSpace3 = "Uses( x ,y)";
        queryList.push_back(queryLhsSpace1);
        queryList.push_back(queryLhsSpace2);
        queryList.push_back(queryLhsSpace3);
        std::string queryRhsSpace1 = "Uses(x, y)";
        std::string queryRhsSpace2 = "Uses(x,y )";
        std::string queryRhsSpace3 = "Uses(x, y )";
        queryList.push_back(queryRhsSpace1);
        queryList.push_back(queryRhsSpace2);
        queryList.push_back(queryRhsSpace3);
        std::string queryLhsRhsSpace1 = "Uses( x, y)";
        std::string queryLhsRhsSpace2 = "Uses(x ,y )";
        std::string queryLhsRhsSpace3 = "Uses( x , y )";
        queryList.push_back(queryLhsRhsSpace1);
        queryList.push_back(queryLhsRhsSpace2);
        queryList.push_back(queryLhsRhsSpace3);

        for (std::string q : queryList) {
            shared_ptr<istringstream> iss = make_shared<istringstream>(q);
            QueryLexer l1 = QueryLexer(iss);
            REQUIRE(l1.getCurrentToken() == "Uses");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == "(");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == "x");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == ",");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == "y");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == ")");
        }
    }

    SECTION("syn,IDENT") {
        std::list<std::string> queryList;
        std::string queryNoSpace = "Uses(x,\"y\")";
        queryList.push_back(queryNoSpace);
        std::string queryLhsSpace1 = "Uses( x,\"y\")";
        std::string queryLhsSpace2 = "Uses(x ,\"y\")";
        std::string queryLhsSpace3 = "Uses( x ,\"y\")";
        queryList.push_back(queryLhsSpace1);
        queryList.push_back(queryLhsSpace2);
        queryList.push_back(queryLhsSpace3);
        std::string queryRhsSpace1 = "Uses(x, \"y\")";
        std::string queryRhsSpace2 = "Uses(x,\"y\" )";
        std::string queryRhsSpace3 = "Uses(x, \"y\" )";
        queryList.push_back(queryRhsSpace1);
        queryList.push_back(queryRhsSpace2);
        queryList.push_back(queryRhsSpace3);
        std::string queryLhsRhsSpace1 = "Uses( x, \"y\")";
        std::string queryLhsRhsSpace2 = "Uses(x ,\"y\" )";
        std::string queryLhsRhsSpace3 = "Uses( x , \"y\" )";
        queryList.push_back(queryLhsRhsSpace1);
        queryList.push_back(queryLhsRhsSpace2);
        queryList.push_back(queryLhsRhsSpace3);

        for (std::string q : queryList) {
            shared_ptr<istringstream> iss = make_shared<istringstream>(q);
            QueryLexer l1 = QueryLexer(iss);
            REQUIRE(l1.getCurrentToken() == "Uses");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == "(");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == "x");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == ",");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == "\"");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == "y");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == "\"");
            l1.nextToken();
            REQUIRE(l1.getCurrentToken() == ")");
        }

    }

}

TEST_CASE("check individual tokens: zero clause valid query") {
    SECTION("end with one char name") {
        std::string input = "stmt s; Select s";
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        QueryLexer l1 = QueryLexer(iss);
        REQUIRE(l1.getCurrentToken() == "stmt");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ";");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "Select");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s");
        REQUIRE(l1.isEnd());
    }

    SECTION("end with multiple char name") {
        std::string input = "stmt stmt; Select stmt";
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        QueryLexer l1 = QueryLexer(iss);
        REQUIRE(l1.getCurrentToken() == "stmt");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "stmt");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ";");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "Select");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "stmt");
        l1.nextToken();
        REQUIRE(l1.isEnd());
    }
}

TEST_CASE("check individual tokens: one clause valid query") {
    SECTION("Uses(syn, syn)") {
        std::string input = "stmt s; Select s such that Uses (s, v)";
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        QueryLexer l1 = QueryLexer(iss);
        REQUIRE(l1.getCurrentToken() == "stmt");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ";");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "Select");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "such");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "that");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "Uses");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "(");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ",");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "v");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ")");
        l1.nextToken();
        REQUIRE(l1.isEnd());
    }

    SECTION("Uses(syn,IDENT)") {
        std::string input = "stmt s; Select s such that Uses (s, \"a\")";
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        QueryLexer l1 = QueryLexer(iss);
        REQUIRE(l1.getCurrentToken() == "stmt");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ";");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "Select");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "such");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "that");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "Uses");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "(");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ",");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "\"");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "a");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "\"");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ")");
        l1.nextToken();
        REQUIRE(l1.isEnd());
    }

    SECTION("Parent* tokenization") {
        std::string input = "stmt s1, s2; Select s1 such that Follows* (s1, s2)";
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        QueryLexer l1 = QueryLexer(iss);
        REQUIRE(l1.getCurrentToken() == "stmt");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s1");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ",");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s2");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ";");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "Select");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s1");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "such");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "that");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "Follows");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "*");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "(");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s1");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ",");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "s2");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ")");
        l1.nextToken();
        REQUIRE(l1.isEnd());
    }

    SECTION("pattern with operators") {
        std::string input = "assign a; Select a pattern a(_, \"v+x*y\")";
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        QueryLexer l1 = QueryLexer(iss);
        REQUIRE(l1.getCurrentToken() == "assign");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "a");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ";");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "Select");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "a");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "pattern");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "a");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "(");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "_");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ",");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "\"");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "v");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "+");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "x");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "*");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "y");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == "\"");
        l1.nextToken();
        REQUIRE(l1.getCurrentToken() == ")");
        l1.nextToken();
        REQUIRE(l1.isEnd());
    }
}

TEST_CASE("Query with advanced SPA select") {
    SECTION("target with attributes") {
        std::string input = "assign a1, a2; Select <a1.stmt#, a2> such that Affects (a1, a2)";
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        QueryLexer l1 = QueryLexer(iss);
        REQUIRE(l1.getCurrentToken() == "assign");
        l1.nextToken();
        while(!l1.isEnd()) {
            std::cout << l1.getCurrentToken() << std::endl;
            l1.nextToken();
        }
    }

//    SECTION("target with attributes") {
//        std::string input = "procedure p, q; Select p.procName such that Calls (p, q)";
//        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
//        QueryLexer l1 = QueryLexer(iss);
//        REQUIRE(l1.getCurrentToken() == "procedure");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "p");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == ",");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "q");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == ";");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "Select");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "p");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == ".");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "procName");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "such");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "that");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "Calls");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "(");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "p");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == ",");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == "q");
//        l1.nextToken();
//        REQUIRE(l1.getCurrentToken() == ")");
//        l1.nextToken();
//        REQUIRE(l1.isEnd());
//    }
}