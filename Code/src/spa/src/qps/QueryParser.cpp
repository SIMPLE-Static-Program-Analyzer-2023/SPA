//
// Created by Vanessa Khor on 5/2/23.
//
#include "QueryParser.h"

QueryParser::QueryParser(QueryLexer& lexer, Query& query) : lexer(lexer), query(query) {}

std::unordered_set<std::string> declaration_keywords_set({ "stmt" , "read" , "print" , "while" , "if" , "assign" ,
                                           "variable" , "constant" , "procedure", "call" });
std::unordered_set<std::string> clause_keywords_set({ "Uses", "Modifies", "Follows", "FollowsT", "Parent", "ParentT", "pattern", "Calls", "CallsT", "Next", "NextT", "Affects", "AffectsT", "with" });
std::unordered_set<std::string> valid_clause_stage_keywords_set({ "Uses", "Modifies", "Follows", "FollowsT", "Parent", "ParentT", "Calls", "CallsT", "Next", "NextT", "Affects", "AffectsT", "pattern", "with"});
std::unordered_set<std::string> sub_clause_keywords_set({ "Uses", "Modifies" });
std::unordered_set<std::string> entRef_clause_keywords_set({ "Calls", "CallsT" });
std::unordered_set<std::string> valid_select_connectors({"such", "pattern", "with"});
std::unordered_set<std::string> valid_clause_connectors({"such", "and"});
std::unordered_set<char> valid_operators({'+', '-', '*', '/', '%'});
std::unordered_set<std::string> valid_attributes({"procName", "varName", "value", "stmt"});
std::unordered_set<std::string> valid_procname_syn({"procedure", "call"});  // expect type NAME
std::unordered_set<std::string> valid_varname_syn({"variable", "read", "print"});  // expect type NAME
std::unordered_set<std::string> valid_stmt_syn({"stmt", "read", "print", "call" , "while", "if", "assign"});  // expect type INTEGER

void QueryParser::advanceState(int nxt) {
    if (nxt > 4 || nxt < 0) {
        // invalid
    } else {
        currentState = static_cast<QueryState>(nxt);
    }
}

void QueryParser::advanceClauseState(int nxt) {
    if (nxt > 6 || nxt < 0) {
        // invalid
    } else {
        expectedClauseState = static_cast<ClauseState>(nxt);
    }
}

void QueryParser::advanceToAndStage() {
    if (expectedClauseState < 4) {
        int nxt = expectedClauseState + 3;
        expectedClauseState = static_cast<ClauseState>(nxt);
    }
}

void QueryParser::advanceConnectorState(int nxt) {
    if (nxt > 3 || nxt < 0) {
        // invalid
    } else {
        currentConnectorState = static_cast<ClauseConnectorState>(nxt);
    }
}

/**
 * Validate token based on current PQL query state
 * @param token
 */
void QueryParser::isTokenValid(std::string token) {
    std::string selectStmt = "Select";

    if (currentState == 0) {  // initialization stage
        if (declaration_keywords_set.find(token) != declaration_keywords_set.end()) {
            advanceState(1);
        } else if (token == selectStmt) {  // case sensitive comparison
            advanceState(2);
        } else {  // invalid query
            throw QpsSyntaxException("Expected a declaration or Select statement", lexer.getLineNo(), token);
        }

    } else if (currentState == 1) {  // declaration stage
        if (clause_keywords_set.find(token) != clause_keywords_set.end()) {
            throw QpsSyntaxException("Expected a select statement", lexer.getLineNo(), token);
        } else if (token == selectStmt) {  // case sensitive comparison
            advanceState(2);  // proceed to parse one select clause
        } else if (declaration_keywords_set.find(token) != declaration_keywords_set.end()) {
//            return true;
        } else {
            throw QpsSyntaxException("Invalid query declaration token", lexer.getLineNo(), token);
        }

    } else if (currentState == 2) {  // select statement stage
        if (token == selectStmt) {
            throw QpsSyntaxException("Select clause can only be declared once", lexer.getLineNo(), token);
        } else {
            throw QpsSyntaxException("Invalid query token", lexer.getLineNo(), token);
        }
    } else if (currentState == 3) {  // post select statement stage
        if (token == selectStmt) {
            throw QpsSyntaxException("Select clause can only be declared once", lexer.getLineNo(), token);
        } else if (valid_select_connectors.find(token) != valid_select_connectors.end()) {
            if (token == "such") {
                lexer.eat("such");
                if (!lexer.match("that")) {
                    throw QpsSyntaxException("such needs to be followed by that");
                }
                advanceClauseState(1);  // expect such that clause
                advanceConnectorState(3);
            } else if (token == "with") {
                advanceClauseState(3);  // expect with clause
                advanceConnectorState(1);
            } else if (token == "pattern") {
                advanceClauseState(2);  // expect pattern clause
                advanceConnectorState(1);
            }
        } else {
            throw QpsSyntaxException("Invalid connector used after Select statement", lexer.getLineNo(), token);
        }
        advanceState(4);  // to clause stage
    } else if (currentState == 4) {  // clause stage
        if (token == selectStmt) {
            throw QpsSyntaxException("Select clause can only be declared once", lexer.getLineNo(), token);
        } else if (valid_clause_connectors.find(token) != valid_clause_connectors.end()) {
            isConnectorTokenValid(token);
        } else if (valid_clause_stage_keywords_set.find(token) != valid_clause_stage_keywords_set.end()) {
            isClauseTokenValid(token);
        } else {
            throw QpsSyntaxException("Unknown token detected", lexer.getLineNo(), token);
        }

    } else {
        // should not reach here
        throw QpsSemanticException("Parsing error, query state should be within 0-3");
    }
};


