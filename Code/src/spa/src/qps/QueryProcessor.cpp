//
// Created by Vanessa Khor on 5/2/23.
//

#include "QueryProcessor.h"
#include "QueryLexer.h"
#include "QueryParser.h"
#include "QueryEvaluator/QueryEvaluator.h"
#include "pkb/Facade/QPSFacade.h"
#include "QueryEvaluator/ClauseResult.h"
#include "ResponseBuilder.h"

QueryProcessor::QueryProcessor(QPSFacade& qpsFacade) : qpsFacade(qpsFacade) {}

// takes in PQL from user input
std::vector<std::string> QueryProcessor::process(const std::string& input) {
    try {
        std::shared_ptr<std::istringstream> iss = std::make_shared<std::istringstream>(input);
        QueryLexer queryLexer = QueryLexer(iss);
        Query query = Query();
        QueryParser queryParser = QueryParser(queryLexer, query);
        queryParser.parse();

        QueryEvaluator queryEvaluator = QueryEvaluator();
        ClauseResult finalResult = queryEvaluator.computeFinalResult(query, qpsFacade);
        ResponseBuilder responseBuilder = ResponseBuilder(qpsFacade, query);
        std::vector<std::string> queryResponse = responseBuilder.formatOutput(finalResult);
        return queryResponse;
    } catch (QpsSemanticException& e) {
        std::vector<std::string> errMessage;
        errMessage.push_back(e.msg);
        return errMessage;
    } catch (QpsSyntaxException& e) {
        std::vector<std::string> errMessage;
        errMessage.push_back(e.msg);
        return errMessage;
    }
}

