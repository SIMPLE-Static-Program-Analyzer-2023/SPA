#include "pkb/Manager/RelationshipManager.h"
#include<algorithm>

#include "catch.hpp"


TEST_CASE("addFollows and getFollows working Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 4);
	rm.addFollows(4, 10);
	rm.addFollows(10, 11);
	rm.addFollows(5, 6);
	rm.addFollows(6, 7);
	rm.addFollows(8, 9);

	int res = rm.getFollows(1);
	int res1 = rm.getFollows(4);
	int res2 = rm.getFollows(8);
	int res3 = rm.getFollows(9);

	CHECK(res == 2);
	CHECK(res1 == 10);
	CHECK(res2 == 9);
	CHECK(res3 != 10);
	CHECK(res3 == 0);
}

TEST_CASE("addFollows and getPreceding working Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 4);
	int res1 = rm.getPreceding(4);
	int res2 = rm.getPreceding(1);

	CHECK(res1 == 3);
	CHECK(res2 == 0);
}

TEST_CASE("addFollows, isFollows and isFollows* working Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 4);
	rm.addFollows(4, 10);
	rm.addFollows(10, 11);
	rm.addFollows(5, 6);
	rm.addFollows(6, 7);
	rm.addFollows(8, 9);

	CHECK(rm.isFollows(1, 2));
	CHECK(rm.isFollows(2, 3));
	CHECK(rm.isFollows(3, 4));
	CHECK(rm.isFollows(4, 10));
	CHECK(rm.isFollows(10, 11));
	CHECK(!rm.isFollows(5, 7));
	CHECK(!rm.isFollows(7, 9));
	CHECK(rm.isFollowsStar(1, 4));
	CHECK(rm.isFollowsStar(1, 11));
	CHECK(rm.isFollowsStar(3, 10));
	CHECK(rm.isFollowsStar(5, 7));
	CHECK(!rm.isFollowsStar(4, 6));
	CHECK(!rm.isFollowsStar(5, 9));
	CHECK(!rm.isFollowsStar(2, 7));
}

