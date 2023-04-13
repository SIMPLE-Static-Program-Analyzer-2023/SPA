#include "pkb/Manager/PatternManager.h"

#include "catch.hpp"
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include "pkb/PKB.h"
#include "sp/Parser.h"
#include "sp/Lexer.h"
#include "sp/SpManager.h"

TEST_CASE("addPattern and getStmtRefLhs test") {
	PatternManager pm;

	Variable lhs1 = std::make_shared<VariableNode>(nullptr, 1, "varX");
	Variable rhs11 = std::make_shared<VariableNode>(nullptr, 1, "varY");
	Constant rhs12 = std::make_shared<ConstantNode>(nullptr, 1, "123");
	Variable rhs13 = std::make_shared<VariableNode>(nullptr, 1, "y123");
	BinaryOp rhsOp11 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs11, rhs12, "+");
	BinaryOp rhsOp12 = std::make_shared<BinaryOpNode>(nullptr, 1, rhsOp11, rhs13, "+");

	Assign root1 = std::make_shared<AssignNode>(nullptr, 1);
	root1->setAssign(lhs1, rhsOp12);

	pm.addPattern(1, root1);

	Variable lhs2 = std::make_shared<VariableNode>(nullptr, 2, "abcd");
	Variable rhs21 = std::make_shared<VariableNode>(nullptr, 2, "varX");
	Constant rhs22 = std::make_shared<ConstantNode>(nullptr, 2, "123");
	BinaryOp rhsOp21 = std::make_shared<BinaryOpNode>(nullptr, 2, rhs21, rhs22, "+");
	Assign root2 = std::make_shared<AssignNode>(nullptr, 2);
	root2->setAssign(lhs2, rhsOp21);

	pm.addPattern(2, root2);

	Variable lhs3 = std::make_shared<VariableNode>(nullptr, 3, "varX");
	Variable rhs31 = std::make_shared<VariableNode>(nullptr, 3, "varY");
	Constant rhs32 = std::make_shared<ConstantNode>(nullptr, 3, "123");
	Variable rhs33 = std::make_shared<VariableNode>(nullptr, 3, "abcd");
	BinaryOp rhsOp31 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs31, rhs32, "*");
	BinaryOp rhsOp32 = std::make_shared<BinaryOpNode>(nullptr, 3, rhsOp31, rhs33, "+");

	Assign root3 = std::make_shared<AssignNode>(nullptr, 3);
	root3->setAssign(lhs3, rhsOp32);

	pm.addPattern(3, root3);

	std::string S1 = "varX = varY + 123 + y123";

	std::string S2 = "abcd = varX + 123";

	std::string S3 = "varX = varY * 123 + abcd";

	auto res = pm.getStmtRefLhs("varX");
	std::vector<int>exp{ 1,3 };

	auto res1 = pm.getStmtRefLhs("abcd");
	std::vector<int>exp1{ 2 };

	auto res2 = pm.getStmtRefLhs("123");
	std::vector<int>exp2{  };

	CHECK(res == exp);
	CHECK(res1 == exp1);
	CHECK(res2 == exp2);
}

