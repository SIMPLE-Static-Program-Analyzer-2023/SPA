#include <cassert>
#include <unordered_map>

#include "ClauseResult.h"

ClauseResult::ClauseResult(bool status)
	: synonymList(),
	table() {
	if (status)
		this->status = StatusType::ValidBooleanResult;
	else
		this->status = StatusType::InvalidBooleanResult;
}

ClauseResult::ClauseResult(std::vector< std::string > &synonymList, std::vector<std::vector<std::string>> &table)
	: status(StatusType::ValidTableResult),
	synonymList(synonymList),
	table(table) {
	for (const auto& row : table) assert(synonymList.size() == row.size());
}

int ClauseResult::size() {
	return this->synonymList.size();
}

bool ClauseResult::isBoolean() {
	return this->status == StatusType::ValidBooleanResult || this->status == StatusType::InvalidBooleanResult;
}

std::vector< std::vector < std::string> > ClauseResult::getTable() {
	return this->table;
}

std::vector<std::string> ClauseResult::getSynonymList() {
    return this->synonymList;
}

bool ClauseResult::operator==(const ClauseResult& other) const {
	if (auto o = dynamic_cast<const ClauseResult*>(&other)) {
		// Early termination
		if (o->status != this->status) return false;
		if (o->status != StatusType::ValidTableResult) return true;
		if (o->synonymList.size() != this->synonymList.size()) return false;
		if (o->table.size() != this->table.size()) return false;

		// Map each synonym to the other's index, if present
		std::vector<std::vector<int>> synonymIndexes;
		if (!this->mapSynonyms(synonymIndexes, o)) return false;

		// Compare each row based on the mapped synonym indexes
		for (auto pair : synonymIndexes) {
			std::unordered_map<std::string, int> values;
			// Save to map
			for (auto currRow : this->table) {
				auto currString = currRow[pair[0]];
				if (values.find(currString) == values.end()) {
					values[currString] = 0;
				}

				values[currString] += 1;
			}
			// Remove from map
			for (auto currRow : o->table) {
				auto currString = currRow[pair[1]];
				if (values.find(currString) != values.end()) {
					values[currString] -= 1;
				} else {
					return false;
				}
			}
			// Check if elements remaining
			for (const auto& valuePair : values) {
				if (valuePair.second != 0) return false;
			}
		}

		return true;
	}
	return false;
}

bool ClauseResult::extractQueryBoolean() {
    switch (this->status) {
        case StatusType::InvalidBooleanResult:
            return false;
        case StatusType::ValidBooleanResult:
            return true;
        case StatusType::ValidTableResult:
            return this->table.size() > 0;
    }
    throw QpsSyntaxException("Invalid status");
}

std::vector<std::string> ClauseResult::extractQueryData(QPSFacade& qpsf,
                                                        const std::string& synonym,
                                                        const std::string& synonymType) {
	switch (this->status) {
	case StatusType::InvalidBooleanResult:
		return { };
	case StatusType::ValidBooleanResult:
		return ClauseResult::getEntitiesFromQps(qpsf, synonymType);
	case StatusType::ValidTableResult:
		return extractFromTable(synonym, synonymType, qpsf);
	}
	throw QpsSyntaxException(synonymType + " is not a synonym type");
}

