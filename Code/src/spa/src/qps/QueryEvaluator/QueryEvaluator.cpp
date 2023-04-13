#include <stack>

#include "QueryEvaluator.h"

ClauseResult QueryEvaluator::computeFinalResult(Query &query, QPSFacade& qpsFacade) {
    std::priority_queue<SynonymClauseGroup, std::vector<SynonymClauseGroup>, SynonymClauseGroupComparator> pq;
	std::vector<std::shared_ptr<Clause>> clauseList = query.getClauseList();

	// Create list of synonyms and their type
	std::unordered_map<std::string, std::string> synonymTypes;
	for (auto declaration : query.getDeclarationList()) {
		for (const auto& synonym : declaration.getSynonymList()) {
			synonymTypes[synonym] = declaration.getType();
		}
	}

    // Group the clauses together
    ClauseGrouper clauseGrouper;
    clauseGrouper.addClauses(query);

    // Check non-synonym clauses
    auto nonSynonymClauses = clauseGrouper.getNonSynonymGroup();
    auto nonSynResult = nonSynonymClauses.reduce(qpsFacade, synonymTypes);
    if (!nonSynResult.extractQueryBoolean()) {
        return nonSynResult;  // Early termination if false
    }

    // Add 1-2 synonym clauses to pq
    auto clauseGroups = clauseGrouper.getClauseGroups();
    for (const auto& clauseGroup : clauseGroups) pq.push(clauseGroup);

    // If pq is empty, no clauses present for select statement; return true
    if (pq.empty()) return ClauseResultBuilder::createBooleanResult(true);

    // Base value
    SynonymClauseGroup baseClauseGroup = pq.top();
    ClauseResult result = baseClauseGroup.reduce(qpsFacade, synonymTypes);
    pq.pop();

    // Cross-product by popping from pq
    while (!pq.empty()) {
        SynonymClauseGroup clauseGroup = pq.top();
        pq.pop();
        auto tempResult = clauseGroup.reduce(qpsFacade, synonymTypes);
        if (!tempResult.extractQueryBoolean()) return tempResult;  // Early termination if false
        result = ClauseResultEvaluator::evaluate(result, tempResult);
    }

	return result;
}
