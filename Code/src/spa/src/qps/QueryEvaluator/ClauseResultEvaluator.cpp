#include "ClauseResultEvaluator.h"

ClauseResult ClauseResultEvaluator::evaluate(ClauseResult& c1, ClauseResult& c2) {
	if (c1.isBoolean())			return ClauseResultEvaluator::evaluateBool(c1, c2);			// Boolean-Boolean and Boolean-Table pairs
	else if (c2.isBoolean())	return ClauseResultEvaluator::evaluateBool(c2, c1);			// Table-Boolean pairs
	else						return ClauseResultEvaluator::evaluateTableTable(c1, c2);    // Table-Table pairs
}

ClauseResult ClauseResultEvaluator::evaluateBool(ClauseResult& boolClause, ClauseResult& c2) {
	return boolClause.status == StatusType::ValidBooleanResult ? c2 : boolClause;
}

ClauseResult ClauseResultEvaluator::evaluateTableTable(ClauseResult& c1, ClauseResult& c2) {
    // Find for all common synonyms and note their indexes in both clauses
    std::unordered_map < std::string, std::vector<int> > commonSynonyms;
    findCommonSynonyms(commonSynonyms, c1.synonymList, c2.synonymList);
	if (commonSynonyms.empty()) return ClauseResultEvaluator::crossProduct(c1, c2);
    return ClauseResultEvaluator::innerJoin(c1, c2, commonSynonyms);
}

void ClauseResultEvaluator::findCommonSynonyms(std::unordered_map<std::string, std::vector<int>> &commonSynonyms,
                                               std::vector<std::string> synonyms1,
                                               std::vector<std::string> synonyms2) {
    for (int i = 0; i < synonyms1.size(); i++) {
        std::string curr_i = synonyms1[i];

        for (int j = 0; j < synonyms2.size(); j++) {
            std::string curr_j = synonyms2[j];
            if (curr_i != curr_j) continue;

            std::vector<int> arr;
            arr.push_back(i);
            arr.push_back(j);
            commonSynonyms.insert(std::make_pair(curr_i, arr));
        }
    }
}

ClauseResult ClauseResultEvaluator::innerJoin(ClauseResult &c1, ClauseResult &c2,
                                                      std::unordered_map<std::string, std::vector<int>> commonSynonyms) {
    std::vector<std::string> synonyms1 = c1.synonymList;
    std::vector<std::string> synonyms2 = c2.synonymList;
    std::vector<std::vector<std::string>> table1 = c1.table;
    std::vector<std::vector<std::string>> table2 = c2.table;

    // New size of table n = total_synonyms in both tables - common_synonyms
    int newSize = synonyms1.size() + synonyms2.size() - commonSynonyms.size();

    // Get synonymList
    // Find list of non-common row indexes to use for appending the elements
    std::vector<std::string> newSynonymList;
    std::vector<int> nonCommonCols1;
    std::vector<int> nonCommonCols2;

    for (auto i = 0; i < synonyms1.size(); i++) {
        std::string synonym = synonyms1[i];
        if (commonSynonyms.find(synonym) == commonSynonyms.end()) {
            newSynonymList.push_back(synonym);
            nonCommonCols1.push_back(i);
        }
    }
    for (auto i = 0; i < synonyms2.size(); i++) {
        std::string synonym = synonyms2[i];
        if (commonSynonyms.find(synonym) == commonSynonyms.end()) {
            newSynonymList.push_back(synonym);
            nonCommonCols2.push_back(i);
        }
    }
    for (const auto& synonym : commonSynonyms) newSynonymList.push_back(synonym.first);

    // For each row, find if both synonyms contain the common two
    // If yes, Initialise vector tuple,
    // append all unique elements in that row to the vector
    // and append to vector result, following synonym order above
    std::vector<std::vector<std::string>> newTable;

    for (auto row1 : table1) {
        for (auto row2 : table2) {
            bool addRow = true;
            for (const auto& commonSynonym : commonSynonyms) {
                std::vector<int> indexes = commonSynonym.second;
                addRow = addRow && (row1[indexes[0]] == row2[indexes[1]]);
            }
            if (!addRow) continue;

            std::vector<std::string> newRow;
            for (auto index : nonCommonCols1) newRow.push_back(row1[index]);
            for (auto index : nonCommonCols2) newRow.push_back(row2[index]);
            for (auto commonSynonym : commonSynonyms) newRow.push_back(row1[commonSynonym.second[0]]);
            newTable.push_back(newRow);
        }
    }

    // Init new clause result with the vector result
    return newTable.empty()
           ? ClauseResultBuilder::createBooleanResult(false)
           : ClauseResultBuilder::createTableResult(newSynonymList, newTable, newSize);
}

ClauseResult ClauseResultEvaluator::crossProduct(ClauseResult &c1, ClauseResult &c2) {
    // Cross product of c1 and c2, assuming no common synonyms

    std::vector<std::string> synonyms1 = c1.synonymList;
    std::vector<std::string> synonyms2 = c2.synonymList;
    std::vector<std::vector<std::string>> table1 = c1.table;
    std::vector<std::vector<std::string>> table2 = c2.table;

    // New size of table n = total_synonyms in both tables
    int newSize = synonyms1.size() + synonyms2.size();

    // Get synonymList
    std::vector<std::string> newSynonymList;
    for (const auto& synonym : synonyms1) newSynonymList.push_back(synonym);
    for (const auto& synonym : synonyms2) newSynonymList.push_back(synonym);

    std::vector<std::vector<std::string>> newTable;

    for (const auto& row1 : table1) {
        for (const auto& row2 : table2) {
            std::vector<std::string> newRow;
            for (const auto& synonym : row1) newRow.push_back(synonym);
            for (const auto& synonym : row2) newRow.push_back(synonym);
            newTable.push_back(newRow);
        }
    }

    return table1.empty()
           ? ClauseResultBuilder::createBooleanResult(false)
           : ClauseResultBuilder::createTableResult(newSynonymList, newTable, newSize);
}