std::vector<std::vector<std::string>> ClauseResult::extractQueryTuple(const std::vector<std::string>& synonyms,
                                                                      const std::unordered_map<std::string, std::string>& synonymTypes,
                                                                      QPSFacade& qpsFacade) {
    std::vector<std::vector<std::string>> result;
    if (this->status == StatusType::InvalidBooleanResult) return result;  // Early termination

    std::vector<std::vector<std::vector<std::string>>> listsOfTuples;

    // Map each synonym to an index.
    std::unordered_map<std::string, int> synonymIndexes;  // Included synonym
    std::unordered_set<std::string> nonIncludedSynonyms;
    this->mapSynonyms(synonymIndexes, nonIncludedSynonyms, synonyms);

    // Get all tuples for included synonyms if any exist
    if (!synonymIndexes.empty()) {
        std::vector<std::vector<std::string>> includedTemp;
        for (auto tuple : this->table) {
            std::vector<std::string> currTuple;
            for (const auto& index : synonymIndexes) {
                currTuple.push_back(tuple[index.second]);
            }
            includedTemp.push_back(currTuple);
        }
        if (!includedTemp.empty()) listsOfTuples.push_back(includedTemp);
    }

    // For those not mapped, get all entities from QPS instead
    for (const auto& synonym : nonIncludedSynonyms) {
        auto entities = ClauseResult::getEntitiesFromQps(qpsFacade, synonymTypes.at(synonym));
        std::vector<std::vector<std::string>> x;
        for (const auto& entity : entities) x.push_back({ entity });
        listsOfTuples.push_back(x);
    }

    // Cross-product
    if (listsOfTuples.empty()) return result;
    for (const auto& tuple : listsOfTuples[0]) result.push_back(tuple);
    for (int i = 1; i < listsOfTuples.size(); i++) {
        std::vector<std::vector<std::string>> temp;
        for (const auto& tuple : result) {
            for (auto tuple2 : listsOfTuples[i]) {
                std::vector<std::string> currTuple = tuple;
                currTuple.insert(currTuple.end(), tuple2.begin(), tuple2.end());
                temp.push_back(currTuple);
            }
        }
        result = temp;
    }

    // Populate newSynonymOrder
    std::unordered_map<std::string, int> newSynonymOrder;
    int currIndex = 0;
    for (const auto& synonymPair : synonymIndexes) newSynonymOrder[synonymPair.first] = currIndex++;
    for (const auto& synonym : nonIncludedSynonyms) newSynonymOrder[synonym] = currIndex++;

    // Sort each tuple from the new order back to the old order
    std::vector<std::vector<std::string>> finalResult;
    for (const auto& tuple : result) {
        std::vector<std::string> currTuple;
        for (const auto& synonym : synonyms) {
            currTuple.push_back(tuple[newSynonymOrder[synonym]]);
        }
        finalResult.push_back(currTuple);
    }

    return finalResult;
}

std::vector<std::string> ClauseResult::extractFromTable(const std::string& target,
                                                        const std::string& synonymType,
                                                        QPSFacade& qpsf) const {
	// Check index of synonym
	int index = -1;
	for (int i = 0; i < this->synonymList.size(); i++) {
		if (!target.compare(this->synonymList[i])) {
			index = i;
			break;
		}
	}
	// Early termination
	// if (index < 0) return { };
	if (index < 0) {
		if (this->table.empty()) {
			return { };
		}
		return ClauseResult::getEntitiesFromQps(qpsf, synonymType);
	}

	std::vector<std::string> res;
	for (auto tuple : this->table) {
		res.push_back(tuple[index]);
	}

	// remove duplicates
	sort(res.begin(), res.end());
	res.erase(unique(res.begin(), res.end()), res.end());

	// inner join with select
	std::vector<std::string> selectAll = ClauseResult::getEntitiesFromQps(qpsf, synonymType);
	std::vector<std::string> finalRes;
	for (const std::string& i : res) {
		// if i is in selectAll
		if (std::find(selectAll.begin(), selectAll.end(), i) != selectAll.end()) {
			finalRes.push_back(i);
		}
	}
	return finalRes;
}

