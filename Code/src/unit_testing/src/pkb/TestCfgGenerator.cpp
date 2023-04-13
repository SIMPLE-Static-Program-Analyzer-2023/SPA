#include "catch.hpp"
#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <memory>
#include <sstream>
#include "pkb/PKB.h"
#include "sp/Parser.h"
#include "sp/Lexer.h"
#include "sp/SpManager.h"

using namespace std;

std::string testCfgGeneratorLocalPath = "C:\\Users\\ongzh\\OneDrive\\Documents\\UNI APP\\CS3203\\spa-cp\\Team09\\Tests09\\integration_test\\pkb\\cfggenerator\\";

TEST_CASE("generateCfgs() multinest working test") {
	//PKB need procedures + stmtNoProcChild + ifThen + ifElse + ifStatements + whileStatements + whileChildren
	const char *input =
        R"(procedure proc3dure {
            1varB = a + varB1;
            2while (Z123 > 0) {
            3    if (print < 10) then{
            4        print = print + 9;
            5        Z123 = print + print * varB + varB1;
                 } else {
            6        helloWorld = 57660785;
            7        while (Z123 < 100) {
            8            print varB1;
            9            read varB2;
            10           varB1 = varB1 * varB2;
            11           if ((varB2 < read) || (Z123 > 10)) then{
            12              varB1 = varB1 * 3 + varB2;
                         } else {
            13               a=1;
            14               b=2;
                         }
                     }
            15       read = helloWorld / helloWorld;
                }
            }
        16  call procedure2;
        }
        procedure procedure2{
        17call procedure3;
        18var = var * 3 + 14 / varB1;
        })";
    PKB pkb;
    shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    spf->addProcedure("proc3dure");
    spf->addProcedure("procedure2");
    spf->addStatement(2, StatementType::StatementWhile);
    spf->addParentChild(2, 3);
    spf->addParentChild(2, 15);
    spf->addStatement(3, StatementType::StatementIf);
    spf->addIfThen(3, 4);
    spf->addIfThen(3, 5);
    spf->addIfElse(3, 6);
    spf->addIfElse(3, 7);
    spf->addIfElse(3, 15);
    spf->addStatement(7, StatementType::StatementWhile);
    spf->addParentChild(7, 8);
    spf->addParentChild(7, 9);
    spf->addParentChild(7, 10);
    spf->addParentChild(7, 11);
    spf->addStatement(11, StatementType::StatementIf);
    spf->addIfThen(11, 12);
    spf->addIfElse(11, 13);
    spf->addIfElse(11, 14);
    for (int i = 1; i <= 16; i++) {
        spf->addProcChild("proc3dure", i);
    }
    spf->addProcChild("procedure2", 17);
    spf->addProcChild("procedure2", 18);
    CFGFacade cfgFacade = pkb.getCFGFacade();
    cfgFacade.generateCfgs();
    cfgFacade.populateRelationshipManager();
    std::vector<std::pair<int, int>> res = qpsf.getStmtNoNodeTable();
    CHECK(res.size()==18);

    CHECK(qpsf.getNode(11)->getNext() == qpsf.getNode(12));
    //cout << qpsf.getNode(12)->toString() << endl;
    //cout<< qpsf.getNode(14)->toString() << endl;
    CHECK(qpsf.getNode(12)->getNext() == qpsf.getNode(7));
    CHECK(qpsf.getNode(14)->getNext() == qpsf.getNode(7));

    CHECK(qpsf.getNode(5)->getNext() == qpsf.getNode(15)->getNext());

    CHECK(qpsf.getNode(2)->getNext() == qpsf.getNode(16));
    CHECK(qpsf.getNode(5)->getNext() == qpsf.getNode(2));
    CHECK(qpsf.getNode(15)->getNext() == qpsf.getNode(2));

    auto while2 = std::dynamic_pointer_cast<WhileCfgNode>(qpsf.getNode(2));
    CHECK(while2->getNext1() == qpsf.getNode(3));
}