TEST_CASE("addPattern and getStmtRefRhsPartial test") {
	PatternManager pm;

	Variable lhs1 = std::make_shared<VariableNode>(nullptr, 1, "varX");
	Variable rhs11 = std::make_shared<VariableNode>(nullptr, 1, "varY");
	Constant rhs12 = std::make_shared<ConstantNode>(nullptr, 1, "123");
	Variable rhs13 = std::make_shared<VariableNode>(nullptr, 1, "y123");
	BinaryOp rhsOp11 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs11, rhs12, "+");
	BinaryOp rhsOp12 = std::make_shared<BinaryOpNode>(nullptr, 1, rhsOp11, rhs13, "+");

	Assign root1 = std::make_shared<AssignNode>(nullptr, 1);
	root1->setAssign(lhs1, rhsOp12);

	pm.addPattern(1, root1);

	Variable lhs2 = std::make_shared<VariableNode>(nullptr, 2, "abcd");
	Variable rhs21 = std::make_shared<VariableNode>(nullptr, 2, "varX");
	Constant rhs22 = std::make_shared<ConstantNode>(nullptr, 2, "123");
	BinaryOp rhsOp21 = std::make_shared<BinaryOpNode>(nullptr, 2, rhs21, rhs22, "+");
	Assign root2 = std::make_shared<AssignNode>(nullptr, 2);
	root2->setAssign(lhs2, rhsOp21);

	pm.addPattern(2, root2);

	Variable lhs3 = std::make_shared<VariableNode>(nullptr, 3, "varX");
	Variable rhs31 = std::make_shared<VariableNode>(nullptr, 3, "varY");
	Constant rhs32 = std::make_shared<ConstantNode>(nullptr, 3, "123");
	Variable rhs33 = std::make_shared<VariableNode>(nullptr, 3, "abcd");
	BinaryOp rhsOp31 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs31, rhs32, "*");
	BinaryOp rhsOp32 = std::make_shared<BinaryOpNode>(nullptr, 3, rhsOp31, rhs33, "+");

	Assign root3 = std::make_shared<AssignNode>(nullptr, 3);
	root3->setAssign(lhs3, rhsOp32);

	pm.addPattern(3, root3);

	std::string S1 = "varX = varY + 123 + y123";

	std::string S2 = "abcd = varX + 123";

	std::string S3 = "varX = varY * 123 + abcd";
	
	//no operand
	auto res = pm.getStmtRefRhsPartial("123");
	std::vector<int>exp{ 1,2,3 };
    std::sort(exp.begin(), exp.end());
    std::sort(res.begin(), res.end());

	auto res1 = pm.getStmtRefRhsPartial("y123");
	std::vector<int>exp1{ 1 };

	auto res2 = pm.getStmtRefRhsPartial("abcd");
	std::vector<int>exp2{ 3 };

	auto res3 = pm.getStmtRefRhsPartial("var");
	std::vector<int>exp3{  };

	auto res4 = pm.getStmtRefRhsPartial("varX");
	std::vector<int>exp4{ 2 };

	//with operand
	auto res5 = pm.getStmtRefRhsPartial("varY * 123");
	std::vector<int>exp5{ 3 };

	auto res6 = pm.getStmtRefRhsPartial("varY + 123");
	std::vector<int>exp6{ 1 };

	auto res7 = pm.getStmtRefRhsPartial("123 + abcd");
	std::vector<int>exp7{ };

	auto res8 = pm.getStmtRefRhsPartial("varX+123");
	std::vector<int>exp8{ 2 };

	auto res9 = pm.getStmtRefRhsPartial("abcd +varY * 123");
	std::vector<int>exp9{ };

	auto res10 = pm.getStmtRefRhsPartial("varY * 123+abcd ");
	std::vector<int>exp10{ 3 };

	auto res11 = pm.getStmtRefRhsPartial("123+y123");
	std::vector<int>exp11{ };

	CHECK(res == exp);
	CHECK(res1 == exp1);
	CHECK(res2 == exp2);
	CHECK(res3 == exp3);
	CHECK(res4 == exp4);
	CHECK(res5 == exp5);
	CHECK(res6 == exp6);
	CHECK(res7 == exp7);
	CHECK(res8 == exp8);
	CHECK(res9 == exp9);
	CHECK(res10 == exp10);
	CHECK(res11 == exp11);
}


