# pragma once
#include <memory>
#include "pkb/CfgGenerator.h"

class CFGFacade {
public:
	explicit CFGFacade(std::shared_ptr<CfgGenerator> cfgGen);

	/**
	 * Generate a CFG for every procedure. 
	 */
	void generateCfgs();

	/**
	 * Populate RelationshipManager with the necessary information.
	 */
	void populateRelationshipManager();
private:
	std::shared_ptr<CfgGenerator> cfgGenerator;
};
