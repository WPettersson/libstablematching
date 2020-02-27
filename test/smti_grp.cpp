#include "catch.hpp"
#include "smti.h"

TEST_CASE( "Read instances in SMTI-GRP format.", "[smti-grp]" ) {
  SMTI grp =  SMTI::create_from_GRP("grp-test-small.instance");
  REQUIRE( grp.agents_left() == 3 );
  REQUIRE( grp.agents_right() == 3 );
}

TEST_CASE( "Read instances in SMTI-GRP format with thresholding.", "[smti-grp]" ) {
  SMTI grp =  SMTI::create_from_GRP("grp-test-small.instance", 4);
  REQUIRE( grp.agents_left() == 3 );
  REQUIRE( grp.agents_right() == 3 );
  Agent a = grp.agent_left(0);
  std::cout << "0 is " << a.pref_list_string() << std::endl;
  REQUIRE( a.num_prefs() == 1);
  a = grp.agent_left(1);
  REQUIRE( a.num_prefs() == 1);
  a = grp.agent_left(2);
  REQUIRE( a.num_prefs() == 1);
  grp =  SMTI::create_from_GRP("grp-test-small.instance", 2);
  REQUIRE( grp.agents_left() == 3 );
  REQUIRE( grp.agents_right() == 3 );
  a = grp.agent_left(0);
  std::cout << "0 is " << a.pref_list_string() << std::endl;
  REQUIRE( a.num_prefs() == 2);
  a = grp.agent_left(1);
  REQUIRE( a.num_prefs() == 2);
  a = grp.agent_left(2);
  REQUIRE( a.num_prefs() == 2);
}