TEST_CASE("generateCfgs() simpler working test") {
    //PKB need procedures + stmtNoProcChild + ifThen + ifElse + ifStatements + whileStatements + whileChildren
    const char* input =
        R"(procedure proc3dure {
            1varB = a + varB1;
            2while (Z123 > 0) {
           3    read = helloWorld / helloWorld;
           4    call procedure2;
            }
            5b=2;
            6if (print < 10) then{
        7        print = print + 9;
        8        Z123 = print + print * varB + varB1;
            } else {
        9        helloWorld = 57660785;
            }
        10   print varB1;
        11   read varB2;
        }
        procedure procedure2{
        12call procedure3;
        13var = var * 3 + 14 / varB1;
        })";
    PKB pkb;
    shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    spf->addProcedure("proc3dure");
    spf->addProcedure("procedure2");
    spf->addStatement(2, StatementType::StatementWhile);
    spf->addParentChild(2, 3);
    spf->addParentChild(2, 4);
    spf->addStatement(6, StatementType::StatementIf);
    spf->addIfThen(6, 7);
    spf->addIfThen(6, 8);
    spf->addIfElse(6, 9);
    for (int i = 1; i <= 11; i++) {
        spf->addProcChild("proc3dure", i);
    }
    spf->addProcChild("procedure2", 12);
    spf->addProcChild("procedure2", 13);
    CFGFacade cfgFacade = pkb.getCFGFacade();
    cfgFacade.generateCfgs();
    cfgFacade.populateRelationshipManager();
    std::vector<std::pair<int, int>> res = qpsf.getStmtNoNodeTable();
    //CNode node = qpsf.getNode(2);
    shared_ptr<WhileCfgNode> wnode = dynamic_pointer_cast<WhileCfgNode>(qpsf.getNode(2));
    if (wnode != nullptr) {
        //toString will creat infinite loop if implemented properly
        CHECK(wnode->getNext1() == qpsf.getNode(3));
        CHECK(wnode->getNext() == qpsf.getNode(5));
        //end of while block not linking back to whileNode
        //if then block not being linked to ifNode
        //cout << wnode->getNext1()->toString() << endl;
        //cout << wnode->getNext()->toString() << endl;
    }

    //cout << qpsf.getNode(6)->getNext()->toString() << endl;
    //cout << qpsf.getNode(7)->toString() << endl;
    CHECK(qpsf.getNode(6)->getNext() == qpsf.getNode(7));

    shared_ptr<IfCfgNode> inode = dynamic_pointer_cast<IfCfgNode>(qpsf.getNode(6));
    //cout << inode->getNext1()->toString() << endl;
    //cout << qpsf.getNode(9)->toString() << endl;
    CHECK(inode->getNext1()== qpsf.getNode(9));

    CHECK_NOTHROW(wnode->getNext1()->toString() == qpsf.getNode(3)->toString());
    CHECK(wnode->getNext()->toString() == qpsf.getNode(5)->toString());
    CHECK(res.size() == 13);

    //std::shared_ptr<WhileCfgNode> res1 = std::make_shared<WhileCfgNode>(2);
    //res1->setNext()
}

