#include "sp/Parser.h"
#include "sp/Lexer.h"
#include "ast/TNode.h"
#include "catch.hpp"
#include <string>
#include <fstream>
#include <memory>
#include <sstream>
using namespace std;

std::string filePath = "../../../../Tests09/integration_test/sp/parser/";


TEST_CASE("basic syntax errors") {
    SECTION("no procedure") {
        string input = string("hello       world23     \n        2332wzasd   \n  asd");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> l = make_shared<Lexer>(iss);
        Parser p = Parser(l);
        AstRoot root = std::make_shared<RootNode>(nullptr);
        REQUIRE_THROWS_WITH(p.parse(root), Catch::Matchers::Contains("expected: procedure"));
    }

    SECTION("invalid identifier") {
        string input = string("procedure 132");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> l = make_shared<Lexer>(iss);
        Parser p = Parser(l);
        AstRoot root = std::make_shared<RootNode>(nullptr);
        REQUIRE_THROWS_WITH(p.parse(root), Catch::Matchers::Contains("received: 132"));
    }

    SECTION("no {") {
        string input = string("procedure proc s");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> l = make_shared<Lexer>(iss);
        Parser p = Parser(l);
        AstRoot root = std::make_shared<RootNode>(nullptr);
        REQUIRE_THROWS_WITH(p.parse(root), Catch::Matchers::Contains("expected: {, got: s"));
    }

    SECTION("no semicolon") {
        string input = string("procedure proc { \n  read x \n print y");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> l = make_shared<Lexer>(iss);
        Parser p = Parser(l);
        AstRoot root = std::make_shared<RootNode>(nullptr);
        REQUIRE_THROWS_WITH(p.parse(root), Catch::Matchers::Contains("expected: ;, got: print"));
    }

    SECTION("assign no =") {
        string input = string("procedure proc { \n  read x; \n x 3;");
        shared_ptr<istringstream> iss = make_shared<istringstream>(input);
        shared_ptr<Lexer> l = make_shared<Lexer>(iss);
        Parser p = Parser(l);
        AstRoot root = std::make_shared<RootNode>(nullptr);
        REQUIRE_THROWS_WITH(p.parse(root), Catch::Matchers::Contains("line 3: Invalid statement found"));
    }

}

TEST_CASE("correct_syntax_1") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "correct_syntax_1.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);
    std::string correctStr = std::string("======================== PROCEDURE foo3 ========================\n")
            + "read x\n"
            + "read y\n"
            + "x:=3\n"
            + "x:=y\n"
            + "print z\n"
            + "#### end procedure ####\n";
    REQUIRE(root->toString() ==  correctStr);
}


TEST_CASE("correct_syntax_spaces_newline") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "correct_syntax_spaces_newline.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);
    std::string correctStr = std::string("======================== PROCEDURE foo3 ========================\n")
                             + "read x\n"
                             + "read y\n"
                             + "x:=3\n"
                             + "x:=y\n"
                             + "print z\n"
                             + "#### end procedure ####\n";
    REQUIRE(root->toString() ==  correctStr);
}

TEST_CASE("correct_syntax_minified") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "correct_syntax_minified.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);
    std::string correctStr = std::string("======================== PROCEDURE foo3 ========================\n")
                             + "read x\n"
                             + "read y\n"
                             + "x:=3\n"
                             + "x:=y\n"
                             + "print z\n"
                             + "#### end procedure ####\n";
    REQUIRE(root->toString() ==  correctStr);
}


TEST_CASE("if_stmt1") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "if_stmt1.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    string expected = string("======================== PROCEDURE bar ========================\n")
        + "rootNode:=proc2\n"
        + "proc2:=stmt1\n"
        + "x:=3\n"
        + "y:=4\n"
        + "if: x < 4\n"
        + "v:=4\n"
        + "if: y > 69\n"
        + "x:=3\n"
        + "q:=99\n"
        + "else:\n"
        + "hello:=world\n"
        + "big:=bang\n"
        + "#### end if ####\n"
        + "else:\n"
        + "q:=asd\n"
        + "#### end if ####\n"
        + "#### end procedure ####\n";
    REQUIRE(root->toString() ==  expected);
}

TEST_CASE("if_stmt2 deeply nested") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "if_stmt2_nested.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    string expected = string("======================== PROCEDURE bar ========================\n")
            + "rootNode:=proc2\n"
            + "proc2:=stmt1\n"
            + "x:=3\n"
            + "y:=4\n"
            + "if: x <= 4\n"
            + "if: y >= 69\n"
            + "if: z < 5\n"
            + "if: cop == q\n"
            + "kobe:=24\n"
            + "else:\n"
            + "b:=123\n"
            + "#### end if ####\n"
            + "else:\n"
            + "happy:=99\n"
            + "#### end if ####\n"
            + "else:\n"
            + "g:=3\n"
            + "#### end if ####\n"
            + "else:\n"
            + "q:=asd\n"
            + "#### end if ####\n"
            + "#### end procedure ####\n";

    REQUIRE(root->toString() ==  expected);
}

