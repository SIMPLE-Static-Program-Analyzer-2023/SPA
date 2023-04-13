#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <utility>
#include <stack>
#include <memory>
#include <functional>
#include "pkb/CfgNode.h"

class RelationshipManager {
private:
	// Follows(s1,s2): s1=key, s2=value
	std::unordered_map<int, int> followsForwardTable;
	// Follows(s1,s2): s2=key, s1=value
	std::unordered_map<int, int> followsBackwardTable;
	// Parent(s1,s2): s1(parent)=key, s2(child)=value
	std::unordered_map<int, std::vector<int>> parentChildTable;
	// Parent(s1, s2) : s2(child)=key, s1(parent)=value
	std::unordered_map<int, int> childParentTable;

	// uses
	std::unordered_map<int, std::unordered_set<std::string>> usesStmtEntTable;
	std::unordered_map<std::string, std::unordered_set<int>> usesEntStmtTable;
	std::unordered_map <std::string, std::unordered_set<std::string>> usesProcEntTable;
	std::unordered_map <std::string, std::unordered_set<std::string>> usesEntProcTable;

	// modifies
	std::unordered_map<int, std::unordered_set<std::string>> modifiesStmtEntTable;
	std::unordered_map<std::string, std::unordered_set<int>> modifiesEntStmtTable;
	std::unordered_map <std::string, std::unordered_set<std::string>> modifiesProcEntTable;
	std::unordered_map <std::string, std::unordered_set<std::string>> modifiesEntProcTable;

	// call
	std::unordered_map<std::string, std::unordered_set<std::string>> callProcProcForwardTable;
	std::unordered_map<std::string, std::unordered_set<std::string>> callProcProcBackwardTable;
	std::unordered_map<int, std::string> callStmtProcTable;
	std::unordered_map<std::string, std::unordered_set<int>> callProcStmtTable;

	// for CFG
	std::unordered_map<std::string, std::unordered_set<int>> procStmtNosTable;
	std::unordered_map<int, std::string> stmtNoProctable;
	std::unordered_map<int, std::unordered_set<int>> ifThenStmtNosTable;
	std::unordered_map<int, std::unordered_set<int>> ifElseStmtNosTable;
	std::unordered_map<std::string, CNode> procCfgTable;
	std::unordered_map<int, CNode> stmtNoNodeTable;
	std::unordered_map<int, std::vector<int>> nextTable;
	std::unordered_map<int, std::vector<int>> previousTable;
	std::unordered_set<int> assignStmtNoSet;
	std::unordered_set<int> readStmtNoSet;

	// helper functions
	std::vector<std::string> getIndirectProcedures(std::string proc);
	std::vector<std::string> getCallerProcedures(std::string proc);
	std::vector<int> getNextHelper(int s1);
	std::vector<int> getPreviousHelper(int s2);
	std::vector<int> getChain(std::unordered_map<int, std::vector<int>> table, int root);
	std::vector<int> getAffectsStarOneSynHelper(int root, bool isForward);

public:
	void addFollows(int stmt1, int stmt2);
	void addParentChild(int parent, int child);
	void addStmtUses(int stmtNo, std::string var);
	void addProcUses(std::string procName, std::string var);
	void addStmtModifies(int stmtNo, std::string var);
	void addProcModifies(std::string procName, std::string var);
	void addCalls(std::string proc1, std::string proc2);
	void addCallStmt(int stmtNo, std::string calledProc);
	bool isFollows(int stmt1, int stmt2);
	int getFollows(int stmt1);
	int getPreceding(int stmt2);
	std::vector<std::pair<int, int>> getAllFollows();

	bool isFollowsStar(int stmt1, int stmt2);

	/**
	 * Gets a vector of all stmtRef that follows the argument stmtRef in a Follows* r/s.
	 *
	 * @param the int stmtRef of the line that precedes the stmtRef(s) in the returned vector in a Follows* r/s.
	 * @return vector<int> of all stmtRefs that follows the param in a Follows* r/s.
	 */
	std::vector<int> getFollowsStarForward(int stmt1);

	/**
	 * Gets a vector of all stmtRef that precedes the argument stmtRef in a Follows* r/s.
	 * 
	 * @param the int stmtRef of the line that follows the stmtRef(s) in the returned vector in a Follows* r/s.
	 * @return vector<int> of all stmtRefs that precedes the param in a Follows* r/s.
	 */
	std::vector<int> getFollowsStarBackward(int stmt2);
	std::vector<std::pair<int, int>> getAllFollowsStar();