void QueryParser::isConnectorTokenValid(std::string token) {
    if (currentConnectorState == 3) {  // connector previously declared
        throw QpsSyntaxException("Invalid syntax, expected a Clause", lexer.getLineNo(), token);
    }

    if (token == "and") {
        advanceToAndStage();  // update clause state to corresponding and state
        advanceConnectorState(2);  // expect a clause connected by "and"

    } else if (token == "such") {
        lexer.eat("such");
        if (lexer.match("that")) {
            advanceClauseState(1);  // expect a such that clause
            advanceConnectorState(3);
        } else {
            throw QpsSyntaxException("Expected a that", lexer.getLineNo(), token);
        }

    } else {
        std::cout << "NOT SUPPOSED TO BE HERE (is connector valid)" << std::endl;
    }
}

/**
 * Checks if clause token received is valid
 * CurrentConnectorState must be 1 in order for a clause token to be valid
 * @param token
 */
void QueryParser::isClauseTokenValid(std::string token) {
        if (token == "pattern") {
            if (currentConnectorState < 2) {
                advanceClauseState(2);
                advanceConnectorState(1);  // expect a connector/clause next
            } else {
                throw QpsSyntaxException("Expected to connect pattern clause", lexer.getLineNo(), token);
            }

        } else if (token == "with") {
            if (currentConnectorState < 2) {
                advanceClauseState(3);
                advanceConnectorState(1);  // expect a connector/clause next
            } else {
                throw QpsSyntaxException("Expected to connect with clause", lexer.getLineNo(), token);
            }

        } else {  // such that clause
            if (expectedClauseState == 1 || expectedClauseState == 4) {  // requires "such that" or "and" before token
                advanceClauseState(1);
                advanceConnectorState(1);
            } else {
                throw QpsSyntaxException("Expected a such that clause", lexer.getLineNo(), token);
            }
        }
}

void QueryParser::handleSelectClause() {
    lexer.eat("Select");
    setSelectedTarget();
}

/**
 * Set query target after identifying keyword <Select>
 * Target could be a synonym, BOOLEAN, tuple or an attribute
 */
void QueryParser::setSelectedTarget() {
    std::string curr = lexer.getCurrentToken();
    std::vector<std::string> targetLst;

    // boolean target
    if (curr == "BOOLEAN" && !query.isSynonymPresent(curr)) {  // ensure BOOLEAN is not declared as synonym
        lexer.eat(curr);
        query.setTargetSelected("bool", targetLst);
    } else if (isSynonym(curr)) {  // single syn target
        if (!query.isSynonymPresent(curr)) {
            throw QpsSemanticException("undeclared target", lexer.getLineNo(), lexer.getCurrentToken());
        }
        lexer.eat(curr);
        if (lexer.match(".")) {  // check for attribute
            lexer.eat(".");
            std::string attr = isAttributeToken(lexer.getCurrentToken());
            // validate attribute
            std::string synType = query.getSynonymType(curr);
            validateSynAndAttribute(synType, attr);
            targetLst.push_back(curr + "." + attr);
        } else {
            targetLst.push_back(curr);
        }
        query.setTargetSelected("syn", targetLst);
    } else if (curr == "<") {  // expect multiple tuple values
        int synCount = 1;
        lexer.eat("<");
        curr = lexer.getCurrentToken();
        while (!lexer.match(">")) {
            if (isSynonym(curr)) {
                synCount--;
                lexer.eat(curr);
                if (lexer.match(".")) {  // check for attribute
                    lexer.eat(".");
                    std::string attr = isAttributeToken(lexer.getCurrentToken());
                    // validate attribute
                    std::string synType = query.getSynonymType(curr);
                    validateSynAndAttribute(synType, attr);

                    targetLst.push_back(curr + "." + attr);
                } else {  // single syn
                    targetLst.push_back(curr);
                }

                if (lexer.match(",")) {
                    synCount++;
                    lexer.eat(",");
                } else if (lexer.match(">")) {
                    // end
                } else {
                    throw QpsSyntaxException("Expected a , in between synonyms");
                }
            } else {
                throw QpsSyntaxException("invalid target type");
            }
            curr = lexer.getCurrentToken();  // next syn
        }
        lexer.eat(">");

        if (targetLst.size() < 1 || synCount == 1) {  // invalid: <>, <m,>, <m,m,>
            throw QpsSyntaxException("Invalid target found");
        }

        query.setTargetSelected("tuple", targetLst);
    } else {
        throw QpsSyntaxException("Invalid target");
    }
    advanceState(3);  // advance to post select stage
}



