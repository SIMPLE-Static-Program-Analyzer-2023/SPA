#pragma once
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "../pkb/Facade/QPSFacade.h"
#include "qps/QueryEvaluator/ClauseResult.h"

class Clause {
private:
    /*
     * constant x represented as '"x"'
     * synonym x represented as "x"
     * */
    std::string LHS;
    std::string RHS;
    std::string assignSyn;
    int priorityScore;

public:
    std::string clauseType;
    explicit Clause(std::string lhs, std::string rhs);
    void printClause();
    std::string getLHS();
    std::string getRHS();
    int getPriorityScore();
    void setPriorityScore(int ps);
    virtual std::string getAssignSynonym();
    bool isSynonym(std::string input);
    bool isSubstring(std::string input);
    bool isWildcard(std::string input);
    bool isConstant(std::string token);
    std::vector< std::string > intArrToStringArr(std::vector<int> arr);
    ClauseResult processTupleList(std::vector<std::pair<int, int>> tupleListIntInt,
                                          std::vector<std::pair<int, std::string>> tupleListIntStr,
                                          std::string LHSSynType, std::string RHSSynType,
                                          QPSFacade& qpsFacade);
    ClauseResult processList(std::vector<int> listInt, std::string LHSSynType, std::string RHSSynType, QPSFacade& qpsFacade);
    virtual ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
    // virtual ClauseResult evaluate(QPSFacade& qpsFacade, std::vector<std::string> target, std::unordered_map<std::string, std::string> synonymTypes, bool hasAssignSynonym, bool hasSynonym, int clauseListSize);

    bool operator==(const Clause& other) const {
        return LHS == other.LHS && RHS == other.RHS && clauseType == other.clauseType;
    }
};

class FollowsClause : public Clause {
public:
    FollowsClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class FollowsTClause : public Clause {
public:
    FollowsTClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class ParentClause : public Clause {
public:
    ParentClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class ParentTClause : public Clause {
public:
    ParentTClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class UsesClause : public Clause {
public:
    UsesClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class ModifiesClause : public Clause {
public:
    ModifiesClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class CallsClause : public Clause {
public:
    CallsClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class CallsTClause : public Clause {
public:
    CallsTClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class NextClause : public Clause {
public:
    NextClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class NextTClause : public Clause {
public:
    NextTClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class AffectsClause : public Clause {
public:
    AffectsClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class AffectsTClause : public Clause {
public:
    AffectsTClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

/*class PatternClause : public Clause {
public:
    PatternClause(std::string lhs, std::string rhs);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::string target, std::unordered_map<std::string, std::string> synonymTypes, bool hasAssignSynonym, bool hasSynonym, int clauseListSize);
};*/

class AssignPatternClause : public Clause {
private:
    std::string assignSynonym;

public:
    AssignPatternClause(std::string lhs, std::string rhs, std::string assignSyn);

    std::string getAssignSynonym();
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class IfPatternClause : public Clause {
private:
    std::string third;
    std::string assignSynonym;

public:
    IfPatternClause(std::string lhs, std::string rhs, std::string third, std::string assignSyn);

    std::string getAssignSynonym();
    std::string getThird();
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class WhilePatternClause : public Clause {
private:
    std::string assignSynonym;

public:
    WhilePatternClause(std::string lhs, std::string rhs, std::string assignSyn);

    std::string getAssignSynonym();
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};

class WithClause : public Clause {
    std::string leftType;
    std::string rightType;

public:
    WithClause(std::string leftType, std::string leftName, std::string rightType, std::string rightName);
    std::vector<std::pair<int, std::string>> processWith(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes, std::string synonym, std::string attribute);
    ClauseResult evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes);
};
