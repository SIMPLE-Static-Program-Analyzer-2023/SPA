#pragma once

#include<stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "pkb/StatementType.h"

class EntityManager {
public:
	EntityManager();
	// Adds a statement of type st to the knowledge base.
	// If the statement has already been added, throws an invalid argument error
	void addStatement(int stmt, StatementType st);

	// Adds a variable to the knowledge base.
	// If the statement has already been added, throws an invalid argument error
	void addVariable(std::string varName);

	void addConstant(std::string value);

	void addProcedure(std::string procName);

	std::vector<int> getStatements();
	std::vector<int> getPrintStatements();
	std::vector<int> getAssignStatements();
	std::vector<int> getWhileStatements();
	std::vector<int> getIfStatements();
	std::vector<int> getCallStatements();
	std::vector<int> getReadStatements();

	std::vector<std::string> getVariables();

	std::vector < std::string > getConstants();

	std::vector<std::string> getProcedures();

	// Get the statement type of a statement
	StatementType getStatementType(int stmtNum);
	// Checks if a certain statement is of a certain StatementType
	bool isStatementOfType(int stmtNum, StatementType st);

private:
	std::unordered_map < int, StatementType > statementTable;

	std::unordered_set < std::string > procedureTable;
	std::unordered_set < std::string > constantTable;
	std::unordered_set < std::string > variableTable;

	std::unordered_set < int > statementReadTable;
	std::unordered_set < int > statementPrintTable;
	std::unordered_set < int > statementAssignTable;
	std::unordered_set < int > statementCallTable;
	std::unordered_set < int > statementWhileTable;
	std::unordered_set < int > statementIfTable;

	// Get the correct editable statement type table from the enum class
	std::unordered_set < int > &getStatementTypeTable(StatementType st);

	// Retrieve list of keys of type T from a given unordered map or set
	template < typename T, typename U >
	std::vector< T > getTableKeys(const std::unordered_map<T, U>& table);
	template < typename T >
	std::vector< T > getTableKeys(const std::unordered_set<T>& table);

	// Checks if an unordered map or set contains a given key of type T
	template < typename T, typename U >
	bool tableContains(T t, const std::unordered_map<T, U> &table);
	template < typename T >
	bool tableContains(T t, const std::unordered_set<T> &table);
};
