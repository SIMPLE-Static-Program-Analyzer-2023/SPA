#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "pkb/Manager/PatternManager.h"
#include "pkb/Manager/EntityManager.h"
#include "pkb/Manager/RelationshipManager.h"

class QPSFacade {
private:
	std::shared_ptr<PatternManager> patternManager;
	std::shared_ptr <EntityManager> entityManager;
	std::shared_ptr <RelationshipManager> relationshipManager;

public:
	QPSFacade(std::shared_ptr<PatternManager> pm, std::shared_ptr <EntityManager> em, std::shared_ptr <RelationshipManager> rm);

	// Relationships

	/**
	 * Checks if Follows(s1,s2) is valid(0 synonym).
	 * 
	 * @param int s1 of Follows(s1,s2) r/s.
	 * @param int s2 of Follows(s1,s2) r/s.
	 * @return boolean value of whether Follows(s1,s2) r/s is valid.
	 */
	bool isFollows(int stmt1, int stmt2);
	/**
	 * Get s2 in a Follows(s1,s2) r/s with s1 as param.
	 * 
	 * @param int s1 stmtRef in a Follows(s1,s2) r/s.
	 * @return int s2 stmtRef in a Follows(s1,s2) r/s.
	 */
	int getFollowing(int s1);

	/**
	 * Get s1 in a Follows(s1,s2) r/s with s2 as param.
	 *
	 * @param int s2 stmtRef in a Follows(s1,s2) r/s.
	 * @return int s1 stmtRef in a Follows(s1,s2) r/s.
	 */
	int getPreceding(int s2);

	/**
	 * Gets all pair of (s1,s2) in a Follows(s1,s2) r/s.
	 * 
	 * @return a vector of pair of (s1,s2) in a Follows(s1,s2) r/s.
	 */
	std::vector<std::pair<int, int>> getAllFollows();

	/**
	 * Checks that Follows*(s1,s2) r/s is valid.
	 * 
	 * @param int stmtRef of s1 in Follows*(s1,s2).
	 * @param int stmtRef of s2 in Follows*(s1,s2).
	 * @return boolean value of whether Follows*(s1,s2) is valid.
	 */
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
	 * @return vector<int> of all stmtRefs that precedes the param in a Follows* r/s, stmtRef order will be backwards.
	 */
	std::vector<int> getFollowsStarBackward(int stmt2);

	/**
	 * Gets a vector of pairs<int,int> of stmtRef and stmtRef that satisfies a Follow*(s1,s2) r/s.
	 * 
	 * @return a vector of pairs<int,int> of stmtRef and stmtRef that satisfies a Follow*(s1,s2) r/s.
	 */
	std::vector<std::pair<int, int>> getAllFollowsStar();

	/**
	 * Checks if Parent(s1, s2) r/s is valid.
	 *
	 * @param int stmtRef of parent.
	 * @param int stmtRef of child.
	 * @return boolean value of whether Parent r/s is valid.
	 */
	bool isParent(int parent, int child);

	/**
	 * Gets the stmtRef of the parent of the given child's stmtRef in a Parent r/s.
	 * 
	 * @param int stmtRef of the child.
	 * @return int stmtRef of the parent of the given child.
	 */
	int getParent(int child);

	/**
	 * Gets all pair of (s1,s2) in Parent(s1,s2) r/s.
	 * 
	 * @retuns a vector of pair of (s1,s2) in Parent(s1,s2) r/s.
	 */
	std::vector<std::pair<int, int>> getAllParent();
	/**
	 * Checks if Parent*(s1,s2) is valid.
	 * 
	 * @param int stmtRef of parent(can be direct parent or an ancestor).
	 * @param int stmtRef of child(can be direct child or descendent).
	 * @return boolean value of whether the Parent* r/s is valid.
	 */
	bool isParentStar(int parent, int child);
	/**
	 * Gets a vector<int> of the children stmtRef of the given stmtRef in a Parent r/s.
	 * 
	 * @param int stmtRef of the parent.
	 * @return vector<int> of stmtRef of the children of the given parent.
	 */
	std::vector<int> getChildren(int parent);