TEST_CASE("addPattern and getStmtRefRhsExact test") {
	PatternManager pm;

	Variable lhs1 = std::make_shared<VariableNode>(nullptr, 1, "varX");
	Variable rhs11 = std::make_shared<VariableNode>(nullptr, 1, "varY");
	Constant rhs12 = std::make_shared<ConstantNode>(nullptr, 1, "123");
	Variable rhs13 = std::make_shared<VariableNode>(nullptr, 1, "y123");
	BinaryOp rhsOp11 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs11, rhs12, "+");
	BinaryOp rhsOp12 = std::make_shared<BinaryOpNode>(nullptr, 1, rhsOp11, rhs13, "+");

	Assign root1 = std::make_shared<AssignNode>(nullptr, 1);
	root1->setAssign(lhs1, rhsOp12);

	pm.addPattern(1, root1);

	Variable lhs2 = std::make_shared<VariableNode>(nullptr, 2, "abcd");
	Variable rhs21 = std::make_shared<VariableNode>(nullptr, 2, "varX");
	Constant rhs22 = std::make_shared<ConstantNode>(nullptr, 2, "123");
	BinaryOp rhsOp21 = std::make_shared<BinaryOpNode>(nullptr, 2, rhs21, rhs22, "+");
	Assign root2 = std::make_shared<AssignNode>(nullptr, 2);
	root2->setAssign(lhs2, rhsOp21);

	pm.addPattern(2, root2);

	Variable lhs3 = std::make_shared<VariableNode>(nullptr, 3, "varX");
	Variable rhs31 = std::make_shared<VariableNode>(nullptr, 3, "varY");
	Constant rhs32 = std::make_shared<ConstantNode>(nullptr, 3, "123");
	Variable rhs33 = std::make_shared<VariableNode>(nullptr, 3, "abcd");
	BinaryOp rhsOp31 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs31, rhs32, "*");
	BinaryOp rhsOp32 = std::make_shared<BinaryOpNode>(nullptr, 3, rhsOp31, rhs33, "+");

	Assign root3 = std::make_shared<AssignNode>(nullptr, 3);
	root3->setAssign(lhs3, rhsOp32);

	pm.addPattern(3, root3);

	std::string S1 = "varX = varY + 123 + y123";

	std::string S2 = "abcd = varX + 123";

	std::string S3 = "varX = varY * 123 + abcd";

	//no operand
	auto res = pm.getStmtRefRhsExact("123");
	std::vector<int>exp{  };

	auto res1 = pm.getStmtRefRhsExact("y123");
	std::vector<int>exp1{  };

	auto res2 = pm.getStmtRefRhsExact("abcd");
	std::vector<int>exp2{  };

	auto res3 = pm.getStmtRefRhsExact("var");
	std::vector<int>exp3{  };

	auto res4 = pm.getStmtRefRhsExact("varX");
	std::vector<int>exp4{  };

	//with operand
	auto res5 = pm.getStmtRefRhsExact("varY * 123");
	std::vector<int>exp5{  };

	auto res6 = pm.getStmtRefRhsExact("varY + 123");
	std::vector<int>exp6{  };

	auto res7 = pm.getStmtRefRhsExact("123 + abcd");
	std::vector<int>exp7{ };

	auto res8 = pm.getStmtRefRhsExact("varX+123");
	std::vector<int>exp8{ 2 };

	auto res9 = pm.getStmtRefRhsExact("abcd +varY * 123");
	std::vector<int>exp9{ };

	auto res10 = pm.getStmtRefRhsExact("varY * 123+abcd ");
	std::vector<int>exp10{ 3 };

	auto res11 = pm.getStmtRefRhsExact("varY+123+y123");
	std::vector<int>exp11{ 1 };

	CHECK(res == exp);
	CHECK(res1 == exp1);
	CHECK(res2 == exp2);
	CHECK(res3 == exp3);
	CHECK(res4 == exp4);
	CHECK(res5 == exp5);
	CHECK(res6 == exp6);
	CHECK(res7 == exp7);
	CHECK(res8 == exp8);
	CHECK(res9 == exp9);
	CHECK(res10 == exp10);
	CHECK(res11 == exp11);
}

