#pragma once

#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include "pkb/Manager/EntityManager.h"
#include "pkb/Manager/RelationshipManager.h"
#include "CfgNode.h"

class CfgGenerator {
private:
	std::shared_ptr <EntityManager> entityManager;
	std::shared_ptr <RelationshipManager> relationshipManager;

	std::unordered_map<std::string, CNode> procCfgTable;
	std::unordered_map<int, CNode> stmtNoNodeTable;
	std::unordered_map<std::string, std::vector<int>> procStmtNosTable;
	std::unordered_map<int, std::vector<int>> ifThenTable;
	std::unordered_map<int, std::vector<int>> ifElseTable;
	std::unordered_map<int, std::vector<int>> whileTable;

	CNode generateCfg(std::string procName);
	CNode generateSubCfg(std::vector<int>& stmtNo);
	std::vector<CNode> getEndOfCfg(CNode root);
	void setNextsForIfCfgNode(std::shared_ptr<IfCfgNode> ifNode, CNode next);
	void populateTables();

public:
	CfgGenerator(std::shared_ptr <EntityManager> em, std::shared_ptr <RelationshipManager> rm);
	// API to call to create CFGs and populate RelationshipManager
	void generateCfgs();
	void populateRelationshipManager();
};