TEST_CASE("addFollows and getFollowsStarForward Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 4);
	rm.addFollows(4, 10);
	rm.addFollows(10, 11);
	rm.addFollows(5, 6);
	rm.addFollows(7, 8);
	rm.addFollows(6, 9);

	std::vector<int> res1 = rm.getFollowsStarForward(1);
	std::sort(res1.begin(), res1.end());
	std::vector<int> exp1 = { 2,3,4,10,11 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getFollowsStarForward(5);
	std::sort(res2.begin(), res2.end());
	std::vector<int> exp2 = { 6,9 };
	CHECK(res2 == exp2);
	
	std::vector<int> res3 = rm.getFollowsStarForward(7);
	std::vector<int> exp3 = { 8 };
	CHECK(res3 == exp3);

	std::vector<int> res4 = rm.getFollowsStarForward(4);
	std::sort(res4.begin(), res4.end());
	std::vector<int> exp4 = { 10,11 };
	CHECK(res4 == exp4);

	std::vector<int> res5 = rm.getFollowsStarForward(11);
	std::vector<int> exp5 = {};
	CHECK(res5 == exp5);
}

TEST_CASE("addFollows and getFollowsStarsBackward Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 4);
	rm.addFollows(4, 10);
	rm.addFollows(10, 11);
	rm.addFollows(5, 6);
	rm.addFollows(7, 8);
	rm.addFollows(6, 9);

	std::vector<int> res1 = rm.getFollowsStarBackward(11);
	std::sort(res1.begin(), res1.end());
	std::vector<int> exp1 = { 1,2,3,4,10 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getFollowsStarBackward(8);
	std::vector<int> exp2 = { 7 };
	CHECK(res2 == exp2);

	std::vector<int> res3 = rm.getFollowsStarBackward(7);
	std::vector<int> exp3 = { };
	CHECK(res3 == exp3);

	std::vector<int> res4 = rm.getFollowsStarBackward(6);
	std::vector<int> exp4 = { 5 };
	CHECK(res4 == exp4);

	std::vector<int> res5 = rm.getFollowsStarBackward(4);
	std::sort(res5.begin(), res5.end());
	std::vector<int> exp5 = { 1,2,3 };
	CHECK(res5 == exp5);
}

TEST_CASE("addFollows and getAllFollowsStars Test") {
	RelationshipManager rm;
	rm.addFollows(1, 2);
	rm.addFollows(2, 3);
	rm.addFollows(3, 9);
	rm.addFollows(9, 10);
	rm.addFollows(4, 5);
	rm.addFollows(6, 7);
	rm.addFollows(5, 8);

	std::vector<std::pair<int, int>> exp;
	exp.push_back(std::pair(1, 2));
	exp.push_back(std::pair(1, 3));
	exp.push_back(std::pair(1, 9));
	exp.push_back(std::pair(1, 10));
	exp.push_back(std::pair(2, 3));
	exp.push_back(std::pair(2, 9));
	exp.push_back(std::pair(2, 10));
	exp.push_back(std::pair(3, 9));
	exp.push_back(std::pair(3, 10));
	exp.push_back(std::pair(9, 10));
	exp.push_back(std::pair(4, 5));
	exp.push_back(std::pair(4, 8));
	exp.push_back(std::pair(5, 8));
	exp.push_back(std::pair(6, 7));
	std::vector<std::pair<int, int>> res = rm.getAllFollowsStar();
	CHECK(exp.size() == res.size());

	std::sort(exp.begin(), exp.end());
	std::sort(res.begin(), res.end());
	CHECK(res == exp);
}

TEST_CASE("addParentChild and getChildren working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);

	std::vector<int> res = rm.getChildren(2);
	std::vector<int> exp{ 3,4 };
	CHECK(res == exp);

	std::vector<int> res1 = rm.getChildren(4);
	std::vector<int> exp1{ 5,6 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getChildren(3);
	std::vector<int> exp2{ };
	CHECK(res2 == exp2);
}

TEST_CASE("getParent and isParent working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);
	int res = rm.getParent(5);

	CHECK(res == 4);
	CHECK(rm.isParent(2, 4));
	CHECK(rm.isParent(4, 5));
	CHECK(!rm.isParent(2, 5));
	CHECK(!rm.isParent(3, 6));
}

TEST_CASE("isParentStar working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);
	rm.addParentChild(6, 7);
	rm.addParentChild(6, 8);
	rm.addParentChild(6, 9);
	rm.addParentChild(4, 10);
	rm.addParentChild(2, 11);

	CHECK(rm.isParentStar(2, 3));
	CHECK(rm.isParentStar(2, 6));
	CHECK(rm.isParentStar(2, 9));
	CHECK(rm.isParentStar(2, 10));
	CHECK(rm.isParentStar(2, 11));
	CHECK(rm.isParentStar(4, 8));
	CHECK(rm.isParentStar(4, 5));
	CHECK(rm.isParentStar(4, 10));
	CHECK(rm.isParentStar(6, 9));
	CHECK(!rm.isParentStar(6, 10));
	CHECK(!rm.isParentStar(6, 11));
	CHECK(!rm.isParentStar(4, 11));
	CHECK(!rm.isParentStar(1, 8));
	CHECK(!rm.isParentStar(1, 12));
	CHECK(!rm.isParentStar(2, 13));
}

TEST_CASE("getParentStar working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);
	rm.addParentChild(6, 7);
	rm.addParentChild(6, 8);
	rm.addParentChild(6, 9);
	rm.addParentChild(4, 10);
	rm.addParentChild(2, 11);

	std::vector<int> res = rm.getParentStar(9);
	std::vector<int> exp{ 6,4,2 };
	CHECK(res == exp);

	std::vector<int> res1 = rm.getParentStar(10);
	std::vector<int> exp1{ 4,2 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getParentStar(6);
	std::vector<int> exp2{ 4,2 };
	CHECK(res2 == exp2);

	std::vector<int> res3 = rm.getParentStar(11);
	std::vector<int> exp3{ 2 };
	CHECK(res3 == exp3);

	std::vector<int> res4 = rm.getParentStar(2);
	std::vector<int> exp4{ };
	CHECK(res4 == exp4);
}

TEST_CASE("getChildStar working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);
	rm.addParentChild(6, 7);
	rm.addParentChild(6, 8);
	rm.addParentChild(6, 9);
	rm.addParentChild(4, 10);
	rm.addParentChild(2, 11);

	std::vector<int> res = rm.getChildStar(2);
	std::vector<int> exp{ 3,4,11,5,6,10,7,8,9 };
	CHECK(res == exp);

	std::vector<int> res1 = rm.getChildStar(4);
	std::vector<int> exp1{ 5,6,10,7,8,9 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getChildStar(6);
	std::vector<int> exp2{ 7,8,9 };
	CHECK(res2 == exp2);

	std::vector<int> res3 = rm.getChildStar(5);
	std::vector<int> exp3{ };
	CHECK(res3 == exp3);

	std::vector<int> res4 = rm.getChildStar(11);
	std::vector<int> exp4{ };
	CHECK(res4 == exp4);

	std::vector<int> res5 = rm.getChildStar(3);
	std::vector<int> exp5{ };
	CHECK(res5 == exp5);
}

TEST_CASE("getAllParentStar working Test") {
	RelationshipManager rm;
	rm.addParentChild(2, 3);
	rm.addParentChild(2, 4);
	rm.addParentChild(4, 5);
	rm.addParentChild(4, 6);
	rm.addParentChild(6, 7);
	rm.addParentChild(6, 8);
	rm.addParentChild(6, 9);
	rm.addParentChild(4, 10);
	rm.addParentChild(2, 11);

	std::vector<std::pair<int, int>> exp;
	exp.push_back(std::pair(2, 3));
	exp.push_back(std::pair(2, 4));
	exp.push_back(std::pair(2, 11));
	exp.push_back(std::pair(2, 5));
	exp.push_back(std::pair(2, 6));
	exp.push_back(std::pair(2, 10));
	exp.push_back(std::pair(2, 7));
	exp.push_back(std::pair(2, 8));
	exp.push_back(std::pair(2, 9));
	exp.push_back(std::pair(4, 5));
	exp.push_back(std::pair(4, 6));
	exp.push_back(std::pair(4, 10));
	exp.push_back(std::pair(4, 7));
	exp.push_back(std::pair(4, 8));
	exp.push_back(std::pair(4, 9));
	exp.push_back(std::pair(6, 7));
	exp.push_back(std::pair(6, 8));
	exp.push_back(std::pair(6, 9));
	std::vector<std::pair<int, int>> res = rm.getAllParentStar();
	CHECK(res == exp);
}

TEST_CASE("addStmtUses and isUses working Test") {
	RelationshipManager rm;
	rm.addStmtUses(1, "x");
	rm.addStmtUses(1, "a1b2c3");
	rm.addStmtUses(1, "y");
	rm.addStmtUses(2, "x");
	rm.addStmtUses(2, "var");

	CHECK(rm.isUses(1, "a1b2c3"));
	CHECK(rm.isUses(1, "x"));
	CHECK(rm.isUses(2, "x"));
	CHECK(rm.isUses(2, "var"));
	CHECK(!rm.isUses(1, "var"));
	CHECK(!rm.isUses(1, "f"));
	CHECK(!rm.isUses(3, "a1b2c3"));
	CHECK(!rm.isUses(3, "rando"));
}

TEST_CASE("addStmtUses, addProcUses, addCalls, addCallStmt and isUsesP and isUses(call,var) working Test") {
	RelationshipManager rm;
	rm.addStmtUses(1, "x");
	rm.addStmtUses(1, "a1b2c3");
	rm.addStmtUses(1, "y");
	rm.addStmtUses(2, "x");
	rm.addStmtUses(2, "var");
	rm.addProcUses("procedure1", "x");
	rm.addProcUses("procedure1", "a1b2c3");
	rm.addProcUses("procedure1", "y");
	rm.addProcUses("procedure1", "x");
	rm.addProcUses("procedure1", "var");

	rm.addCalls("procedure1", "procedure2");
	rm.addCallStmt(3, "procedure2");
	rm.addStmtUses(3, "x");
	rm.addStmtUses(3, "vary");
	rm.addStmtUses(3, "p321");

	rm.addStmtUses(4, "abcd");
	rm.addStmtUses(4, "varx");
	rm.addProcUses("procedure1", "abcd");
	rm.addProcUses("procedure1", "varx");

	rm.addStmtUses(5, "x");
	rm.addStmtUses(6, "vary");
	rm.addProcUses("procedure2", "x");
	rm.addProcUses("procedure2", "vary");

	rm.addCalls("procedure2", "procedure3");
	rm.addCallStmt(7, "procedure3");
	rm.addStmtUses(7, "p321");

	rm.addStmtUses(8, "p321");
	rm.addProcUses("procedure3", "p321");

	CHECK(rm.isUsesP("procedure1", "p321"));
	CHECK(rm.isUsesP("procedure1", "vary"));
	CHECK(rm.isUsesP("procedure1", "var"));
	CHECK(rm.isUsesP("procedure2", "p321"));
	CHECK(rm.isUsesP("procedure2", "x"));
	CHECK(!rm.isUsesP("procedure2", "a1b2c3"));
	CHECK(!rm.isUsesP("procedure2", "abcd"));
	CHECK(rm.isUsesP("procedure3", "p321"));
	CHECK(!rm.isUsesP("procedure3", "x"));

	//isUses(call stmtNo, var)
	CHECK(rm.isUses(3, "vary"));
	CHECK(rm.isUses(3, "x"));
	CHECK(rm.isUses(3, "p321"));
	CHECK(rm.isUses(7, "p321"));
	CHECK(!rm.isUses(7, "x"));
	CHECK(!rm.isUses(3, "varx"));
	CHECK(!rm.isUses(3, "var"));
}

TEST_CASE("getUses(int) working Test") {
	RelationshipManager rm;
	rm.addStmtUses(1, "x");
	rm.addStmtUses(1, "a1b2c3");
	rm.addStmtUses(1, "y");
	rm.addStmtUses(2, "x");
	rm.addStmtUses(2, "var");
	rm.addStmtUses(3, "x");
	rm.addStmtUses(3, "y");
	rm.addStmtUses(3, "3");

	std::vector<std::string> res = rm.getUses(1);
	std::vector<std::string> exp{ "x","a1b2c3","y" };
	CHECK(res == exp);

	std::vector<std::string> res1 = rm.getUses(2);
	std::vector<std::string> exp1{ "x","var" };
	CHECK(res1 == exp1);

	std::vector<std::string> res2 = rm.getUses(3);
	std::vector<std::string> exp2{ "x","y","3"};
	CHECK(res2 == exp2);

	std::vector<std::string> res3 = rm.getUses(0);
	std::vector<std::string> exp3{ };
	CHECK(res3 == exp3);

	std::vector<std::string> res4 = rm.getUses(5);
	std::vector<std::string> exp4{ };
	CHECK(res4 == exp4);
}

TEST_CASE("addStmtUses, addProcUses, addCalls, addCallStmt and getUsesPProc(proc) and getUses(callStmtNo) working Test") {
	RelationshipManager rm;
	rm.addStmtUses(1, "x");
	rm.addStmtUses(1, "a1b2c3");
	rm.addStmtUses(1, "y");
	rm.addStmtUses(2, "x");
	rm.addStmtUses(2, "var");
	rm.addProcUses("procedure1", "x");
	rm.addProcUses("procedure1", "a1b2c3");
	rm.addProcUses("procedure1", "y");
	rm.addProcUses("procedure1", "x");
	rm.addProcUses("procedure1", "var");
	rm.addCalls("procedure1", "procedure2");
	rm.addCallStmt(3, "procedure2");
	rm.addStmtUses(3, "x");
	rm.addStmtUses(3, "vary");
	rm.addStmtUses(3, "p321");
	rm.addStmtUses(4, "abcd");
	rm.addStmtUses(4, "varx");
	rm.addProcUses("procedure1", "abcd");
	rm.addProcUses("procedure1", "varx");
	rm.addStmtUses(5, "x");
	rm.addStmtUses(6, "vary");
	rm.addProcUses("procedure2", "x");
	rm.addProcUses("procedure2", "vary");
	rm.addCalls("procedure2", "procedure3");
	rm.addCallStmt(7, "procedure3");
	rm.addStmtUses(7, "p321");
	rm.addStmtUses(8, "p321");
	rm.addProcUses("procedure3", "p321");

	auto res = rm.getUsesPProc("procedure1");
	std::vector<std::string> exp{ "x","y","a1b2c3","var","abcd","varx", "vary","p321" };
	std::sort(res.begin(), res.end());
	std::sort(exp.begin(), exp.end());
	CHECK(res == exp);

	auto res1 = rm.getUsesPProc("procedure2");
	std::vector<std::string> exp1{ "x", "vary","p321" };
	std::sort(res1.begin(), res1.end());
	std::sort(exp1.begin(), exp1.end());
	CHECK(res1 == exp1);

	auto res2 = rm.getUsesPProc("procedure3");
	std::vector<std::string> exp2{ "p321" };
	CHECK(res2 == exp2);

	auto res3 = rm.getUses(3);
	std::vector<std::string> exp3{ "x", "vary","p321" };
	std::sort(res3.begin(), res3.end());
	std::sort(exp3.begin(), exp3.end());
	CHECK(res3 == exp3);

	auto res4 = rm.getUses(7);
	std::vector<std::string> exp4{ "p321" };
	CHECK(res4 == exp4);
}

TEST_CASE("getUses(string) working Test") {
	RelationshipManager rm;
	rm.addStmtUses(1, "x");
	rm.addStmtUses(1, "a1b2c3");
	rm.addStmtUses(1, "y");
	rm.addStmtUses(2, "x");
	rm.addStmtUses(2, "var");
	rm.addStmtUses(2, "2");
	rm.addStmtUses(3, "x");
	rm.addStmtUses(3, "y");
	rm.addStmtUses(3, "3");

	std::vector<int> res = rm.getUses("x");
	std::sort(res.begin(), res.end());
	std::vector<int> exp{ 1,2,3 };
	CHECK(res == exp);

	std::vector<int> res1 = rm.getUses("a1b2c3");
	std::vector<int> exp1{ 1 };
	CHECK(res1 == exp1);

	std::vector<int> res2 = rm.getUses("y");
	std::sort(res2.begin(), res2.end());
	std::vector<int> exp2{ 1,3 };
	CHECK(res2 == exp2);

	std::vector<int> res3 = rm.getUses("var");
	std::vector<int> exp3{ 2 };
	CHECK(res3 == exp3);

	std::vector<int> res4 = rm.getUses("3");
	std::vector<int> exp4{ 3 };
	CHECK(res4 == exp4);

	std::vector<int> res5 = rm.getUses("2");
	std::vector<int> exp5{ 2 };
	CHECK(res5 == exp5);
}

TEST_CASE("addStmtUses, addProcUses, addCalls, addCallStmt and getUsesPVar(var) and getUses(var) working Test") {
	RelationshipManager rm;
	rm.addStmtUses(1, "x");
	rm.addStmtUses(1, "a1b2c3");
	rm.addStmtUses(1, "y");
	rm.addStmtUses(2, "x");
	rm.addStmtUses(2, "var");
	rm.addProcUses("procedure1", "x");
	rm.addProcUses("procedure1", "a1b2c3");
	rm.addProcUses("procedure1", "y");
	rm.addProcUses("procedure1", "x");
	rm.addProcUses("procedure1", "var");
	rm.addCalls("procedure1", "procedure2");
	rm.addCallStmt(3, "procedure2");
	rm.addStmtUses(3, "x");
	rm.addStmtUses(3, "vary");
	rm.addStmtUses(3, "p321");
	rm.addStmtUses(4, "abcd");
	rm.addStmtUses(4, "varx");
	rm.addProcUses("procedure1", "abcd");
	rm.addProcUses("procedure1", "varx");
	rm.addStmtUses(5, "x");
	rm.addStmtUses(6, "vary");
	rm.addProcUses("procedure2", "x");
	rm.addProcUses("procedure2", "vary");
	rm.addCalls("procedure2", "procedure3");
	rm.addCallStmt(7, "procedure3");
	rm.addStmtUses(7, "p321");
	rm.addStmtUses(8, "p321");
	rm.addProcUses("procedure3", "p321");

	auto res = rm.getUsesPVar("x");
	std::vector<std::string> exp{ "procedure1", "procedure2" };
	std::sort(res.begin(), res.end());
	CHECK(res == exp);

	auto res1 = rm.getUsesPVar("vary");
	std::vector<std::string> exp1{ "procedure1", "procedure2" };
	std::sort(res1.begin(), res1.end());
	CHECK(res1 == exp1);

	auto res2 = rm.getUsesPVar("p321");
	std::vector<std::string> exp2{ "procedure1", "procedure2", "procedure3"};
	std::sort(res2.begin(), res2.end());
	CHECK(res2 == exp2);

	auto res3 = rm.getUsesPVar("a1b2c3");
	std::vector<std::string> exp3{ "procedure1" };
	CHECK(res3 == exp3);

	auto res4 = rm.getUsesPVar("var");
	std::vector<std::string> exp4{ "procedure1" };
	CHECK(res4 == exp4);

	auto res5 = rm.getUsesPVar("procedure2");
	std::vector<std::string> exp5{  };
	CHECK(res5 == exp5);

	//for uses(call stmts, var)
	auto res6 = rm.getUses("p321");
	std::vector<int> exp6{ 8,7,3 };
	std::sort(res6.begin(), res6.end());
	std::sort(exp6.begin(), exp6.end());
	CHECK(res6 == exp6);

	auto res7 = rm.getUses("vary");
	std::vector<int> exp7{ 6,3 };
	std::sort(res7.begin(), res7.end());
	std::sort(exp7.begin(), exp7.end());
	CHECK(res7 == exp7);

	auto res8 = rm.getUses("x");
	std::vector<int> exp8{ 1,2,3,5 };
	std::sort(res8.begin(), res8.end());
	std::sort(exp8.begin(), exp8.end());
	CHECK(res8 == exp8);
}

TEST_CASE("getUses() working Test") {
	RelationshipManager rm;
	rm.addStmtUses(1, "x");
	rm.addStmtUses(1, "a1b2c3");
	rm.addStmtUses(1, "y");
	rm.addStmtUses(2, "x");
	rm.addStmtUses(2, "var");
	rm.addStmtUses(2, "2");
	rm.addStmtUses(3, "x");
	rm.addStmtUses(3, "y");
	rm.addStmtUses(3, "3");

	std::vector<std::pair<int, std::string>> exp;
	exp.push_back(std::pair(1, "x"));
	exp.push_back(std::pair(1, "a1b2c3"));
	exp.push_back(std::pair(1, "y"));
	exp.push_back(std::pair(2, "x"));
	exp.push_back(std::pair(2, "var"));
	exp.push_back(std::pair(2, "2"));
	exp.push_back(std::pair(3, "x"));
	exp.push_back(std::pair(3, "y"));
	exp.push_back(std::pair(3, "3"));
	auto res = rm.getUses();
	std::sort(res.begin(), res.end());
	std::sort(exp.begin(), exp.end());
	CHECK(res == exp);
}

TEST_CASE("addStmtUses, addProcUses, addCalls, addCallStmt and getUsesP() and getUses() working Test") {
	RelationshipManager rm;
	rm.addStmtUses(1, "x");
	rm.addStmtUses(1, "a1b2c3");
	rm.addStmtUses(1, "y");
	rm.addStmtUses(2, "x");
	rm.addStmtUses(2, "var");
	rm.addProcUses("procedure1", "x");
	rm.addProcUses("procedure1", "a1b2c3");
	rm.addProcUses("procedure1", "y");
	rm.addProcUses("procedure1", "x");
	rm.addProcUses("procedure1", "var");
	rm.addCalls("procedure1", "procedure2");
	rm.addCallStmt(3, "procedure2");
	rm.addStmtUses(3, "x");
	rm.addStmtUses(3, "vary");
	rm.addStmtUses(3, "p321");
	rm.addStmtUses(4, "abcd");
	rm.addStmtUses(4, "varx");
	rm.addProcUses("procedure1", "abcd");
	rm.addProcUses("procedure1", "varx");
	rm.addStmtUses(5, "x");
	rm.addStmtUses(6, "vary");
	rm.addProcUses("procedure2", "x");
	rm.addProcUses("procedure2", "vary");
	rm.addCalls("procedure2", "procedure3");
	rm.addCallStmt(7, "procedure3");
	rm.addStmtUses(7, "p321");
	rm.addStmtUses(8, "p321");
	rm.addProcUses("procedure3", "p321");

	std::vector<std::pair<std::string, std::string>> exp;
	exp.push_back(std::pair("procedure1", "x"));
	exp.push_back(std::pair("procedure1", "a1b2c3"));
	exp.push_back(std::pair("procedure1", "y"));
	exp.push_back(std::pair("procedure1", "var"));
	exp.push_back(std::pair("procedure1", "abcd"));
	exp.push_back(std::pair("procedure1", "varx"));
	exp.push_back(std::pair("procedure1", "vary"));
	exp.push_back(std::pair("procedure1", "p321"));
	exp.push_back(std::pair("procedure2", "x"));
	exp.push_back(std::pair("procedure2", "vary"));
	exp.push_back(std::pair("procedure2", "p321"));
	exp.push_back(std::pair("procedure3", "p321"));
	std::sort(exp.begin(), exp.end());
	auto res = rm.getUsesP();
	std::sort(res.begin(), res.end());
	CHECK(res == exp);

	std::vector<std::pair<int, std::string>> exp1;
	exp1.push_back(std::pair(1, "x"));
	exp1.push_back(std::pair(1, "a1b2c3"));
	exp1.push_back(std::pair(1, "y"));
	exp1.push_back(std::pair(2, "x"));
	exp1.push_back(std::pair(2, "var"));
	exp1.push_back(std::pair(3, "x"));
	exp1.push_back(std::pair(3, "vary"));
	exp1.push_back(std::pair(3, "p321"));
	exp1.push_back(std::pair(4, "abcd"));
	exp1.push_back(std::pair(4, "varx"));
	exp1.push_back(std::pair(5, "x"));
	exp1.push_back(std::pair(6, "vary"));
	exp1.push_back(std::pair(7, "p321"));
	exp1.push_back(std::pair(8, "p321"));
	std::sort(exp1.begin(), exp1.end());
	auto res1 = rm.getUses();
	std::sort(res1.begin(), res1.end());
	CHECK(res1 == exp1);
}


TEST_CASE("addStmtModifies and isModifies working Test") {
	RelationshipManager rm;
	rm.addStmtModifies(1, "x");
	rm.addStmtModifies(2, "read");
	rm.addStmtModifies(3, "print");
	rm.addStmtModifies(4, "xyz123");
	rm.addStmtModifies(5, "aBC");

	CHECK(rm.isModifies(1, "x"));
	CHECK(rm.isModifies(2, "read"));
	CHECK(rm.isModifies(3, "print"));
	CHECK(rm.isModifies(4, "xyz123"));
	CHECK(!rm.isModifies(5, "abc"));
	CHECK(!rm.isModifies(1, "print"));
	CHECK(!rm.isModifies(0, "x"));
	CHECK(!rm.isModifies(6, "aBC"));
}

TEST_CASE("addStmtModifies, addProcModifies, addCalls, addCallStmt and isModifies(call, v) and isModifiesP working Test") {
	RelationshipManager rm;
	rm.addStmtModifies(1, "x");
	rm.addStmtModifies(2, "read");
	rm.addStmtModifies(3, "print");
	rm.addCalls("procedure1", "procedure2");
	rm.addCallStmt(4, "procedure2");
	rm.addStmtModifies(4, "xyz123");
	rm.addStmtModifies(4, "aBC");
	rm.addStmtModifies(4, "var10");
	rm.addStmtModifies(4, "var12");
	rm.addStmtModifies(4, "var13");
	rm.addStmtModifies(5, "xyz123");
	rm.addStmtModifies(6, "aBC");
	rm.addProcModifies("procedure1", "x");
	rm.addProcModifies("procedure1", "read");
	rm.addProcModifies("procedure1", "print");
	rm.addProcModifies("procedure1", "xyz123");
	rm.addProcModifies("procedure1", "aBC");
	rm.addCalls("procedure1", "procedure3");
	rm.addCallStmt(7, "procedure3");
	rm.addStmtModifies(7, "var12");
	rm.addStmtModifies(7, "var13");

	rm.addStmtModifies(8, "xyz123");
	rm.addStmtModifies(9, "aBC");
	rm.addStmtModifies(10, "var10");
	rm.addProcModifies("procedure2", "xyz123");
	rm.addProcModifies("procedure2", "aBC");
	rm.addProcModifies("procedure2", "var10");
	rm.addCalls("procedure2", "procedure3");
	rm.addCallStmt(11, "procedure3");
	rm.addStmtModifies(11, "var12");
	rm.addStmtModifies(11, "var13");

	rm.addStmtModifies(12, "var12");
	rm.addStmtModifies(13, "var13");
	rm.addProcModifies("procedure3", "var12");
	rm.addProcModifies("procedure3", "var13");

	//call stmt
	CHECK(rm.isModifies(4, "xyz123"));
	CHECK(rm.isModifies(4, "aBC"));
	CHECK(rm.isModifies(4, "var10"));
	CHECK(rm.isModifies(4, "var12"));
	CHECK(rm.isModifies(4, "var13"));
	CHECK(rm.isModifies(7, "var12"));
	CHECK(rm.isModifies(7, "var13"));
	CHECK(rm.isModifies(11, "var12"));
	CHECK(rm.isModifies(11, "var13"));
	CHECK(!rm.isModifies(4, "procedure2"));
	CHECK(!rm.isModifies(4, "procedure3"));
	CHECK(rm.isModifies(6, "aBC"));
	CHECK(rm.isModifies(9, "aBC"));
	CHECK(rm.isModifies(10, "var10"));

	CHECK(rm.isModifiesP("procedure1", "xyz123"));
	CHECK(rm.isModifiesP("procedure1", "aBC"));
	CHECK(rm.isModifiesP("procedure1", "read"));
	CHECK(rm.isModifiesP("procedure1", "var10"));
	CHECK(rm.isModifiesP("procedure1", "var12"));
	CHECK(rm.isModifiesP("procedure1", "var13"));
	CHECK(rm.isModifiesP("procedure2", "xyz123"));
	CHECK(rm.isModifiesP("procedure2", "aBC"));
	CHECK(rm.isModifiesP("procedure2", "var10"));
	CHECK(rm.isModifiesP("procedure2", "var12"));
	CHECK(rm.isModifiesP("procedure2", "var13"));
	CHECK(rm.isModifiesP("procedure3", "var12"));
	CHECK(rm.isModifiesP("procedure3", "var13"));
	CHECK(!rm.isModifiesP("procedure2", "x"));
	CHECK(!rm.isModifiesP("procedure2", "read"));
	CHECK(!rm.isModifiesP("procedure2", "print"));
	CHECK(!rm.isModifiesP("procedure3", "x"));
	CHECK(!rm.isModifiesP("procedure3", "read"));
	CHECK(!rm.isModifiesP("procedure3", "print"));
	CHECK(!rm.isModifiesP("procedure3", "xyz123"));
	CHECK(!rm.isModifiesP("procedure3", "aBC"));
	CHECK(!rm.isModifiesP("procedure3", "var10"));
}

TEST_CASE("getModifies(int) working Test") {
	RelationshipManager rm;
	rm.addStmtModifies(1, "x");
	rm.addStmtModifies(2, "read");
	rm.addStmtModifies(3, "print");
	rm.addStmtModifies(4, "xyz123");
	rm.addStmtModifies(5, "aBC987");

	std::vector<std::string>exp1{ "x" };
	std::vector<std::string>exp2{ "read" };
	std::vector<std::string>exp3{ "print" };
	std::vector<std::string>exp4{ "xyz123" };
	std::vector<std::string>exp5{ "aBC987" };
	std::vector<std::string>exp6{ "abc987" };

	CHECK(rm.getModifies(1) == exp1);
	CHECK(rm.getModifies(2) == exp2);
	CHECK(rm.getModifies(3) == exp3);
	CHECK(rm.getModifies(4) == exp4);
	CHECK(rm.getModifies(5) == exp5);
	CHECK(rm.getModifies(5) != exp6);
}

TEST_CASE("addStmtModifies, addProcModifies, addCalls, addCallStmt and getModifies(call) and getModifiesPProc working Test") {
	RelationshipManager rm;
	rm.addStmtModifies(1, "x");
	rm.addStmtModifies(2, "read");
	rm.addStmtModifies(3, "print");
	rm.addCalls("procedure1", "procedure2");
	rm.addCallStmt(4, "procedure2");
	rm.addStmtModifies(4, "xyz123");
	rm.addStmtModifies(4, "aBC");
	rm.addStmtModifies(4, "var10");
	rm.addStmtModifies(4, "var12");
	rm.addStmtModifies(4, "var13");
	rm.addStmtModifies(5, "xyz123");
	rm.addStmtModifies(6, "aBC");
	rm.addProcModifies("procedure1", "x");
	rm.addProcModifies("procedure1", "read");
	rm.addProcModifies("procedure1", "print");
	rm.addProcModifies("procedure1", "xyz123");
	rm.addProcModifies("procedure1", "aBC");
	rm.addCalls("procedure1", "procedure3");
	rm.addCallStmt(7, "procedure3");
	rm.addStmtModifies(7, "var12");
	rm.addStmtModifies(7, "var13");

	rm.addStmtModifies(8, "xyz123");
	rm.addStmtModifies(9, "aBC");
	rm.addStmtModifies(10, "var10");
	rm.addProcModifies("procedure2", "xyz123");
	rm.addProcModifies("procedure2", "aBC");
	rm.addProcModifies("procedure2", "var10");
	rm.addCalls("procedure2", "procedure3");
	rm.addCallStmt(11, "procedure3");
	rm.addStmtModifies(11, "var12");
	rm.addStmtModifies(11, "var13");

	rm.addStmtModifies(12, "var12");
	rm.addStmtModifies(13, "var13");
	rm.addProcModifies("procedure3", "var12");
	rm.addProcModifies("procedure3", "var13");

	//call stmt
	auto res = rm.getModifies(4);
	std::vector<std::string> exp{ "xyz123","aBC","var10","var12","var13" };
	std::sort(res.begin(), res.end());
	std::sort(exp.begin(), exp.end());
	CHECK(res == exp);

	auto res1 = rm.getModifies(7);
	std::vector<std::string> exp1{ "var12","var13" };
	std::sort(res1.begin(), res1.end());
	std::sort(exp1.begin(), exp1.end());
	CHECK(res1 == exp1);

	auto res2 = rm.getModifies(11);
	std::vector<std::string> exp2{ "var12","var13" };
	std::sort(res2.begin(), res2.end());
	std::sort(exp2.begin(), exp2.end());
	CHECK(res2 == exp2);

	auto res3 = rm.getModifiesPProc("procedure1");
	std::vector<std::string> exp3{ "x","read","print","xyz123","aBC","var10","var12","var13"};
	std::sort(res3.begin(), res3.end());
	std::sort(exp3.begin(), exp3.end());
	CHECK(res3 == exp3);

	auto res4 = rm.getModifiesPProc("procedure2");
	std::vector<std::string> exp4{ "xyz123","aBC","var10","var12","var13" };
	std::sort(res4.begin(), res4.end());
	std::sort(exp4.begin(), exp4.end());
	CHECK(res4 == exp4);

	auto res5 = rm.getModifiesPProc("procedure3");
	std::vector<std::string> exp5{ "var12","var13" };
	std::sort(res5.begin(), res5.end());
	std::sort(exp5.begin(), exp5.end());
	CHECK(res5 == exp5);

	auto res6 = rm.getModifiesPProc("procedure");
	std::vector<std::string> exp6{  };
	CHECK(res6 == exp6);
}

TEST_CASE("getModifies(string) working Test") {
	RelationshipManager rm;
	rm.addStmtModifies(1, "x");
	rm.addStmtModifies(2, "read");
	rm.addStmtModifies(3, "print");
	rm.addStmtModifies(4, "xyz123");
	rm.addStmtModifies(5, "aBC987");
	rm.addStmtModifies(6, "aBC987");
	rm.addStmtModifies(7, "x");
	rm.addStmtModifies(8, "aBC987");
	rm.addStmtModifies(9, "x");

	auto res = rm.getModifies("x");
	std::vector<int> exp{ 1, 7, 9 };
	std::sort(res.begin(), res.end());
	std::sort(exp.begin(), exp.end());
	CHECK(res == exp);

	auto res1 = rm.getModifies("aBC987");
	std::vector<int>exp1{ 5, 6, 8 };
	std::sort(res1.begin(), res1.end());
	std::sort(exp1.begin(), exp1.end());
	CHECK(res1 == exp1);

	CHECK(rm.getModifies("read") == std::vector<int>{2});
	CHECK(rm.getModifies("xyz123") == std::vector<int>{4});
	CHECK(rm.getModifies("null") == std::vector<int>{});
}

TEST_CASE("addStmtModifies, addProcModifies, addCalls, addCallStmt and getModifies(variable) and getModifiesPVar working Test") {
	RelationshipManager rm;
	rm.addStmtModifies(1, "x");
	rm.addStmtModifies(2, "read");
	rm.addStmtModifies(3, "print");
	rm.addCalls("procedure1", "procedure2");
	rm.addCallStmt(4, "procedure2");
	rm.addStmtModifies(4, "xyz123");
	rm.addStmtModifies(4, "aBC");
	rm.addStmtModifies(4, "var10");
	rm.addStmtModifies(4, "var12");
	rm.addStmtModifies(4, "var13");
	rm.addStmtModifies(5, "xyz123");
	rm.addStmtModifies(6, "aBC");
	rm.addProcModifies("procedure1", "x");
	rm.addProcModifies("procedure1", "read");
	rm.addProcModifies("procedure1", "print");
	rm.addProcModifies("procedure1", "xyz123");
	rm.addProcModifies("procedure1", "aBC");
	rm.addCalls("procedure1", "procedure3");
	rm.addCallStmt(7, "procedure3");
	rm.addStmtModifies(7, "var12");
	rm.addStmtModifies(7, "var13");

	rm.addStmtModifies(8, "xyz123");
	rm.addStmtModifies(9, "aBC");
	rm.addStmtModifies(10, "var10");
	rm.addProcModifies("procedure2", "xyz123");
	rm.addProcModifies("procedure2", "aBC");
	rm.addProcModifies("procedure2", "var10");
	rm.addCalls("procedure2", "procedure3");
	rm.addCallStmt(11, "procedure3");
	rm.addStmtModifies(11, "var12");
	rm.addStmtModifies(11, "var13");

	rm.addStmtModifies(12, "var12");
	rm.addStmtModifies(13, "var13");
	rm.addProcModifies("procedure3", "var12");
	rm.addProcModifies("procedure3", "var13");

	//call stmt
	auto res = rm.getModifies("var12");
	std::vector<int> exp{ 12,11,7,4 };
	std::sort(res.begin(), res.end());
	std::sort(exp.begin(), exp.end());
	CHECK(res == exp);

	auto res1 = rm.getModifies("aBC");
	std::vector<int> exp1{ 9,4,6 };
	std::sort(res1.begin(), res1.end());
	std::sort(exp1.begin(), exp1.end());
	CHECK(res1 == exp1);

	auto res2 = rm.getModifies("read");
	std::vector<int> exp2{ 2 };
	CHECK(res2 == exp2);

	auto res3 = rm.getModifiesPVar("var12");
	std::vector<std::string> exp3{ "procedure3","procedure2","procedure1" };
	std::sort(res3.begin(), res3.end());
	std::sort(exp3.begin(), exp3.end());
	CHECK(res3 == exp3);

	auto res4 = rm.getModifiesPVar("var10");
	std::vector<std::string> exp4{ "procedure2","procedure1" };
	std::sort(res4.begin(), res4.end());
	std::sort(exp4.begin(), exp4.end());
	CHECK(res4 == exp4);

	auto res5 = rm.getModifiesPVar("read");
	std::vector<std::string> exp5{ "procedure1" };
	CHECK(res5 == exp5);
}

TEST_CASE("getModifies() working Test") {
	RelationshipManager rm;
	rm.addStmtModifies(1, "x");
	rm.addStmtModifies(2, "read");
	rm.addStmtModifies(3, "print");
	rm.addStmtModifies(4, "xyz123");
	rm.addStmtModifies(5, "aBC987");
	rm.addStmtModifies(6, "aBC987");
	rm.addStmtModifies(7, "x");
	rm.addStmtModifies(8, "aBC987");
	rm.addStmtModifies(9, "x");

	std::vector<std::pair<int, std::string>> exp;
	exp.push_back(std::pair(1, "x"));
	exp.push_back(std::pair(2, "read"));
	exp.push_back(std::pair(3, "print"));
	exp.push_back(std::pair(4, "xyz123"));
	exp.push_back(std::pair(5, "aBC987"));
	exp.push_back(std::pair(6, "aBC987"));
	exp.push_back(std::pair(7, "x"));
	exp.push_back(std::pair(8, "aBC987"));
	exp.push_back(std::pair(9, "x"));
	CHECK(rm.getModifies() == exp);
}

TEST_CASE("addStmtModifies, addProcModifies, addCalls, addCallStmt and getModifies() and getModifiesP() working Test") {
	RelationshipManager rm;
	rm.addStmtModifies(1, "x");
	rm.addStmtModifies(2, "read");
	rm.addStmtModifies(3, "print");
	rm.addCalls("procedure1", "procedure2");
	rm.addCallStmt(4, "procedure2");
	rm.addStmtModifies(4, "xyz123");
	rm.addStmtModifies(4, "aBC");
	rm.addStmtModifies(4, "var10");
	rm.addStmtModifies(4, "var12");
	rm.addStmtModifies(4, "var13");
	rm.addStmtModifies(5, "xyz123");
	rm.addStmtModifies(6, "aBC");
	rm.addProcModifies("procedure1", "x");
	rm.addProcModifies("procedure1", "read");
	rm.addProcModifies("procedure1", "print");
	rm.addProcModifies("procedure1", "xyz123");
	rm.addProcModifies("procedure1", "aBC");
	rm.addCalls("procedure1", "procedure3");
	rm.addCallStmt(7, "procedure3");
	rm.addStmtModifies(7, "var12");
	rm.addStmtModifies(7, "var13");

	rm.addStmtModifies(8, "xyz123");
	rm.addStmtModifies(9, "aBC");
	rm.addStmtModifies(10, "var10");
	rm.addProcModifies("procedure2", "xyz123");
	rm.addProcModifies("procedure2", "aBC");
	rm.addProcModifies("procedure2", "var10");
	rm.addCalls("procedure2", "procedure3");
	rm.addCallStmt(11, "procedure3");
	rm.addStmtModifies(11, "var12");
	rm.addStmtModifies(11, "var13");

	rm.addStmtModifies(12, "var12");
	rm.addStmtModifies(13, "var13");
	rm.addProcModifies("procedure3", "var12");
	rm.addProcModifies("procedure3", "var13");

	auto res = rm.getModifiesP();
	std::vector < std::pair < std::string, std::string>> exp;
	exp.push_back(std::pair("procedure1", "x"));
	exp.push_back(std::pair("procedure1", "read"));
	exp.push_back(std::pair("procedure1", "print"));
	exp.push_back(std::pair("procedure1", "xyz123"));
	exp.push_back(std::pair("procedure1", "aBC"));
	exp.push_back(std::pair("procedure1", "var10"));
	exp.push_back(std::pair("procedure1", "var12"));
	exp.push_back(std::pair("procedure1", "var13"));
	exp.push_back(std::pair("procedure2", "xyz123"));
	exp.push_back(std::pair("procedure2", "aBC"));
	exp.push_back(std::pair("procedure2", "var10"));
	exp.push_back(std::pair("procedure2", "var12"));
	exp.push_back(std::pair("procedure2", "var13"));
	exp.push_back(std::pair("procedure3", "var12"));
	exp.push_back(std::pair("procedure3", "var13"));
	std::sort(res.begin(), res.end());
	std::sort(exp.begin(), exp.end());
	CHECK(res == exp);

	auto res1 = rm.getModifies();
	std::vector < std::pair < int, std::string>> exp1;
	exp1.push_back(std::pair(1, "x"));
	exp1.push_back(std::pair(2, "read"));
	exp1.push_back(std::pair(3, "print"));
	exp1.push_back(std::pair(5, "xyz123"));
	exp1.push_back(std::pair(6, "aBC"));
	exp1.push_back(std::pair(4, "xyz123"));
	exp1.push_back(std::pair(4, "aBC"));
	exp1.push_back(std::pair(4, "var10"));
	exp1.push_back(std::pair(4, "var12"));
	exp1.push_back(std::pair(4, "var13"));
	exp1.push_back(std::pair(7, "var12"));
	exp1.push_back(std::pair(7, "var13"));
	exp1.push_back(std::pair(8, "xyz123"));
	exp1.push_back(std::pair(9, "aBC"));
	exp1.push_back(std::pair(10, "var10"));
	exp1.push_back(std::pair(11, "var12"));
	exp1.push_back(std::pair(11, "var13"));
	exp1.push_back(std::pair(12, "var12"));
	exp1.push_back(std::pair(13, "var13"));
	std::sort(res1.begin(), res1.end());
	std::sort(exp1.begin(), exp1.end());
	CHECK(res1 == exp1);
}

TEST_CASE("addCalls, isCalls, isCallsStar test") {
	RelationshipManager rm;
	rm.addCalls("proc1", "proc2");
	rm.addCalls("proc2", "proc3");
	rm.addCalls("proc3", "proc4");
	rm.addCalls("proc4", "proc5");
	rm.addCalls("proc3", "proc6");

	CHECK(rm.isCalls("proc1", "proc2"));
	CHECK(rm.isCalls("proc2", "proc3"));
	CHECK(rm.isCalls("proc4", "proc5"));
	CHECK(rm.isCalls("proc3", "proc6"));
	CHECK(!rm.isCalls("proc1", "proc6"));
	CHECK(!rm.isCalls("proc1", "proc3"));
	CHECK(!rm.isCalls("proc1", "proc5"));
	CHECK(rm.isCallsStar("proc1", "proc2"));
	CHECK(rm.isCallsStar("proc1", "proc4"));
	CHECK(rm.isCallsStar("proc1", "proc5"));
	CHECK(rm.isCallsStar("proc1", "proc6"));
	CHECK(rm.isCallsStar("proc2", "proc5"));
	CHECK(rm.isCallsStar("proc2", "proc6"));
	CHECK(!rm.isCallsStar("proc5", "proc6"));
	CHECK(!rm.isCallsStar("proc4", "proc6"));
	CHECK(!rm.isCallsStar("proc6", "proc1"));
}

TEST_CASE("addCalls, getCaller, getCallerStar test") {
	RelationshipManager rm;
	rm.addCalls("proc1", "proc2");
	rm.addCalls("proc2", "proc3");
	rm.addCalls("proc3", "proc4");
	rm.addCalls("proc4", "proc5");
	rm.addCalls("proc3", "proc6");
	rm.addCalls("proc1", "proc6");

	auto res = rm.getCaller("proc6");
	std::vector<std::string> exp{ "proc1","proc3" };
	std::sort(res.begin(), res.end());
	std::sort(exp.begin(), exp.end());
	CHECK(res == exp);

	auto res1 = rm.getCaller("proc5");
	std::vector<std::string> exp1{ "proc4" };
	CHECK(res1 == exp1);

	auto res2 = rm.getCaller("proc2");
	std::vector<std::string> exp2{ "proc1" };
	CHECK(res2 == exp2);

	auto res3 = rm.getCaller("proc1");
	std::vector<std::string> exp3{ };
	CHECK(res3 == exp3);

	auto res4 = rm.getCallerStar("proc6");
	std::vector<std::string> exp4{ "proc1","proc3","proc2"};
	std::sort(res4.begin(), res4.end());
	std::sort(exp4.begin(), exp4.end());
	CHECK(res4 == exp4);

	auto res5 = rm.getCallerStar("proc5");
	std::vector<std::string> exp5{ "proc1","proc2","proc3","proc4"};
	std::sort(res5.begin(), res5.end());
	std::sort(exp5.begin(), exp5.end());
	CHECK(res5 == exp5);

	auto res6 = rm.getCallerStar("proc4");
	std::vector<std::string> exp6{ "proc1","proc2","proc3" };
	std::sort(res6.begin(), res6.end());
	std::sort(exp6.begin(), exp6.end());
	CHECK(res6 == exp6);

	auto res7 = rm.getCallerStar("proc1");
	std::vector<std::string> exp7{ };
	CHECK(res7 == exp7);
}

TEST_CASE("addCalls, getCallee, getCalleeStar test") {
	RelationshipManager rm;
	rm.addCalls("proc1", "proc2");
	rm.addCalls("proc2", "proc3");
	rm.addCalls("proc3", "proc4");
	rm.addCalls("proc4", "proc5");
	rm.addCalls("proc3", "proc6");

	auto res = rm.getCallee("proc1");
	std::vector<std::string> exp{ "proc2" };
	CHECK(res == exp);

	auto res1 = rm.getCallee("proc3");
	std::vector<std::string> exp1{ "proc4", "proc6"};
	std::sort(res1.begin(), res1.end());
	std::sort(exp1.begin(), exp1.end());
	CHECK(res1 == exp1);

	auto res2 = rm.getCallee("proc2");
	std::vector<std::string> exp2{ "proc3" };
	CHECK(res2 == exp2);

	auto res3 = rm.getCallee("proc6");
	std::vector<std::string> exp3{ };
	CHECK(res3 == exp3);

	auto res4 = rm.getCalleeStar("proc1");
	std::vector<std::string> exp4{ "proc2","proc3","proc4","proc5", "proc6"};
	std::sort(res4.begin(), res4.end());
	std::sort(exp4.begin(), exp4.end());
	CHECK(res4 == exp4);

	auto res5 = rm.getCalleeStar("proc3");
	std::vector<std::string> exp5{ "proc4","proc5", "proc6" };
	std::sort(res5.begin(), res5.end());
	std::sort(exp5.begin(), exp5.end());
	CHECK(res5 == exp5);

	auto res6 = rm.getCalleeStar("proc4");
	std::vector<std::string> exp6{ "proc5" };
	CHECK(res6 == exp6);

	auto res7 = rm.getCalleeStar("proc5");
	std::vector<std::string> exp7{ };
	CHECK(res7 == exp7);
}

TEST_CASE("addCalls, getCalls, getCallsStar test") {
	RelationshipManager rm;
	rm.addCalls("proc1", "proc2");
	rm.addCalls("proc2", "proc3");
	rm.addCalls("proc3", "proc4");
	rm.addCalls("proc4", "proc5");
	rm.addCalls("proc3", "proc6");

	auto res = rm.getCalls();
	std::vector<std::pair<std::string, std::string>> exp;
	exp.push_back(std::pair("proc1", "proc2"));
	exp.push_back(std::pair("proc2", "proc3"));
	exp.push_back(std::pair("proc3", "proc4"));
	exp.push_back(std::pair("proc4", "proc5"));
	exp.push_back(std::pair("proc3", "proc6"));
	std::sort(res.begin(), res.end());
	std::sort(exp.begin(), exp.end());
	CHECK(exp == res);

	auto res1 = rm.getCallsStar();
	std::vector<std::pair<std::string, std::string>> exp1;
	exp1.push_back(std::pair("proc1", "proc2"));
	exp1.push_back(std::pair("proc1", "proc3"));
	exp1.push_back(std::pair("proc1", "proc4"));
	exp1.push_back(std::pair("proc1", "proc5"));
	exp1.push_back(std::pair("proc1", "proc6"));
	exp1.push_back(std::pair("proc2", "proc3"));
	exp1.push_back(std::pair("proc2", "proc4"));
	exp1.push_back(std::pair("proc2", "proc5"));
	exp1.push_back(std::pair("proc2", "proc6"));
	exp1.push_back(std::pair("proc3", "proc4"));
	exp1.push_back(std::pair("proc3", "proc5"));
	exp1.push_back(std::pair("proc3", "proc6"));
	exp1.push_back(std::pair("proc4", "proc5"));
	std::sort(res1.begin(), res1.end());
	std::sort(exp1.begin(), exp1.end());
	CHECK(res1 == exp1);
}


