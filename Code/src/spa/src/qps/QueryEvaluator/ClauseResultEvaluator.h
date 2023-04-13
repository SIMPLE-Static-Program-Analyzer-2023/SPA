#pragma once

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include "ClauseResult.h"
#include "ClauseResultBuilder.h"

// Forward declarations
class ClauseResult;

class ClauseResultEvaluator {
private:
	// Evaluate any combination of clause results where one of them is a boolean clause result
	static ClauseResult evaluateBool(ClauseResult& boolClause, ClauseResult& c2);
	// Evaluates any combination of two table clause results. Joining of the tables are non-destructive.
	static ClauseResult evaluateTableTable(ClauseResult& c1, ClauseResult& c2);
    static void findCommonSynonyms(std::unordered_map < std::string, std::vector<int> >& commonSynonyms,
                                   std::vector<std::string> synonyms1,
                                   std::vector<std::string> synonyms2);
    static ClauseResult innerJoin(ClauseResult& c1,
                                          ClauseResult& c2,
                                          std::unordered_map < std::string, std::vector<int> > commonSynonyms);
    static ClauseResult crossProduct(ClauseResult& c1, ClauseResult& c2);
public:
	static ClauseResult evaluate(ClauseResult& c1, ClauseResult& c2);
};