std::vector<std::string> ClauseResult::getEntitiesFromQps(QPSFacade& qpsf, const std::string& type) {
	if (type == "variable" || type == "variable.varName") {
		return qpsf.getVariables();
	}
	else if (type == "stmt" || type == "stmt.stmt#") {
		auto qpsResult = qpsf.getStatements();
		return ClauseResult::intArrToStringArr(qpsResult);
	}
	else if (type == "constant" || type == "constant.value") {
		return qpsf.getConstants();
	}
	else if (type == "read" || type == "read.stmt#") {
		auto qpsResult = qpsf.getReadStatements();
		return ClauseResult::intArrToStringArr(qpsResult);
	}
	else if (type == "print" || type == "print.stmt#") {
		auto qpsResult = qpsf.getPrintStatements();
		return ClauseResult::intArrToStringArr(qpsResult);
	}
	else if (type == "while" || type == "while.stmt#") {
		auto qpsResult = qpsf.getWhileStatements();
		return ClauseResult::intArrToStringArr(qpsResult);
	}
	else if (type == "if" || type == "if.stmt#") {
		auto qpsResult = qpsf.getIfStatements();
		return ClauseResult::intArrToStringArr(qpsResult);
	}
	else if (type == "assign" || type == "assign.stmt#") {
		auto qpsResult = qpsf.getAssignStatements();
		return ClauseResult::intArrToStringArr(qpsResult);
	}
	else if (type == "procedure" || type == "procedure.procName") {
		auto qpsResult = qpsf.getProcedures();
		return qpsResult;
	}
	else if (type == "call" || type == "call.stmt#") {
		auto qpsResult = qpsf.getCallStatements();
		return ClauseResult::intArrToStringArr(qpsResult);
	}
	else if (type == "call.procName") {
		std::vector<std::string> qpsResult;
		auto calls = qpsf.getCallStatements();
		auto procs = qpsf.getProcedures();
		for (auto call : calls) {
			for (auto proc : procs) {
				if (proc == qpsf.getCallee(call)) {
					qpsResult.push_back(proc);
				}
			}
		}
		return qpsResult;
	}
	else if (type == "read.varName") {
		std::vector<std::string> qpsResult;
		auto reads = qpsf.getReadStatements();
		auto vars = qpsf.getVariables();
		for (auto read : reads) {
			for (auto var : vars) {
				if (qpsf.isModifies(read, var)) {
					qpsResult.push_back(var);
				}
			}
		}
		return qpsResult;
	}
	else if (type == "print.varName") {
		std::vector<std::string> qpsResult;
		auto prints = qpsf.getPrintStatements();
		auto vars = qpsf.getVariables();
		for (auto print : prints) {
			for (auto var : vars) {
				if (qpsf.isUses(print, var)) {
					qpsResult.push_back(var);
				}
			}
		}
		return qpsResult;
	}
	throw QpsSyntaxException("Synonym type does not exist");
}

std::vector< std::string > ClauseResult::intArrToStringArr(std::vector<int>& arr) {
	std::vector < std::string > res;
	for (const auto& integer : arr) {
		res.push_back(std::to_string(integer));
	}
	return res;
}

bool ClauseResult::mapSynonyms(std::vector<std::vector<int>>& synonymIndexes, const ClauseResult* o) const {
	for (int i = 0; i < this->synonymList.size(); i++) {
		bool oHasSynonym = false;
		for (int j = 0; j < o->synonymList.size(); j++) {
			if (this->synonymList[i] != o->synonymList[j]) continue;
			synonymIndexes.push_back({ i, j });
			oHasSynonym = true;
			break;
		}

		if (!oHasSynonym) return false;
	}
	return true;
}

bool ClauseResult::mapSynonyms(std::unordered_map<std::string, int> &synonymIndexes,
                               std::unordered_set<std::string>& nonIncludedSynonyms,
                               const std::vector<std::string> &synonyms) const {
    for (const auto& synonym : synonyms) {
        bool synonymFound = false;
        for (int i = 0; i < this->synonymList.size(); i++) {
            if (synonym == this->synonymList[i]) {
                synonymIndexes.insert({ synonym, i });
                synonymFound = true;
                break;
            }
        }

        if (!synonymFound) {
            nonIncludedSynonyms.insert(synonym);
        }
    }

    return synonymIndexes.size() == synonyms.size();
}
