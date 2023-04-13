#include "CFGFacade.h"

CFGFacade::CFGFacade(std::shared_ptr<CfgGenerator> cfgGen) {
	cfgGenerator = cfgGen;
}

void CFGFacade::generateCfgs() {
	cfgGenerator->generateCfgs();
}

void CFGFacade::populateRelationshipManager() {
	cfgGenerator->populateRelationshipManager();
}
