#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <stack>
#include <sstream>
#include <memory>

#include "ast/ExpressionNode.h"
#include "ast/StmtNode.h"
#include "sp/ExprParser.h"


class PatternManager {
private:
	std::unordered_map<std::string, Expression> LhsRhsTable;
	std::unordered_map<std::string, std::vector<int>> LhsStmtRefTable;
	std::unordered_map<int, Expression> StmtRefAssignTable;
	// std::map<std::string, std::string> RHSValTable;

	std::unordered_map<int, std::unordered_set<std::string>> WhileStmtRefVarTable;
	std::unordered_map<std::string, std::unordered_set<int>> WhileVarStmtRefTable;

	std::unordered_map<int, std::unordered_set<std::string>> IfStmtRefVarTable;
	std::unordered_map<std::string, std::unordered_set<int>> IfVarStmtRefTable;

	// helper function
	bool dfsNodeTree(Expression rhsNode, Expression target);
	std::unordered_set<std::string> getVariablesFromCondExpr(CondExpr condexpr);
	std::unordered_set<std::string> getVariablesFromExpression(Expression expr);
	bool compareAst(Expression exp1, Expression exp2);

public:
	// assign
	void addPattern(int stmtRef, std::shared_ptr<AssignNode> assignNode);
	std::vector<int> getStmtRefLhsVarRhsPartial(std::string lhs, std::string rhs);
	std::vector<int> getStmtRefLhsVarRhsExact(std::string lhs, std::string rhs);
	std::vector<int> getStmtRefRhsPartial(std::string rhs);
	std::vector<int> getStmtRefRhsExact(std::string rhs);
	std::vector<int> getStmtRefLhs(std::string lhs);
	std::vector<int> getStmtRefBothWild();

	// while
	void addWhilePattern(int stmtRef, std::shared_ptr<CondExprNode> condExprNode);
	std::vector<int> getWhileStmtRefVar(std::string var);
	std::vector<int> getWhileStmtRefWild();

	// if
	void addIfPattern(int stmtRef, std::shared_ptr<CondExprNode> condExprNode);
	std::vector<int> getIfStmtRefVar(std::string var);
	std::vector<int> getIfStmtRefWild();
};
