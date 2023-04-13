#include <iostream>
#include <string>
#include <utility>

#include "SPFacade.h"

SPFacade::SPFacade(std::shared_ptr <PatternManager> pm, std::shared_ptr <EntityManager> em, std::shared_ptr <RelationshipManager> rm) :
	entityManager(std::move(em)),
	relationshipManager(std::move(rm)),
	patternManager(std::move(pm))
{}

void SPFacade::addFollows(int stmt1, int stmt2) {
	relationshipManager->addFollows(stmt1, stmt2);
}

void SPFacade::addParentChild(int parent, int child) {
	relationshipManager->addParentChild(parent, child);
}

void SPFacade::addStmtUses(int stmtNo, std::string var) {
	relationshipManager->addStmtUses(stmtNo, var);
}

void SPFacade::addProcUses(std::string procName, std::string var) {
	relationshipManager->addProcUses(procName, var);
}

void SPFacade::addStmtModifies(int stmtNo, std::string var) {
	relationshipManager->addStmtModifies(stmtNo, var);
}

void SPFacade::addProcModifies(std::string procName, std::string var) {
	relationshipManager->addProcModifies(procName, var);
}

void SPFacade::addCalls(std::string proc1, std::string proc2) {
	relationshipManager->addCalls(proc1, proc2);
}

void SPFacade::addCallStmt(int stmtNo, std::string calledProc) {
	relationshipManager->addCallStmt(stmtNo, calledProc);
}

void SPFacade::addProcedure(std::string procName) {
	entityManager->addProcedure(procName);
}

void SPFacade::addStatement(int stmt, StatementType st) {
	entityManager->addStatement(stmt, st);
	if (st == StatementType::StatementAssign) {
		relationshipManager->addAssignStmt(stmt);
	}
	if (st == StatementType::StatementRead) {
		relationshipManager->addReadStmt(stmt);
	}
}

void SPFacade::addVariable(std::string var) {
	entityManager->addVariable(var);
}

void SPFacade::addConstant(std::string value) {
	entityManager->addConstant(value);
}

void SPFacade::addPattern(int stmtRef, AssignNode* assign) {
	patternManager->addPattern(stmtRef, std::make_shared<AssignNode>(*assign));
}

void SPFacade::addWhilePattern(int stmtRef, CondExpr condExpr) {
	patternManager->addWhilePattern(stmtRef, condExpr);
}

void SPFacade::addIfPattern(int stmtRef, CondExpr condExpr) {
	patternManager->addIfPattern(stmtRef, condExpr);
}

void SPFacade::addProcChild(std::string procName, int stmtNo) {
	relationshipManager->addProcChild(procName, stmtNo);
}

void SPFacade::addIfThen(int If, int stmtNo) {
	relationshipManager->addIfThen(If, stmtNo);
}

void SPFacade::addIfElse(int If, int stmtNo) {
	relationshipManager->addIfElse(If, stmtNo);
}

