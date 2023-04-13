#include "CfgGenerator.h"

CfgGenerator::CfgGenerator(std::shared_ptr <EntityManager> em, std::shared_ptr <RelationshipManager> rm) {
	entityManager = em;
	relationshipManager = rm;
}

void CfgGenerator::populateTables() {
	std::vector<std::string> procedures = entityManager->getProcedures();
	for (auto& proc : procedures) {
		auto children = relationshipManager->getChildOfProc(proc);
		std::sort(children.begin(), children.end());
		procStmtNosTable[proc] = children;
	}
	std::vector<int> ifs = entityManager->getIfStatements();
	for (auto& If : ifs) {
		std::vector<int> ifThen = relationshipManager->getThen(If);
		std::vector<int> ifElse = relationshipManager->getElse(If);
		std::sort(ifThen.begin(), ifThen.end());
		std::sort(ifElse.begin(), ifElse.end());
		ifThenTable[If] = ifThen;
		ifElseTable[If] = ifElse;
	}

	std::vector<int> whiles = entityManager->getWhileStatements();
	for (auto& While : whiles) {
		std::vector<int> whileChild = relationshipManager->getChildren(While);
		std::sort(whileChild.begin(), whileChild.end());
		whileTable[While] = whileChild;
	}
}

CNode CfgGenerator::generateCfg(std::string procName) {
	std::vector<int> list = procStmtNosTable[procName];
	CNode root = std::make_shared<CfgNode>(0);
	root->setNext(generateSubCfg(list));
	return root;
}

CNode CfgGenerator::generateSubCfg(std::vector<int>& stmtNos) {
	CNode root = nullptr;
	CNode curr = nullptr;
	std::vector<CNode> hangingVertice{};
	// std::vector<int> group;
	auto iter = stmtNos.begin();
	while (iter != stmtNos.end()) {
		if (stmtNoNodeTable.count(*iter) != 0) {
			iter++;
			continue;
		}
		if (ifThenTable.count(*iter) != 0) {  // is a if stmt
			// create IfNode
			std::shared_ptr<IfCfgNode> ifNode = std::make_shared<IfCfgNode>(*iter);
			for (auto& node : hangingVertice) {
				auto inode = std::dynamic_pointer_cast<IfCfgNode>(node);
				if (inode != nullptr) {  // node is ifnode, have to check which next is empty
					setNextsForIfCfgNode(inode, ifNode);
				}
				node->setNext(ifNode);
				ifNode->addParent(node);
			}
			hangingVertice.clear();
			if (curr != nullptr && std::dynamic_pointer_cast<IfCfgNode>(curr) == nullptr) {
				curr->setNext(ifNode);
				ifNode->addParent(curr);
			}
			curr = ifNode;
			if (root == nullptr) {
				root = ifNode;
			}
			std::vector<int> ifThen = ifThenTable[*iter];
			std::vector<int> ifElse = ifElseTable[*iter];
			if (!ifThen.empty()) {
				CNode subBlock = generateSubCfg(ifThen);
				ifNode->setNext(subBlock);
				subBlock->addParent(ifNode);
			}
			if (!ifElse.empty()) {
				CNode subBlock = generateSubCfg(ifElse);
				ifNode->setNext1(subBlock);
				subBlock->addParent(ifNode);
			}
			// get end of both if route, connect to next
			hangingVertice = getEndOfCfg(ifNode);
		}
		else if (whileTable.count(*iter) != 0) {  // is a while stmt
			// create WhileNode
			std::shared_ptr<WhileCfgNode> whileNode = std::make_shared<WhileCfgNode>(*iter);
			for (auto& node : hangingVertice) {
				auto inode = std::dynamic_pointer_cast<IfCfgNode>(node);
				if (inode != nullptr) {  // node is ifnode, have to check which next is empty
					setNextsForIfCfgNode(inode, whileNode);
				}
				node->setNext(whileNode);
				whileNode->addParent(node);
			}
			hangingVertice.clear();
			if (curr != nullptr && std::dynamic_pointer_cast<IfCfgNode>(curr) == nullptr) {
				curr->setNext(whileNode);
				whileNode->addParent(curr);
			}
			curr = whileNode;
			if (root == nullptr) {
				root = whileNode;
			}
			std::vector<int> whileChild = whileTable[*iter];
			// check if while has no child ie while blk empty
			if (whileChild.empty()) {
				whileNode->setNext1(whileNode);
				whileNode->addParent(whileNode);
			}
			else {
				CNode subBlock = generateSubCfg(whileChild);
				whileNode->setNext1(subBlock);
				subBlock->addParent(whileNode);
				auto end = getEndOfCfg(whileNode->getNext1());
				for (auto& node : end) {
					node->setNext(whileNode);
					whileNode->addParent(node);
				}
			}
		}
		else {
			CNode cNode = std::make_shared<CfgNode>(*iter);
			for (auto& node : hangingVertice) {
				auto inode = std::dynamic_pointer_cast<IfCfgNode>(node);
				if (inode != nullptr) {  // node is ifnode, have to check which next is empty
					setNextsForIfCfgNode(inode, cNode);
				}
				node->setNext(cNode);
				cNode->addParent(node);
			}
			hangingVertice.clear();
			if (curr != nullptr && std::dynamic_pointer_cast<IfCfgNode>(curr) == nullptr) {
				curr->setNext(cNode);
				cNode->addParent(curr);
			}
			curr = cNode;
			if (root == nullptr) {
				root = cNode;
			}
		}
		iter++;
		if (stmtNoNodeTable.count(curr->getStmtNo()) == 0) {  // nvr added before
			stmtNoNodeTable[curr->getStmtNo()] = curr;
		}
	}
	return root;
}

std::vector<CNode> CfgGenerator::getEndOfCfg(CNode root) {
	std::unordered_set<CNode> res;
	std::stack<CNode> stack;
	stack.push(root);
	while (!stack.empty()) {
		CNode curr = stack.top();
		stack.pop();
		auto currIf = std::dynamic_pointer_cast<IfCfgNode>(curr);
		if (currIf != nullptr) {  // curr is if node
			CNode next = currIf->getNext();
			CNode next1 = currIf->getNext1();
			if (next == nullptr) {
				res.insert(curr);
			}
			else {
				stack.push(next);
			}
			if (next1 == nullptr) {
				res.insert(curr);
			}
			else {
				stack.push(next1);
			}
		}
		else {  // while and normal
			CNode next = curr->getNext();
			if (next == nullptr) {
				res.insert(curr);
			}
			else {
				stack.push(next);
			}
		}
	}
	std::vector<CNode> ret(res.begin(), res.end());
	return ret;
}

void CfgGenerator::setNextsForIfCfgNode(std::shared_ptr<IfCfgNode> ifNode, CNode next) {
	if (ifNode->getNext() == nullptr) {
		ifNode->setNext(next);
		next->addParent(ifNode);
	}
	if (ifNode->getNext1() == nullptr) {
		ifNode->setNext1(next);
		next->addParent(ifNode);
	}
}

// API to call to create CFGs and populate RelationshipManager
void CfgGenerator::generateCfgs() {
	std::vector<std::string> procedures = entityManager->getProcedures();
	populateTables();
	for (const auto& proc : procedures) {
		CNode root = generateCfg(proc);
		procCfgTable[proc] = root;
	}
}

void CfgGenerator::populateRelationshipManager() {
	for (const auto& [proc, root] : procCfgTable) {
		relationshipManager->addProcCfg(proc, root);
	}
	relationshipManager->addStmtNoNodeTable(stmtNoNodeTable);
	relationshipManager->populateNextTable();
	relationshipManager->populatePreviousTable();
}
