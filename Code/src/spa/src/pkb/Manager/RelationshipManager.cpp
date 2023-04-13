#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include "RelationshipManager.h"



void RelationshipManager::addFollows(int stmt1, int stmt2) {
	followsForwardTable[stmt1] = stmt2;
	followsBackwardTable[stmt2] = stmt1;
}

void RelationshipManager::addParentChild(int parent, int child) {
	parentChildTable[parent].push_back(child);
	childParentTable[child] = parent;
}

void RelationshipManager::addStmtUses(int stmtNo, std::string var) {
	usesStmtEntTable[stmtNo].insert(var);
	usesEntStmtTable[var].insert(stmtNo);
}

void RelationshipManager::addProcUses(std::string procName, std::string var) {
	usesProcEntTable[procName].insert(var);
	usesEntProcTable[var].insert(procName);
}

void RelationshipManager::addStmtModifies(int stmtNo, std::string var) {
	modifiesStmtEntTable[stmtNo].insert(var);
	modifiesEntStmtTable[var].insert(stmtNo);
}

void RelationshipManager::addProcModifies(std::string procName, std::string var) {
	modifiesProcEntTable[procName].insert(var);
	modifiesEntProcTable[var].insert(procName);
}

void RelationshipManager::addCalls(std::string proc1, std::string proc2) {
	callProcProcForwardTable[proc1].insert(proc2);
	callProcProcBackwardTable[proc2].insert(proc1);
}

void RelationshipManager::addCallStmt(int stmtNo, std::string calledProc) {
	callStmtProcTable[stmtNo] = calledProc;
	callProcStmtTable[calledProc].insert(stmtNo);
}

void RelationshipManager::addProcChild(std::string procName, int stmtNo) {
	procStmtNosTable[procName].insert(stmtNo);
	stmtNoProctable[stmtNo] = procName;
}

void RelationshipManager::addProcCfg(std::string procName, CNode root) {
	procCfgTable[procName] = root;
}

void RelationshipManager::addStmtNoNodeTable(std::unordered_map<int, CNode> table) {
	stmtNoNodeTable = table;
}

void RelationshipManager::addAssignStmt(int stmtNo) {
	assignStmtNoSet.insert(stmtNo);
}

void RelationshipManager::addReadStmt(int stmtNo) {
	readStmtNoSet.insert(stmtNo);
}

bool RelationshipManager::isFollows(int stmt1, int stmt2) {
	if (followsForwardTable.count(stmt1) == 0) {
		return false;
	} else {
		return stmt2 == followsForwardTable[stmt1];
	}
}

int RelationshipManager::getFollows(int stmt1) {
	if (followsForwardTable.count(stmt1) == 0) {
		return 0;
	} else {
		return followsForwardTable[stmt1];
	}
}

int RelationshipManager::getPreceding(int stmt2) {
	if (followsBackwardTable.count(stmt2) == 0) {
		return 0;
	} else {
		return followsBackwardTable[stmt2];
	}
}

std::vector<std::pair<int, int>> RelationshipManager::getAllFollows() {
	std::vector<std::pair<int, int>> vec;
	for (auto& [key, value] : followsForwardTable) {
		vec.push_back(std::pair(key, value));
	}
	return vec;
}

bool RelationshipManager::isFollowsStar(int stmt1, int stmt2) {
	int s1 = stmt1;
	while (followsForwardTable.count(s1) == 1) {
		s1 = followsForwardTable[s1];
		if (s1 == stmt2) {
			return true;
		}
	}
	return false;
}

std::vector<int> RelationshipManager::getFollowsStarForward(int stmt1) {
	std::vector<int> vector;
	int s1 = stmt1;
	while (followsForwardTable.count(s1) == 1) {
		s1 = followsForwardTable[s1];
		vector.push_back(s1);
	}
	return vector;
}

std::vector<int> RelationshipManager::getFollowsStarBackward(int stmt2) {
	std::vector<int> vector;
	int s2 = stmt2;
	while (followsBackwardTable.count(s2) == 1) {
		s2 = followsBackwardTable[s2];
		vector.push_back(s2);
	}
	return vector;
}

std::vector<std::pair<int, int>> RelationshipManager::getAllFollowsStar() {
	std::vector<std::pair<int, int>> vec;
	for (auto& [key, value] : followsForwardTable) {
		std::vector<int> followingElem = getFollowsStarForward(key);
		for (auto& elem : followingElem) {
			vec.push_back(std::pair(key, elem));
		}
	}
	return vec;
}

