#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "pkb/Manager/PatternManager.h"
#include "pkb/Manager/EntityManager.h"
#include "pkb/Manager/RelationshipManager.h"
#include "CfgGenerator.h"
#include "pkb/Facade/SPFacade.h"
#include "pkb/Facade/QPSFacade.h"
#include "pkb/Facade/CFGFacade.h"



class PKB {
private:
	std::shared_ptr<PatternManager> patternManager = std::make_shared<PatternManager>();
	std::shared_ptr<EntityManager> entityManager = std::make_shared<EntityManager>();
	std::shared_ptr<RelationshipManager> relationshipManager = std::make_shared<RelationshipManager>();
	std::shared_ptr<CfgGenerator> cfgGen = std::make_shared<CfgGenerator>(entityManager, relationshipManager);
public:
    std::shared_ptr<SPFacade> getSPFacade();
	QPSFacade getQPSFacade();
	CFGFacade getCFGFacade();
};