	/**
	 * Gets a vector of all parent, grandparent, xgreatgrandparent stmtRef of input stmtRef in a Parent* r/s.
	 *
	 * @param int stmtRef of child whose ancestors are to be found.
	 * @return vector<int> of stmtRefs of param's ancestors.
	 */
	std::vector<int> getParentStar(int child);

	/**
	 * Gets a vector of all descendent's stmtRef of input stmtRef in a Parent* r/s.
	 * In order of nesting level.
	 *
	 * @param int stmtRef of parent.
	 * @return vector<int> of stmtRefs of all the inputs descendents.
	 */
	std::vector<int> getChildStar(int parent);

	/**
	 * Gets all pair of stmtRefs that satisfifes the Parent*(s1,s2) r/s.
	 * 
	 * @return a vector of pair<int,int> of stmtRefs that satisfifes the Parent*(s1,s2) r/s.
	 */
	std::vector<std::pair<int, int>> getAllParentStar();

	/**
	 * Checks if Uses(s1, v1) r/s is valid.
	 * 
	 * @param int stmtRef.
	 * @param string entRef, variable name.
	 * @return boolean value of whether the Uses r/s is valid.
	 */
	bool isUses(int stmtNo, std::string var);

	/**
	 * Checks if Uses(proc1, v1) r/s is valid. 
	 * 
	 * @param string procedure name.
	 * @param string entref, variable name.
	 * @return boolean value of whether the Uses r/s is valid.
	 */
	bool isUsesP(std::string proc, std::string var);

	/**
	 * Gets a vector<string> of entRef given a stmtRef.
	 * 
	 * @param stmtRef.
	 * @return vector<string> of entRef in the given stmtRef which follows the Uses r/s.
	 */
	std::vector<std::string> getUses(int stmtNo);

	/**
	 * Gets a vector<string> of all entRef used directly/indirectly in a procedure.
	 *
	 * @param string procedure name.
	 * @return a vector<string> of all entRef used directly/indirectly in a procedure.
	 */
	std::vector<std::string> getUsesPProc(std::string proc);

	/**
	 * Gets a vector<int> of stmtRefs that uses the given entRef.
	 * 
	 * @param entRef.
	 * @return vector<int> of stmtRefs that uses the given entRef.
	 */
	std::vector<int> getUses(std::string var);

	/**
	 * Gets a vector<string> of procedure names where the given variable is used.
	 * 
	 * @param string variable name.
	 * @return vector<string> of procedure names where the given variable is used.
	 */
	std::vector<std::string> getUsesPVar(std::string var);

	/**
	 * Gets all pair of stmtRef and entRef that satisfies the Use r/s.
	 * 
	 * @return a vector of pair of stmtRef and entRef that satisfies the Use r/s.
	 */
	std::vector<std::pair<int, std::string>> getUses();

	/**
	 * Gets all pair of procedure name and entRef(variable) that satisfies the Use r/s.
	 * 
	 * @return a vector of pair of procedure name and entRef(variable) that satisfies the Use r/s.
	 */
	std::vector<std::pair<std::string, std::string>> getUsesP();

	/**
	 * Checks if Modifies(s1,v1) r/s is valid. 
	 * 
	 * @param int stmtRef.
	 * @param string entRef(variable name).
	 * @return boolean value of whether the Modifies r/s is valid.
	 */
	bool isModifies(int stmtNo, std::string var);

	/**
	 * Checks if Modifies(proc,v1) r/s is valid. 
	 * 
	 * @param string procedure name.
	 * @param string entRef(variable name).
	 * @return boolean value of whether the Modifies r/s is valid.
	 */
	bool isModifiesP(std::string proc, std::string var);

	/**
	 * Gets a vector<string> of the entRef(variable) being Modified in the given stmtRef.
	 * 
	 * @param stmtRef.
	 * @return string of the entRef being Modified in the given stmtRef.
	 */
	std::vector<std::string> getModifies(int stmtNo);

	/**
	 * Gets a vector<string> of the entRef(variable) being Modified in the given procedure.
	 *
	 * @param string procedure name.
	 * @return string name of the entRef(variable) being Modified in the given procedure.
	 */
	std::vector<std::string> getModifiesPProc(std::string proc);

	/**
	 * Gets a vector<int> of all stmtRefs that modifies the given entRef.
	 * 
	 * @param entRef.
	 * @return vector<int> of all stmtRefs that modifies the given entRef.
	 */
	std::vector<int> getModifies(std::string var);

