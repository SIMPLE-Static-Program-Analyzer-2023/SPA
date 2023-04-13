#include "PatternManager.h"

void PatternManager::addPattern(int stmtRef, std::shared_ptr<AssignNode> assignNode) {
	LhsRhsTable[assignNode->getLhs()->toString()] = assignNode->getRhs();
	LhsStmtRefTable[assignNode->getLhs()->toString()].push_back(stmtRef);
	StmtRefAssignTable[stmtRef] = assignNode->getRhs();
}

std::vector<int> PatternManager::getStmtRefLhsVarRhsPartial(std::string lhs, std::string rhs) {
	std::vector<int> res;
	if (LhsStmtRefTable.count(lhs) == 0) {
		return res;
	}
    // note that == comparison between ExpressionNode does not consider stmtNo
    Expression exprToTest = ExprParser::generateExpression(rhs);
	std::vector<int> matchedLhsStmtRef = LhsStmtRefTable[lhs];
	for (auto& stmtNo : matchedLhsStmtRef) {
		Expression sourceExprNode = StmtRefAssignTable[stmtNo];
        if (sourceExprNode->contains(exprToTest)) {
            res.push_back(stmtNo);
        }
	}
	return res;
}

std::vector<int> PatternManager::getStmtRefLhsVarRhsExact(std::string lhs, std::string rhs) {
	std::vector<int> res;
	if (LhsStmtRefTable.count(lhs) == 0) {
		return res;
	}
    Expression exprToTest = ExprParser::generateExpression(rhs);
	std::vector<int> matchedLhsStmtRef = LhsStmtRefTable[lhs];
	for (auto& stmtNo : matchedLhsStmtRef) {
		Expression sourceExprNode = StmtRefAssignTable[stmtNo];
        if (*sourceExprNode == *exprToTest) {
            res.push_back(stmtNo);
        }
	}
	return res;
}

std::vector<int> PatternManager::getStmtRefRhsPartial(std::string rhs) {
	std::vector<int> res;
    Expression exprToTest = ExprParser::generateExpression(rhs);
	for (auto& [stmtNo, expressionNode] : StmtRefAssignTable) {
		if (expressionNode->contains(exprToTest)) {
			res.push_back(stmtNo);
		}
	}
	return res;
}

std::vector<int> PatternManager::getStmtRefRhsExact(std::string rhs) {
	std::vector<int> res;
    Expression otherExpression = ExprParser::generateExpression(rhs);
    for (auto& [stmtNo, expressionNode] : StmtRefAssignTable) {
        if (*expressionNode == *otherExpression) {
            res.push_back(stmtNo);
        }
    }
	return res;
}

std::vector<int> PatternManager::getStmtRefLhs(std::string lhs) {
	// return LHSValTable[RHS];
	if (LhsStmtRefTable.count(lhs) == 0) {
		return std::vector<int>{};
	}
	return LhsStmtRefTable[lhs];
}

std::vector<int> PatternManager::getStmtRefBothWild() {
	std::vector<int> res;
	for (auto& [key, value] : StmtRefAssignTable) {
		res.push_back(key);
	}
	return res;
}

std::unordered_set<std::string> PatternManager::getVariablesFromCondExpr(CondExpr expr) {
	std::unordered_set<std::string> res;
	std::stack<CondExpr> condExprStack;
	condExprStack.push(expr);
	while (!condExprStack.empty()) {
		CondExpr condEx = condExprStack.top();
		condExprStack.pop();
		if (condEx->getType() == CondExprType::AND_OR) {
			condExprStack.push(condEx->getLhs());
			condExprStack.push(condEx->getRhs());
		}
		else if (condEx->getType() == CondExprType::NOT) {
			condExprStack.push(condEx->getLhs());
		}
		else {  // is relExpr
			RelExpr castRel = std::dynamic_pointer_cast<RelExprNode>(condEx->getRelExpr());
			if (castRel != nullptr) {
				Expression lhs = castRel->getLhs();
				Expression rhs = castRel->getRhs();
				auto lhsVariables = getVariablesFromExpression(lhs);
				auto rhsVariables = getVariablesFromExpression(rhs);
				for (const auto& var : lhsVariables) {
					res.insert(var);
				}
				for (const auto& var : rhsVariables) {
					res.insert(var);
				}
			}
		}
	}
	return res;
}

std::unordered_set<std::string> PatternManager::getVariablesFromExpression(Expression expr) {
	std::unordered_set<std::string> res;
	std::stack<Expression> exprStack;
	exprStack.push(expr);
	while (!exprStack.empty()) {
		Expression curr = exprStack.top();
		exprStack.pop();
		Variable var = std::dynamic_pointer_cast<VariableNode>(curr);
		if (var != nullptr) {
			res.insert(var->toString());
		} else {  // is binaryOp
			BinaryOp binOp = std::dynamic_pointer_cast<BinaryOpNode>(curr);
			if (binOp != nullptr) {
				exprStack.push(binOp->getLhs());
				exprStack.push(binOp->getRhs());
			}
		}
	}
	return res;
}

void PatternManager::addWhilePattern(int stmtRef, std::shared_ptr<CondExprNode> condExprNode) {
	auto variables = getVariablesFromCondExpr(condExprNode);
	if (variables.size() > 0) {
		WhileStmtRefVarTable[stmtRef].insert(variables.begin(), variables.end());
		for (auto& elem : variables) {
			WhileVarStmtRefTable[elem].insert(stmtRef);
		}
	}
}

std::vector<int> PatternManager::getWhileStmtRefVar(std::string var) {
	if (WhileVarStmtRefTable.count(var) == 0) {
		return std::vector<int>{};
	}
	auto res = WhileVarStmtRefTable[var];
	std::vector<int> ret(res.begin(), res.end());
	return ret;
}

std::vector<int> PatternManager::getWhileStmtRefWild() {
	std::vector<int> ret;
	for (auto& [stmt, variables] : WhileStmtRefVarTable) {
		ret.push_back(stmt);
	}
	return ret;
}

void PatternManager::addIfPattern(int stmtRef, std::shared_ptr<CondExprNode> condExprNode) {
	auto vars = getVariablesFromCondExpr(condExprNode);
	if (vars.size() > 0) {
		IfStmtRefVarTable[stmtRef].insert(vars.begin(), vars.end());
		for (auto& elem : vars) {
			IfVarStmtRefTable[elem].insert(stmtRef);
		}
	}
}

std::vector<int> PatternManager::getIfStmtRefVar(std::string var) {
	if (IfVarStmtRefTable.count(var) == 0) {
		return std::vector<int>{};
	}
	auto res = IfVarStmtRefTable[var];
	std::vector<int> ret(res.begin(), res.end());
	return ret;
}

std::vector<int> PatternManager::getIfStmtRefWild() {
	std::vector<int> ret;
	for (auto& [stmt, variables] : IfStmtRefVarTable) {
		ret.push_back(stmt);
	}
	return ret;
}