/**
 * Check if incoming token is an attribute
 * Returns attribute if true, else throw error
 * @return bool
 */
std::string QueryParser::isAttributeToken(std::string token) {
    std::string attrToken = "";
    if (valid_attributes.find(token) != valid_attributes.end()) {
        attrToken += token;
        lexer.eat(token);
        if (token == "stmt") {  // require #
            if (lexer.match("#")) {
                lexer.eat("#");
                attrToken += "#";
            } else {
                throw QpsSyntaxException("Invalid attribute");
            }
        }
        return attrToken;
    } else {
        throw QpsSyntaxException("Invalid attribute");
    }
}

void QueryParser::parse() {
    while (!lexer.isEnd()) {  // [x, eof]
        std::string token1 = lexer.getCurrentToken();
        isTokenValid(token1);
        if (expectedClauseState == 5) {  // received and <pattern>
            lexer.eat("and");
            parseAndPattern(lexer.getCurrentToken());
        } else if (expectedClauseState == 6) {  // received and <with>
            lexer.eat("and");
            parseWithClause(lexer.getCurrentToken());
        } else if (declaration_keywords_set.find(token1) != declaration_keywords_set.end()) {  // if token1 is a design-entity
            parseDeclarations(token1);
        } else if (clause_keywords_set.find(token1) != clause_keywords_set.end()) {  // if token1 is a clause
            parseClauses(token1);
        } else if (token1 == "Select") {
            handleSelectClause();
        } else {
            // cut away valid connectors
            if (lexer.match("that")) {
                lexer.eat("that");
            } else {
                lexer.eat(token1);
            }
        }
    }

    if (lexer.isEnd()) {
        if (lexer.getCurrentToken() != "") {
            throw QpsSyntaxException("Incomplete query!");
        }
        if (currentState != 3 && currentState != 4) {
            throw QpsSyntaxException("Incomplete query!");
        }
        if (currentConnectorState > 1) {
            throw QpsSyntaxException("Syntax error, query ended abruptly!");
        }
    }
}

void QueryParser::parseDeclarations(std::string token1) {
    std::string designEntity = token1;
    std::vector<std::string> synonymLst;

        lexer.eat(designEntity);  // advance lexer
        if (lexer.matchIdentifier()) {
            while (!lexer.match(";")) {
                    std::string id = lexer.eatIdentifier();  // remove id and advance lexer

                    // check for duplicates
                    if (query.isSynonymPresent(id)) {
                        throw QpsSyntaxException("Synonym already declared!", lexer.getLineNo(), id);
                    }

                    synonymLst.push_back(id);

                    if (lexer.getCurrentToken() == ";") {
                        // end
                    } else if (lexer.getCurrentToken() == ",") {
                        lexer.eat(",");  // continue
                    } else {
                        throw QpsSyntaxException("Invalid declaration syntax", lexer.getLineNo(), lexer.getCurrentToken());
                    }
            }
        } else {
            throw QpsSyntaxException("Invalid identifier", lexer.getLineNo(), lexer.getCurrentToken());
        }

        lexer.eat(";");

    // populate declaration list
    Declaration currDeclaration = Declaration(designEntity, synonymLst);
    query.addDeclaration(currDeclaration);
}

// LETTER ( LETTER | DIGIT )*
bool QueryParser::isSynonym(std::string token) {
    char firstChar = token[0];
    if (!isalpha(firstChar)) {
        return false;
    }
    for (int i = 1; i < token.length(); i++) {
        if (isalnum(token.at(i)) == 0) {
            return false;
        }
    }
    return true;
}

// LETTER ( LETTER | DIGIT )*
bool QueryParser::isStringIdent(std::string token) {
    int len = token.length();

    char firstChar = token[0];
    if (firstChar != '\"') {
        return false;
    }

    // check if contains Synonym
    if (!isSynonym(token.substr(1, len - 2))) {
        return false;
    }

    char lastChar = token[len -1];
    if (lastChar != '\"') {
        return false;
    }

    return true;
}

bool isConstant(std::string token) {
    char firstChar = token[0];
    if (firstChar == '0') {
        return false;
    }

    for (int i = 0; i < token.length(); i++) {
        if (!isdigit(token[i])) {
            return false;
        }
    }
    return true;
}

