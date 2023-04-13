#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

PKB pkb;
std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
QPSFacade qpsFacade = pkb.getQPSFacade();
CFGFacade cfgFacade = pkb.getCFGFacade();
// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
    // ...rest of your code...
    std::shared_ptr<std::ifstream> src = std::make_shared<std::ifstream>(filename);
    SpManager spManager;
    AstRoot root = spManager.processSIMPLE(src);
    spManager.extractAndPopulate(root, pkb);
    cfgFacade.generateCfgs();
    cfgFacade.populateRelationshipManager();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
    auto qps = QueryProcessor(qpsFacade);
    std::vector<std::string> list = qps.process(query);
    for (const std::string& i : list) {
        results.push_back(i);
    }
}
