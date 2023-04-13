#include"QPSFacade.h"

QPSFacade::QPSFacade(std::shared_ptr <PatternManager> pm, std::shared_ptr <EntityManager> em, std::shared_ptr <RelationshipManager> rm) {
	patternManager = pm;
	entityManager = em;
	relationshipManager = rm;
}

bool QPSFacade::isFollows(int stmt1, int stmt2) {
	return relationshipManager->isFollows(stmt1, stmt2);
}

int QPSFacade::getFollowing(int s1) {
	return relationshipManager->getFollows(s1);
}

int QPSFacade::getPreceding(int s2) {
	return relationshipManager->getPreceding(s2);
}

std::vector<std::pair<int, int>> QPSFacade::getAllFollows() {
	return relationshipManager->getAllFollows();
}

bool QPSFacade::isFollowsStar(int stmt1, int stmt2) {
	return relationshipManager->isFollowsStar(stmt1, stmt2);
}

std::vector<int> QPSFacade::getFollowsStarForward(int stmt1) {
	return relationshipManager->getFollowsStarForward(stmt1);
}

std::vector<int> QPSFacade::getFollowsStarBackward(int stmt2) {
	return relationshipManager->getFollowsStarBackward(stmt2);
}

std::vector<std::pair<int, int>> QPSFacade::getAllFollowsStar() {
	return relationshipManager->getAllFollowsStar();
}

bool QPSFacade::isParent(int parent, int child) {
	return relationshipManager->isParent(parent, child);
}

int QPSFacade::getParent(int child) {
	return relationshipManager->getParent(child);
}

std::vector<int> QPSFacade::getChildren(int parent) {
	return relationshipManager->getChildren(parent);
}

std::vector<std::pair<int, int>> QPSFacade::getAllParent() {
	return relationshipManager->getAllParent();
}

bool QPSFacade::isParentStar(int parent, int child) {
	return relationshipManager->isParentStar(parent, child);
}

std::vector<int> QPSFacade::getParentStar(int child) {
	return relationshipManager->getParentStar(child);
}

std::vector<int> QPSFacade::getChildStar(int parent) {
	return relationshipManager->getChildStar(parent);
}

std::vector<std::pair<int, int>> QPSFacade::getAllParentStar() {
	return relationshipManager->getAllParentStar();
}

bool QPSFacade::isUses(int stmtNo, std::string var) {
	return relationshipManager->isUses(stmtNo, var);
}

bool QPSFacade::isUsesP(std::string proc, std::string var) {
	return relationshipManager->isUsesP(proc, var);
}

std::vector<std::string> QPSFacade::getUses(int stmtNo) {
	return relationshipManager->getUses(stmtNo);
}

std::vector<std::string> QPSFacade::getUsesPProc(std::string proc) {
	return relationshipManager->getUsesPProc(proc);
}

std::vector<int> QPSFacade::getUses(std::string var) {
	return relationshipManager->getUses(var);
}

std::vector<std::string> QPSFacade::getUsesPVar(std::string var) {
	return relationshipManager->getUsesPVar(var);
}

std::vector<std::pair<int, std::string>> QPSFacade::getUses() {
	return relationshipManager->getUses();
}

std::vector<std::pair<std::string, std::string>> QPSFacade::getUsesP() {
	return relationshipManager->getUsesP();
}

bool QPSFacade::isModifies(int stmtNo, std::string var) {
	return relationshipManager->isModifies(stmtNo, var);
}

bool QPSFacade::isModifiesP(std::string proc, std::string var) {
	return relationshipManager->isModifiesP(proc, var);
}

std::vector<std::string> QPSFacade::getModifies(int stmtNo) {
	return relationshipManager->getModifies(stmtNo);
}

std::vector<std::string> QPSFacade::getModifiesPProc(std::string proc) {
	return relationshipManager->getModifiesPProc(proc);
}

std::vector<int> QPSFacade::getModifies(std::string var) {
	return relationshipManager->getModifies(var);
}

std::vector<std::string> QPSFacade::getModifiesPVar(std::string var) {
	return relationshipManager->getModifiesPVar(var);
}

std::vector<std::pair<int, std::string>> QPSFacade::getModifies() {
	return relationshipManager->getModifies();
}

std::vector<std::pair<std::string, std::string>> QPSFacade::getModifiesP() {
	return relationshipManager->getModifiesP();
}

bool QPSFacade::isCalls(std::string proc1, std::string proc2) {
	return relationshipManager->isCalls(proc1, proc2);
}

std::vector<std::string> QPSFacade::getCaller(std::string callee) {
	return relationshipManager->getCaller(callee);
}

std::vector<std::string> QPSFacade::getCallee(std::string caller) {
	return relationshipManager->getCallee(caller);
}

std::string QPSFacade::getCallee(int stmtNo) {
	return relationshipManager->getCallee(stmtNo);
}

std::vector<std::pair<std::string, std::string>> QPSFacade::getCalls() {
	return relationshipManager->getCalls();
}

bool QPSFacade::isCallsStar(std::string proc1, std::string proc2) {
	return relationshipManager->isCallsStar(proc1, proc2);
}

std::vector<std::string> QPSFacade::getCallerStar(std::string callee) {
	return relationshipManager->getCallerStar(callee);
}