	/**
	 * Gets a vector<string> of all procedure name that modifies the given entRef(variable).
	 *
	 * @param entRef(variable).
	 * @return vector<string> of all procedure name that modifies the given entRef(variable).
	 */
	std::vector<std::string> getModifiesPVar(std::string var);

	/**
	 * Gets all pairs(stmtRef, entRef(variable)) that satisfies Modifies r/s.
	 * 
	 * @return a vector of pair(int, string) of stmtRef and entRef that satisfies Modifies r/s.
	 */
	std::vector<std::pair<int, std::string>> getModifies();

	/**
	 * Gets all pairs(procedureName, entRef(variable)) that satisfies Modifies r/s.
	 *
	 * @return a vector of pair(string, string) of procedure name and entRef(variable) that satisfies Modifies r/s.
	 */
	std::vector<std::pair<std::string, std::string>> getModifiesP();

	/**
	 * Checks if the first procedure calls the second procedure according to Calls(proc1, proc2) r/s.
	 * 
	 * @param string procedure name of the first procedure.
	 * @param string procedure name of the second procedure.
	 * @return boolean result of whether Calls(proc1, proc2) r/s holds.
	 */
	bool isCalls(std::string proc1, std::string proc2);

	/**
	 * Gets a vector<string> of procedures that calls the callee procedure.
	 * 
	 * @param string procedure name of the callee.
	 * @return vector<string> of procedure names that called the callee.
	 */
	std::vector<std::string> getCaller(std::string callee);

	/**
	 * Gets a vector<string> of procedures that are called by the caller procedure.
	 * 
	 * @param string procedure name of the caller.
	 * @return vector<string> of procedure name that were called by the caller procedure.
	 */
	std::vector<std::string> getCallee(std::string caller);

	std::string getCallee(int stmtNo);
	/**
	 * Gets a vector of all pairs of procedure name in the Calls(proc1,proc2) r/s. 
	 * 
	 * @return vector<pair<string,string>> vector of all pairs of procedure name where Calls(proc1,proc2) r/s holds.
	 */
	std::vector<std::pair<std::string, std::string>> getCalls();

	/**
	 * Checks if the first procedure indirectly called the second procedure according to Call*(proc1,proc2) r/s.
	 * 
	 * @param string procedure name of the caller.
	 * @param string procedure name of the callee.
	 * @return boolean value of whether the Calls*(proc1,proc2) r/s holds.
	 */
	bool isCallsStar(std::string proc1, std::string proc2);

	/**
	 * Gets names of procedures that directly/indirectly calls the callee procedure.
	 * 
	 * @param string the callee procedure name.
	 * @return vector<string> name of procedures that directly/indirectly calls the callee.
	 */
	std::vector<std::string> getCallerStar(std::string callee);

	/**
	 * Gets names of procedures that were directly/indirectly called by the caller procedure.
	 * 
	 * @param string the caller procedure name.
	 * @return vector<string> name of procedures that are directly/indirectly called by the caller.
	 */
	std::vector<std::string> getCalleeStar(std::string caller);

	/**
	 * Gets all pairs of name of procedures that satisfies the Calls*(proc1,proc2) r/s.
	 * 
	 * @return vector of pair(proc1,proc2) where Calls*(proc1,proc2) r/s is satisfied.
	 */
	std::vector<std::pair<std::string, std::string>> getCallsStar();

	// Next

	/**
	 * Checks if the r/s Next(s1,s2) holds for stmt# s1 and stmt# s2.
	 * 
	 * @param int stmt# of s1.
	 * @param int stmt# of s2.
	 * @return bool of whether the r/s Next(s1,s2) holds.
	 */
	bool isNext(int s1, int s2);

	/**
	 * Gets all stmt# Si that satisfies the r/s Next(s1,Si).
	 * 
	 * @param int stmt# of s1.
	 * @return vector<int> of stmt# Si that satisfies the r/s Next(s1,Si).
	 */
	std::vector<int> getNext(int s1);

	/**
	 * Gets all stmt# Si that satisfies the r/s Next(Si,s2).
	 *
	 * @param int stmtRef of s2.
	 * @return vector<int> of stmt# Si that satisfies the r/s Next(Si,s2).
	 */
	std::vector<int> getPrevious(int s2);