TEST_CASE("addPattern, getStmtRefLhsVarRhsPartial test") {
	PatternManager pm;

	Variable lhs1 = std::make_shared<VariableNode>(nullptr, 1, "varX");
	Variable rhs11 = std::make_shared<VariableNode>(nullptr, 1, "varY");
	Constant rhs12 = std::make_shared<ConstantNode>(nullptr, 1, "123");
	Variable rhs13 = std::make_shared<VariableNode>(nullptr, 1, "y123");
	BinaryOp rhsOp11 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs11, rhs12, "+");
	BinaryOp rhsOp12 = std::make_shared<BinaryOpNode>(nullptr, 1, rhsOp11, rhs13, "+");

	Assign root1 = std::make_shared<AssignNode>(nullptr, 1);
	root1->setAssign(lhs1, rhsOp12);

	pm.addPattern(1, root1);

	Variable lhs2 = std::make_shared<VariableNode>(nullptr, 2, "abcd");
	Variable rhs21 = std::make_shared<VariableNode>(nullptr, 2, "varX");
	Constant rhs22 = std::make_shared<ConstantNode>(nullptr, 2, "123");
	BinaryOp rhsOp21 = std::make_shared<BinaryOpNode>(nullptr, 2, rhs21, rhs22, "+");
	Assign root2 = std::make_shared<AssignNode>(nullptr, 2);
	root2->setAssign(lhs2, rhsOp21);

	pm.addPattern(2, root2);

	Variable lhs3 = std::make_shared<VariableNode>(nullptr, 3, "varX");
	Variable rhs31 = std::make_shared<VariableNode>(nullptr, 3, "varY");
	Constant rhs32 = std::make_shared<ConstantNode>(nullptr, 3, "123");
	Variable rhs33 = std::make_shared<VariableNode>(nullptr, 3, "abcd");
	BinaryOp rhsOp31 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs31, rhs32, "*");
	BinaryOp rhsOp32 = std::make_shared<BinaryOpNode>(nullptr, 3, rhsOp31, rhs33, "+");
	Assign root3 = std::make_shared<AssignNode>(nullptr, 3);
	root3->setAssign(lhs3, rhsOp32);

	pm.addPattern(3, root3);

	std::string S1 = "varX = varY + 123 + y123";

	std::string S2 = "abcd = varX + 123";

	std::string S3 = "varX = varY * 123 + abcd";

	//no operand
	std::vector<int> res = pm.getStmtRefLhsVarRhsPartial("varX", "varY");
	std::vector<int> exp{ 1,3 };

	std::vector<int> res1 = pm.getStmtRefLhsVarRhsPartial("varX", "vary");
	std::vector<int> exp1{  };

	std::vector<int> res2 = pm.getStmtRefLhsVarRhsPartial("varx", "123");
	std::vector<int> exp2{  };

	std::vector<int> res3 = pm.getStmtRefLhsVarRhsPartial("abcd", "defy");
	std::vector<int> exp3{  };

	std::vector<int> res4 = pm.getStmtRefLhsVarRhsPartial("varX", "123");
	std::vector<int> exp4{ 1,3 };

	std::vector<int> res5 = pm.getStmtRefLhsVarRhsPartial("varX", "abcd");
	std::vector<int> exp5{ 3 };

	//with operand
	std::vector<int> res6 = pm.getStmtRefLhsVarRhsPartial("varX", "varY+123");
	std::vector<int> exp6{ 1 };

	std::vector<int> res7 = pm.getStmtRefLhsVarRhsPartial("varX", "varY*123");
	std::vector<int> exp7{ 3 };

	std::vector<int> res8 = pm.getStmtRefLhsVarRhsPartial("varX", "123+abcd");
	std::vector<int> exp8{  };

	std::vector<int> res9 = pm.getStmtRefLhsVarRhsPartial("varX", "varY*123+abcd");
	std::vector<int> exp9{ 3 };

	CHECK(res == exp);
	CHECK(res1 == exp1);
	CHECK(res2 == exp2);
	CHECK(res3 == exp3);
	CHECK(res4 == exp4);
	CHECK(res5 == exp5);
	CHECK(res6 == exp6);
	CHECK(res7 == exp7);
	CHECK(res8 == exp8);
	CHECK(res9 == exp9);
}

