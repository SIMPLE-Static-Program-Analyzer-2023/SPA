#pragma once

#include <memory>
#include <string>
#include <vector>

class CfgNode;
class IfCfgNode;
class WhileCfgNode;
using CNode = std::shared_ptr<CfgNode>;

class CfgNode {
protected:
	std::vector<CNode> parents;
	CNode next;
	int stmtNo;
public:
	explicit CfgNode(int stmtNo);
	virtual ~CfgNode() = default;
	virtual bool operator==(const CfgNode& other);
	void addParent(CNode parent);
	void setNext(CNode next);
	void setStmtNo(int stmtNo);
	std::vector<CNode> getParents();
	CNode getNext();
	int getStmtNo() const;
	virtual std::string toString();
};

class IfCfgNode : public CfgNode {
protected:
	CNode next1;
public:
	explicit IfCfgNode(int stmtNo);
	bool operator==(const CfgNode& other) override;
	void setNext1(CNode next1);
	CNode getNext1();
	std::string toString() override;
};

class WhileCfgNode : public CfgNode {
protected:
	CNode next1;
public:
	explicit WhileCfgNode(int stmtNo);
	bool operator==(const CfgNode& other) override;
	void setNext1(CNode next1);
	CNode getNext1();
	std::string toString() override;
};
