#include "CfgNode.h"

// ========================
// CNode
// ========================

/*CNode::CNode(int stmtNo) : stmtNo(stmtNo) {}

void CNode::setNext(Node next) {
	this->next = next;
}

Node CNode::getNext() const{
	return next;
}

int CNode::getStmtNo() const {
	return stmtNo;
}*/

// ========================
// CfgNode
// ========================

CfgNode::CfgNode(int stmtNo) : stmtNo(stmtNo) { }

bool CfgNode::operator==(const CfgNode& other) {
	if (const CfgNode* o = dynamic_cast<const CfgNode*>(&other)) {
		return *next == *(o->next) && stmtNo == o->stmtNo;
	}
	return false;
}

void CfgNode::addParent(CNode parent) {
	this->parents.push_back(parent);
}

void CfgNode::setNext(CNode next) {
	this->next = next;
}

void CfgNode::setStmtNo(int stmtNo) {
	this->stmtNo = stmtNo;
}

CNode CfgNode::getNext() {
	return next;
}

int CfgNode::getStmtNo() const {
	return stmtNo;
}

std::vector<CNode> CfgNode::getParents() {
	return parents;
}

std::string CfgNode::toString() {
	std::string res;
	if (next == nullptr) {
		res = res + std::to_string(stmtNo) + " -> END";
	}
	else {
		res = res + std::to_string(stmtNo) + " -> " + next->toString();
	}
	return res;
}

// ========================
// IfCfgNode
// ========================
IfCfgNode::IfCfgNode(int stmtNo) : CfgNode(stmtNo) {}

bool IfCfgNode::operator==(const CfgNode& other) {
	if (const IfCfgNode* o = dynamic_cast<const IfCfgNode*>(&other)) {
		return *next == *(o->next) && *next1 == *(o->next1) && stmtNo == o->stmtNo;
	}
	return false;
}

void IfCfgNode::setNext1(CNode next1) {
	this->next1 = next1;
}

CNode IfCfgNode::getNext1() {
	return next1;
}

std::string IfCfgNode::toString() {
	std::string res;
	res = res + std::to_string(stmtNo);
	if (next == nullptr) {
		res = res + "if -> END";
	}
	else {
		res = res + "if -> " + next->toString();
	}
	if (next1 == nullptr) {
		res = res + "if1 -> END";
	}
	else {
		res = res + "if1 -> " + next1->toString();
	}
	return res;
}

// ========================
// WhileCfgNode
// ========================
WhileCfgNode::WhileCfgNode(int stmtNo) : CfgNode(stmtNo) {}

bool WhileCfgNode::operator==(const CfgNode& other) {
	if (const WhileCfgNode* o = dynamic_cast<const WhileCfgNode*>(&other)) {
		// will cause infinite loop
		return *next == *(o->next) && stmtNo == o->stmtNo;
	}
	return false;
}

void WhileCfgNode::setNext1(CNode next1) {
	this->next1 = next1;
}

CNode WhileCfgNode::getNext1() {
	return next1;
}

std::string WhileCfgNode::toString() {
	std::string res;
	res = res + std::to_string(stmtNo);
	if (next == nullptr) {
		res = res + "while -> END";
	}
	else {
		res = res + "while -> " + next->toString();
	}
	if (next1 == nullptr) {
		res = res + "while1 -> END";
	}
	else {
		// res = res + "while1 -> " + next1->toString();
	}
	return res;
}
