//
// Created by Vanessa Khor on 5/2/23.
//
#include "Query.h"

Query::Query() {}

Query::Query(std::vector<Declaration>& d, std::vector<std::shared_ptr<Clause>>& c, std::string target) : declarations(d), clauses(c), target(target) {};

std::vector<std::shared_ptr<Clause>> Query::getClauseList() {
	return clauses;
}

std::string Query::getTarget() {
    return target;
}

std::vector<std::string> Query::getTargetLst() {
    return selected.getTarget();
}

std::vector<Declaration> Query::getDeclarationList() {
    return declarations;
}

void Query::printQuery() {
    std::cout << "clauses: ";
    for (std::shared_ptr<Clause> c : clauses) {
        c->printClause();
    }
    std::cout << "declarations: ";
    for (Declaration d : declarations) {
        d.printDeclaration();
    }
}

void Query::addClause(std::shared_ptr<Clause> c) {
    clauses.push_back(c);
}

void Query::addDeclaration(Declaration d) {
    declarations.push_back(d);
}

void Query::setTarget(std::string t) {
    target = t;
}

void Query::setTargetSelected(std::string type, std::vector<std::string> s) {
    if (type == "bool") {
        selected.setBool();
    } else if (type == "syn") {
        selected.setSyn(s[0]);
    } else if (type == "tuple") {
        selected.setTuple(s);
    } else {
        // should not reach here
    }
}

bool Query::isDeclarationPresent(std::string type, std::string syn) {
    for (Declaration d : declarations) {
        std::vector<std::string> curr = d.getSynonymList();
        if (d.getType() == type && (std::find(curr.begin(), curr.end(), syn) != curr.end())) {
            return true;
        }
    }
    return false;
}

bool Query::isDeclarationEmpty() {
    return declarations.empty();
}

bool Query::isSynonymPresent(std::string syn) {
    for (Declaration d : declarations) {
        std::vector<std::string> curr = d.getSynonymList();
        if (std::find(curr.begin(), curr.end(), syn) != curr.end()) {
            return true;
        }
    }
    return false;
}

/**
 * Returns type of synonym
 * given that synonyms present in a query are unique
 * given that synonym is present
 * @param syn
 * @return
 */
std::string Query::getSynonymType(std::string syn) {
    for (Declaration d : declarations) {
        std::vector<std::string> synonymList = d.getSynonymList();
        for (std::string s : synonymList) {
            if (s == syn) {
                return d.getType();
            }
        }
    }
    return "none";  // synonym will be present, should not return "none"
}
