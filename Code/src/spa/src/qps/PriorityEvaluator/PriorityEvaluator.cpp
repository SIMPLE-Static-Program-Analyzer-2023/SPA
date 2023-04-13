//
// Created by Vanessa Khor on 8/4/23.
//
#include "PriorityEvaluator.h"

std::unordered_map<std::string, int> priorityMap =
        {{"FollowsClause", 2},
         {"FollowsTClause", 2},
         {"ParentClause", 2},
         {"ParentTClause", 2},
         {"ModifiesClause", 2},
         {"UsesClause", 2},
         {"CallsClause", 2},
         {"CallsTClause", 2},
         {"NextClause", 2},
         {"NextTClause", 2},
         {"AffectsClause", 8},
         {"AffectsTClause", 8},
         {"WithClause", 1},
         {"AssignPatternClause", 6},
         {"IfPatternClause", 6},
         {"WhilePatternClause", 6},
        };

PriorityEvaluator::PriorityEvaluator(std::shared_ptr<Clause> clause): clause(clause) {}

int PriorityEvaluator::evaluatePriority() {
    int synCount = getSynCount();
    int clauseTypeScore = getClauseTypeScore();
    return (synCount - 1) * 10 + clauseTypeScore;
}

bool isSynonym(std::string input) {
    return isalpha(input[0]);
}

int PriorityEvaluator::getSynCount() {
    int count = 0;
    if (isSynonym(clause->getLHS())) {
        count++;
    }
    if (isSynonym(clause->getRHS())) {
        count++;
    }
    return count;
}

int PriorityEvaluator::getClauseTypeScore() {
    return priorityMap[clause->clauseType];
}