bool RelationshipManager::isParent(int parent, int child) {
	if (childParentTable.count(child) == 0) {
		return false;
	} else {
		return parent == childParentTable[child];
	}
}

std::vector<int> RelationshipManager::getChildren(int parent) {
	if (parentChildTable.count(parent) == 0) {
		return std::vector<int>{};
	} else {
		return parentChildTable[parent];
	}
}

int RelationshipManager::getParent(int child) {
	if (childParentTable.count(child) == 1) {
		return childParentTable[child];
	} else {
		return 0;
	}
}

bool RelationshipManager::isParentStar(int ancestor, int child) {
	int parent = getParent(child);
	if (parent == 0) {
		return false;
	}
	while (parent != 0) {
		if (ancestor == parent) {
			return true;
		}
		parent = getParent(parent);
	}
	return false;
}

std::vector<int> RelationshipManager::getParentStar(int child) {
	std::vector<int> vector;
	int s2 = child;
	while (childParentTable.count(s2) == 1) {
		s2 = childParentTable[s2];
		vector.push_back(s2);
	}
	return vector;
}

std::vector<std::pair<int, int>> RelationshipManager::getAllParent() {
	std::vector<std::pair<int, int>> vec;
	for (auto& [key, valueVec] : parentChildTable) {
		for (const auto& elem : valueVec) {
			vec.push_back(std::pair(key, elem));
		}
	}
	return vec;
}

std::vector<int> RelationshipManager::getChildStar(int parent) {
	std::vector<int> descendents = RelationshipManager::getChildren(parent);
	std::queue<int> yetToVisit;
	for (const auto& elem : descendents) {
		yetToVisit.push(elem);
	}
	while (!yetToVisit.empty()) {
		std::vector<int> children = RelationshipManager::getChildren(yetToVisit.front());
		yetToVisit.pop();
		descendents.insert(descendents.end(), children.begin(), children.end());
		for (const auto& elem : children) {
			yetToVisit.push(elem);
		}
	}
	return descendents;
}

std::vector<std::pair<int, int>> RelationshipManager::getAllParentStar() {
	std::vector<std::pair<int, int>> vec;
	for (auto& [parent, children] : parentChildTable) {
		std::vector<int> descendents = getChildStar(parent);
		for (auto& elem : descendents) {
			vec.push_back(std::pair(parent, elem));
		}
	}
	return vec;
}

bool RelationshipManager::isUses(int stmtNo, std::string var) {
	if (usesStmtEntTable.count(stmtNo) == 0) {
		return false;
	} else {
		std::unordered_set<std::string> set = usesStmtEntTable[stmtNo];
		return set.find(var) != set.end();
	}
}

bool RelationshipManager::isUsesP(std::string proc, std::string var) {
	std::vector<std::string> setProc = getIndirectProcedures(proc);
	setProc.push_back(proc);
	for (auto& nextProc : setProc) {
		if (usesProcEntTable.count(nextProc) == 0) {
			continue;
		}
		std::unordered_set<std::string> set = usesProcEntTable[nextProc];
		if (set.find(var) != set.end()) {
			return true;
		}
	}
	return false;
}

std::vector<std::string> RelationshipManager::getUses(int stmtNo) {
	if (usesStmtEntTable.count(stmtNo) == 0) {
		return std::vector<std::string>{};
	} else {
		std::unordered_set<std::string> set = usesStmtEntTable[stmtNo];
		std::vector<std::string> vec(set.begin(), set.end());
		return vec;
	}
}

std::vector<std::string> RelationshipManager::getUsesPProc(std::string proc) {
	std::vector<std::string> setProc = getIndirectProcedures(proc);
	setProc.push_back(proc);
	std::unordered_set<std::string> res;
	for (auto& proc : setProc) {
		if (usesProcEntTable.count(proc) == 0) {
			continue;
		}
		for (auto& elem : usesProcEntTable[proc]) {
			res.insert(elem);
		}
	}
	std::vector<std::string> ret(res.begin(), res.end());
	return ret;
}

std::vector<int> RelationshipManager::getUses(std::string var) {
	if (usesEntStmtTable.count(var) == 0) {
		return std::vector<int>{};
	} else {
		std::unordered_set<int> set = usesEntStmtTable[var];
		std::vector<int> vec(set.begin(), set.end());
		return vec;
	}
}

