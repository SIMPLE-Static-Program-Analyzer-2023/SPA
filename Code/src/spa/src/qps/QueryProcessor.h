//
// Created by Vanessa Khor on 5/2/23.
//
#pragma once
#include <iostream>
#include <string>
#include <istream>
#include <memory>
#include <vector>
#include "pkb/Facade/QPSFacade.h"
#include "pkb/PKB.h"

class QueryProcessor {
    QPSFacade& qpsFacade;
public:
    explicit QueryProcessor(QPSFacade& qpsFacade);
    std::vector<std::string> process(const std::string& input);
};

