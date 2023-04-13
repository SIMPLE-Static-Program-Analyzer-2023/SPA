#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iterator>
#include <random>
#include <string>
#include <cassert>
#include "qps/Clause.h"
#include "qps/QueryEvaluator/ClauseResult.h"
#include "qps/QueryEvaluator/ClauseResultBuilder.h"

using ClausePtr = std::shared_ptr<Clause>;

/**
 * Virtual base class for ClauseGroups
 */
class ClauseGroup {
public:
    /**
     * Add a clause to this ClauseGroup
     * @param c Clause to add
     */
    virtual void addClause(ClausePtr& c) = 0;

    /**
     * Reduces all the Clauses in a single ClauseResult
     * @return Final evaluation result
     */
    virtual ClauseResult reduce(QPSFacade& qpsFacade,
                        const std::unordered_map<std::string, std::string>& synonymTypes) = 0;
    /**
     * Returns the number of clauses in this ClauseGroup
     * @return Number of clauses
     */
    [[nodiscard]] int getNumClauses() const;

protected:
    int numClauses = 0;
};

/**
 * A group of clauses that share the same common synonym
 */
class SynonymClauseGroup : public ClauseGroup {
public:
    SynonymClauseGroup();
    SynonymClauseGroup(std::unordered_map<std::string, std::vector<ClausePtr>>& commonSynonyms,
                       const std::unordered_set<std::string>& synonymList);


    void addClause(ClausePtr& c) final;
    ClauseResult reduce(QPSFacade& qpsFacade,
                        const std::unordered_map<std::string, std::string>& synonymTypes) final;

    /**
     * Combines a ClauseGroup with another ClauseGroup.
     * Should be used when two ClauseGroups share a common synonym.
     * @param other ClauseGroup to combine with
     * @return Combined ClauseGroup
     */
    void combine(SynonymClauseGroup& other);

    /**
     * Returns the common synonyms in this ClauseGroup
     * @return Common synonyms
     */
    [[nodiscard]] std::unordered_set<std::string> getCommonSynonyms() const;

    /**
     * Returns the total priority score of all the clauses in this ClauseGroup
     * @return Total priority score
     */
    [[nodiscard]] int getTotalPriorityScore() const;

private:
    std::unordered_map<std::string, std::vector<ClausePtr>> synonymMap;
    std::unordered_set<std::string> commonSynonyms;
    int totalPriorityScore = 0;
};

/**
 * A group of 0-synonym clauses
 */
class NonSynonymClauseGroup : public ClauseGroup {
public:
    void addClause(ClausePtr& c) final;
    ClauseResult reduce(QPSFacade& qpsFacade,
                        const std::unordered_map<std::string, std::string>& synonymTypes) final;
private:
    std::vector<ClausePtr> clauses;
};

class ClauseComparator {
    public:
    bool operator()(const ClausePtr& lhs, const ClausePtr& rhs) const {
        // The clause with the least score has the highest priority
        return lhs->getPriorityScore() > rhs->getPriorityScore();
    }
};

class SynonymClauseGroupComparator {
public:
    bool operator()(const SynonymClauseGroup& lhs, const SynonymClauseGroup& rhs) const {
        // The ClauseGroup with lowest Score should be evaluated first
        return lhs.getTotalPriorityScore() > rhs.getTotalPriorityScore();
    }
};