TEST_CASE("generateCfgs() multinest and next methods working test") {
    //PKB need procedures + stmtNoProcChild + ifThen + ifElse + ifStatements + whileStatements + whileChildren
    const char* input =
        R"(procedure proc3dure {
            1varB = a + varB1;
            2while (Z123 > 0) {
            3    if (print < 10) then{
            4        print = print + 9;
            5        Z123 = print + print * varB + varB1;
                 } else {
            6        helloWorld = 57660785;
            7        while (Z123 < 100) {
            8            print varB1;
            9            read varB2;
            10           varB1 = varB1 * varB2;
            11           if ((varB2 < read) || (Z123 > 10)) then{
            12              varB1 = varB1 * 3 + varB2;
                         } else {
            13               a=1;
            14               b=2;
                         }
                     }
            15       read = helloWorld / helloWorld;
                }
            }
        16  call procedure2;
        }
        procedure procedure2{
        17call procedure3;
        18var = var * 3 + 14 / varB1;
        })";
    PKB pkb;
    shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    spf->addProcedure("proc3dure");
    spf->addProcedure("procedure2");
    spf->addStatement(2, StatementType::StatementWhile);
    spf->addParentChild(2, 3);
    spf->addParentChild(2, 15);
    spf->addStatement(3, StatementType::StatementIf);
    spf->addIfThen(3, 4);
    spf->addIfThen(3, 5);
    spf->addIfElse(3, 6);
    spf->addIfElse(3, 7);
    spf->addIfElse(3, 15);
    spf->addStatement(7, StatementType::StatementWhile);
    spf->addParentChild(7, 8);
    spf->addParentChild(7, 9);
    spf->addParentChild(7, 10);
    spf->addParentChild(7, 11);
    spf->addStatement(11, StatementType::StatementIf);
    spf->addIfThen(11, 12);
    spf->addIfElse(11, 13);
    spf->addIfElse(11, 14);
    for (int i = 1; i <= 16; i++) {
        spf->addProcChild("proc3dure", i);
    }
    spf->addProcChild("procedure2", 17);
    spf->addProcChild("procedure2", 18);
    CFGFacade cfgFacade = pkb.getCFGFacade();
    cfgFacade.generateCfgs();
    cfgFacade.populateRelationshipManager();
    CHECK(qpsf.isNext(1, 2));
    CHECK(qpsf.isNext(2, 3));
    CHECK(qpsf.isNext(2, 16));
    CHECK(qpsf.isNext(3, 4));
    CHECK(qpsf.isNext(3, 6));
    CHECK(qpsf.isNext(5, 2));
    CHECK(qpsf.isNext(10, 11));
    CHECK(qpsf.isNext(12, 7));
    CHECK(qpsf.isNext(15, 2));

    CHECK(!qpsf.isNext(16, 17));
    CHECK(!qpsf.isNext(0, 17));
    CHECK(qpsf.isNext(17, 18));

    auto res = qpsf.getNext(2);
    vector<int> exp{ 3,16 };
    sort(res.begin(), res.end());
    sort(exp.begin(), exp.end());
    CHECK(res == exp);

    auto res1 = qpsf.getNext(3);
    vector<int> exp1{ 4,6 };
    sort(res1.begin(), res1.end());
    sort(exp1.begin(), exp1.end());
    CHECK(res1 == exp1);

    auto res2 = qpsf.getNext(7);
    vector<int> exp2{ 8,15 };
    sort(res2.begin(), res2.end());
    sort(exp2.begin(), exp2.end());
    CHECK(res2 == exp2);

    auto res3 = qpsf.getNext(11);
    vector<int> exp3{ 13,12 };
    sort(res3.begin(), res3.end());
    sort(exp3.begin(), exp3.end());
    CHECK(res3 == exp3);

    auto res4 = qpsf.getNext(16);
    vector<int> exp4{  };
    sort(res4.begin(), res4.end());
    sort(exp4.begin(), exp4.end());
    CHECK(res4 == exp4);

    CHECK(qpsf.getNext(12) == qpsf.getNext(14));
    CHECK(qpsf.getNext(5) == qpsf.getNext(15));
    CHECK(qpsf.getNext(16) == qpsf.getNext(18));

    auto prevres = qpsf.getPrevious(2);
    vector<int> prevexp{ 1,5,15 };
    sort(prevres.begin(), prevres.end());
    sort(prevexp.begin(), prevexp.end());
    CHECK(prevres == prevexp);

    auto prevres1 = qpsf.getPrevious(7);
    vector<int> prevexp1{ 6,12,14 };
    sort(prevres1.begin(), prevres1.end());
    sort(prevexp1.begin(), prevexp1.end());
    CHECK(prevres1 == prevexp1);

    auto prevres2 = qpsf.getPrevious(13);
    vector<int> prevexp2{ 11 };
    sort(prevres2.begin(), prevres2.end());
    sort(prevexp2.begin(), prevexp2.end());
    CHECK(prevres2 == prevexp2);

    auto prevres3 = qpsf.getPrevious(1);
    vector<int> prevexp3{  };
    CHECK(prevres3 == prevexp3);

    auto prevres4 = qpsf.getPrevious(16);
    vector<int> prevexp4{ 2 };
    CHECK(prevres4 == prevexp4);

    auto allres = qpsf.getAllNext();
    sort(allres.begin(), allres.end());
    /*for (auto& res : allres) {
        cout << res.first << ", " << res.second << endl;
    }*/
    vector<pair<int, int>> allexp;
    allexp.push_back(pair(1, 2));
    allexp.push_back(pair(2, 3));
    allexp.push_back(pair(2, 16));
    allexp.push_back(pair(3, 4));
    allexp.push_back(pair(3, 6));
    allexp.push_back(pair(4, 5));
    allexp.push_back(pair(5, 2));
    allexp.push_back(pair(6, 7));
    allexp.push_back(pair(7, 8));
    allexp.push_back(pair(7, 15));
    allexp.push_back(pair(8, 9));
    allexp.push_back(pair(9, 10));
    allexp.push_back(pair(10, 11));
    allexp.push_back(pair(11, 12));
    allexp.push_back(pair(11, 13));
    allexp.push_back(pair(12, 7));
    allexp.push_back(pair(13, 14));
    allexp.push_back(pair(14, 7));
    allexp.push_back(pair(15, 2));
    allexp.push_back(pair(17, 18));
    CHECK(allres == allexp);
}

