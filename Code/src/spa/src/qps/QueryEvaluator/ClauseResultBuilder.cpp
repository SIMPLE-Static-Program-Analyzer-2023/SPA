#include <iostream>

#include "ClauseResultBuilder.h"

ClauseResult ClauseResultBuilder::createBooleanResult(bool status) {
	return ClauseResult(status);
}

ClauseResult ClauseResultBuilder::createListResult(const std::string& synonym, const std::vector<std::string>& values) {
	// Validation
	if (synonym == "") throw QpsSyntaxException("Synonym cannot be empty string");
	for (const auto& value : values) {
		if (value == "") throw QpsSyntaxException("Value cannot be empty string");
	}

	if (values.size() == 0) return ClauseResultBuilder::createBooleanResult(false);

	std::vector<std::string> synonymList;
	std::vector < std::vector<std::string> > table;

	synonymList.push_back(synonym);
	for (const auto& value : values) {
		std::vector<std::string> row;
		row.push_back(value);
		table.push_back(row);
	}

	return ClauseResult(synonymList, table);
}

ClauseResult ClauseResultBuilder::createTupleResult(std::vector<std::string> &synonymList, std::vector < std::vector<std::string> > &tupleList) {
	// Validation
	ClauseResultBuilder::validateTables(synonymList, tupleList, 2);

	if (tupleList.size() == 0) return ClauseResultBuilder::createBooleanResult(false);

	return ClauseResult(synonymList, tupleList);
}

ClauseResult ClauseResultBuilder::createTableResult(std::vector<std::string> &synonymList, std::vector < std::vector<std::string> > &tupleList, int n) {
	// Validation
	ClauseResultBuilder::validateTables(synonymList, tupleList, n);

	if (n == 0) return ClauseResultBuilder::createBooleanResult(false);
	if (tupleList.size() == 0) return ClauseResultBuilder::createBooleanResult(false);

	return ClauseResult(synonymList, tupleList);
}

void ClauseResultBuilder::validateTables(std::vector<std::string> &synonymList, std::vector < std::vector<std::string> >& tupleList, int n) {
	if (synonymList.size() != n) throw QpsSyntaxException("A tuple clause result must have " + std::to_string(n) + " synonyms");
	for (const auto& synonym : synonymList) if (synonym == "") throw QpsSyntaxException("Synonym cannot be empty string");
	for (const auto& tuple : tupleList) {
		if (tuple.size() != n) throw QpsSyntaxException("Tuple size in a tuple clause result must be of size " + std::to_string(n));
		for (const auto& element : tuple) {
			if (element == "") throw QpsSyntaxException("Value cannot be empty string");
		}
	}
}
