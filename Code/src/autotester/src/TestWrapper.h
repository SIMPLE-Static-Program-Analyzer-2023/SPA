#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <memory>
#include <sstream>
// include your other headers here
#include <vector>
#include "AbstractWrapper.h"
#include "pkb/PKB.h"
#include "sp/Parser.h"
#include "sp/Lexer.h"
#include "sp/SpManager.h"
#include "qps/QueryProcessor.h"

class TestWrapper : public AbstractWrapper {
 public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);

  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif
