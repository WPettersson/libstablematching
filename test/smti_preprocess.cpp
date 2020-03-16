#include "catch.hpp"
#include "smti.h"
#include <iostream>

TEST_CASE( "Preprocess trivial instance.", "[preprocess]" ) {
  {
    SMTI grp =  SMTI::create_from_GRP("grp-test-small.instance");
    REQUIRE( grp.agents_left() == 3 );
    REQUIRE( grp.agents_right() == 3 );
    for(int i = 0; i < 3; ++i) {
      REQUIRE( grp.agent_left(i).num_prefs() == 3 );
      REQUIRE( grp.agent_right(i).num_prefs() == 3 );
    }
    grp.preprocess(SMTI::PreprocessMode::Complete);
    REQUIRE( grp.agents_left() == 3 );
    REQUIRE( grp.agents_right() == 3 );
    REQUIRE( grp.agent_left(0).num_prefs() == 1 );
    REQUIRE( grp.agent_right(0).num_prefs() == 1 );
    REQUIRE( grp.agent_left(1).num_prefs() == 2 );
    REQUIRE( grp.agent_right(1).num_prefs() == 2 );
    REQUIRE( grp.agent_left(2).num_prefs() == 2 );
    REQUIRE( grp.agent_right(2).num_prefs() == 2 );
  }
}

TEST_CASE( "Preprocess small unbalanced instance.", "[preprocess]" ) {
  {
    SMTI grp =  SMTI::create_from_GRP("grp-test-small-unbalanced.instance");
    REQUIRE( grp.agents_left() == 3 );
    REQUIRE( grp.agents_right() == 4 );
    for(int i = 0; i < 3; ++i) {
      REQUIRE( grp.agent_left(i).num_prefs() == 4 );
      REQUIRE( grp.agent_right(i).num_prefs() == 3 );
    }
    REQUIRE( grp.agent_right(3).num_prefs() == 3 );
    grp.preprocess(SMTI::PreprocessMode::Complete);
    REQUIRE( grp.agents_left() == 3 );
    REQUIRE( grp.agents_right() == 4 );
    REQUIRE( grp.agent_left(0).num_prefs() == 1 );
    REQUIRE( grp.agent_right(0).num_prefs() == 1 );
    REQUIRE( grp.agent_left(1).num_prefs() == 2 );
    REQUIRE( grp.agent_right(1).num_prefs() == 2 );
    REQUIRE( grp.agent_left(2).num_prefs() == 2 );
    REQUIRE( grp.agent_right(2).num_prefs() == 0 );
    REQUIRE( grp.agent_right(3).num_prefs() == 2 );
  }
}

TEST_CASE( "Preprocess medium instance.", "[preprocess]" ) {
  {
    SMTI grp =  SMTI::create_from_GRP("grp-test-medium.instance");
    REQUIRE( grp.agents_left() == 10 );
    REQUIRE( grp.agents_right() == 10 );
    for(int i = 0; i < 10; ++i) {
      REQUIRE( grp.agent_left(i).num_prefs() == 10 );
      REQUIRE( grp.agent_right(i).num_prefs() == 10 );
    }
    grp.preprocess(SMTI::PreprocessMode::Complete);
    REQUIRE( grp.agents_left() == 10 );
    REQUIRE( grp.agents_right() == 10 );
    REQUIRE( grp.agent_left(0).num_prefs() == 3 );
    REQUIRE( grp.agent_right(0).num_prefs() == 3 );
    REQUIRE( grp.agent_left(1).num_prefs() == 1 );
    REQUIRE( grp.agent_right(1).num_prefs() == 2 );
    REQUIRE( grp.agent_left(2).num_prefs() == 4 );
    REQUIRE( grp.agent_right(2).num_prefs() == 6 );
    REQUIRE( grp.agent_left(3).num_prefs() == 6 );
    REQUIRE( grp.agent_right(3).num_prefs() == 1 );
    REQUIRE( grp.agent_left(4).num_prefs() == 1 );
    REQUIRE( grp.agent_right(4).num_prefs() == 6 );
    REQUIRE( grp.agent_left(5).num_prefs() == 2 );
    REQUIRE( grp.agent_right(5).num_prefs() == 6 );
    REQUIRE( grp.agent_left(6).num_prefs() == 4 );
    REQUIRE( grp.agent_right(6).num_prefs() == 3 );
    REQUIRE( grp.agent_left(7).num_prefs() == 5 );
    REQUIRE( grp.agent_right(7).num_prefs() == 4 );
    REQUIRE( grp.agent_left(8).num_prefs() == 6 );
    REQUIRE( grp.agent_right(8).num_prefs() == 1 );
    REQUIRE( grp.agent_left(9).num_prefs() == 1 );
    REQUIRE( grp.agent_right(9).num_prefs() == 1 );
  }
}