/**
 * Used to validate LHS and RHS of Calls and CallsT clause
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidProcedureEntRef(std::string token) {
    // mt1 valid pattern: "_", synonym, "IDENT"

    // verify if token is valid
    if (token == "_") {
        return true;
    } else if (isSynonym(token)) {
        if (!query.isSynonymPresent(token)) {
            throw QpsSemanticException("Synonym not declared!", lexer.getLineNo(), token);
        }

        // verify if token is a declared variable
        if (!query.isDeclarationPresent("procedure", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        }
        return true;
    } else {
        return isStringIdent(token);
    }
}

/**
 * Used to validate RHS of Modifies and Uses clause
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidVarEntRef(std::string token) {
    // mt1 valid pattern: "_", synonym, "IDENT"

    // verify if token is valid
    if (token == "_") {
        return true;
    } else if (isSynonym(token)) {
        if (!query.isSynonymPresent(token)) {
            throw QpsSemanticException("Synonym not declared!", lexer.getLineNo(), token);
        }

        // verify if token is a declared variable
        if (!query.isDeclarationPresent("variable", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        }
        return true;
    } else {
        return isStringIdent(token);
    }
}

/**
 * Used to validate LHS of Modifies and Uses Clause
 * stmtRef grammar with no wildcard allowed
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidNonWildcardStmtRef(std::string token, std::string clauseType) {
    // mt1 valid pattern: synonym, Integer

    // verify if token is valid
    if (token == "_") {
        throw QpsSemanticException("Semantic Error, invalid use of \"_\"", lexer.getLineNo());
    } else if (isSynonym(token)) {
        if (!query.isSynonymPresent(token)) {
            throw QpsSemanticException("Synonym not declared!", lexer.getLineNo(), token);
        }

        // verify if token is a valid declared stmt type synonym
        if (query.isDeclarationPresent("constant", token) || query.isDeclarationPresent("variable", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        } else if (clauseType == "Uses" && query.isDeclarationPresent("read", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        } else if (clauseType == "Modifies" && query.isDeclarationPresent("print", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        } else {
            return true;
        }
    } else if (isStringIdent(token)) {  // e.g. allow "proc"
        return true;
    } else {
        return isConstant(token);
    }
}

/**
 * Used to validate <LHS, RHS> of Follows, FollowsT, Parent, ParentT Clause
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidStrictStmtRef(std::string token) {
    // mt1 valid pattern: "_", synonym (except variable and procedure) , Integer
    // Semantic error: IDENT

    // verify if token is valid
    if (isStringIdent(token)) {
        throw QpsSemanticException("IDENT is not allowed", lexer.getLineNo(), token);
    }

    if (token == "_") {
        return true;
    } else if (isSynonym(token)) {
        if (!query.isSynonymPresent(token)) {
            throw QpsSemanticException("Synonym not declared!", lexer.getLineNo(), token);
        }

        // verify if token is a valid declared stmt type synonym
        if (query.isDeclarationPresent("variable", token) || query.isDeclarationPresent("procedure", token) || query.isDeclarationPresent("constant", token)) {
            throw QpsSemanticException("Invalid synonym used", lexer.getLineNo(), token);
        } else {
            return true;
        }

    } else {
        return isConstant(token);
    }
}

/**
 * Validates RHS of pattern clause
 * @param token rhs string to be verified
 * @return boolean indicating if rhs is valid
 */
bool QueryParser::matchValidExpressionSpec(std::string token) {
    // mt1 valid pattern: _, "_"x"_"
    // mt2 (additional): "_"v + x * y"_", "v + x", "v"
    // e.g. ((a + ((b) * (c))) - (((d) / (e)) % i))

    if (token.length() == 0) {
        return false;
    }

    if (token == "_") {
        return true;
    }

    int len = token.size();
    std::regex wildCardFormat("[_][\"][\\s\\da-zA-Z+\\-*\\/%\\(\\)]*[\"][_]");
    std::regex exactMatchFormat("[\"][\\s\\da-zA-Z+\\-*\\/%\\(\\)]*[\"]");

    if (std::regex_match(token, wildCardFormat)) {
        std::string substr = token.substr(2, len-4);
        return ExprParser::isValidExpression(substr);

    } else if (std::regex_match(token, exactMatchFormat)) {
        std::string substr = token.substr(1, len-2);
        return ExprParser::isValidExpression(substr);
    }
    return false;
}

bool QueryParser::matchWildcard(std::string token) {
    if (token == "_") {
        return true;
    }
    return false;
}

/**
 * Used to validate LHS of pattern clause
 * @param token
 * @return is token valid
 */
bool QueryParser::matchValidLPattern(std::string token) {
    // mt1 valid pattern: "_", variable synonym, ""x""

    if (!matchValidVarEntRef(token)) {
        return false;
    }

    // further filter entRef
    if (isSynonym(token)) {
        if (query.isDeclarationPresent("variable", token)) {
            return true;
        } else {
            throw QpsSemanticException("Invalid synonym for pattern clause", lexer.getLineNo(), token);
        }
    }
    return true;
}

