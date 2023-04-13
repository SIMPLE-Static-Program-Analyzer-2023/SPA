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

using namespace std;
string testPath = "C:\\Users\\ongzh\\OneDrive\\Documents\\UNI APP\\CS3203\\spa-cp\\Team09\\Tests09\\integration_test\\pkb\\";

TEST_CASE("while pattern") {
	std::string path = R"(..\\..\\..\\..\\Tests09\\)";
	PKB pkb;
	//shared_ptr<SPFacade> spf = pkb.getSPFacade();
	QPSFacade qpsf = pkb.getQPSFacade();
	std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(testPath + "whileif.txt");
	//std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(path + "whileif.txt");
	SpManager spManager;
	AstRoot root = spManager.processSIMPLE(src);
	spManager.extractAndPopulate(root, pkb);
	CFGFacade cfgFacade = pkb.getCFGFacade();
	cfgFacade.generateCfgs();
	cfgFacade.populateRelationshipManager();

	auto res = qpsf.getIfStmtRefWild();
	vector<int> exp{ 4 };
	CHECK(res == exp);

	auto res1 = qpsf.getWhileStmtRefWild();
	vector<int>exp1{ 5 };
	CHECK(res1 == exp1);
}