	/**
	 * Gets every pair of stmt# s1 and s2 that satisfies the r/s Next(s1,s2).
	 * 
	 * @return vector of pair<int,int> of stmt# s1 and s2 that satisfies the r/s Next(s1,s2).
	 */
	std::vector<std::pair<int, int>> getAllNext();

	// Next*

	/**
	 * Checks if the r/s Next*(s1,s2) holds for stmt# s1 and stmt# s2.
	 *
	 * @param int stmt# of s1.
	 * @param int stmt# of s2.
	 * @return bool of whether the r/s Next*(s1,s2) holds.
	 */
	bool isNextStar(int s1, int s2);

	/**
	 * Gets all stmt# Si that satisfies the r/s Next*(s1,Si).
	 *
	 * @param int stmt# of s1.
	 * @return vector<int> of stmt# Si that satisfies the r/s Next*(s1,Si).
	 */
	std::vector<int> getNextStar(int s1);

	/**
	 * Gets all stmt# Si that satisfies the r/s Next*(Si,s2).
	 *
	 * @param int stmtRef of s2.
	 * @return vector<int> of stmt# Si that satisfies the r/s Next*(Si,s2).
	 */
	std::vector<int> getPreviousStar(int s2);

	/**
	 * Gets every pair of stmt# s1 and s2 that satisfies the r/s Next*(s1,s2).
	 *
	 * @return vector of pair<int,int> of stmt# s1 and s2 that satisfies the r/s Next*(s1,s2).
	 */
	std::vector<std::pair<int, int>> getAllNextStar();

	// Affects

	/**
	 * Checks if the r/s Affects(s1,s2) holds for stmt# s1 and stmt# s2.
	 *
	 * @param int stmt# of s1.
	 * @param int stmt# of s2.
	 * @return bool of whether the r/s Affects(s1,s2) holds.
	 */
	bool isAffects(int s1, int s2);

	/**
	 * Gets all stmt# Si that satisfies the r/s Affects(s1,Si).
	 *
	 * @param int stmt# of s1.
	 * @return vector<int> of stmt# Si that satisfies the r/s Affects(s1,Si).
	 */
	std::vector<int> getAffectsAffected(int s1);

	/**
	 * Gets all stmt# Si that satisfies the r/s Affects(Si,s2).
	 *
	 * @param int stmtRef of s2.
	 * @return vector<int> of stmt# Si that satisfies the r/s Affects(Si,s2).
	 */
	std::vector<int> getAffectsCause(int s2);

	/**
	 * Gets every pair of stmt# s1 and s2 that satisfies the r/s Affects(s1,s2).
	 *
	 * @return vector of pair<int,int> of stmt# s1 and s2 that satisfies the r/s Affects(s1,s2).
	 */
	std::vector<std::pair<int, int>> getAllAffects();

	// Affects*

	/**
	 * Checks if the r/s Affects*(s1,s2) holds for stmt# s1 and stmt# s2.
	 *
	 * @param int stmt# of s1.
	 * @param int stmt# of s2.
	 * @return bool of whether the r/s Affects*(s1,s2) holds.
	 */
	bool isAffectsStar(int s1, int s2);

	/**
	 * Gets all stmt# Si that satisfies the r/s Affects*(s1,Si).
	 *
	 * @param int stmt# of s1.
	 * @return vector<int> of stmt# Si that satisfies the r/s Affects*(s1,Si).
	 */
	std::vector<int> getAffectsStarAffected(int s1);

	/**
	 * Gets all stmt# Si that satisfies the r/s Affects*(Si,s2).
	 *
	 * @param int stmtRef of s2.
	 * @return vector<int> of stmt# Si that satisfies the r/s Affects*(Si,s2).
	 */
	std::vector<int> getAffectsStarCause(int s2);

	/**
	 * Gets every pair of stmt# s1 and s2 that satisfies the r/s Affects*(s1,s2).
	 *
	 * @return vector of pair<int,int> of stmt# s1 and s2 that satisfies the r/s Affects*(s1,s2).
	 */
	std::vector<std::pair<int, int>> getAllAffectsStar();


	// Entities