	/**
	 * checks if Parent(s1, s2) r/s is valid.
	 * 
	 * @param int stmtRef of parent.
	 * @param int stmtRef of child.
	 * @return boolean value of whether Parent r/s is valid.
	 */
	bool isParent(int parent, int child);
 	std::vector<int> getChildren(int parent);
	int getParent(int child);
	std::vector<std::pair<int, int>>  getAllParent();

	bool isParentStar(int parent, int child);

	/**
	 * Gets a vector of all parent, grandparent, xgreatgrandparent stmtRef of input stmtRef in a Parent* r/s.
	 * 
	 * @param stmtRef of child whose ancestors are to be found.
	 * @return vector<int> of stmtRefs of param's ancestors.
	 */
	std::vector<int> getParentStar(int child);

	/**
	 * Gets a vector of all descendent's stmtRef of input stmtRef in a Parent* r/s.
	 * 
	 * @param stmtRef of parent.
	 * @return vector<int> of stmtRefs of all the inputs descendents.
	 */
	std::vector<int> getChildStar(int parent);
	std::vector<std::pair<int, int>> getAllParentStar();

	// uses
	bool isUses(int stmtNo, std::string var);
	bool isUsesP(std::string proc, std::string var);
	std::vector<std::string> getUses(int stmtNo);
	std::vector<std::string> getUsesPProc(std::string proc);
	std::vector<int> getUses(std::string var);
	std::vector<std::string> getUsesPVar(std::string var);
	std::vector<std::pair<int, std::string>> getUses();
	std::vector<std::pair<std::string, std::string>> getUsesP();

	// modifies
	bool isModifies(int stmtNo, std::string var);
	bool isModifiesP(std::string proc, std::string var);
	std::vector<std::string> getModifies(int stmtNo);
	std::vector<std::string> getModifiesPProc(std::string proc);
	std::vector<int> getModifies(std::string var);
	std::vector<std::string> getModifiesPVar(std::string var);
	std::vector<std::pair<int, std::string>> getModifies();
	std::vector<std::pair<std::string, std::string>> getModifiesP();

	// calls
	bool isCalls(std::string proc1, std::string proc2);
	std::vector<std::string> getCaller(std::string callee);
	std::vector<std::string> getCallee(std::string caller);
	std::string getCallee(int stmtNo);
	std::vector<std::pair<std::string, std::string>> getCalls();
	bool isCallsStar(std::string proc1, std::string proc2);
	std::vector<std::string> getCallerStar(std::string callee);
	std::vector<std::string> getCalleeStar(std::string caller);
	std::vector<std::pair<std::string, std::string>> getCallsStar();

	// CFG
	void addProcChild(std::string procName, int stmtNo);
	std::vector<int> getChildOfProc(std::string procName);
	void addIfThen(int If, int stmtNo);
	void addIfElse(int If, int stmtNo);
	void addAssignStmt(int stmtNo);
	void addReadStmt(int stmtNo);
	std::vector<int> getThen(int If);
	std::vector<int> getElse(int If);
	void addProcCfg(std::string procName, CNode root);
	void addStmtNoNodeTable(std::unordered_map<int, CNode> table);
	void populateNextTable();
	void populatePreviousTable();

	// CFG testing
	std::vector<std::pair<int, int>> getStmtNoNodeTable();
	CNode getNode(int stmtNo);

	// Next
	bool isNext(int s1, int s2);
	std::vector<int> getNext(int s1);
	std::vector<int> getPrevious(int s2);
	std::vector<std::pair<int, int>> getAllNext();

	// Next*
	bool isNextStar(int s1, int s2);
	std::vector<int> getNextStar(int s1);
	std::vector<int> getPreviousStar(int s2);
	std::vector<std::pair<int, int>> getAllNextStar();

	// Affects
	bool isAffects(int s1, int s2);
	std::vector<int> getAffectsAffected(int s1);
	std::vector<int> getAffectsCause(int s2);
	std::vector<std::pair<int, int>> getAllAffects();

	// Affects*
	bool isAffectsStar(int s1, int s2);
	std::vector<int> getAffectsStarAffected(int s1);
	std::vector<int> getAffectsStarCause(int s2);
	std::vector<std::pair<int, int>> getAllAffectsStar();
};
