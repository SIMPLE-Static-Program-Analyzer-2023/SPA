#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>

#include "ClauseResultBuilder.h"
#include "ClauseResultEvaluator.h"
#include "pkb/Facade/QPSFacade.h"
#include "qps/QpsException.h"

enum class StatusType {
	ValidTableResult,
	ValidBooleanResult,
	InvalidBooleanResult
};

class ClauseResult {
private:
	StatusType status;
	std::vector< std::string > synonymList;
	std::vector< std::vector < std::string> > table;

	// boolean Result
	explicit ClauseResult(bool status);

	// n-table result
	ClauseResult(std::vector< std::string > &synonymList, std::vector< std::vector < std::string> > &table);

    // Helper function to retrieve a single-synonym select from the table, or from PKB if not present
	[[nodiscard]] std::vector< std::string > extractFromTable(const std::string& synonym,
                                                              const std::string& synonymType,
                                                              QPSFacade& qpsf) const;

    // Helper function to retrieve a certain synonym type from the QPSFacade
	static std::vector< std::string > getEntitiesFromQps(QPSFacade &qpsf, const std::string& type);

    /**
     * Populates synonymIndexes with the mapped indexes of the same synonyms in this and o.
     * synonymIndexes[i][0] is the index of the synonym in this, synonymIndexes[i][1] is the index of the synonym in o.
     *
     * @param synonymIndexes Vector to populate
     * @param o ClauseResult to compare with
     * @return false if there is any synonym in this that does not exist in o, or vice versa. True otherwise
     */
	bool mapSynonyms(std::vector<std::vector<int>>& synonymIndexes, const ClauseResult* o) const;

    /**
     * Populates synonymIndexes with the indexes of all synonyms in this that are present in synonyms.
     * @param synonymIndexes Map to populate, synonym: index
     * @param synonyms List of synonyms to check for
     * @return Returns true if all synonyms requested for have valid indexes, false otherwise.
     */
    bool mapSynonyms(std::unordered_map<std::string, int> &synonymIndexes,
                     std::unordered_set<std::string>& nonIncludedSynonyms,
                     const std::vector<std::string> &synonyms) const;

    static std::vector< std::string > intArrToStringArr(std::vector<int>& arr);
	friend class ClauseResultBuilder;
	friend class ClauseResultEvaluator;
public:
	bool isBoolean();
    std::vector< std::vector < std::string> > getTable();
    std::vector< std::string > getSynonymList();
    int size();
	bool operator==(const ClauseResult& other) const;

    /**
     * For select BOOLEAN queries.
     * @return true if the query is valid, false otherwise
     */
    bool extractQueryBoolean();

    /**
     * For select-such-that, select-pattern and select-with queries.
     *
     * Returns a list of strings. If the synonym is present in the table, returns the list of strings in the column.
     * Otherwise, returns a list of strings representing the entities of the specified type from PKB.
     *
     * @param qpsf QPS Facade
     * @param synonym Synonym name to extract
     * @param synonymType Type of synonym (used only if synonym is not present in table)
     * @return
     */
    std::vector< std::string > extractQueryData(QPSFacade& qpsf, const std::string& synonym, const std::string& synonymType);

    /**
     * For select-tuple queries.
     *
     * Returns a list of tuples, where for each tuple, there exists a combination of synonym instances satisfying ALL
     * the conditions specified in such-that, pattern and with clauses.
     *
     * @param synonyms Names of all synonyms required in the tuple
     * @return List of tuples, where each tuple is a list of strings
     */
    std::vector<std::vector<std::string>> extractQueryTuple(const std::vector<std::string>& synonyms,
                                                            const std::unordered_map<std::string, std::string>& synonymTypes,
                                                            QPSFacade& qpsFacade);
};