TEST_CASE("generateCfgs() multinest and affects methods working test") {
    // PKB need procedures + stmtNoProcChild + ifThen + ifElse + ifStatements + whileStatements + whileChildren
    const char* input =
        R"(procedure proc3dure {
            1varB = a + varB1;
            2while (Z123 > 0) {
            3    if (print < 10) then{
            4        print = print + 9;
            5        Z123 = print + print * varB + varB1;
                 } else {
            6        helloWorld = 57660785;
            7        while (Z123 < 100) {
            8            print varB1;
            9            read varB2;
            10           varB1 = varB1 * varB2;
            11           if ((varB2 < read) || (Z123 > 10)) then{
            12              varB1 = varB1 * 3 + varB2;
                         } else {
            13               a=1;
            14               b=2;
                         }
                     }
            15       read = helloWorld / helloWorld;
                }
            }
        16  call procedure2;
        }
        procedure procedure2{
        17call procedure3;
        18var = var * 3 + 14 / varB1;
        })";
    PKB pkb;
    shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    spf->addProcedure("proc3dure");
    spf->addProcedure("procedure2");
    spf->addStatement(2, StatementType::StatementWhile);
    spf->addParentChild(2, 3);
    spf->addParentChild(2, 15);
    spf->addStatement(3, StatementType::StatementIf);
    spf->addIfThen(3, 4);
    spf->addIfThen(3, 5);
    spf->addIfElse(3, 6);
    spf->addIfElse(3, 7);
    spf->addIfElse(3, 15);
    spf->addStatement(7, StatementType::StatementWhile);
    spf->addParentChild(7, 8);
    spf->addParentChild(7, 9);
    spf->addParentChild(7, 10);
    spf->addParentChild(7, 11);
    spf->addStatement(11, StatementType::StatementIf);
    spf->addIfThen(11, 12);
    spf->addIfElse(11, 13);
    spf->addIfElse(11, 14);
    for (int i = 1; i <= 16; i++) {
        spf->addProcChild("proc3dure", i);
    }
    spf->addProcChild("procedure2", 17);
    spf->addProcChild("procedure2", 18);
    CFGFacade cfgFacade = pkb.getCFGFacade();
    cfgFacade.generateCfgs();
    cfgFacade.populateRelationshipManager();
    CHECK(qpsf.isNextStar(1, 12));
    CHECK(qpsf.isNextStar(1, 14));
    CHECK(qpsf.isNextStar(1, 16));
    CHECK(!qpsf.isNextStar(2, 1));
    CHECK(qpsf.isNextStar(5, 4));
    CHECK(qpsf.isNextStar(12, 5));
    CHECK(qpsf.isNextStar(14, 12));
    CHECK(qpsf.isNextStar(14, 8));
    CHECK(qpsf.isNextStar(8, 14));
    CHECK(qpsf.isNextStar(15, 4));
    CHECK(!qpsf.isNextStar(16, 2));
    CHECK(qpsf.isNextStar(11, 14));

    auto res = qpsf.getNextStar(12);
    vector<int> exp{ 15,2,3,4,5,6,7,8,9,10,11,12,13,14,16 };
    sort(res.begin(), res.end());
    sort(exp.begin(), exp.end());
    CHECK(res == exp);

    auto res1 = qpsf.getNextStar(4);
    vector<int> exp1{ 5,2,16,3,4,6,7,8,9,10,11,12,13,14,15 };
    sort(res1.begin(), res1.end());
    sort(exp1.begin(), exp1.end());
    CHECK(res1 == exp1);

    auto res2 = qpsf.getPreviousStar(16);
    vector<int> exp2{ 1,2,15,7,14,12,13,11,10,9,8,6,5,4,3 };
    sort(res2.begin(), res2.end());
    sort(exp2.begin(), exp2.end());
    CHECK(res2 == exp2);

    auto res3 = qpsf.getNextStar(16);
    vector<int> exp3{  };
    CHECK(res3 == exp3);

    auto allresstar = qpsf.getAllNextStar();
    sort(allresstar.begin(), allresstar.end());
    /*for (auto& res : allresstar) {
        cout << res.first << ", " << res.second << endl;
    }*/
}