TEST_CASE("if_stmt3 nested conditional ") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "if_stmt3_nested_condExpr.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    string expected = string("======================== PROCEDURE bar ========================\n")
            + "rootNode:=proc2\n"
            + "proc2:=stmt1\n"
            + "x:=3\n"
            + "y:=4\n"
            + "if: ((cop == q && !(((cop == q && cop == q) || (cop == q && cop == q)))) || (cop == q && cop == q))\n"
            + "if: !(!(!(y >= 69)))\n"
            + "if: (z < 5 && !((3 < 5 || x > 23)))\n"
            + "if: ((cop == q && cop == q) || (cop == q && cop == q))\n"
            + "kobe:=24\n"
            + "else:\n"
            + "b:=123\n"
            + "#### end if ####\n"
            + "else:\n"
            + "happy:=99\n"
            + "#### end if ####\n"
            + "else:\n"
            + "g:=3\n"
            + "#### end if ####\n"
            + "else:\n"
            + "q:=asd\n"
            + "#### end if ####\n"
            + "#### end procedure ####\n";
    REQUIRE(root->toString() ==  expected);
}

TEST_CASE("while_stmt1") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "while_stmt1.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    string expected = string("======================== PROCEDURE bar ========================\n")
                      + "x:=3\n"
                      + "y:=4\n"
                      + "if: x < 4\n"
                      + "v:=4\n"
                      + "else:\n"
                      + "q:=asd\n"
                      + "#### end if ####\n"
                      + "while: (x < y && !(y > 99))\n"
                      + "x:=4\n"
                      + "#### end while ####\n"
                      + "while: hello <= world\n"
                      + "x:=4\n"
                      + "hello:=p\n"
                      + "#### end while ####\n"
                      + "#### end procedure ####\n";

    REQUIRE(root->toString() ==  expected);
}

TEST_CASE("while_stmt2 nested") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "while_stmt2_nested.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    string expected = string("======================== PROCEDURE bar ========================\n")
            + "x:=3\n"
            + "y:=4\n"
            + "if: x < 4\n"
            + "v:=4\n"
            + "else:\n"
            + "q:=asd\n"
            + "#### end if ####\n"
            + "while: (x < y && !(y > 99))\n"
            + "x:=4\n"
            + "while: hello <= world\n"
            + "hello:=p\n"
            + "while: if > while\n"
            + "test:=3\n"
            + "#### end while ####\n"
            + "k:=3\n"
            + "#### end while ####\n"
            + "read asd\n"
            + "a:=b\n"
            + "c:=d\n"
            + "#### end while ####\n"
            + "#### end procedure ####\n";

    REQUIRE(root->toString() ==  expected);
}

