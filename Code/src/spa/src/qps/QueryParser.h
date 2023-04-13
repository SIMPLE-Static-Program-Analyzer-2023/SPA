#pragma once
#include <string>
#include <list>
#include <unordered_set>
#include <memory>
#include <iostream>
#include <vector>
#include <set>
#include <regex> // NOLINT

#include "QueryLexer.h"
#include "Declaration.h"
#include "Query.h"
#include "Clause.h"
#include "Query.h"
#include "QpsException.h"
#include <tuple>
#include "sp/ExprParser.h"
#include "PriorityEvaluator/PriorityEvaluator.h"

class QueryParser {
private:
    QueryLexer lexer;
    Query& query;

    enum QueryState {
        Initialization,
        DeclarationStage,
        SelectStage,
        PostSelectStage,
        ClauseStage,
    };

    enum ClauseState {
        ClauseInitialization,
        SuchThatClause,
        PatternClause,
        WithClauseStage,
        andSuchThatClause,
        andPatternClause,
        andWithClause,
    };

    enum ClauseConnectorState {  // for "and"
        ConnectorInitialization,  // can have connector or new clause type
        ExpectingConnector,  // or new clause type
        ExpectingAndClause,  // only if "and" is identified
        ConnectorPreviouslyDeclared,
    };

    QueryState currentState = QueryState::Initialization;

    ClauseState expectedClauseState = ClauseState::ClauseInitialization;

    ClauseConnectorState currentConnectorState = ClauseConnectorState::ConnectorInitialization;

    void advanceState(int nxt);

    void advanceClauseState(int nxt);

    void advanceToAndStage();

    void advanceConnectorState(int nxt);

    std::string assignClause(std::string clauseType, std::string side);

    std::string assignPatternClause(std::string side, std::string patternType);

    void isTokenValid(std::string input);

    void isConnectorTokenValid(std::string token);

    void isClauseTokenValid(std::string token);

    bool isStringIdent(std::string token);

    bool isSynonym(std::string token);

    void parseDeclarations(std::string input);

    void parseClauses(std::string input);

    void parseAndPattern(std::string syn);

    void parseWithClause(std::string syn);

    void handleSelectClause();

    void setSelectedTarget();

    std::tuple<std::string, std::string> validateDoubleBrackets(std::string patternType);
    std::tuple<std::string, std::string,  std::string> validateTripleBrackets(std::string patternType);

    std::string getPatternToken(std::string side);

    std::string getClauseToken(std::string side);

    std::string getStringToken();

    std::string isAttributeToken(std::string token);

    void validateSynAndAttribute(std::string synType, std::string attr);

    void isValidWithClause(std::string leftType, std::string leftName, std::string rightType, std::string rightName);

    // to validate clause tokens
    bool matchValidVarEntRef(std::string token);
    bool matchValidProcedureEntRef(std::string token);
    bool matchValidExpressionSpec(std::string token);
    bool matchValidStrictStmtRef(std::string token);
    bool matchValidNonWildcardStmtRef(std::string token, std::string clauseType);
    bool matchValidLPattern(std::string token);
    bool matchWildcard(std::string token);

public:
    void parse();
    explicit QueryParser(QueryLexer& lexer, Query& query);
};