std::string QueryParser::getPatternToken(std::string side) {
    std::string terminator = side == "LHS" ? "," : ")";
    int bracketCount = 0;
    std::string res;

    if (side == "LHS") {
        while (!lexer.match(terminator) && !lexer.isEnd()) {
            std::string curr = lexer.getCurrentToken();
            res = res + curr;
            lexer.eat(curr);
        }
    } else {  // RHS
        while (!(lexer.match(terminator) && bracketCount == 0) && !lexer.isEnd()) {
            std::string curr = lexer.getCurrentToken();
            if (curr == "(") {
                bracketCount += 1;
            } else if (curr == ")") {
                bracketCount -= 1;
            }

            if (bracketCount < 0 || lexer.isEnd()) {
                throw QpsSyntaxException("Brackets not matched");
            }

            res = res + curr;
            lexer.eat(curr);
        }
    }

    if (res == "") {
        throw QpsSyntaxException("Clause cannot be empty");
    }
    // std::cout <<"PATTERN TOKEN GIVEN: " << res << std::endl;
    return res;
}

std::string QueryParser::getClauseToken(std::string side) {
    std::string terminator = side == "LHS" ? "," : ")";
    std::string res;

    while (!lexer.match(terminator) && !lexer.isEnd()) {
        std::string curr = lexer.getCurrentToken();
        res = res + curr;
        lexer.eat(curr);
    }
    if (res == "") {
        throw QpsSyntaxException("Clause cannot be empty");
    }
    return res;
}

/**
 * Returns a string token
 * Where current lexer token is "
 * @return string token
 */
std::string QueryParser::getStringToken() {
    std::string terminator = "\"";
    std::string res = "\"";

    if (lexer.match("\"")) {
        lexer.eat("\"");
    } else {
        throw QpsSyntaxException("Error parsing string");
    }

    while (!lexer.match(terminator) && !lexer.isEnd()) {
        std::string curr = lexer.getCurrentToken();
        res = res + curr;
        lexer.eat(curr);
    }

    if (lexer.match("\"")) {
        lexer.eat("\"");
        res += "\"";
    } else {
        throw QpsSyntaxException("Expected a \"", lexer.getLineNo(), lexer.getCurrentToken());
    }

    if (res == "" || res == "\"\"") {
        throw QpsSyntaxException("Clause cannot be empty");
    }
    return res;
}

std::string QueryParser::assignClause(std::string clauseType, std::string side) {
    std::string currToken = getClauseToken(side);

    // grammar: <stmtRef, entRef>
    if (sub_clause_keywords_set.find(clauseType) != sub_clause_keywords_set.end()) {
        if (side == "LHS") {
            if (matchValidNonWildcardStmtRef(currToken, clauseType)) {
                return currToken;
            } else {
                throw QpsSyntaxException("invalid grammar", lexer.getLineNo(), currToken);
            }
        } else {  // RHS
            if (matchValidVarEntRef(currToken)) {
                return currToken;
            } else {
                throw QpsSyntaxException("invalid grammar", lexer.getLineNo(), currToken);
            }
        }
    } else if (entRef_clause_keywords_set.find(clauseType) != entRef_clause_keywords_set.end()) {
        if (matchValidProcedureEntRef(currToken)) {
            return currToken;
        } else {
            throw QpsSyntaxException("invalid grammar", lexer.getLineNo(), currToken);
        }
      } else {  // grammar: <stmtRef, stmtRef> (Parent, ParentT, Follows, FollowsT, Next, NextT, Affects, AffectsT)
        if (matchValidStrictStmtRef(currToken)) {
            return currToken;
        } else {
            throw QpsSyntaxException("invalid grammar", lexer.getLineNo(), currToken);
        }
    }
}

std::string QueryParser::assignPatternClause(std::string side, std::string patternType) {
    std::string terminator = side == "LHS" ? "," : ")";

    if (side == "LHS") {  // same rules for assign, while and if pattern
        std::string lPattern = getPatternToken("LHS");
        if (matchValidLPattern(lPattern)) {
            return lPattern;
        } else {
            throw QpsSyntaxException("invalid pattern", lexer.getLineNo(), lPattern);
        }
    } else if (side == "RHS") {  // RHS
        std::string rPattern = getPatternToken("RHS");
        if (patternType == "assign") {
            if (matchValidExpressionSpec(rPattern)) {
                return rPattern;
            } else {
                throw QpsSyntaxException("invalid pattern", lexer.getLineNo(), rPattern);
            }
        } else {
            // must be wildcard
            if (matchWildcard(rPattern)) {
                return rPattern;
            } else {
                throw QpsSyntaxException("invalid pattern, only wildcard allowed", lexer.getLineNo(), rPattern);
            }
        }
    } else {  // "mid"
        std::string mPattern = getPatternToken("LHS");
        if (matchWildcard(mPattern)) {
            return mPattern;
        } else {
            throw QpsSyntaxException("invalid pattern, only wildcard allowed", lexer.getLineNo(), mPattern);
        }
    }
}