TEST_CASE("statement_num1") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "statement_num1.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);


    Procedure proc = root->getProcedures().at(0);
    auto stmts = proc->getStmts();
    auto root_eq_proc = stmts.at(0);
    REQUIRE(root_eq_proc->getStmtNo() == 1);
    auto proc_eq_stmt = stmts.at(1);
    REQUIRE(proc_eq_stmt->getStmtNo() == 2);
    auto x_eq_3 = stmts.at(2);
    REQUIRE(x_eq_3->getStmtNo() == 3);
    auto x = static_cast<AssignNode*>(x_eq_3.get())->getLhs();
    REQUIRE(x->getStmtNo() == 3);
    auto const_3 = static_cast<AssignNode*>(x_eq_3.get())->getRhs();
    REQUIRE(const_3->getStmtNo() == 3);

    //if block
    auto if1 = static_cast<IfNode*>(stmts.at(3).get());
    REQUIRE(if1->getStmtNo() == 4);
    auto condExpr1 = if1->getCondExpr();
    REQUIRE(condExpr1->getStmtNo() == 4);
    auto if1_then = if1->getThenStmts();
    auto if2 = static_cast<IfNode*>(if1_then.at(0).get());
    REQUIRE(if2->getStmtNo() == 5);
    auto if2_then = if2->getThenStmts();

    auto if3 = static_cast<IfNode*>(if2_then.at(0).get());
    REQUIRE(if3->getStmtNo() == 6);
    auto if3_then = if3->getThenStmts();
    auto kobe_24 = static_cast<AssignNode*>(if3_then.at(0).get());
    REQUIRE(kobe_24->getStmtNo() == 7);
    auto if3_else = if3->getElseStmts();
    auto happy_99 = static_cast<AssignNode*>(if3_else.at(0).get());
    REQUIRE(happy_99->getStmtNo() == 8);

    auto if2_else = if2->getElseStmts();
    auto g_3 = static_cast<AssignNode*>(if2_else.at(0).get());
    REQUIRE(g_3->getStmtNo() == 9);

    auto qq_pp = static_cast<AssignNode*>(if1_then.at(1).get());
    REQUIRE(qq_pp->getStmtNo() == 10);


    auto if1_else = if1->getElseStmts();
    auto q_asd = static_cast<AssignNode*>(if1_else.at(0).get());
    REQUIRE(q_asd->getStmtNo() == 11);

    //while block
    auto while1 = static_cast<WhileNode*>(stmts.at(4).get());
    REQUIRE(while1->getStmtNo() == 12);
    REQUIRE(while1->getCondExpr()->getStmtNo() == 12);
    auto whileStmts = while1->getStmts();

    auto while_if = static_cast<IfNode*>(whileStmts.at(0).get());
    REQUIRE(while_if->getStmtNo() == 13);

    auto while_if_then = while_if->getThenStmts();
    auto while2 = static_cast<WhileNode*>(while_if_then.at(0).get());
    REQUIRE(while2->getStmtNo() == 14);

    auto while2_stmts = while2->getStmts();
    REQUIRE(while2_stmts.at(0)->getStmtNo() == 15); //print 87

    REQUIRE(while_if_then.at(1)->getStmtNo() == 16); //read x

    auto while_if_else = while_if->getElseStmts();
    REQUIRE(while_if_else.at(0)->getStmtNo() == 17); // v = x

    REQUIRE(whileStmts.at(1)->getStmtNo() == 18);
}

TEST_CASE("expression_all_operators assignment") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "expression_all_operators.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    string expected = string("======================== PROCEDURE bar ========================\n")
                      + "x:=3 + 4 * 6 - 6 / 2\n"
                      + "y:=4 % 2 + 4 / 2\n"
                      + "z:=l + l - r / 2\n"
                      + "if: !(f % g + h - i / j * k < d / e % f + g * h - i - j)\n"
                      + "x:=f % g + h - i / j * k + you + me\n"
                      + "else:\n"
                      + "hello:=world + you\n"
                      + "#### end if ####\n"
                      + "while: !((x / y % z + a + b * c - d > 3 && q + r / s - t % u * v - w < 69))\n"
                      + "qps:=k - l % m * n + o + p\n"
                      + "sp:=d / e % f + g * h - i - j\n"
                      + "#### end while ####\n"
                      + "#### end procedure ####\n";
    REQUIRE(root->toString() ==  expected);
}

TEST_CASE("expression have correct stmt number ") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "expression_stmt_no.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);
    auto bar = root->getProcedures().at(0);

    // assign
    auto assign = static_cast<AssignNode*>(bar->getStmts().at(0).get());
    auto a_rhs = static_cast<BinaryOpNode*>(assign->getRhs().get());
    REQUIRE(assign->getStmtNo() == 1);
    REQUIRE(a_rhs->getStmtNo() == 1);
    auto divide = static_cast<BinaryOpNode*>(a_rhs->getRhs().get());
    REQUIRE(divide->getStmtNo() == 1);
    auto two = static_cast<ConstantNode*>(divide->getRhs().get());
    auto six = static_cast<ConstantNode*>(divide->getLhs().get());
    REQUIRE(two->getStmtNo() == 1);
    REQUIRE(two->getConstant() == "2");
    REQUIRE(six->getStmtNo() == 1);
    REQUIRE(six->getConstant() == "6");

    // while
    auto whileNode = static_cast<WhileNode*>(bar->getStmts().at(1).get());
    auto whileExpr = static_cast<BinaryOpNode*>(whileNode->getCondExpr()->getRelExpr()->getLhs().get());
    REQUIRE(whileExpr->getStmtNo() == 2);

    // if
    auto ifNode = static_cast<IfNode*>(bar->getStmts().at(2).get());
    auto ifExpr = static_cast<BinaryOpNode*>(ifNode->getCondExpr()->getLhs()->getRelExpr()->getLhs().get());
    REQUIRE(ifExpr->getStmtNo() == 4);
}