std::vector<std::string> RelationshipManager::getUsesPVar(std::string var) {
	std::unordered_set<std::string> res;
	if (usesEntProcTable.count(var) == 0) {
		return std::vector<std::string>{};
	} else {
		for (auto& proc : usesEntProcTable[var]) {
			std::vector<std::string> setProcs = getCallerProcedures(proc);
			setProcs.push_back(proc);
			for (auto& it : setProcs) {
				res.insert(it);
			}
		}
	}
	std::vector<std::string> ret(res.begin(), res.end());
	return ret;
}

std::vector<std::pair<int, std::string>> RelationshipManager::getUses() {
	std::vector<std::pair<int, std::string>> vec;
	for (auto& [keyTable, stringSet] : usesStmtEntTable) {
		for (auto& elem : stringSet) {
			vec.push_back(std::pair(keyTable, elem));
		}
	}
	return vec;
}

std::vector<std::pair<std::string, std::string>> RelationshipManager::getUsesP() {
	std::vector<std::pair<std::string, std::string>> vec;
	for (auto& [proc, varSet] : usesProcEntTable) {
		auto variables = getUsesPProc(proc);
		for (auto& var : variables) {
			vec.push_back(std::pair(proc, var));
		}
	}
	return vec;
}

bool RelationshipManager::isModifies(int stmtNo, std::string var) {
	if (modifiesStmtEntTable.count(stmtNo) == 0) {
		return false;
	} else {
		std::unordered_set<std::string> set = modifiesStmtEntTable[stmtNo];
		return set.find(var) != set.end();
	}
}

bool RelationshipManager::isModifiesP(std::string proc, std::string var) {
	std::vector<std::string> setProc = getIndirectProcedures(proc);
	setProc.push_back(proc);
	for (auto& nextProc : setProc) {
		if (modifiesProcEntTable.count(nextProc) == 0) {
			continue;
		}
		std::unordered_set<std::string> set = modifiesProcEntTable[nextProc];
		if (set.find(var) != set.end()) {
			return true;
		}
	}
	return false;
}

std::vector<std::string> RelationshipManager::getModifies(int stmtNo) {
	if (modifiesStmtEntTable.count(stmtNo) == 0) {
		return std::vector<std::string>{};
	} else {
		std::unordered_set<std::string> set = modifiesStmtEntTable[stmtNo];
		std::vector<std::string> vec(set.begin(), set.end());
		return vec;
	}
}

std::vector<std::string> RelationshipManager::getModifiesPProc(std::string proc) {
	std::vector<std::string> setProc = getIndirectProcedures(proc);
	setProc.push_back(proc);
	std::unordered_set<std::string> res;
	for (auto& proc : setProc) {
		if (modifiesProcEntTable.count(proc) == 0) {
			continue;
		}
		for (auto& elem : modifiesProcEntTable[proc]) {
			res.insert(elem);
		}
	}
	std::vector<std::string> ret(res.begin(), res.end());
	return ret;
}

std::vector<int> RelationshipManager::getModifies(std::string var) {
	if (modifiesEntStmtTable.count(var) == 0) {
		return std::vector<int>{};
	} else {
		std::unordered_set<int> set = modifiesEntStmtTable[var];
		std::vector<int> vec(set.begin(), set.end());
		return vec;
	}
}

std::vector<std::string> RelationshipManager::getModifiesPVar(std::string var) {
	std::unordered_set<std::string> res;
	if (modifiesEntProcTable.count(var) == 0) {
		return std::vector<std::string>{};
	} else {
		for (auto& proc : modifiesEntProcTable[var]) {
			std::vector<std::string> setProcs = getCallerProcedures(proc);
			setProcs.push_back(proc);
			for (auto& it : setProcs) {
				res.insert(it);
			}
		}
	}
	std::vector<std::string> ret(res.begin(), res.end());
	return ret;
}

std::vector<std::pair<int, std::string>> RelationshipManager::getModifies() {
	std::vector<std::pair<int, std::string>> vec;
	for (auto& [key, valueSet] : modifiesStmtEntTable) {
		for (auto& value : valueSet) {
			vec.push_back(std::pair(key, value));
		}
	}
	return vec;
}

std::vector<std::pair<std::string, std::string>> RelationshipManager::getModifiesP() {
	std::vector<std::pair<std::string, std::string>> vec;
	for (auto& [proc, varSet] : modifiesProcEntTable) {
		auto variables = getModifiesPProc(proc);
		for (auto& var : variables) {
			vec.push_back(std::pair(proc, var));
		}
	}
	return vec;
}

