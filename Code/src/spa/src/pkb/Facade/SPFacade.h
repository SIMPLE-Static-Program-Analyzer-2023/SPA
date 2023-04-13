#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "pkb/Manager/EntityManager.h"
#include "pkb/Manager/RelationshipManager.h"
#include "pkb/Manager/PatternManager.h"
#include "pkb/StatementType.h"

class SPFacade {
public:
	SPFacade(std::shared_ptr <PatternManager> pm, std::shared_ptr <EntityManager> em, std::shared_ptr <RelationshipManager> rm);

	// Relationships

	void addFollows(int stmt1, int stmt2);
	void addParentChild(int parent, int child);
	void addStmtUses(int stmtNo, std::string var);
	void addProcUses(std::string procName, std::string var);
	void addStmtModifies(int stmtNo, std::string var);
	void addProcModifies(std::string procName, std::string var);
	void addCalls(std::string proc1, std::string proc2);
	void addCallStmt(int stmtNo, std::string calledProc);

	// Entities

	void addProcedure(std::string procName);
	void addStatement(int stmtNo, StatementType st);
	void addVariable(std::string var);
	void addConstant(std::string const);

	// Patterns

	void addPattern(int stmtRef, AssignNode* assign);
	void addWhilePattern(int stmtRef, CondExpr condExpr);
	void addIfPattern(int stmtRef, CondExpr condExprNode);

	// CFG

	void addProcChild(std::string procName, int stmtNo);
	void addIfThen(int If, int stmtNo);
	void addIfElse(int If, int stmtNo);


private:
	std::shared_ptr <EntityManager> entityManager;
	std::shared_ptr <RelationshipManager> relationshipManager;
	std::shared_ptr <PatternManager> patternManager;
};