TEST_CASE("it should parse call statements") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "call_stmts_1.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);
    auto computeCentroid = root->getProcedures().at(0)->getStmts();

    // call read point
    auto callReadPoint = computeCentroid.at(3);
    auto callReadPointCasted = static_cast<CallNode*>(callReadPoint.get());
    REQUIRE(callReadPointCasted->getProcName() == "readPoint");

    // call read point 2
    auto whileStmt = root->getProcedures().at(0)->getStmts().at(4);
    auto whileStmtCasted = static_cast<WhileNode*>(whileStmt.get());
    auto callReadPoint2 = whileStmtCasted->getStmts().at(3);
    auto callReadPoint2Casted = static_cast<CallNode*>(callReadPoint2.get());
    REQUIRE(callReadPoint2Casted->getProcName() == "foobar");

    string expected = string("======================== PROCEDURE computeCentroid ========================\n")
                      + "count:=0\n"
                      + "cenX:=0\n"
                      + "cenY:=0\n"
                      + "call readPoint\n"
                      + "while: (x != 0 && y != 0)\n"
                      + "count:=count + 1\n"
                      + "call century\n"
                      + "cenY:=cenY + y\n"
                      + "call foobar\n"
                      + "#### end while ####\n"
                      + "if: count == 0\n"
                      + "call flag\n"
                      + "else:\n"
                      + "call main\n"
                      + "cenY:=cenY / count\n"
                      + "#### end if ####\n"
                      + "normSq:=cenX * cenX + cenY * cenY\n"
                      + "call bangbangbang\n"
                      + "#### end procedure ####\n";
    REQUIRE(root->toString() ==  expected);
}

TEST_CASE("it should parse multiple procedures and have correct statement numbers") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "multiple_procedures.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);
    auto main = root->getProcedures().at(0)->getStmts();
    auto readPoint = root->getProcedures().at(1)->getStmts();
    auto printResults = root->getProcedures().at(2)->getStmts();

    string expected = string("======================== PROCEDURE main ========================\n")
                      + "flag:=0\n"
                      + "call computeCentroid\n"
                      + "call printResults\n"
                      + "#### end procedure ####\n"
                      + "======================== PROCEDURE readPoint ========================\n"
                      + "read x\n"
                      + "read y\n"
                      + "#### end procedure ####\n"
                      + "======================== PROCEDURE printResults ========================\n"
                      + "print flag\n"
                      + "print cenX\n"
                      + "print cenY\n"
                      + "print normSq\n"
                      + "#### end procedure ####\n";

    REQUIRE(readPoint.at(0)->getStmtNo() == 4);

    REQUIRE(printResults.at(0)->getStmtNo() == 6);

    REQUIRE(root->toString() ==  expected);
}

TEST_CASE("it should differentiate between top level relational expr "
          "with redundant brackets and AND_OR conditional expressions") {
    shared_ptr<ifstream> src = make_shared<ifstream>(filePath + "expression_redundant_brackets.txt");
    shared_ptr<Lexer> l = make_shared<Lexer>(src);
    Parser p = Parser(l);
    AstRoot root = std::make_shared<RootNode>(nullptr);
    p.parse(root);

    string expected = string("======================== PROCEDURE A ========================\n")
                             + "while: i != 0\n"
                             + "while: x % 5 == y\n"
                             + "while: a >= 5 + x\n"
                             + "call C\n"
                             + "#### end while ####\n"
                             + "if: b * 3 < 2\n"
                             + "c:=c + 20\n"
                             + "else:\n"
                             + "c:=d\n"
                             + "#### end if ####\n"
                             + "#### end while ####\n"
                             + "if: x + z / 2 != d\n"
                             + "while: x + q <= z + c\n"
                             + "c:=3\n"
                             + "#### end while ####\n"
                             + "else:\n"
                             + "c:=4\n"
                             + "#### end if ####\n"
                             + "#### end while ####\n"
                             + "if: (a + d > 10 || a - d + y > 20)\n"
                             + "if: c / b == 10\n"
                             + "y:=5\n"
                             + "else:\n"
                             + "while: z + x + y != 20\n"
                             + "a:=20\n"
                             + "#### end while ####\n"
                             + "#### end if ####\n"
                             + "else:\n"
                             + "while: 5 % a == 0\n"
                             + "if: (a + b > 10 && c + d < 50)\n"
                             + "a:=b\n"
                             + "else:\n"
                             + "b:=c\n"
                             + "#### end if ####\n"
                             + "while: e + e < 20\n"
                             + "z:=a / b % c\n"
                             + "#### end while ####\n"
                             + "print z\n"
                             + "#### end while ####\n"
                             + "#### end if ####\n"
                             + "#### end procedure ####\n";

    REQUIRE(root->toString() == expected);
}
