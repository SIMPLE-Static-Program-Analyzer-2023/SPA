//
// Created by Vanessa Khor on 8/4/23.
//

#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include <memory>
#include "../Query.h"
#include "../Clause.h"

class PriorityEvaluator {
    std::shared_ptr<Clause> clause;
public:
    explicit PriorityEvaluator(std::shared_ptr<Clause> clause);
    int evaluatePriority();
    int getSynCount();
    int getClauseTypeScore();
};