TEST_CASE("addPattern, getStmtRefLhsVarRhsExact test") {
	PatternManager pm;

	Variable lhs1 = std::make_shared<VariableNode>(nullptr, 1, "varX");
	Variable rhs11 = std::make_shared<VariableNode>(nullptr, 1, "varY");
	Constant rhs12 = std::make_shared<ConstantNode>(nullptr, 1, "123");
	Variable rhs13 = std::make_shared<VariableNode>(nullptr, 1, "y123");
	BinaryOp rhsOp11 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs11, rhs12, "+");
	BinaryOp rhsOp12 = std::make_shared<BinaryOpNode>(nullptr, 1, rhsOp11, rhs13, "+");

	Assign root1 = std::make_shared<AssignNode>(nullptr, 1);
	root1->setAssign(lhs1, rhsOp12);

	pm.addPattern(1, root1);

	Variable lhs2 = std::make_shared<VariableNode>(nullptr, 2, "abcd");
	Variable rhs21 = std::make_shared<VariableNode>(nullptr, 2, "varX");
	Constant rhs22 = std::make_shared<ConstantNode>(nullptr, 2, "123");
	BinaryOp rhsOp21 = std::make_shared<BinaryOpNode>(nullptr, 2, rhs21, rhs22, "+");
	Assign root2 = std::make_shared<AssignNode>(nullptr, 2);
	root2->setAssign(lhs2, rhsOp21);

	pm.addPattern(2, root2);

	Variable lhs3 = std::make_shared<VariableNode>(nullptr, 3, "varX");
	Variable rhs31 = std::make_shared<VariableNode>(nullptr, 3, "varY");
	Constant rhs32 = std::make_shared<ConstantNode>(nullptr, 3, "123");
	Variable rhs33 = std::make_shared<VariableNode>(nullptr, 3, "abcd");
	BinaryOp rhsOp31 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs31, rhs32, "*");
	BinaryOp rhsOp32 = std::make_shared<BinaryOpNode>(nullptr, 3, rhsOp31, rhs33, "+");
	Assign root3 = std::make_shared<AssignNode>(nullptr, 3);
	root3->setAssign(lhs3, rhsOp32);

	pm.addPattern(3, root3);

	std::string S1 = "varX = varY + 123 + y123";

	std::string S2 = "abcd = varX + 123";

	std::string S3 = "varX = varY * 123 + abcd";

	//no operand
	std::vector<int> res = pm.getStmtRefLhsVarRhsExact("varX", "varY");
	std::vector<int> exp{  };

	std::vector<int> res1 = pm.getStmtRefLhsVarRhsExact("varX", "vary");
	std::vector<int> exp1{  };

	std::vector<int> res2 = pm.getStmtRefLhsVarRhsExact("varx", "123");
	std::vector<int> exp2{  };

	std::vector<int> res3 = pm.getStmtRefLhsVarRhsExact("abcd", "defy");
	std::vector<int> exp3{  };

	std::vector<int> res4 = pm.getStmtRefLhsVarRhsExact("varX", "123");
	std::vector<int> exp4{  };

	std::vector<int> res5 = pm.getStmtRefLhsVarRhsExact("varX", "abcd");
	std::vector<int> exp5{  };

	//with operand
	std::vector<int> res6 = pm.getStmtRefLhsVarRhsExact("varX", "varY+123");
	std::vector<int> exp6{  };

	std::vector<int> res7 = pm.getStmtRefLhsVarRhsExact("varX", "vary*123");
	std::vector<int> exp7{  };

	std::vector<int> res8 = pm.getStmtRefLhsVarRhsExact("varX", "varY+123+y123");
	std::vector<int> exp8{ 1 };

	std::vector<int> res9 = pm.getStmtRefLhsVarRhsExact("varX", "varY*123+abcd");
	std::vector<int> exp9{ 3 };

	std::vector<int> res10 = pm.getStmtRefLhsVarRhsExact("abcd", "varX + 123");
	std::vector<int> exp10{ 2 };

	CHECK(res == exp);
	CHECK(res1 == exp1);
	CHECK(res2 == exp2);
	CHECK(res3 == exp3);
	CHECK(res4 == exp4);
	CHECK(res5 == exp5);
	CHECK(res6 == exp6);
	CHECK(res7 == exp7);
	CHECK(res8 == exp8);
	CHECK(res9 == exp9);
	CHECK(res10 == exp10);
}