std::vector<std::string> QPSFacade::getCalleeStar(std::string caller) {
	return relationshipManager->getCalleeStar(caller);
}

std::vector<std::pair<std::string, std::string>> QPSFacade::getCallsStar() {
	return relationshipManager->getCallsStar();
}

bool QPSFacade::isNext(int s1, int s2) {
	return relationshipManager->isNext(s1, s2);
}

std::vector<int> QPSFacade::getNext(int s1) {
	return relationshipManager->getNext(s1);
}

std::vector<int> QPSFacade::getPrevious(int s2) {
	return relationshipManager->getPrevious(s2);
}

std::vector<std::pair<int, int>> QPSFacade::getAllNext() {
	return relationshipManager->getAllNext();
}

// Next*
bool QPSFacade::isNextStar(int s1, int s2) {
	return relationshipManager->isNextStar(s1, s2);
}

std::vector<int> QPSFacade::getNextStar(int s1) {
	return relationshipManager->getNextStar(s1);
}

std::vector<int> QPSFacade::getPreviousStar(int s2) {
	return relationshipManager->getPreviousStar(s2);
}

std::vector<std::pair<int, int>> QPSFacade::getAllNextStar() {
	return relationshipManager->getAllNextStar();
}

// Affects
bool QPSFacade::isAffects(int s1, int s2) {
	return relationshipManager->isAffects(s1, s2);
}

std::vector<int> QPSFacade::getAffectsAffected(int s1) {
	return relationshipManager->getAffectsAffected(s1);
}

std::vector<int> QPSFacade::getAffectsCause(int s2) {
	return relationshipManager->getAffectsCause(s2);
}

std::vector<std::pair<int, int>> QPSFacade::getAllAffects() {
	return relationshipManager->getAllAffects();
}

// Affects*

bool QPSFacade::isAffectsStar(int s1, int s2) {
	return relationshipManager->isAffectsStar(s1, s2);
}

std::vector<int> QPSFacade::getAffectsStarAffected(int s1) {
	return relationshipManager->getAffectsStarAffected(s1);
}

std::vector<int> QPSFacade::getAffectsStarCause(int s2) {
	return relationshipManager->getAffectsStarCause(s2);
}

std::vector<std::pair<int, int>> QPSFacade::getAllAffectsStar() {
	return relationshipManager->getAllAffectsStar();
}

// Entities

std::vector<int> QPSFacade::getStatements() {
	return entityManager->getStatements();
}

std::vector<int> QPSFacade::getPrintStatements() {
	return entityManager->getPrintStatements();
}

std::vector<int> QPSFacade::getAssignStatements() {
	return entityManager->getAssignStatements();
}

std::vector<int> QPSFacade::getWhileStatements() {
	return entityManager->getWhileStatements();
}

std::vector<int> QPSFacade::getIfStatements() {
	return entityManager->getIfStatements();
}

std::vector<int> QPSFacade::getCallStatements() {
	return entityManager->getCallStatements();
}

std::vector<int> QPSFacade::getReadStatements() {
	return entityManager->getReadStatements();
}

std::vector<std::string> QPSFacade::getVariables() {
	return entityManager->getVariables();
}

std::vector<std::string> QPSFacade::getConstants() {
	return entityManager->getConstants();
}

std::vector<std::string> QPSFacade::getProcedures() {
	return entityManager->getProcedures();
}

std::vector<int> QPSFacade::getStmtRefLhsVarRhsPartial(std::string lhs, std::string rhs) {
	return patternManager->getStmtRefLhsVarRhsPartial(lhs, rhs);
}

std::vector<int> QPSFacade::getStmtRefLhsVarRhsExact(std::string lhs, std::string rhs) {
	return patternManager->getStmtRefLhsVarRhsExact(lhs, rhs);
}

std::vector<int> QPSFacade::getStmtRefRhsPartial(std::string rhs) {
	return patternManager->getStmtRefRhsPartial(rhs);
}

std::vector<int> QPSFacade::getStmtRefRhsExact(std::string rhs) {
	return patternManager->getStmtRefRhsExact(rhs);
}

std::vector<int> QPSFacade::getStmtRefLhs(std::string lhs) {
	return patternManager->getStmtRefLhs(lhs);
}

std::vector<int> QPSFacade::getStmtRefBothWild() {
	return patternManager->getStmtRefBothWild();
}

std::vector<int> QPSFacade::getWhileStmtRefVar(std::string var) {
	return patternManager->getWhileStmtRefVar(var);
}

std::vector<int> QPSFacade::getWhileStmtRefWild() {
	return patternManager->getWhileStmtRefWild();
}

std::vector<int> QPSFacade::getIfStmtRefVar(std::string var) {
	return patternManager->getIfStmtRefVar(var);
}

std::vector<int> QPSFacade::getIfStmtRefWild() {
	return patternManager->getIfStmtRefWild();
}

// CFG testing
std::vector<std::pair<int, int>> QPSFacade::getStmtNoNodeTable() {
	return relationshipManager->getStmtNoNodeTable();
}
CNode QPSFacade::getNode(int stmtNo) {
	return relationshipManager->getNode(stmtNo);
}
