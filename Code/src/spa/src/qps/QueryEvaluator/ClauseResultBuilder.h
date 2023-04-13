#pragma once

#include <string>
#include <vector>
#include "qps/QpsException.h"
#include "ClauseResult.h"

// Forward declarations
class ClauseResult;
class ClauseResultEvaluator;

class ClauseResultBuilder {
public:
	static ClauseResult createBooleanResult(bool status);
	static ClauseResult createListResult(const std::string& synonym, const std::vector<std::string>& values);

	// Creates a tuple clause result. values is a list of tuples, where tupleList[x] is the (x+1)th tuple of size 2.
	// The synonym list must be of size 2.
	// Throws an error is any tuple or the synonymList is not of size 2.
	static ClauseResult createTupleResult(
		std::vector<std::string> &synonymList,
		std::vector < std::vector<std::string> > &tupleList
	);

	// Creates an n-tuple clause result. values is a list of tuples, where tupleList[x] is the (x+1)th row of size n.
	// The synonym list must be of size n.
	// Throws an error if any tuple or the synonymList is not of size n.
	static ClauseResult createTableResult(
		std::vector<std::string>& synonymList,
		std::vector < std::vector<std::string> >& tupleList,
		int n
	);

private:
	// Checks if the size of the synonym list and the tuples are of size n and are not empty strings. Throws an error otherwise.
	static void validateTables(
		std::vector<std::string> &synonymList,
		std::vector < std::vector<std::string> > &tupleList,
		int n
	);

	friend ClauseResult;
	friend ClauseResultEvaluator;
};
