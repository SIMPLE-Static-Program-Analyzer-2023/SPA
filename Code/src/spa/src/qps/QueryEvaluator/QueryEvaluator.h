#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <unordered_map>
#include "qps/ClauseGroup/ClauseGroup.h"
#include "qps/ClauseGroup/ClauseGrouper.h"
#include "qps/Query.h"
#include "qps/Clause.h"
#include "ClauseResult.h"
#include "pkb/Facade/QPSFacade.h"
#include "ClauseResultEvaluator.h"

class QueryEvaluator {
public:
    // Get results from PKB for each clause and combines them
    // to obtain the final result to pass to ResponseBuilder
    static ClauseResult computeFinalResult(Query &query, QPSFacade& qpsFacade);
};
