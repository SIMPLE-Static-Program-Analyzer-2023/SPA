//
// Created by Vanessa Khor on 5/2/23.
//

#include <string>
#include <vector>
#include <unordered_map>
#include "pkb/Facade/QPSFacade.h"
#include "QueryEvaluator/ClauseResult.h"
#include "Query.h"
#include "QpsException.h"

class ResponseBuilder {
private:
	QPSFacade qpsf;
	Query query;

	std::string findTypeOf(const std::string& name);
	std::string getAttributeSynonym(const std::string& s);
public:
	ResponseBuilder(QPSFacade& qpsf, Query& query);
	std::vector<std::string> formatOutput(ClauseResult &res);
	// std::string formatString(std::vector<std::string> list);
};
