//
// Created by Vanessa Khor on 5/2/23.
//
#pragma once
#include <list>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include "Declaration.h"
#include "Clause.h"
#include <tuple>
#include "QueryTarget.h"

// #include "../../../../../../../CLionProjects/untitled/qps/clause.h"

class Query {
private:
    std::vector<Declaration> declarations;
    std::vector<std::shared_ptr<Clause>> clauses;
    std::string target;
    QueryTarget selected = QueryTarget();

public:
    Query();
    explicit Query(std::vector<Declaration>& d, std::vector<std::shared_ptr<Clause>>& c, std::string target);

    void printQuery();  // output query to console

    // get functions for query evaluator
    std::string getTarget();  // return usual single synonym target
    std::vector<std::string> getTargetLst();  // return advance SPA target
    std::vector<std::shared_ptr<Clause>> getClauseList();
    std::vector<Declaration> getDeclarationList();

    // set functions for query parser
    void addClause(std::shared_ptr<Clause> c);
    void addDeclaration(Declaration d);
    void setTarget(std::string t);
    void setTargetSelected(std::string type, std::vector<std::string> s);
    bool isDeclarationEmpty();
    bool isDeclarationPresent(std::string type, std::string syn);
    bool isSynonymPresent(std::string syn);

    std::string getSynonymType(std::string syn);

    bool operator==(const Query& other) const {
    return target == other.target && declarations == other.declarations && clauses == other.clauses;
    }
};