TEST_CASE("generateCfgs() simpler and affects working test") {
    // PKB need procedures + stmtNoProcChild + ifThen + ifElse + ifStatements + whileStatements + whileChildren
    const char* input =
      R"(procedure proc3dure {
        1   varB = a + varB1;
        2   var = 2;
        3   while (Z123 > 0) {
        4       call procedure2;
            }
        5   b=2;
        6   if (print < 10) then{
        7       print = print + 9;
        8       varB = helloWorld / varB;
            } else {
        9       helloWorld = 57660785;
            }
        10  Z123 = print + print * varB + var;
        11  print varB1;
        12  read varB2;
        }

        procedure procedure2{
        13    var = var * 3 + 14 / varB1;
        })";
    PKB pkb;
    // shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    std::string path = R"(..\\..\\..\\..\\Tests09\\integration_test\\pkb\\cfggenerator\\)";
    // change before pushing
    // cout << path << endl;
    // std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(testCfgGeneratorLocalPath + "simple.txt");
    std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(path + "simple.txt");
    SpManager spManager;
    AstRoot root = spManager.processSIMPLE(src);
    spManager.extractAndPopulate(root, pkb);
    CFGFacade cfgFacade = pkb.getCFGFacade();
    cfgFacade.generateCfgs();
    cfgFacade.populateRelationshipManager();
    auto varmod = qpsf.getModifies(1);
    auto varuses = qpsf.getUses(8);
    /*for (auto& var : varmod) {
        cout << var << endl;
    }
    for (auto& var : varuses) {
        cout << var << endl;
    }
    cout << "next of 1:" << qpsf.getNext(1)[0]<< endl;*/
    CHECK(qpsf.isAffects(1, 8));
    // cout << "var isModifies in 3: " << qpsf.isModifies(3, "var") << endl;
    CHECK(qpsf.isAffects(2, 10));
    CHECK(qpsf.isAffects(7, 10));
    CHECK(!qpsf.isAffects(3, 10));//false cos 3 is not assign
}

TEST_CASE("generateCfgs() complex and affects working test") {
    // PKB need procedures + stmtNoProcChild + ifThen + ifElse + ifStatements + whileStatements + whileChildren
    const char* input =
      R"(procedure proc1 {
         1  var1 = 123;
         2  var2 = var1 + 2;
         3  if(x>0) then {
         4      var5 = var2+var1;
         5      var1 = var1*15;
            } else {
         6      call proc2;
            }
         7  var6 = var2/5;
         8  while(var5 !=0){
         9     test = var5/var2;
         10    var6 = var6*99;
            }
         11 test3 = var6;
         12 test4 = var1;
        }

        procedure proc2 {
         13 var3 = 5;
         14 var2 = var3 * 5;
         15 var1 = var3;
        })";
    PKB pkb;
    // shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    std::string path = R"(..\\..\\..\\..\\Tests09\\integration_test\\pkb\\cfggenerator\\)";
    // change before pushing
    // cout << path << endl;
    // std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(testCfgGeneratorLocalPath + "complex_affects.txt");
    std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(path + "complex_affects.txt");
    SpManager spManager;
    AstRoot root = spManager.processSIMPLE(src);
    spManager.extractAndPopulate(root, pkb);
    CFGFacade cfgFacade = pkb.getCFGFacade();
    cfgFacade.generateCfgs();
    cfgFacade.populateRelationshipManager();

    auto res = qpsf.getAffectsAffected(1);
    vector<int> exp{ 2,4,5 };
    sort(res.begin(), res.end());
    CHECK(res == exp);

    auto res1 = qpsf.getAffectsAffected(2);
    vector<int> exp1{ 4,7,9 };
    sort(res1.begin(), res1.end());
    CHECK(res1 == exp1);

    auto res2 = qpsf.getAffectsCause(12);
    vector<int> exp2{ 5 };
    CHECK(res2 == exp2);

    auto res3 = qpsf.getAffectsCause(11);
    vector<int> exp3{ 7,10 };
    sort(res3.begin(), res3.end());
    CHECK(res3 == exp3);

    auto res4 = qpsf.getAffectsCause(5);
    vector<int> exp4{ 1 };
    CHECK(res4 == exp4);
    res4 = qpsf.getAffectsCause(2);
    CHECK(res4 == exp4);

    auto res5 = qpsf.getAffectsCause(4);
    vector<int> exp5{ 1,2 };
    sort(res5.begin(), res5.end());
    CHECK(res5 == exp5);

    auto testing = qpsf.getPreviousStar(9);
    auto res6 = qpsf.getAffectsCause(9);
    vector<int> exp6{ 2,4 };
    sort(res6.begin(), res6.end());
    CHECK(res6 == exp6);

    auto res7 = qpsf.getAllAffects();
    vector<pair<int,int>> exp7;
    exp7.push_back(pair(1,2));
    exp7.push_back(pair(1, 4));
    exp7.push_back(pair(1, 5));
    exp7.push_back(pair(2, 4));
    exp7.push_back(pair(2, 7));
    exp7.push_back(pair(2, 9));
    exp7.push_back(pair(4, 9));
    exp7.push_back(pair(5, 12));
    exp7.push_back(pair(7, 10));
    exp7.push_back(pair(7, 11));
    exp7.push_back(pair(10, 10));
    exp7.push_back(pair(10,11));
    exp7.push_back(pair(13, 14));
    exp7.push_back(pair(13,15));
    sort(res7.begin(), res7.end());
    sort(exp7.begin(), exp7.end());
    CHECK(res7 == exp7);
}