	/**
	 * Gets all stmtRefs.
	 * 
	 * @return vector<int> of all stmtRefs.
	 */
	std::vector<int> getStatements();
	std::vector<int> getPrintStatements();
	std::vector<int> getAssignStatements();
	std::vector<int> getWhileStatements();
	std::vector<int> getIfStatements();
	std::vector<int> getCallStatements();
	std::vector<int> getReadStatements();

	/**
	 * Gets all variable names.
	 *
	 * @return vector<string> of all variables.
	 */
	std::vector<std::string> getVariables();

	/**
	 * Gets all constants.
	 *
	 * @return vector<int> of all constants.
	 */
	std::vector<std::string> getConstants();

	/**
	 * Gets all procedure names.
	 *
	 * @return vector<string> of all procedures.
	 */
	std::vector<std::string> getProcedures();

	// Pattern

	// assign

	/**
	 * Gets a vector<int> of stmtRefs of assign statements where lhs is the first param and the second param is a 
	 * variable/constant in rhs(partial match).
	 * 
	 * @param string of lhs in an assign stmt.
	 * @param string a expression/constant/variable which is part of the rhs(partial).
	 * @return a vector<int> of stmtRefs where the lhs matches the partial rhs.
	 */
	std::vector<int> getStmtRefLhsVarRhsPartial(std::string lhs, std::string rhs);

	/**
	 * Gets a vector<int> of stmtRefs of assign statements where lhs is the first param and the second param is 
	 * the expression/variable/constant in rhs(exact match).
	 * 
	 * @param string of lhs in an assign stmt.
	 * @param string an expression/constant/variable which the rhs(exact).
	 * @return a vector<int> of stmtRefs where the lhs matches the exact rhs.
	 */
	std::vector<int> getStmtRefLhsVarRhsExact(std::string lhs, std::string rhs);

	/**
	 * Gets a vector<int> of stmtRefs where the provided param at the RHS is an expression/variable/constant in the RHS(partial match)
	 * while LHS is wildcard.
	 * 
	 * @param a rhs variable of an assign stmt.
	 * @return a vector<int> of stmtRefs where the param is a variable in the rhs(partial).
	 */
	std::vector<int> getStmtRefRhsPartial(std::string rhs);

	/**
	 * Gets a vector<int> of stmtRefs where the provided param at the RHS is the expression/variable/constant in the RHS(exact match)
	 * while LHS is wildcard.
	 *
	 * @param the rhs variable of an assign stmt.
	 * @return a vector<int> of stmtRefs where the param is the expression/variable/constant in the rhs(exact).
	 */
	std::vector<int> getStmtRefRhsExact(std::string rhs);

	/**
	 * Gets a vector<int> of stmtRefs where the provided param at the LHS is a variable while LHS is wildcard.
	 * 
	 * @param the lhs variable of an assign stmt.
	 * @return a vector<int> of stmtRefs where the param is the variable in the lhs.
	 */
	std::vector<int> getStmtRefLhs(std::string lhs);

	/**
	 * Gets a vector<int> of stmtRefs where both lhs and rhs are wildcards.
	 * 
	 * @return a vector<int> of all assign stmtRefs.
	 */
	std::vector<int> getStmtRefBothWild();

	// while

	/**
	 * Gets a vector<int> of stmtRefs where the given variable is a control variable in a while control block.
	 * 
	 * @param string variable in a while control block.
	 * @return vector<int> of while stmtRefs where the param is a control variable.
	 */
	std::vector<int> getWhileStmtRefVar(std::string var);

	/**
	 * Gets a vector<int> of while stmtRefs.
	 * 
	 * @return vector<int> of all while stmtRefs.
	 */
	std::vector<int> getWhileStmtRefWild();

	// If

	/**
	 * Gets a vector<int> of stmtRefs where the given variable is a control variable in an if control block.
	 * 
	 * @param string variable in a if control block.
	 * @return vector<int> of if stmtRefs where the param is a control variable.
	 */
	std::vector<int> getIfStmtRefVar(std::string var);

	/**
	 * Gets a vector<int> of wif stmtRefs.
	 * 
	 * @return vector<int> of all if stmtRefs. 
	 */
	std::vector<int> getIfStmtRefWild();

	// CFG testing
	std::vector<std::pair<int, int>> getStmtNoNodeTable();
	CNode getNode(int stmtNo);
};
