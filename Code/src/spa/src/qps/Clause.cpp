#include "Clause.h"

Clause::Clause(std::string lhs, std::string rhs) : LHS(lhs), RHS(rhs) {
    // to be implemented
};
FollowsClause::FollowsClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "FollowsClause";
}

FollowsTClause::FollowsTClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "FollowsTClause";
}

ParentClause::ParentClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "ParentClause";
}

ParentTClause::ParentTClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "ParentTClause";
}

UsesClause::UsesClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "UsesClause";
}

ModifiesClause::ModifiesClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "ModifiesClause";
}

CallsClause::CallsClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "CallsClause";
}

CallsTClause::CallsTClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "CallsTClause";
}

AssignPatternClause::AssignPatternClause(std::string lhs, std::string rhs, std::string assignSyn) : Clause(lhs, rhs), assignSynonym(assignSyn) {
    clauseType = "AssignPatternClause";
}

IfPatternClause::IfPatternClause(std::string lhs, std::string rhs, std::string third, std::string assignSyn) : Clause(lhs, rhs), third(third), assignSynonym(assignSyn) {
    clauseType = "IfPatternClause";
}

WhilePatternClause::WhilePatternClause(std::string lhs, std::string rhs, std::string assignSyn) : Clause(lhs, rhs), assignSynonym(assignSyn) {
    clauseType = "WhilePatternClause";
}

NextClause::NextClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "NextClause";
}

NextTClause::NextTClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "NextTClause";
}

AffectsClause::AffectsClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "AffectsClause";
}

AffectsTClause::AffectsTClause(std::string lhs, std::string rhs) : Clause(lhs, rhs) {
    clauseType = "AffectsTClause";
}

WithClause::WithClause(std::string leftType, std::string leftName, std::string rightType, std::string rightName) : Clause(leftName, rightName), leftType(leftType), rightType(rightType) {
    clauseType = "WithClause";
}

void Clause::printClause() {
    std::cout << clauseType << "(" + LHS + "," + RHS + ")" << std::endl;
}

std::string Clause::getLHS() {
    return LHS;
}

std::string Clause::getRHS() {
    return RHS;
}

std::string Clause::getAssignSynonym() {
    return assignSyn;
}

int Clause::getPriorityScore() {
    return priorityScore;
}

void Clause::setPriorityScore(int ps) {
    priorityScore = ps;
}

bool Clause::isSynonym(std::string input) {  // TODO implement assingTypeToArg
    // return input[0] != '"' && !std::isdigit(input[0]);
    return isalpha(input[0]) || isWildcard(input);
}

bool Clause::isSubstring(std::string input) {  // TODO implement assignTypeToArg
    return input[0] == '_' && input.length() > 1;
}

bool Clause::isWildcard(std::string input) {  // TODO implement assignTypeToArg
    return input[0] == '_' && input.length() == 1;
}