std::vector<std::string> RelationshipManager::getIndirectProcedures(std::string proc) {
	std::unordered_set<std::string> setProc;
	std::stack<std::string> unvisited;
	unvisited.push(proc);
	while (!unvisited.empty()) {
		std::string procFocus = unvisited.top();
		unvisited.pop();
		if(callProcProcForwardTable.count(procFocus) != 0) {  // get all procedures indirectly called
			std::unordered_set<std::string> setProcCalls = callProcProcForwardTable[procFocus];
			for (auto& it : setProcCalls) {
				setProc.insert(it);
				unvisited.push(it);
			}
		}
	}
	std::vector<std::string> res(setProc.begin(), setProc.end());
	return res;
}

std::vector<std::string> RelationshipManager::getCallerProcedures(std::string proc) {
	std::unordered_set<std::string> setProcs;
	std::stack<std::string> unvisited;
	unvisited.push(proc);
	while (!unvisited.empty()) {
		std::string procFocus = unvisited.top();
		unvisited.pop();
		if (callProcProcBackwardTable.count(procFocus) != 0) {
			std::unordered_set<std::string> setProcCaller = callProcProcBackwardTable[procFocus];
			for (auto& it : setProcCaller) {
				setProcs.insert(it);
				unvisited.push(it);
			}
		}
	}
	std::vector<std::string> res(setProcs.begin(), setProcs.end());
	return res;
}

bool RelationshipManager::isCalls(std::string proc1, std::string proc2) {
	if (callProcProcForwardTable.count(proc1) == 0) {
		return false;
	} else {
		auto set = callProcProcForwardTable[proc1];
		return set.find(proc2) != set.end();
	}
}

std::vector<std::string> RelationshipManager::getCaller(std::string callee) {
	if (callProcProcBackwardTable.count(callee) == 0) {
		return std::vector<std::string>{};
	} else {
		auto res = callProcProcBackwardTable[callee];
		std::vector<std::string> ret(res.begin(), res.end());
		return ret;
	}
}

std::vector<std::string> RelationshipManager::getCallee(std::string caller) {
	if (callProcProcForwardTable.count(caller) == 0) {
		return std::vector<std::string>{};
	} else {
		auto res = callProcProcForwardTable[caller];
		std::vector<std::string> ret(res.begin(), res.end());
		return ret;
	}
}

std::string RelationshipManager::getCallee(int stmtNo) {
	if (callStmtProcTable.count(stmtNo) == 0) {
		return std::string();
	} else {
		return callStmtProcTable[stmtNo];
	}
}

std::vector<std::pair<std::string, std::string>> RelationshipManager::getCalls() {
	std::vector<std::pair<std::string, std::string>> res;
	for (auto& [proc1, setProc2] : callProcProcForwardTable) {
		for (auto& proc2 : setProc2) {
			res.push_back(std::pair(proc1, proc2));
		}
	}
	return res;
}

bool RelationshipManager::isCallsStar(std::string proc1, std::string proc2) {
	if (callProcProcForwardTable.count(proc1) == 0) {
		return false;
	} else {
		auto vecCallee = getIndirectProcedures(proc1);
		return std::find(vecCallee.begin(), vecCallee.end(), proc2) != vecCallee.end();
	}
}

std::vector<std::string> RelationshipManager::getCallerStar(std::string callee) {
	if (callProcProcBackwardTable.count(callee) == 0) {
		return std::vector<std::string>{};
	} else {
		return getCallerProcedures(callee);
	}
}

std::vector<std::string> RelationshipManager::getCalleeStar(std::string caller) {
	if (callProcProcForwardTable.count(caller) == 0) {
		return std::vector<std::string>{};
	} else {
		return getIndirectProcedures(caller);
	}
}

std::vector<std::pair<std::string, std::string>> RelationshipManager::getCallsStar() {
	std::vector<std::pair<std::string, std::string>> res;
	for (auto& [proc1, proc2] : callProcProcForwardTable) {
		auto vecCallee = getIndirectProcedures(proc1);
		for (auto& callee : vecCallee) {
			res.push_back(std::pair(proc1, callee));
		}
	}
	return res;
}

std::vector<int> RelationshipManager::getChildOfProc(std::string procName) {
	auto res = procStmtNosTable[procName];
	std::vector<int> ret(res.begin(), res.end());
	return ret;
}