TEST_CASE("addPattern and getStmtRefBothWild test") {
	PatternManager pm;

	Variable lhs1 = std::make_shared<VariableNode>(nullptr, 1, "varX");
	Variable rhs11 = std::make_shared<VariableNode>(nullptr, 1, "varY");
	Constant rhs12 = std::make_shared<ConstantNode>(nullptr, 1, "123");
	Variable rhs13 = std::make_shared<VariableNode>(nullptr, 1, "y123");
	BinaryOp rhsOp11 = std::make_shared<BinaryOpNode>(nullptr, 1, rhs11, rhs12, "+");
	BinaryOp rhsOp12 = std::make_shared<BinaryOpNode>(nullptr, 1, rhsOp11, rhs13, "+");
	Assign root1 = std::make_shared<AssignNode>(nullptr, 1);
	root1->setAssign(lhs1, rhsOp12);

	pm.addPattern(1, root1);

	Variable lhs2 = std::make_shared<VariableNode>(nullptr, 2, "abcd");
	Variable rhs21 = std::make_shared<VariableNode>(nullptr, 2, "varX");
	Constant rhs22 = std::make_shared<ConstantNode>(nullptr, 2, "123");
	BinaryOp rhsOp21 = std::make_shared<BinaryOpNode>(nullptr, 2, rhs21, rhs22, "+");
	Assign root2 = std::make_shared<AssignNode>(nullptr, 2);
	root2->setAssign(lhs2, rhsOp21);

	pm.addPattern(2, root2);

	Variable lhs3 = std::make_shared<VariableNode>(nullptr, 3, "varX");
	Variable rhs31 = std::make_shared<VariableNode>(nullptr, 3, "varY");
	Constant rhs32 = std::make_shared<ConstantNode>(nullptr, 3, "123");
	Variable rhs33 = std::make_shared<VariableNode>(nullptr, 3, "abcd");
	BinaryOp rhsOp31 = std::make_shared<BinaryOpNode>(nullptr, 3, rhs31, rhs32, "*");
	BinaryOp rhsOp32 = std::make_shared<BinaryOpNode>(nullptr, 3, rhsOp31, rhs33, "+");
	Assign root3 = std::make_shared<AssignNode>(nullptr, 3);
	root3->setAssign(lhs3, rhsOp32);

	pm.addPattern(3, root3);

	std::string S1 = "varX = varY + 123 + y123";

	std::string S2 = "abcd = varX + 123";

	std::string S3 = "varX = varY * 123 + abcd";

	auto res = pm.getStmtRefBothWild();
	std::vector<int>exp{ 1,2,3 };
    std::sort(res.begin(), res.end());
    std::sort(exp.begin(), exp.end());

	CHECK(res == exp);
}