std::tuple<std::string, std::string> QueryParser::validateDoubleBrackets(std::string patternType) {
    std::string LHS;
    std::string RHS;

    if (lexer.match("(")) {
        lexer.eat("(");
        LHS = this->assignPatternClause("LHS", patternType);
        if (lexer.match(",")) {
            lexer.eat(",");
            RHS = this->assignPatternClause("RHS", patternType);
            if (lexer.match(")")) {
                lexer.eat(")");
            } else {
                throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ")", lexer.getCurrentToken());
            }
        } else {
            throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ",", lexer.getCurrentToken());
        }
    } else {
        throw QpsSyntaxException("Syntax error", lexer.getLineNo(), "(", lexer.getCurrentToken());
    }

    // std::tuple<std::string, std::string> result;
    return std::make_tuple(LHS, RHS);
}

std::tuple<std::string, std::string, std::string> QueryParser::validateTripleBrackets(std::string patternType) {
    std::string LHS;
    std::string MID;
    std::string RHS;

    if (lexer.match("(")) {
        lexer.eat("(");
        LHS = this->assignPatternClause("LHS", patternType);

        if (lexer.match(",")) {
            lexer.eat(",");
            MID = this->assignPatternClause("LHS", patternType);

            if (lexer.match(",")) {
                lexer.eat(",");
                RHS = this->assignPatternClause("RHS", patternType);

                if (lexer.match(")")) {
                    lexer.eat(")");
                } else {
                    throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ")", lexer.getCurrentToken());
                }

            } else {
                throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ")", lexer.getCurrentToken());
            }
        } else {
            throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ",", lexer.getCurrentToken());
        }
    } else {
        throw QpsSyntaxException("Syntax error", lexer.getLineNo(), "(", lexer.getCurrentToken());
    }

    // std::tuple<std::string, std::string> result;
    return std::make_tuple(LHS, MID, RHS);
}

/**
 * Parse and validate pattern clause with syntax <a(LHS, RHS)>
 * where current lexer token is "a"
 * @param syn
 */
void QueryParser::parseAndPattern(std::string syn) {
    // check if patternId is declared
    if (!query.isSynonymPresent(syn)) {
        throw QpsSemanticException("Undeclared pattern assign synonym", lexer.getLineNo(), syn);
    }
    lexer.eat(syn);  // a

    // check patternId type
    std::string patternIdType = query.getSynonymType(syn);
    if (patternIdType == "assign") {
        std::tuple<std::string, std::string> result = validateDoubleBrackets(patternIdType);
        // populate clause list
        std::shared_ptr<Clause> currClause = std::make_shared<AssignPatternClause>(std::get<0>(result), std::get<1>(result), syn);
        PriorityEvaluator pe = PriorityEvaluator(currClause);
        currClause->setPriorityScore(pe.evaluatePriority());
        query.addClause(currClause);

    } else if (patternIdType == "while") {
        std::tuple<std::string, std::string> result = validateDoubleBrackets(patternIdType);
        // populate clause list
        std::shared_ptr<Clause> currClause = std::make_shared<WhilePatternClause>(std::get<0>(result), std::get<1>(result), syn);
        PriorityEvaluator pe = PriorityEvaluator(currClause);
        currClause->setPriorityScore(pe.evaluatePriority());
        query.addClause(currClause);

    } else if (patternIdType == "if") {
        std::tuple<std::string, std::string, std::string> result = validateTripleBrackets(patternIdType);
        // populate clause list
        std::shared_ptr<Clause> currClause = std::make_shared<IfPatternClause>(std::get<0>(result), std::get<1>(result), std::get<2>(result), syn);
        PriorityEvaluator pe = PriorityEvaluator(currClause);
        currClause->setPriorityScore(pe.evaluatePriority());
        query.addClause(currClause);

    } else {
        throw QpsSemanticException("Invalid pattern type", lexer.getLineNo(), patternIdType);
    }
    advanceConnectorState(1);
}

/**
 * Parse and validate with clause with syntax <LHS.attribute = RHS.attribute>
 * where current lexer token is "LHS"
 * @param syn
 */