void RelationshipManager::addIfThen(int If, int stmtNo) {
	ifThenStmtNosTable[If].insert(stmtNo);
}

void RelationshipManager::addIfElse(int If, int stmtNo) {
	ifElseStmtNosTable[If].insert(stmtNo);
}

std::vector<int> RelationshipManager::getThen(int If) {
	auto res = ifThenStmtNosTable[If];
	std::vector<int> ret(res.begin(), res.end());
	return ret;
}
std::vector<int> RelationshipManager::getElse(int If) {
	auto res = ifElseStmtNosTable[If];
	std::vector<int> ret(res.begin(), res.end());
	return ret;
}

std::vector<std::pair<int, int>> RelationshipManager::getStmtNoNodeTable() {
	std::vector<std::pair<int, int>> ret;
	for (auto& [stmtNo, Node] : stmtNoNodeTable) {
		ret.push_back(std::pair(stmtNo, Node->getStmtNo()));
	}
	return ret;
}

CNode RelationshipManager::getNode(int stmtNo) {
	if (stmtNoNodeTable.count(stmtNo) == 0) {
		return std::make_shared<CfgNode>(0);
	}
	return stmtNoNodeTable[stmtNo];
}

void RelationshipManager::populateNextTable() {
	for (auto& [no, node] : stmtNoNodeTable) {
		std::vector<int> res = getNextHelper(no);
		nextTable[no] = res;
	}
}

void RelationshipManager::populatePreviousTable() {
	for (auto& [no, node] : stmtNoNodeTable) {
		std::vector<int> res = getPreviousHelper(no);
		previousTable[no] = res;
	}
}

std::vector<int> RelationshipManager::getNextHelper(int s1) {
	std::vector<int> res;
	CNode node = stmtNoNodeTable[s1];
	auto inode = std::dynamic_pointer_cast<IfCfgNode>(node);
	auto wnode = std::dynamic_pointer_cast<WhileCfgNode>(node);
	if (inode != nullptr) {
		CNode next = inode->getNext();
		CNode next1 = inode->getNext1();
		if (next != nullptr) {
			res.push_back(next->getStmtNo());
		}
		if (next1 != nullptr) {
			res.push_back(next1->getStmtNo());
		}
	}
	else if (wnode != nullptr) {
		CNode next = wnode->getNext();
		CNode next1 = wnode->getNext1();
		if (next != nullptr) {
			res.push_back(next->getStmtNo());
		}
		if (next1 != nullptr) {
			res.push_back(next1->getStmtNo());
		}
	}
	else {
		CNode next = node->getNext();
		if (next != nullptr) {
			res.push_back(next->getStmtNo());
		}
	}
	return res;
}

std::vector<int> RelationshipManager::getPreviousHelper(int s2) {
	if (stmtNoNodeTable.count(s2) == 0) {
		return std::vector<int>{};
	}
	std::vector<int> res;
	CNode node = stmtNoNodeTable[s2];
	std::vector<CNode> parents = node->getParents();
	for (auto& parent : parents) {
		res.push_back(parent->getStmtNo());
	}
	return res;
}

// Next
bool RelationshipManager::isNext(int s1, int s2) {
	if (nextTable.count(s1) != 0) {
		auto nexts = nextTable[s1];
		return std::find(nexts.begin(), nexts.end(), s2) != nexts.end();
	}
	return false;
}

std::vector<int> RelationshipManager::getNext(int s1) {
	if (nextTable.count(s1) == 0) {
		return std::vector<int>{};
	}
	return nextTable[s1];
}

std::vector<int> RelationshipManager::getPrevious(int s2) {
	if (previousTable.count(s2) == 0) {
		return std::vector<int>{};
	}
	return previousTable[s2];
}

std::vector<std::pair<int, int>> RelationshipManager::getAllNext() {
	std::vector<std::pair<int, int>> res;
	for (auto& [stmtNo, node] : stmtNoNodeTable) {
		auto nexts = getNext(stmtNo);
		for (auto next : nexts) {
			res.push_back(std::pair(stmtNo, next));
		}
	}
	return res;
}

