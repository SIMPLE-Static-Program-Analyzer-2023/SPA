#include "catch.hpp"
#include "pkb/PKB.h"

TEST_CASE("PKB, getQPSFacade works test") {
	PKB pkb;
    std::shared_ptr<SPFacade> spFacade = pkb.getSPFacade();
	QPSFacade qpsFacade = pkb.getQPSFacade();
	spFacade->addFollows(1, 2);
	REQUIRE(qpsFacade.getPreceding(2) == 1);
	REQUIRE(qpsFacade.getFollowing(1) == 2);
}