void QueryParser::parseWithClause(std::string syn) {
    std::string lhs = syn;
    std::string lhsType;
    std::string rhs;
    std::string rhsType;

        if (lhs == "\"") {
            std::string strToken = getStringToken();
            lhs = strToken;
            lhsType = "_name";  // lhs is a name
        } else if (isConstant(lhs)) {
            lexer.eat(lhs);
            lhsType = "_constant";  // lhs is a constant
        } else {  // expect an attribute
            lexer.eat(lhs);
            if (lexer.match(".")) {
                lexer.eat(".");
                lhsType = isAttributeToken(lexer.getCurrentToken());
            } else {
                throw QpsSyntaxException("Expected an attribute");
            }
        }

        if (lexer.match("=")) {
            lexer.eat("=");
            rhs = lexer.getCurrentToken();

            if (rhs == "\"") {
                std::string strToken = getStringToken();
                rhs = strToken;
                rhsType = "_name";  // rhs is a name
            } else if (isConstant(rhs)) {
                lexer.eat(rhs);
                rhsType = "_constant";  // rhs is a constant
            } else {  // expect an attribute
                lexer.eat(rhs);
                if (lexer.match(".")) {
                    lexer.eat(".");
                    rhsType = isAttributeToken(lexer.getCurrentToken());
                } else {
                    throw QpsSyntaxException("Expected an attribute");
                }
            }
            isValidWithClause(lhsType, lhs, rhsType, rhs);
            std::shared_ptr<Clause> currClause = std::make_shared<WithClause>(lhsType, lhs, rhsType, rhs);
            PriorityEvaluator pe = PriorityEvaluator(currClause);
            currClause->setPriorityScore(pe.evaluatePriority());
            query.addClause(currClause);
        } else {
            throw QpsSyntaxException("Expected a = for with clause", lexer.getLineNo(), lexer.getCurrentToken());
        };
        advanceConnectorState(1);
}

/**
 * Check if <syn.attribute> is valid
 * Throws error if wrong synonym type is used
 * @param synType
 * @param attr
 */
void QueryParser::validateSynAndAttribute(std::string synType, std::string attr) {
    if (attr == "procName") {
        if (!(valid_procname_syn.find(synType) != valid_procname_syn.end())) {
            throw QpsSemanticException("Invalid synonym type for attribute procName");
        }
    } else if (attr == "varName") {
        if (!(valid_varname_syn.find(synType) != valid_varname_syn.end())) {
            throw QpsSemanticException("Invalid synonym type for attribute varName");
        }
    } else if (attr == "value") {
        if (synType != "constant") {
            throw QpsSemanticException("Invalid synonym type for attribute value");
        }
    } else if (attr == "stmt#") {
        if (!(valid_stmt_syn.find(synType) != valid_stmt_syn.end())) {
            throw QpsSemanticException("Invalid synonym type for attribute stmt#");
        }
    } else {
        std::cout << "Invalid attr (Should not be here)" << std::endl;
    }
}

/**
 * Returns the expected type of the attribute
 * @param attr
 * @return type "_name" or "_constant"
 */
std::string getAttributeAssignmentType(std::string attr) {
    if (attr == "procName" || attr == "varName") {
        return "_name";
    } else {  // for value or stmt#
        return "_constant";
    }
}

/**
 * Validate with clause parameters
 * Throws error if types used or compared are not valid
 * @param leftType
 * @param leftName
 * @param rightType
 * @param rightName
 */
void QueryParser::isValidWithClause(std::string leftType, std::string leftName, std::string rightType, std::string rightName) {
    if (leftType == "_name") {
        if (rightType == "_name" || rightType == "_constant") {
            throw QpsSemanticException("Invalid with clause comparison");
        }

        if (query.isSynonymPresent(rightName)) {
            std::string rightSynType = query.getSynonymType(rightName);
            validateSynAndAttribute(rightSynType, rightType);
        } else {
            throw QpsSemanticException("Undeclared with clause synonym");
        }

        if (getAttributeAssignmentType(rightType) != "_name") {
            throw QpsSemanticException("Invalid with clause comparison");
        }

    } else if (leftType == "_constant") {
        if (rightType == "_name" || rightType == "_constant") {
            throw QpsSemanticException("Invalid with clause comparison");
        }

        if (query.isSynonymPresent(rightName)) {
            std::string rightSynType = query.getSynonymType(rightName);
            validateSynAndAttribute(rightSynType, rightType);
        } else {
            throw QpsSemanticException("Undeclared with clause synonym");
        }

        if (getAttributeAssignmentType(rightType) != "_constant") {
            throw QpsSemanticException("Invalid with clause comparison");
        }

    } else {  // left type is an attribute
        if (query.isSynonymPresent(leftName)) {
            std::string leftSynType = query.getSynonymType(leftName);
            validateSynAndAttribute(leftSynType, leftType);
        } else {
            throw QpsSemanticException("Undeclared with clause synonym");
        }

        if (rightType == "_name") {
            if (getAttributeAssignmentType(leftType) != "_name") {
                throw QpsSemanticException("Invalid with clause comparison");
            }
        } else if (rightType == "_constant") {
            if (getAttributeAssignmentType(leftType) != "_constant") {
                throw QpsSemanticException("Invalid with clause comparison");
            }

        } else {  // rhs is an attribute
            if (query.isSynonymPresent(rightName)) {
                std::string rightSynType = query.getSynonymType(rightName);
                validateSynAndAttribute(rightSynType, rightType);

                if (getAttributeAssignmentType(leftType) != getAttributeAssignmentType(rightType)) {
                    throw QpsSemanticException("Invalid with clause comparison");
                }
            } else {
                throw QpsSemanticException("Undeclared with clause synonym");
            }
        }
    }
}