// Next*
bool RelationshipManager::isNextStar(int s1, int s2) {
	if (nextTable.count(s1) != 0) {
		std::unordered_set<int> visited;
		std::stack<int> stack;
		stack.push(s1);
		while (!stack.empty()) {
			int curr = stack.top();
			stack.pop();
			if (visited.count(curr) == 0) {  // nvr visited before can get add nexts
				visited.insert(curr);
				auto nexts = nextTable[curr];
				for (auto& next : nexts) {
					if (next == s2) {
						return true;
					}
					stack.push(next);
				}
			}
		}
	}
	return false;
}

std::vector<int> RelationshipManager::getChain(std::unordered_map<int, std::vector<int>> table, int root) {
	std::unordered_set<int> visited;
	std::stack<int> stack;
	auto dirChain = table[root];
	for (const auto& dir : dirChain) {
		stack.push(dir);
	}
	while (!stack.empty()) {
		int curr = stack.top();
		stack.pop();
		if (visited.count(curr) == 0) {  // nvr visited before can get add nexts
			visited.insert(curr);
			auto nexts = table[curr];
			for (auto& next : nexts) {
				stack.push(next);
			}
		}
	}
	std::vector<int> res(visited.begin(), visited.end());
	return res;
}

std::vector<int> RelationshipManager::getNextStar(int s1) {
	if (nextTable.count(s1) != 0) {
		return getChain(nextTable, s1);
	}
	return std::vector<int>{};
}

std::vector<int> RelationshipManager::getPreviousStar(int s2) {
	if (previousTable.count(s2) != 0) {
		return getChain(previousTable, s2);
	}
	return std::vector<int>{};
}

std::vector<std::pair<int, int>> RelationshipManager::getAllNextStar() {
	std::vector<std::pair<int, int>> res;
	for (auto& [no, nexts] : nextTable) {
		auto nextsStar = getNextStar(no);
		for (auto& nextStar : nextsStar) {
			res.push_back(std::pair(no, nextStar));
		}
	}
	return res;
}

// Affects

bool RelationshipManager::isAffects(int s1, int s2) {
	if (stmtNoProctable.count(s1) != 0 && stmtNoProctable.count(s2) != 0 && stmtNoProctable[s1] == stmtNoProctable[s2]) {  // same proc
		if (assignStmtNoSet.count(s1) != 0 && assignStmtNoSet.count(s2) != 0) {  // both are assign stmts
			// s1 can only have 1 next since assign
			auto varsMod = getModifies(s1);
			auto varsUses = usesStmtEntTable[s2];
			std::vector<int> vecNext = getNext(s1);
			if (vecNext.size() == 0) {  // there is nothing after next
				return false;
			}
			std::string var = varsMod[0];  // shld only have 1 elem
			if (varsUses.count(var) == 0) {  // s2 uses sth modifies in s1
				return false;
			}
			std::unordered_set<int> visited;
			std::stack<int> stack;
			for (const auto& next : vecNext) {
				stack.push(next);
			}
			while (!stack.empty()) {
				int stop = stack.top();
				stack.pop();
				if (visited.count(stop) == 0) {
					if (assignStmtNoSet.count(stop) != 0 && usesStmtEntTable[stop].count(var) != 0) {  // stop is an assign stmt and uses modified var
						if (stop == s2) {  // reach the end, found a valid route
							return true;
						}
					}
					if (assignStmtNoSet.count(stop) != 0 || readStmtNoSet.count(stop) != 0 || callStmtProcTable.count(stop) != 0) {
						//  stop is either a assign or read or proc
						if (isModifies(stop, var)) {  // route is valid keep going
							visited.insert(stop);
							continue;
						}
					}
					for (auto& next : getNext(stop)) {
						stack.push(next);
					}
					visited.insert(stop);
				}
			}
		}
	}
	return false;
}

std::vector<int> RelationshipManager::getAffectsAffected(int s1) {
	if (assignStmtNoSet.count(s1) == 0) {  // s1 is not an assign stmt
		return std::vector<int>{};
	}
	std::unordered_set<int> res;
	std::unordered_set<int> visited;
	std::stack<int> stack;
	auto varsMod = getModifies(s1);
	std::string var = varsMod[0];
	std::vector<int> vecNext = getNext(s1);
	if (vecNext.size() == 0) {  // there is nothing after next
		return std::vector<int>{};
	}
	for (const auto& next : vecNext) {
		stack.push(next);
	}
	while (!stack.empty()) {
		int stop = stack.top();
		stack.pop();
		if (visited.count(stop) == 0) {
			if (assignStmtNoSet.count(stop) != 0 && usesStmtEntTable[stop].count(var) != 0) {  // stop is an assign stmt and uses modified var
				res.insert(stop);
			}
			if (assignStmtNoSet.count(stop) != 0 || readStmtNoSet.count(stop) != 0 || callStmtProcTable.count(stop) != 0) {
				//  stop is either a assign or read or proc
				if (isModifies(stop, var)) {  // route is valid keep going
					visited.insert(stop);
					continue;
				}
			}
			for (auto& next : getNext(stop)) {
				stack.push(next);
			}
			visited.insert(stop);
		}
	}
	return std::vector<int>(res.begin(), res.end());
}