bool Clause::isConstant(std::string token) {
    char firstChar = token[0];
    if (firstChar == '0') {
        return false;
    }

    for (char i : token) {
        if (!isdigit(i)) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> Clause::intArrToStringArr(std::vector<int> arr) {
    std::vector < std::string > res;
    for (const auto& integer : arr) {
        res.push_back(std::to_string(integer));
    }
    return res;
}

// methods for pattern clause
std::string AssignPatternClause::getAssignSynonym() {
    return assignSynonym;
}
std::string IfPatternClause::getAssignSynonym() {
    return assignSynonym;
}
std::string WhilePatternClause::getAssignSynonym() {
    return assignSynonym;
}

std::string IfPatternClause::getThird() {
    return third;
}

ClauseResult Clause::processTupleList(std::vector<std::pair<int, int>> tupleListIntInt,
                                      std::vector<std::pair<int, std::string>> tupleListIntStr,
                                      std::string LHSSynType, std::string RHSSynType,
                                      QPSFacade& qpsFacade) {
    std::vector<int> ifs = qpsFacade.getIfStatements();
    std::vector<int> whiles = qpsFacade.getWhileStatements();
    std::vector<int> prints = qpsFacade.getPrintStatements();
    std::vector<int> reads = qpsFacade.getReadStatements();
    std::vector<int> assigns = qpsFacade.getAssignStatements();
    std::vector<int> calls = qpsFacade.getCallStatements();
    if (LHSSynType == "if") {
        for (int i = 0; i < tupleListIntInt.size(); i++) {
            if (!(std::find(ifs.begin(), ifs.end(), tupleListIntInt[i].first) != ifs.end())) {
                tupleListIntInt.erase(tupleListIntInt.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < tupleListIntStr.size(); i++) {
            if (!(std::find(ifs.begin(), ifs.end(), tupleListIntStr[i].first) != ifs.end())) {
                tupleListIntStr.erase(tupleListIntStr.begin() + i);
                i--;
            }
        }
    }
    else if (LHSSynType == "while") {
        for (int i = 0; i < tupleListIntInt.size(); i++) {
            if (!(std::find(whiles.begin(), whiles.end(), tupleListIntInt[i].first) != whiles.end())) {
                tupleListIntInt.erase(tupleListIntInt.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < tupleListIntStr.size(); i++) {
            if (!(std::find(whiles.begin(), whiles.end(), tupleListIntStr[i].first) != whiles.end())) {
                tupleListIntStr.erase(tupleListIntStr.begin() + i);
                i--;
            }
        }
    }
    else if (LHSSynType == "read") {
        for (int i = 0; i < tupleListIntInt.size(); i++) {
            if (!(std::find(reads.begin(), reads.end(), tupleListIntInt[i].first) != reads.end())) {
                tupleListIntInt.erase(tupleListIntInt.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < tupleListIntStr.size(); i++) {
            if (!(std::find(reads.begin(), reads.end(), tupleListIntStr[i].first) != reads.end())) {
                tupleListIntStr.erase(tupleListIntStr.begin() + i);
                i--;
            }
        }
    }
    else if (LHSSynType == "print") {
        for (int i = 0; i < tupleListIntInt.size(); i++) {
            if (!(std::find(prints.begin(), prints.end(), tupleListIntInt[i].first) != prints.end())) {
                tupleListIntInt.erase(tupleListIntInt.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < tupleListIntStr.size(); i++) {
            if (!(std::find(prints.begin(), prints.end(), tupleListIntStr[i].first) != prints.end())) {
                tupleListIntStr.erase(tupleListIntStr.begin() + i);
                i--;
            }
        }
    }
    else if (LHSSynType == "assign") {
        for (int i = 0; i < tupleListIntInt.size(); i++) {
            if (!(std::find(assigns.begin(), assigns.end(), tupleListIntInt[i].first) != assigns.end())) {
                tupleListIntInt.erase(tupleListIntInt.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < tupleListIntStr.size(); i++) {
            if (!(std::find(assigns.begin(), assigns.end(), tupleListIntStr[i].first) != assigns.end())) {
                tupleListIntStr.erase(tupleListIntStr.begin() + i);
                i--;
            }
        }
    }
    else if (LHSSynType == "call") {
        for (int i = 0; i < tupleListIntStr.size(); i++) {
            if (!(std::find(calls.begin(), calls.end(), tupleListIntStr[i].first) != calls.end())) {
                tupleListIntStr.erase(tupleListIntStr.begin() + i);
                i--;
            }
        }
    }
    if (RHSSynType == "if") {
        for (int i = 0; i < tupleListIntInt.size(); i++) {
            if (!(std::find(ifs.begin(), ifs.end(), tupleListIntInt[i].second) != ifs.end())) {
                tupleListIntInt.erase(tupleListIntInt.begin() + i);
                i--;
            }
        }
    }
    else if (RHSSynType == "while") {
        for (int i = 0; i < tupleListIntInt.size(); i++) {
            if (!(std::find(whiles.begin(), whiles.end(), tupleListIntInt[i].second) != whiles.end())) {
                tupleListIntInt.erase(tupleListIntInt.begin() + i);
                i--;
            }
        }
    }
    else if (RHSSynType == "read") {
        for (int i = 0; i < tupleListIntInt.size(); i++) {
            if (!(std::find(reads.begin(), reads.end(), tupleListIntInt[i].second) != reads.end())) {
                tupleListIntInt.erase(tupleListIntInt.begin() + i);
                i--;
            }
        }
    }
    else if (RHSSynType == "print") {
        for (int i = 0; i < tupleListIntInt.size(); i++) {
            if (!(std::find(prints.begin(), prints.end(), tupleListIntInt[i].second) != prints.end())) {
                tupleListIntInt.erase(tupleListIntInt.begin() + i);
                i--;
            }
        }
    }
    else if (RHSSynType == "assign") {
        for (int i = 0; i < tupleListIntInt.size(); i++) {
            if (!(std::find(assigns.begin(), assigns.end(), tupleListIntInt[i].second) != assigns.end())) {
                tupleListIntInt.erase(tupleListIntInt.begin() + i);
                i--;
            }
        }
    }
    std::vector<std::vector<std::string>> list;
    for (std::pair<int, int> val : tupleListIntInt) {
        std::vector<std::string> temp;
        temp.push_back(std::to_string(val.first));
        temp.push_back(std::to_string(val.second));
        list.push_back(temp);
    }
    for (std::pair<int, std::string> val : tupleListIntStr) {
        std::vector<std::string> temp;
        temp.push_back(std::to_string(val.first));
        temp.push_back(val.second);
        list.push_back(temp);
    }
    std::vector<std::string> synList;
    synList.push_back(LHS);
    synList.push_back(RHS);
    return ClauseResultBuilder::createTupleResult(synList, list);
}

ClauseResult Clause::processList(std::vector<int> listInt, std::string LHSSynType, std::string RHSSynType, QPSFacade& qpsFacade) {
    std::vector<int> ifs = qpsFacade.getIfStatements();
    std::vector<int> whiles = qpsFacade.getWhileStatements();
    std::vector<int> prints = qpsFacade.getPrintStatements();
    std::vector<int> reads = qpsFacade.getReadStatements();
    std::vector<int> assigns = qpsFacade.getAssignStatements();
    std::vector<int> calls = qpsFacade.getCallStatements();
    std::vector<std::string> list;
    if (LHSSynType == "if") {
        for (int i = 0; i < listInt.size(); i++) {
            if (!(std::find(ifs.begin(), ifs.end(), listInt[i]) != ifs.end())) {
                listInt.erase(listInt.begin() + i);
                i--;
            }
        }
    }
    else if (LHSSynType == "while") {
        for (int i = 0; i < listInt.size(); i++) {
            if (!(std::find(whiles.begin(), whiles.end(), listInt[i]) != whiles.end())) {
                listInt.erase(listInt.begin() + i);
                i--;
            }
        }
    }
    else if (LHSSynType == "read") {
        for (int i = 0; i < listInt.size(); i++) {
            if (!(std::find(reads.begin(), reads.end(), listInt[i]) != reads.end())) {
                listInt.erase(listInt.begin() + i);
                i--;
            }
        }
    }
    else if (LHSSynType == "print") {
        for (int i = 0; i < listInt.size(); i++) {
            if (!(std::find(prints.begin(), prints.end(), listInt[i]) != prints.end())) {
                listInt.erase(listInt.begin() + i);
                i--;
            }
        }
    }
    else if (LHSSynType == "assign") {
        for (int i = 0; i < listInt.size(); i++) {
            if (!(std::find(assigns.begin(), assigns.end(), listInt[i]) != assigns.end())) {
                listInt.erase(listInt.begin() + i);
                i--;
            }
        }
    }
    else if (LHSSynType == "call") {
        for (int i = 0; i < listInt.size(); i++) {
            if (!(std::find(calls.begin(), calls.end(), listInt[i]) != calls.end())) {
                listInt.erase(listInt.begin() + i);
                i--;
            }
        }
    }
    for (int val : listInt) {
        list.push_back(std::to_string(val));
    }
    return ClauseResultBuilder::createListResult(isSynonym(LHS) ? LHS : RHS, list);
}

std::vector<std::pair<int, std::string>> WithClause::processWith(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes, std::string synonym, std::string attribute) {
    std::string synonymType = synonymTypes[synonym];
    std::vector<std::string> list;
    std::vector<std::pair<int, std::string>> listPair;

    if (synonymType == "call" && attribute == "procName") {
        auto calls = qpsFacade.getCallStatements();
        auto procs = qpsFacade.getProcedures();
        for (auto call : calls) {
            for (auto proc : procs) {
                if (proc == qpsFacade.getCallee(call)) {
                    std::pair<int, std::string> pair;
                    pair.first = call;
                    pair.second = proc;
                    listPair.push_back(pair);
                }
            }
        }
        return listPair;
    }
    else if (synonymType == "read" && attribute == "varName") {
        auto reads = qpsFacade.getReadStatements();
        auto vars = qpsFacade.getVariables();
        for (auto read : reads) {
            for (auto var : vars) {
                if (qpsFacade.isModifies(read, var)) {
                    std::pair<int, std::string> pair;
                    pair.first = read;
                    pair.second = var;
                    listPair.push_back(pair);
                }
            }
        }
        return listPair;
    }
    else if (synonymType == "print" && attribute == "varName") {
        auto prints = qpsFacade.getPrintStatements();
        auto vars = qpsFacade.getVariables();
        for (auto print : prints) {
            for (auto var : vars) {
                if (qpsFacade.isUses(print, var)) {
                    std::pair<int, std::string> pair;
                    pair.first = print;
                    pair.second = var;
                    listPair.push_back(pair);
                }
            }
        }
        return listPair;
    }

    if (synonymType == "stmt" && attribute == "stmt#") {
        list = intArrToStringArr(qpsFacade.getStatements());
    }
    else if (synonymType == "read" && attribute == "stmt#") {
        list = intArrToStringArr(qpsFacade.getReadStatements());
    }
    else if (synonymType == "print" && attribute == "stmt#") {
        list = intArrToStringArr(qpsFacade.getPrintStatements());
    }
    else if (synonymType == "call" && attribute == "stmt#") {
        list = intArrToStringArr(qpsFacade.getCallStatements());
    }
    else if (synonymType == "while" && attribute == "stmt#") {
        list = intArrToStringArr(qpsFacade.getWhileStatements());
    }
    else if (synonymType == "if" && attribute == "stmt#") {
        list = intArrToStringArr(qpsFacade.getIfStatements());
    }
    else if (synonymType == "assign" && attribute == "stmt#") {
        list = intArrToStringArr(qpsFacade.getAssignStatements());
    }
    else if (synonymType == "constant" && attribute == "value") {
        list = qpsFacade.getConstants();
    }
    else if (synonymType == "procedure" && attribute == "procName") {
        list = qpsFacade.getProcedures();
    }
    else if (synonymType == "variable" && attribute == "varName") {
        list = qpsFacade.getVariables();
    } else {
        synonym.erase(remove(synonym.begin(), synonym.end(), '"'), synonym.end());
        list.push_back(synonym);
    }
    for (auto val : list) {
        std::pair<int, std::string> pair;
        pair.first = -1;
        pair.second = val;
        listPair.push_back(pair);
    }
    return listPair;
}

/**
 * Methods below override base clause evaluate method
 * @param qpsFacade
 * @param target
 * @param synonymTypes
 * @return ClauseResult
 */

ClauseResult Clause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    return defaultResult;
}

/*ClauseResult Clause::evaluate(QPSFacade& qpsFacade, std::vector<std::string> target, std::unordered_map<std::string, std::string> synonymTypes, bool hasAssignSynonym, bool hasSynonym, int clauseListSize) {
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    return defaultResult;
}*/

ClauseResult FollowsClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms (check if have)
        if (LHS == RHS && (LHS != "_" && RHS != "_")) {
            return ClauseResultBuilder::createBooleanResult(false);
        }
        return Clause::processTupleList(qpsFacade.getAllFollows(), std::vector<std::pair<int, std::string>>(), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        std::vector<int> list;
        if (qpsFacade.getPreceding(stoi(RHS)) != 0) {
            list.push_back(qpsFacade.getPreceding(stoi(RHS)));
        }
        return processList(list, synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        std::vector<std::string> list;
        int following = qpsFacade.getFollowing(stoi(LHS));
        if (following != 0) {
            list.push_back(std::to_string(following));
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    }
    else {
        // if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isFollows(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult FollowsTClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms
        if (LHS == RHS && (LHS != "_" && RHS != "_")) {
            return ClauseResultBuilder::createBooleanResult(false);
        }
        return Clause::processTupleList(qpsFacade.getAllFollowsStar(), std::vector<std::pair<int, std::string>>(), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        return processList(qpsFacade.getFollowsStarBackward(stoi(RHS)), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        std::vector<std::string> list;
        for (int val : qpsFacade.getFollowsStarForward(stoi(LHS))) {
            list.push_back(std::to_string(val));
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    }
    else {
        // if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isFollowsStar(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult ParentClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms (check if have)
        if (LHS == RHS && (LHS != "_" && RHS != "_")) {
            return ClauseResultBuilder::createBooleanResult(false);
        }
        return Clause::processTupleList(qpsFacade.getAllParent(), std::vector<std::pair<int, std::string>>(), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        std::vector<int> list;
        if (qpsFacade.getParent(stoi(RHS)) != 0) {
            list.push_back(qpsFacade.getParent(stoi(RHS)));
        }
        return processList(list, synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        std::vector<std::string> list;
        for (int val : qpsFacade.getChildren(stoi(LHS))) {
            list.push_back(std::to_string(val));
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    }
    else {
        // if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isParent(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult ParentTClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms
        if (LHS == RHS && (LHS != "_" && RHS != "_")) {
            return ClauseResultBuilder::createBooleanResult(false);
        }
        return Clause::processTupleList(qpsFacade.getAllParentStar(), std::vector<std::pair<int, std::string>>(), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        return processList(qpsFacade.getParentStar(stoi(RHS)), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        std::vector<std::string> list;
        for (int val : qpsFacade.getChildStar(stoi(LHS))) {
            list.push_back(std::to_string(val));
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    }
    else {
        // if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isParentStar(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult UsesClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::vector<std::string>> list;
        std::vector<std::pair<int, std::string>> tupleList = qpsFacade.getUses();
        if (synonymTypes[LHS] == "procedure") {
            auto procVariablePairs = qpsFacade.getUsesP();
            // proc and variable are both strings; cannot use tupleList, add directly to list
            for (const auto& pair : procVariablePairs) {
                std::vector<std::string> temp;
                temp.push_back(pair.first);
                temp.push_back(pair.second);
                list.push_back(temp);
            }
            std::vector<std::string> synList;
            synList.push_back(LHS);
            synList.push_back(RHS);
            return ClauseResultBuilder::createTupleResult(synList, list);
        }
        else {
            return processTupleList(std::vector<std::pair<int, int>>(), tupleList, synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
        }
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> list;
        if (synonymTypes[LHS] == "procedure") {
            auto procVariablePairs = qpsFacade.getUsesP();
            for (const auto& pair : procVariablePairs) {
                if (pair.second == RHS) {
                    list.push_back(pair.first);
                }
            }
            return ClauseResultBuilder::createListResult(LHS, list);
        }
        else {
            return processList(qpsFacade.getUses(RHS), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
        }
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        std::vector<std::string> list;
        if (isConstant(LHS)) {
            list = qpsFacade.getUses(stoi(LHS));
        }
        else {
            list = qpsFacade.getUsesPProc(LHS);
        }

        return ClauseResultBuilder::createListResult(RHS, list);
    }
    else {
        // if 0 synonym
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        return ClauseResultBuilder::createBooleanResult(
            isConstant(LHS)
            ? qpsFacade.isUses(stoi(LHS), RHS)
            : qpsFacade.isUsesP(LHS, RHS));
    }
    return defaultResult;
}

ClauseResult ModifiesClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::vector<std::string>> list;
        std::vector<std::pair<int, std::string>> tupleList = qpsFacade.getModifies();
        if (synonymTypes[LHS] == "procedure") {
            auto procVariablePairs = qpsFacade.getModifiesP();
            for (const auto& pair : procVariablePairs) {
                std::vector<std::string> temp;
                temp.push_back(pair.first);
                temp.push_back(pair.second);
                list.push_back(temp);
            }
            std::vector<std::string> synList;
            synList.push_back(LHS);
            synList.push_back(RHS);
            return ClauseResultBuilder::createTupleResult(synList, list);
        }
        else {
            return processTupleList(std::vector<std::pair<int, int>>(), tupleList, synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
        }
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> list;
        if (synonymTypes[LHS] == "procedure") {
            auto procVariablePairs = qpsFacade.getModifiesP();
            for (const auto& pair : procVariablePairs) {
                if (pair.second == RHS) {
                    list.push_back(pair.first);
                }
            }
            return ClauseResultBuilder::createListResult(LHS, list);
        }
        else {
            return processList(qpsFacade.getModifies(RHS), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
        }
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        std::vector<std::string> list;
        if (isConstant(LHS)) {
            list = qpsFacade.getModifies(stoi(LHS));
        }
        else {
            list = qpsFacade.getModifiesPProc(LHS);
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    }
    else {
        // if 0 synonym
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        return ClauseResultBuilder::createBooleanResult(
            isConstant(LHS)
            ? qpsFacade.isModifies(stoi(LHS), RHS)
            : qpsFacade.isModifiesP(LHS, RHS));
    }
    return defaultResult;
}

ClauseResult AssignPatternClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::string assignSynonym = getAssignSynonym();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    std::vector<std::string> list;
    // (_, _) LHS wildcard && RHS wildcard
    if (isWildcard(LHS) && isWildcard(RHS)) {
        list = intArrToStringArr(qpsFacade.getStmtRefBothWild());
        return ClauseResultBuilder::createListResult(assignSynonym, list);
    }
    // (_, _"x"_) LHS wildcard && RHS substring wrong
    else if (isWildcard(LHS) && isSubstring(RHS)) {
        RHS.erase(remove(RHS.begin(), RHS.end(), '_'), RHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        list = intArrToStringArr(qpsFacade.getStmtRefRhsPartial(RHS));
        return ClauseResultBuilder::createListResult(assignSynonym, list);
    }
    // (_, "x") LHS wildcard && RHS exact TODO
    else if (isWildcard(LHS) && !isSubstring(RHS)) {
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        list = intArrToStringArr(qpsFacade.getStmtRefRhsExact(RHS));
        return ClauseResultBuilder::createListResult(assignSynonym, list);
    }
    // (s, _) // LHS syn && RHS wildcard
    else if (isSynonym(LHS) && isWildcard(RHS)) {
        std::vector<std::string> vars = qpsFacade.getVariables();
        std::vector<int> intList;
        std::vector<std::vector<std::string>> tupleList;
        for (std::string var : vars) {
            std::vector<int> stmtRefs;
            stmtRefs = qpsFacade.getStmtRefLhs(var);
            for (int stmtRef : stmtRefs) {
                std::vector<std::string> row;
                row.push_back(var);
                row.push_back(std::to_string(stmtRef));
                tupleList.push_back(row);
            }
        }
        std::vector<std::string> synList = {LHS, assignSynonym};
        return ClauseResultBuilder::createTupleResult(synList, tupleList);
    }
    // (s, _"x"_) LHS syn && RHS substring
    else if (isSynonym(LHS) && isSubstring(RHS)) {
        RHS.erase(remove(RHS.begin(), RHS.end(), '_'), RHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> vars = qpsFacade.getVariables();
        std::vector<int> intList;
        std::vector<std::vector<std::string>> tupleList;
        for (std::string var : vars) {
            std::vector<int> stmtRefs = qpsFacade.getStmtRefLhsVarRhsPartial(var, RHS);
            for (int stmtRef : stmtRefs) {
                std::vector<std::string> row;
                row.push_back(var);
                row.push_back(std::to_string(stmtRef));
                tupleList.push_back(row);
            }
        }
        std::vector<std::string> synList = {LHS, assignSynonym};
        return ClauseResultBuilder::createTupleResult(synList, tupleList);
    }
    // (s, "x") // LHS syn && RHS exact TODO
    else if (isSynonym(LHS) && !isSubstring(RHS)) {
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> vars = qpsFacade.getVariables();
        std::vector<int> intList;
        std::vector<std::vector<std::string>> tupleList;
        for (std::string var : vars) {
            std::vector<int> stmtRefs = qpsFacade.getStmtRefLhsVarRhsExact(var, RHS);
            for (int stmtRef : stmtRefs) {
                std::vector<std::string> row;
                row.push_back(var);
                row.push_back(std::to_string(stmtRef));
                tupleList.push_back(row);
            }
        }
        std::vector<std::string> synList = { LHS, assignSynonym };
        return ClauseResultBuilder::createTupleResult(synList, tupleList);
    }
    // ("x", _) LHS var && RHS wildcard
    else if (!isSynonym(LHS) && isWildcard(RHS)) {
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        list = intArrToStringArr(qpsFacade.getStmtRefLhs(LHS));
        return ClauseResultBuilder::createListResult(assignSynonym, list);
    }
    // ("x", (_"x"_) LHS var && RHS substring
    else if (!isSynonym(LHS) && isSubstring(RHS)) {
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '_'), RHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        list = intArrToStringArr(qpsFacade.getStmtRefLhsVarRhsPartial(LHS, RHS));
        return ClauseResultBuilder::createListResult(assignSynonym, list);
    }
    // ("x", "x") TODO
    else if (!isSynonym(LHS) && !isSubstring(RHS)) {
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        list = intArrToStringArr(qpsFacade.getStmtRefLhsVarRhsExact(LHS, RHS));
        return ClauseResultBuilder::createListResult(assignSynonym, list);
    }
    return defaultResult;
}

ClauseResult WhilePatternClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::string assignSynonym = getAssignSynonym();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    std::vector<std::string> list;
    // (_, _) LHS wildcard && RHS wildcard
    if (isWildcard(LHS) && isWildcard(RHS)) {
        list = intArrToStringArr(qpsFacade.getWhileStmtRefWild());
        return ClauseResultBuilder::createListResult(assignSynonym, list);
    }
    // (s, _) // LHS syn && RHS wildcard
    else if (isSynonym(LHS) && isWildcard(RHS)) {
        std::vector<std::string> vars = qpsFacade.getVariables();
        std::vector<int> intList;
        std::vector<std::vector<std::string>> tupleList;
        for (std::string var : vars) {
            std::vector<int> stmtRefs;
            stmtRefs = qpsFacade.getWhileStmtRefVar(var);
            for (int stmtRef : stmtRefs) {
                std::vector<std::string> row;
                row.push_back(var);
                row.push_back(std::to_string(stmtRef));
                tupleList.push_back(row);
            }
        }
        std::vector<std::string> synList = { LHS, assignSynonym };
        return ClauseResultBuilder::createTupleResult(synList, tupleList);
    }

    // ("x", _) LHS var && RHS wildcard
    else if (!isSynonym(LHS) && isWildcard(RHS)) {
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        list = intArrToStringArr(qpsFacade.getWhileStmtRefVar(LHS));
        return ClauseResultBuilder::createListResult(assignSynonym, list);
    }
    return defaultResult;
}

ClauseResult IfPatternClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::string assignSynonym = getAssignSynonym();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    std::vector<std::string> list;
    // (_, _) LHS wildcard && RHS wildcard
    if (isWildcard(LHS) && isWildcard(RHS)) {
        list = intArrToStringArr(qpsFacade.getIfStmtRefWild());
        return ClauseResultBuilder::createListResult(assignSynonym, list);
    }
    // (s, _) // LHS syn && RHS wildcard
    else if (isSynonym(LHS) && isWildcard(RHS)) {
        std::vector<std::string> vars = qpsFacade.getVariables();
        std::vector<int> intList;
        std::vector<std::vector<std::string>> tupleList;
        for (std::string var : vars) {
            std::vector<int> stmtRefs;
            stmtRefs = qpsFacade.getIfStmtRefVar(var);
            for (int stmtRef : stmtRefs) {
                std::vector<std::string> row;
                row.push_back(var);
                row.push_back(std::to_string(stmtRef));
                tupleList.push_back(row);
            }
        }
        std::vector<std::string> synList = { LHS, assignSynonym };
        return ClauseResultBuilder::createTupleResult(synList, tupleList);
    }

    // ("x", _) LHS var && RHS wildcard
    else if (!isSynonym(LHS) && isWildcard(RHS)) {
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        list = intArrToStringArr(qpsFacade.getIfStmtRefVar(LHS));
        return ClauseResultBuilder::createListResult(assignSynonym, list);
    }
    return defaultResult;
}

ClauseResult CallsClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();

    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(false);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms
        if (LHS == RHS && (LHS != "_" && RHS != "_")) {
            return ClauseResultBuilder::createBooleanResult(false);
        }
        std::vector<std::vector<std::string>> list;
        std::vector<std::pair<std::string, std::string>> tupleList = qpsFacade.getCalls();

        for (std::pair<std::string, std::string> procPair : tupleList) {
            std::vector<std::string> temp;
            temp.push_back(procPair.first);
            temp.push_back(procPair.second);
            list.push_back(temp);
        }

        std::vector<std::string> synList;
        synList.push_back(LHS);
        synList.push_back(RHS);
        return ClauseResultBuilder::createTupleResult(synList, list);
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        std::vector<std::string> list;
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> procList = qpsFacade.getCaller(RHS);
        for (std::string procName : procList) {
            list.push_back(procName);
        }

        return ClauseResultBuilder::createListResult(LHS, list);
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        std::vector<std::string> list;
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        std::vector<std::string> procList = qpsFacade.getCallee(LHS);
        for (std::string procName : procList) {
            list.push_back(procName);
        }

        return ClauseResultBuilder::createListResult(RHS, list);
    }
    else {
        // if 0 synonym
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isCalls(LHS, RHS));
    }

    return defaultResult;
}

ClauseResult CallsTClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();

    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms
        if (LHS == RHS && (LHS != "_" && RHS != "_")) {
            return ClauseResultBuilder::createBooleanResult(false);
        }
        std::vector<std::vector<std::string>> list;
        std::vector<std::pair<std::string, std::string>> tupleList = qpsFacade.getCallsStar();

        for (std::pair<std::string, std::string> procPair : tupleList) {
            std::vector<std::string> temp;
            temp.push_back(procPair.first);
            temp.push_back(procPair.second);
            list.push_back(temp);
        }

        std::vector<std::string> synList;
        synList.push_back(LHS);
        synList.push_back(RHS);
        return ClauseResultBuilder::createTupleResult(synList, list);
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        std::vector<std::string> list;
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        std::vector<std::string> procList = qpsFacade.getCallerStar(RHS);
        for (std::string procName : procList) {
            list.push_back(procName);
        }

        return ClauseResultBuilder::createListResult(LHS, list);
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        std::vector<std::string> list;
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        std::vector<std::string> procList = qpsFacade.getCalleeStar(LHS);
        for (std::string procName : procList) {
            list.push_back(procName);
        }

        return ClauseResultBuilder::createListResult(RHS, list);
    }
    else {
        // if 0 synonym
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isCallsStar(LHS, RHS));
    }

    return defaultResult;
}

ClauseResult NextClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms (check if have)
        if (LHS == RHS && (LHS != "_" && RHS != "_")) {
            std::vector<std::vector<std::string>> list;
            for (std::pair<int, int> val : qpsFacade.getAllNext()) {
                if (val.first == val.second) {
                    std::vector<std::string> temp;
                    temp.push_back(std::to_string(val.first));
                    temp.push_back(std::to_string(val.second));
                    list.push_back(temp);
                }
            }
            std::vector<std::string> synList;
            synList.push_back(LHS);
            synList.push_back(RHS);
            return ClauseResultBuilder::createTupleResult(synList, list);
        }
        return Clause::processTupleList(qpsFacade.getAllNext(), std::vector<std::pair<int, std::string>>(), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        std::vector<int> list;
        return processList(qpsFacade.getPrevious(stoi(RHS)), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        std::vector<int> list = qpsFacade.getNext(stoi(LHS));
        return processList(qpsFacade.getNext(stoi(LHS)), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else {
        // if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isNext(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult NextTClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms
        if (LHS == RHS && (LHS != "_" && RHS != "_")) {
            std::vector<std::vector<std::string>> list;
            for (std::pair<int, int> val : qpsFacade.getAllNextStar()) {
                if (val.first == val.second) {
                    std::vector<std::string> temp;
                    temp.push_back(std::to_string(val.first));
                    temp.push_back(std::to_string(val.second));
                    list.push_back(temp);
                }
            }
            std::vector<std::string> synList;
            synList.push_back(LHS);
            synList.push_back(RHS);
            return ClauseResultBuilder::createTupleResult(synList, list);
        }
        return Clause::processTupleList(qpsFacade.getAllNextStar(), std::vector<std::pair<int, std::string>>(), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        return processList(qpsFacade.getPreviousStar(stoi(RHS)), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        std::vector<std::string> list;
        for (int val : qpsFacade.getNextStar(stoi(LHS))) {
            list.push_back(std::to_string(val));
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    }
    else {
        // if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isNextStar(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult AffectsClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms (check if have)
        if (LHS == RHS && (LHS != "_" && RHS != "_")) {
            std::vector<std::vector<std::string>> list;
            for (std::pair<int, int> val : qpsFacade.getAllAffects()) {
                if (val.first == val.second) {
                    std::vector<std::string> temp;
                    temp.push_back(std::to_string(val.first));
                    temp.push_back(std::to_string(val.second));
                    list.push_back(temp);
                }
            }
            std::vector<std::string> synList;
            synList.push_back(LHS);
            synList.push_back(RHS);
            return ClauseResultBuilder::createTupleResult(synList, list);
        }
        return Clause::processTupleList(qpsFacade.getAllAffects(), std::vector<std::pair<int, std::string>>(), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        std::vector<int> list;
        return processList(qpsFacade.getAffectsCause(stoi(RHS)), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        std::vector<int> list = qpsFacade.getAffectsAffected(stoi(LHS));
        return processList(list, synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else {
        // if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isAffects(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult AffectsTClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    if (isSynonym(LHS) && isSynonym(RHS)) {
        // if 2 synonyms
        if (LHS == RHS && (LHS != "_" && RHS != "_")) {
            std::vector<std::vector<std::string>> list;
            for (std::pair<int, int> val : qpsFacade.getAllAffectsStar()) {
                if (val.first == val.second) {
                    std::vector<std::string> temp;
                    temp.push_back(std::to_string(val.first));
                    temp.push_back(std::to_string(val.second));
                    list.push_back(temp);
                }
            }
            std::vector<std::string> synList;
            synList.push_back(LHS);
            synList.push_back(RHS);
            return ClauseResultBuilder::createTupleResult(synList, list);
        }
        return Clause::processTupleList(qpsFacade.getAllAffectsStar(), std::vector<std::pair<int, std::string>>(), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(LHS)) {
        // if 1 LHS synonym
        return processList(qpsFacade.getAffectsStarCause(stoi(RHS)), synonymTypes[LHS], synonymTypes[RHS], qpsFacade);
    }
    else if (isSynonym(RHS)) {
        // if 1 RHS synonym
        std::vector<std::string> list;
        for (int val : qpsFacade.getAffectsStarAffected(stoi(LHS))) {
            list.push_back(std::to_string(val));
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    }
    else {
        // if 0 synonym
        return ClauseResultBuilder::createBooleanResult(qpsFacade.isAffectsStar(stoi(LHS), stoi(RHS)));
    }
    return defaultResult;
}

ClauseResult WithClause::evaluate(QPSFacade& qpsFacade, std::unordered_map<std::string, std::string> synonymTypes) {
    ClauseResult defaultResult = ClauseResultBuilder::createBooleanResult(true);
    std::string LHS = getLHS();
    std::string RHS = getRHS();
    std::vector<std::vector<std::string>> tupleList;

    std::vector<std::string> synList;
    std::vector<std::string> list;
    std::vector<std::pair<int, std::string>> left = processWith(qpsFacade, synonymTypes, LHS, leftType);
    std::vector<std::pair<int, std::string>> right = processWith(qpsFacade, synonymTypes, RHS, rightType);
    if ((rightType == "_name" || rightType == "_constant") && (leftType == "_name" || leftType == "_constant")) {
        return ClauseResultBuilder::createBooleanResult(RHS == LHS);
    }
    else if (rightType == "_name" || rightType == "_constant") {
        RHS.erase(remove(RHS.begin(), RHS.end(), '"'), RHS.end());
        if (!left.empty() && left[0].first == -1) {
            for (auto leftPair : left) {
                if (leftPair.second == RHS) {
                    list.push_back(leftPair.second);
                }
            }
            return ClauseResultBuilder::createListResult(LHS, list);
        }
        else if (!left.empty() && left[0].first != -1) {
            for (auto leftPair : left) {
                if (leftPair.second == RHS) {
                    std::vector<std::string> row;
                    row.push_back(std::to_string(leftPair.first));
                    row.push_back(leftPair.second);
                    tupleList.push_back(row);
                }
            }
            synList.push_back(LHS);
            synList.push_back(LHS + "." + leftType);
            return ClauseResultBuilder::createTupleResult(synList, tupleList);
        }
    }
    else if (leftType == "_name" || leftType == "_constant") {
        LHS.erase(remove(LHS.begin(), LHS.end(), '"'), LHS.end());
        if (!right.empty() && right[0].first == -1) {
            for (auto rightPair : right) {
                if (rightPair.second == LHS) {
                    list.push_back(rightPair.second);
                }
            }
            return ClauseResultBuilder::createListResult(RHS, list);
        }
        else if (!right.empty() && right[0].first != -1) {
            for (auto rightPair : right) {
                if (rightPair.second == LHS) {
                    std::vector<std::string> row;
                    row.push_back(std::to_string(rightPair.first));
                    row.push_back(rightPair.second);
                    tupleList.push_back(row);
                }
            }
            synList.push_back(RHS);
            synList.push_back(RHS + "." + rightType);
            return ClauseResultBuilder::createTupleResult(synList, tupleList);
        }
    }
    else {
        for (auto leftVal : left) {
            for (auto rightVal : right) {
                std::vector<std::string> row;
                if (leftVal.second == rightVal.second) {
                    if (leftVal.first != -1) {
                        row.push_back(std::to_string(leftVal.first));
                    }
                    row.push_back(leftVal.second);
                    if (rightVal.first != -1) {
                        row.push_back(std::to_string(rightVal.first));
                    }
                    row.push_back(rightVal.second);
                    tupleList.push_back(row);
                }
            }
        }
        if (!left.empty() && left[0].first != -1) {
            synList.push_back(LHS);
            synList.push_back(LHS + "." + leftType);
        }
        else if (!left.empty() && left[0].first == -1) {
            synList.push_back(LHS);
        }
        if (!right.empty() && right[0].first != -1) {
            synList.push_back(RHS);
            synList.push_back(RHS + "." + rightType);
        }
        else if (!right.empty() && right[0].first == -1) {
            synList.push_back(RHS);
        }

        if (synList.size() > 2) {
            return ClauseResultBuilder::createTableResult(synList, tupleList, synList.size());
        }
        else {
            return ClauseResultBuilder::createTupleResult(synList, tupleList);
        }
        return ClauseResultBuilder::createListResult(RHS, list);
    }
}

