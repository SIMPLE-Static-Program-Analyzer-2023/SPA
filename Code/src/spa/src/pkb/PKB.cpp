#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB.h"

std::shared_ptr<SPFacade> PKB::getSPFacade() {
    std::shared_ptr<SPFacade> facade = std::make_shared<SPFacade>(patternManager, entityManager,
                                                            relationshipManager);
	return facade;
}

QPSFacade PKB::getQPSFacade() {
	return QPSFacade(patternManager, entityManager, relationshipManager);
}

CFGFacade PKB::getCFGFacade() {
	return CFGFacade(cfgGen);
}
