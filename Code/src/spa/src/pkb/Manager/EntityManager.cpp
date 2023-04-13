#include "EntityManager.h"
#include "pkb/StatementType.h"

EntityManager::EntityManager()
	:  variableTable(),
	statementTable(),
	statementReadTable(),
	statementPrintTable(),
	statementAssignTable(),
	statementCallTable(),
	statementWhileTable(),
	statementIfTable()
{}

void EntityManager::addStatement(int stmtNum, StatementType st) {
	// Disallow adding an already-added statement
	// if (tableContains(stmtNum, statementTable)) {
	//	 throw std::invalid_argument("Statement has already been added for statement number " + stmtNum);
	// }

	statementTable.insert({ stmtNum, st });
	getStatementTypeTable(st).insert(stmtNum);
}

void EntityManager::addVariable(std::string varName) {
	// Disallow adding an already-added variable
	// if (tableContains(varName, variableTable)) {
	//   throw std::invalid_argument("Variable has already been added for variable " + varName);
	// }

	variableTable.insert(varName);
}

void EntityManager::addConstant(std::string value) {
	// There can be multiple instances of the same variable,
	// we allow adding, but there is only one instance in the set
	constantTable.insert(value);
}

void EntityManager::addProcedure(std::string procName) {
	procedureTable.insert(procName);
}

StatementType EntityManager::getStatementType(int stmtNum) {
	return statementTable.at(stmtNum);
}

bool EntityManager::isStatementOfType(int stmtNum, StatementType st) {
	std::unordered_set< int > statementTypeTable = getStatementTypeTable(st);
	return tableContains(stmtNum, statementTypeTable);
}

std::vector<int> EntityManager::getStatements() {
	return getTableKeys(statementTable);
}

std::vector<int> EntityManager::getPrintStatements() {
	return getTableKeys(statementPrintTable);
}

std::vector<int> EntityManager::getAssignStatements() {
	return getTableKeys(statementAssignTable);
}

std::vector<int> EntityManager::getWhileStatements() {
	return getTableKeys(statementWhileTable);
}

std::vector<int> EntityManager::getIfStatements() {
	return getTableKeys(statementIfTable);
}

std::vector<int> EntityManager::getCallStatements() {
	return getTableKeys(statementCallTable);
}

std::vector<int> EntityManager::getReadStatements() {
	return getTableKeys(statementReadTable);
}

std::vector<std::string> EntityManager::getVariables() {
	return getTableKeys(variableTable);
}

std::vector<std::string> EntityManager::getConstants() {
	return getTableKeys(constantTable);
}

std::vector<std::string> EntityManager::getProcedures() {
	return getTableKeys(procedureTable);
}

std::unordered_set < int > &EntityManager::getStatementTypeTable(StatementType st) {
	switch (st) {
	case StatementType::StatementRead:		return statementReadTable;
	case StatementType::StatementPrint:		return statementPrintTable;
	case StatementType::StatementAssign:	return statementAssignTable;
	case StatementType::StatementCall:		return statementCallTable;
	case StatementType::StatementWhile:		return statementWhileTable;
	case StatementType::StatementIf:		return statementIfTable;
	default:
		throw std::invalid_argument("Invalid Statement Type");
	}
}

template < typename T, typename U >
std::vector< T > EntityManager::getTableKeys(const std::unordered_map<T, U>& table) {
	std::vector< T > res;
	for (auto const keyElementPair : table) {
		res.push_back(keyElementPair.first);
	}
	return res;
}

template < typename T >
std::vector< T > EntityManager::getTableKeys(const std::unordered_set<T>& table) {
	std::vector < T > res;
	for (auto element : table) {
		res.push_back(element);
	}
	return res;
}

template < typename T, typename U >
bool EntityManager::tableContains(T t, const std::unordered_map<T, U> &table) {
	return table.find(t) != table.end();
}

template < typename T >
bool EntityManager::tableContains(T t, const std::unordered_set<T> &table) {
	return table.find(t) != table.end();
}