TEST_CASE("addWhilePattern and getWhileStmtRefVar(std::string var) and getWhileStmtRefWild() test") {
	PatternManager pm;
	std::string while1 = "while((x<y)||(z<=10)||(k==z))";
	Variable x = std::make_shared<VariableNode>(nullptr, 1, "x");
	Variable y = std::make_shared<VariableNode>(nullptr, 2, "y");
	Variable z = std::make_shared<VariableNode>(nullptr, 3, "z");
	Variable k = std::make_shared<VariableNode>(nullptr, 4, "k");
	Constant con = std::make_shared<ConstantNode>(nullptr, 5, "10");
	RelExpr re = std::make_shared<RelExprNode>(nullptr, 5);
	re->setRelExpr(x, y, "<");
	CondExpr cre = std::make_shared<CondExprNode>(nullptr, 5, CondExprType::REL_EXPR);
	cre->setRelExpr(re);
	RelExpr re1 = std::make_shared<RelExprNode>(nullptr, 5);
	re1->setRelExpr(z, con, "<=");
	CondExpr cre1 = std::make_shared<CondExprNode>(nullptr, 5, CondExprType::REL_EXPR);
	cre1->setRelExpr(re1);
	RelExpr re2 = std::make_shared<RelExprNode>(nullptr, 5);
	re2->setRelExpr(k, z, "==");
	CondExpr cre2 = std::make_shared<CondExprNode>(nullptr, 5, CondExprType::REL_EXPR);
	cre2->setRelExpr(re2);
	CondExpr ceright = std::make_shared<CondExprNode>(nullptr, 5, CondExprType::AND_OR);
	ceright->setAndOr(cre1, cre2, "||");
	CondExpr celeft = std::make_shared<CondExprNode>(nullptr, 5, CondExprType::AND_OR);
	celeft->setAndOr(cre, ceright, "||");
	pm.addWhilePattern(5, celeft);

	std::string while2 = "while((x>50)&&(y>=10))";
	Constant con1 = std::make_shared<ConstantNode>(nullptr, 7, "50");
	Constant con2 = std::make_shared<ConstantNode>(nullptr, 7, "10");
	RelExpr re3 = std::make_shared<RelExprNode>(nullptr, 7);
	re3->setRelExpr(x, con1, ">");
	CondExpr cre3 = std::make_shared<CondExprNode>(nullptr, 7, CondExprType::REL_EXPR);
	cre3->setRelExpr(re3);
	RelExpr re4 = std::make_shared<RelExprNode>(nullptr, 7);
	re4->setRelExpr(y, con2, ">=");
	CondExpr cre4 = std::make_shared<CondExprNode>(nullptr, 7, CondExprType::REL_EXPR);
	cre4->setRelExpr(re4);
	CondExpr ce = std::make_shared<CondExprNode>(nullptr, 7, CondExprType::AND_OR);
	ce->setAndOr(cre3, cre4, "&&");
	pm.addWhilePattern(7, ce);

	auto res = pm.getWhileStmtRefVar("x");
	std::vector<int> exp{ 5,7 };
	CHECK(res == exp);

	auto res1 = pm.getWhileStmtRefVar("y");
	std::vector<int> exp1{ 5,7 };
	CHECK(res1 == exp1);

	auto res2 = pm.getWhileStmtRefVar("z");
	std::vector<int> exp2{ 5 };
	CHECK(res2 == exp2);

	auto res3 = pm.getWhileStmtRefVar("k");
	std::vector<int> exp3{ 5 };
	CHECK(res3 == exp3);

	auto res4 = pm.getWhileStmtRefVar("10");
	std::vector<int> exp4{  };
	CHECK(res4 == exp4);

	auto resWild = pm.getWhileStmtRefWild();
	std::vector<int> expWild{ 5,7 };
    std::sort(resWild.begin(), resWild.end());
    std::sort(expWild.begin(), expWild.end());
	CHECK(resWild == expWild);
}

