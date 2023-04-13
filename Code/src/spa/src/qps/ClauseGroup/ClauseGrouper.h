#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "ClauseGroup.h"
#include "qps/Query.h"

struct SynonymInfo {
    bool isSynonym;
    int synonymIndex;
};

/**
 * Facilitates grouping of Query's clauses
 */
class ClauseGrouper {
private:
    // 1-2 synonym ClauseGroups
    std::vector<SynonymClauseGroup> clauseGroups;

    // 0-synonym Clauses
    // TODO: Can we optimise further here? Evaluate first instead of storing, and just stop evaluation if false
    NonSynonymClauseGroup nonSynonymGroup;

    // Map from synonym -> index of its corresponding ClauseGroup in clauseGroups
    std::unordered_map<std::string, int> synonymMap;

    // Gets the synonym data from synonymMap, if it exists
    SynonymInfo extractSynonymInfo(const std::string& synonym, const std::shared_ptr<Clause>& c);

    // Updates the synonym indexes of c's synonyms to the given index.
    void updateClauseGroupMapIndex(SynonymClauseGroup& c, int index);

    void combineClauseGroups(int index, int indexToCombine);

public:
    void addClause(const std::shared_ptr<Clause>& clause);

    /**
     * Utility function to add all the clauses in the query to its respective ClauseGroups
     * @param query Query to group its clauses
     */
    void addClauses(Query& query);

    /**
     * Adds 1 clause to its respective ClauseGroup
     * @param clause clause to be grouped
     */
    void addClause(std::shared_ptr<Clause> clause,
                   const std::string& LHS, SynonymInfo LHSInfo,
                   const std::string& RHS, SynonymInfo RHSInfo);

    /**
     * Returns all 1-2 synonym clausegroups
     * @return clause groups with 1-2 synonyms
     */
    std::vector<SynonymClauseGroup> getClauseGroups();

    /**
     * Returns all 0-synonym clauses
     * @return clause group with 0 synonyms
     */
    NonSynonymClauseGroup getNonSynonymGroup();
};