void QueryParser::parseClauses(std::string token) {
    std::string clauseType = token;
    std::shared_ptr<Clause> currClause;
    std::string LHS;
    std::string RHS;

    if (clauseType == "pattern") {  // assign, while and if pattern clause
        std::string patternId;
        lexer.eat(clauseType);
        if (lexer.matchIdentifier()) {
            patternId = lexer.eatIdentifier();  // "a" of a (LHS, RHS)

            // check if patternId is declared
            if (!query.isSynonymPresent(patternId)) {
                throw QpsSemanticException("Undeclared pattern assign synonym", lexer.getLineNo(), patternId);
            }

            // check patternId type
            std::string patternIdType = query.getSynonymType(patternId);
            if (patternIdType == "assign") {
                std::tuple<std::string, std::string> result = validateDoubleBrackets(patternIdType);
                // populate clause list
                currClause = std::make_shared<AssignPatternClause>(std::get<0>(result), std::get<1>(result), patternId);

            } else if (patternIdType == "while") {
                std::tuple<std::string, std::string> result = validateDoubleBrackets(patternIdType);
                // populate clause list
                currClause = std::make_shared<WhilePatternClause>(std::get<0>(result), std::get<1>(result), patternId);

            } else if (patternIdType == "if") {
                std::tuple<std::string, std::string, std::string> result = validateTripleBrackets(patternIdType);
                // populate clause list
                currClause = std::make_shared<IfPatternClause>(std::get<0>(result), std::get<1>(result), std::get<2>(result), patternId);

            } else {
                throw QpsSemanticException("Invalid pattern type", lexer.getLineNo(), patternIdType);
            }

        } else {
            throw QpsSyntaxException("Expected an assign synonym", lexer.getLineNo(), lexer.getCurrentToken());
        }

        PriorityEvaluator pe = PriorityEvaluator(currClause);
        currClause->setPriorityScore(pe.evaluatePriority());
        query.addClause(currClause);
        advanceClauseState(2);  // pattern state

    } else if (clauseType == "with") {
        lexer.eat(clauseType);
        std::string curr = lexer.getCurrentToken();  // lhs name
        parseWithClause(curr);  // with clause will be instantiated here
        advanceClauseState(3);  // with clause state

    } else {  // such that clause
        lexer.eat(clauseType);

        // identify clauses of type Clause*
        if (lexer.getCurrentToken() == "*") {
            lexer.eat("*");
            clauseType = clauseType + "T";
        }

        if (lexer.match("(")) {
            lexer.eat("(");
            LHS = this->assignClause(clauseType , "LHS");
            if (lexer.match(",")) {
                lexer.eat(",");
                RHS = this->assignClause(clauseType, "RHS");
                if (lexer.match(")")) {
                    lexer.eat(")");
                } else {
                    throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ")", lexer.getCurrentToken());
                }
            } else {
                throw QpsSyntaxException("Syntax error", lexer.getLineNo(), ",", lexer.getCurrentToken());
            }
        } else {
            throw QpsSyntaxException("Syntax error", lexer.getLineNo(), "(", lexer.getCurrentToken());
        }

        // populate clause list
        if (clauseType == "Uses") {
            currClause = std::make_shared<UsesClause>(LHS, RHS);
        } else if (clauseType == "Modifies") {
            currClause = std::make_shared<ModifiesClause>(LHS, RHS);
        } else if (clauseType == "Follows") {
            currClause = std::make_shared<FollowsClause>(LHS, RHS);
        } else if (clauseType == "FollowsT") {
            currClause = std::make_shared<FollowsTClause>(LHS, RHS);
        } else if (clauseType == "Parent") {
            currClause = std::make_shared<ParentClause>(LHS, RHS);
        } else if (clauseType == "ParentT") {
            currClause = std::make_shared<ParentTClause>(LHS, RHS);
        } else if (clauseType == "Calls") {
            currClause = std::make_shared<CallsClause>(LHS, RHS);
        } else if (clauseType == "CallsT") {
            currClause = std::make_shared<CallsTClause>(LHS, RHS);
        } else if (clauseType == "Next") {
            currClause = std::make_shared<NextClause>(LHS, RHS);
        } else if (clauseType == "NextT") {
            currClause = std::make_shared<NextTClause>(LHS, RHS);
        } else if (clauseType == "Affects") {
            currClause = std::make_shared<AffectsClause>(LHS, RHS);
        } else if (clauseType == "AffectsT") {
            currClause = std::make_shared<AffectsTClause>(LHS, RHS);
        } else {
            // unable to evaluate yet (should not reach here)
            throw QpsSemanticException("Unable to evaluate current clause type");
        }
        PriorityEvaluator pe = PriorityEvaluator(currClause);
        currClause->setPriorityScore(pe.evaluatePriority());
        query.addClause(currClause);
        advanceClauseState(1);  // such that clause state
    }
};


