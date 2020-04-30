#include "catch.hpp"
#include "smti.h"

TEST_CASE( "Read instance in SMTI-GRP format.", "[smti-grp]" ) {
  SMTI grp =  SMTI::create_from_GRP("grp-test-small.instance");
  REQUIRE( grp.num_agents_left() == 3 );
  REQUIRE( grp.num_agents_right() == 3 );
}

TEST_CASE( "Read unbalanced instance in SMTI-GRP format.", "[smti-grp]" ) {
  SMTI grp =  SMTI::create_from_GRP("grp-test-small-unbalanced.instance");
  REQUIRE( grp.num_agents_left() == 3 );
  REQUIRE( grp.num_agents_right() == 4 );
}

TEST_CASE( "Read instances in SMTI-GRP format with thresholding.", "[smti-grp]" ) {
  SMTI grp =  SMTI::create_from_GRP("grp-test-small.instance", 4);
  REQUIRE( grp.num_agents_left() == 3 );
  REQUIRE( grp.num_agents_right() == 3 );
  Agent a = grp.agent_left(0);
  REQUIRE( a.num_prefs() == 1);
  a = grp.agent_left(1);
  REQUIRE( a.num_prefs() == 1);
  a = grp.agent_left(2);
  REQUIRE( a.num_prefs() == 1);
  grp =  SMTI::create_from_GRP("grp-test-small.instance", 2);
  REQUIRE( grp.num_agents_left() == 3 );
  REQUIRE( grp.num_agents_right() == 3 );
  a = grp.agent_left(0);
  REQUIRE( a.num_prefs() == 2);
  a = grp.agent_left(1);
  REQUIRE( a.num_prefs() == 2);
  a = grp.agent_left(2);
  REQUIRE( a.num_prefs() == 2);
}