TEST_CASE("generateCfgs() complex and affects* working test") {
    // PKB need procedures + stmtNoProcChild + ifThen + ifElse + ifStatements + whileStatements + whileChildren
    const char* input =
        R"(procedure proc1 {
         1  var1 = 123;
         2  var2 = var1 + 2;
         3  if(x>0) then {
         4      var5 = var2+var1;
         5      var1 = var1*15;
            } else {
         6      call proc2;
            }
         7  var6 = var2/5;
         8  while(var5 !=0){
         9     test = var5/var2;
         10    var6 = var6*99;
            }
         11 test3 = var6;
         12 test4 = var1;
        }

        procedure proc2 {
         13 var3 = 5;
         14 var2 = var3 * 5;
         15 var1 = var3;
        })";
    PKB pkb;
    // shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    std::string path = R"(..\\..\\..\\..\\Tests09\\integration_test\\pkb\\cfggenerator\\)";
    // change before pushing
    cout << "complex" << endl;
    // std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(testCfgGeneratorLocalPath + "complex_affects.txt");
    std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(path + "complex_affects.txt");
    SpManager spManager;
    AstRoot root = spManager.processSIMPLE(src);
    spManager.extractAndPopulate(root, pkb);
    CFGFacade cfgFacade = pkb.getCFGFacade();
    cfgFacade.generateCfgs();
    cfgFacade.populateRelationshipManager();
    CHECK(qpsf.isAffectsStar(1, 12));
    CHECK(qpsf.isAffectsStar(1, 7));
    CHECK(qpsf.isAffectsStar(1, 9));

    auto res = qpsf.getAffectsStarAffected(1);
    vector<int> exp{ 2,4,5,7,9,10,11,12 };
    sort(res.begin(), res.end());
    CHECK(res == exp);

    auto res1 = qpsf.getAffectsStarAffected(2);
    vector<int> exp1{ 4,7,9,10,11 };
    sort(res1.begin(), res1.end());
    CHECK(res1 == exp1);

    auto res2 = qpsf.getAffectsStarCause(12);
    vector<int> exp2{ 1,5 };
    sort(res2.begin(), res2.end());
    CHECK(res2 == exp2);

    auto res3 = qpsf.getAffectsStarCause(11);
    vector<int> exp3{ 1,2,7,10 };
    sort(res3.begin(), res3.end());
    CHECK(res3 == exp3);
}

TEST_CASE("Affects test case") {
    PKB pkb;
    // shared_ptr<SPFacade> spf = pkb.getSPFacade();
    QPSFacade qpsf = pkb.getQPSFacade();
    std::string path = R"(..\\..\\..\\..\\Tests09\\integration_test\\pkb\\cfggenerator\\)";
    // change before pushing
    cout << "short" << endl;
    // std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(testCfgGeneratorLocalPath + "affects-short_source.txt");
    std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(path + "affects-short_source.txt");
    SpManager spManager;
    AstRoot root = spManager.processSIMPLE(src);
    spManager.extractAndPopulate(root, pkb);
    CFGFacade cfgFacade = pkb.getCFGFacade();
    cfgFacade.generateCfgs();
    cfgFacade.populateRelationshipManager();

    auto res = qpsf.getAffectsAffected(11);
    vector<int> exp{ 13 };
    CHECK(res == exp);

    auto res1 = qpsf.getAffectsCause(13);
    vector<int> exp1{ 11 };
    CHECK(res1 == exp1);
}