std::vector<int> RelationshipManager::getAffectsCause(int s2) {
	if (assignStmtNoSet.count(s2) == 0) {  // s2 is not an assign stmt
		return std::vector<int>{};
	}
	if (usesStmtEntTable.count(s2) == 0) {  // s2 dont use anything
		return std::vector<int>{};
	}
	std::unordered_set<int> res;
	auto varUsesSet = usesStmtEntTable[s2];
	std::vector<int> vecPrev = getPrevious(s2);
	if (vecPrev.size() == 0) {  // there is nothing after next
		return std::vector<int>{};
	}
	for (auto& var : varUsesSet) {
		std::unordered_set<int> visited;
		std::stack<int> stack;
		for (const auto& prev : vecPrev) {
			stack.push(prev);
		}
		while (!stack.empty()) {
			int stop = stack.top();
			stack.pop();
			if (visited.count(stop) == 0) {
				if (assignStmtNoSet.count(stop) != 0) {  // stop is an assign stmt and modifies a var used in s2
					std::string varMod = getModifies(stop)[0];
					if (varMod == var) {
						res.insert(stop);
					}
				}
				if (assignStmtNoSet.count(stop) != 0 || readStmtNoSet.count(stop) != 0 || callStmtProcTable.count(stop) != 0) {
					//  stop is either a assign or read or proc
					if (isModifies(stop, var)) {  // route is invalid, modifies along the way, check nxt route
						visited.insert(stop);
						continue;
					}
				}
				for (auto& prev : getPrevious(stop)) {
					stack.push(prev);
				}
				visited.insert(stop);
			}
		}
	}
	return std::vector<int>(res.begin(), res.end());
}

std::vector<std::pair<int, int>> RelationshipManager::getAllAffects() {
	std::vector<std::pair<int, int>> res;
	for (const auto& assign : assignStmtNoSet) {
		auto affecteds = getAffectsAffected(assign);
		for (const auto& affected : affecteds) {
			res.push_back(std::pair(assign, affected));
		}
	}
	return res;
}

// Affects*

bool RelationshipManager::isAffectsStar(int s1, int s2) {
	if (isAffects(s1, s2)) {
		return true;
	} else {
		std::stack<int> stack;
		stack.push(s1);
		while (!stack.empty()) {
			int curr = stack.top();
			stack.pop();
			auto currAffected = getAffectsAffected(curr);
			for (const auto& affected : currAffected) {
				if (affected == s2) {
					return true;
				}
				stack.push(affected);
			}
		}
		return false;
	}
}

std::vector<int> RelationshipManager::getAffectsStarOneSynHelper(int root, bool isForward) {
	std::unordered_set<int> res;
	std::stack<int> stack;
	stack.push(root);
	while (!stack.empty()) {
		int curr = stack.top();
		stack.pop();
		std::vector<int> currAffected;
		if (isForward) {
			currAffected = getAffectsAffected(curr);
		} else {
			currAffected = getAffectsCause(curr);
		}
		for (const auto& affected : currAffected) {
			if (res.count(affected) == 0) {  // dont add repeat and waste time
				res.insert(affected);
				stack.push(affected);
			}
		}
	}
	std::vector<int> ret(res.begin(), res.end());
	return ret;
}

std::vector<int> RelationshipManager::getAffectsStarAffected(int s1) {
	return getAffectsStarOneSynHelper(s1, true);
}

std::vector<int> RelationshipManager::getAffectsStarCause(int s2) {
	return getAffectsStarOneSynHelper(s2, false);
}

std::vector<std::pair<int, int>> RelationshipManager::getAllAffectsStar() {
	std::vector<std::pair<int, int>> res;
	for (const auto& assign : assignStmtNoSet) {
		auto affecteds = getAffectsStarAffected(assign);
		for (const auto& affected : affecteds) {
			res.push_back(std::pair(assign, affected));
		}
	}
	return res;
}