TEST_CASE("addIfPattern and getIfStmtRefVar(std::string var) and getIfStmtRefWild() test") {
	PatternManager pm;
	std::string if1 = "if((x<y)||(z<=10)||(k==z))";
	Variable x = std::make_shared<VariableNode>(nullptr, 1, "x");
	Variable y = std::make_shared<VariableNode>(nullptr, 2, "y");
	Variable z = std::make_shared<VariableNode>(nullptr, 3, "z");
	Variable k = std::make_shared<VariableNode>(nullptr, 4, "k");
	Constant con = std::make_shared<ConstantNode>(nullptr, 5, "10");
	RelExpr re = std::make_shared<RelExprNode>(nullptr, 5);
	re->setRelExpr(x, y, "<");
	CondExpr cre = std::make_shared<CondExprNode>(nullptr, 5, CondExprType::REL_EXPR);
	cre->setRelExpr(re);
	RelExpr re1 = std::make_shared<RelExprNode>(nullptr, 5);
	re1->setRelExpr(z, con, "<=");
	CondExpr cre1 = std::make_shared<CondExprNode>(nullptr, 5, CondExprType::REL_EXPR);
	cre1->setRelExpr(re1);
	RelExpr re2 = std::make_shared<RelExprNode>(nullptr, 5);
	re2->setRelExpr(k, z, "==");
	CondExpr cre2 = std::make_shared<CondExprNode>(nullptr, 5, CondExprType::REL_EXPR);
	cre2->setRelExpr(re2);
	CondExpr ceright = std::make_shared<CondExprNode>(nullptr, 5, CondExprType::AND_OR);
	ceright->setAndOr(cre1, cre2, "||");
	CondExpr celeft = std::make_shared<CondExprNode>(nullptr, 5, CondExprType::AND_OR);
	celeft->setAndOr(cre, ceright, "||");
	pm.addIfPattern(5, celeft);

	std::string if2 = "if((x>50)&&(y>=10))";
	Constant con1 = std::make_shared<ConstantNode>(nullptr, 7, "50");
	Constant con2 = std::make_shared<ConstantNode>(nullptr, 7, "10");
	RelExpr re3 = std::make_shared<RelExprNode>(nullptr, 7);
	re3->setRelExpr(x, con1, ">");
	CondExpr cre3 = std::make_shared<CondExprNode>(nullptr, 7, CondExprType::REL_EXPR);
	cre3->setRelExpr(re3);
	RelExpr re4 = std::make_shared<RelExprNode>(nullptr, 7);
	re4->setRelExpr(y, con2, ">=");
	CondExpr cre4 = std::make_shared<CondExprNode>(nullptr, 7, CondExprType::REL_EXPR);
	cre4->setRelExpr(re4);
	CondExpr ce = std::make_shared<CondExprNode>(nullptr, 7, CondExprType::AND_OR);
	ce->setAndOr(cre3, cre4, "&&");
	pm.addIfPattern(7, ce);

	auto res = pm.getIfStmtRefVar("x");
	std::vector<int> exp{ 5,7 };
	CHECK(res == exp);

	auto res1 = pm.getIfStmtRefVar("y");
	std::vector<int> exp1{ 5,7 };
	CHECK(res1 == exp1);

	auto res2 = pm.getIfStmtRefVar("z");
	std::vector<int> exp2{ 5 };
	CHECK(res2 == exp2);

	auto res3 = pm.getIfStmtRefVar("k");
	std::vector<int> exp3{ 5 };
	CHECK(res3 == exp3);

	auto res4 = pm.getIfStmtRefVar("10");
	std::vector<int> exp4{  };
	CHECK(res4 == exp4);

	auto resWild = pm.getIfStmtRefWild();
	std::vector<int> expWild{ 5,7 };
    std::sort(resWild.begin(), resWild.end());
    std::sort(expWild.begin(), expWild.end());
	CHECK(resWild == expWild);
}

TEST_CASE("pattern a MS2 test") {
    std::string path = R"(..\\..\\..\\..\\Tests09\\)";
	PKB pkb;
	//shared_ptr<SPFacade> spf = pkb.getSPFacade();
	QPSFacade qpsf = pkb.getQPSFacade();
	//std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>((testCfgGeneratorPath + "simple.txt").c_str());
	std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(path + "ms2-one_source.txt");
	SpManager spManager;
	AstRoot root = spManager.processSIMPLE(src);
	spManager.extractAndPopulate(root, pkb);
	std::shared_ptr<std::istringstream> iss = std::make_shared<std::istringstream>("(a+b) * c - d / e % i");
	std::shared_ptr<Lexer> lex = std::make_shared<Lexer>(iss);
	ExprParser p(lex);
	Expression e = p.parseExpression(nullptr, 70);
	std::cout <<"stmt70: " << e->toString() << std::endl;
	auto res = qpsf.getStmtRefRhsPartial("a + b * c - d / (e % i)");
	CHECK(res.size() == 0);
	auto res1 = qpsf.getStmtRefRhsPartial("a + b * (c-d) / e % i");
	CHECK(res1.size() == 0);
	auto res2 = qpsf.getStmtRefRhsPartial("(a+b) * c - d / e % i");
	CHECK(res2.size() == 0);
	auto res3 = qpsf.getStmtRefRhsPartial("a + (b * c - d / e % i)");
	CHECK(res3.size() == 0);

	auto res4 = qpsf.getStmtRefRhsPartial("a + b * (c - d / e % i)");
	CHECK(res4.size() == 0);
}
