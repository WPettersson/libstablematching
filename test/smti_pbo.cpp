#include "catch.hpp"
#include "smti.h"
#include <iostream>

//TEST_CASE( "Encode trivial instance.", "[PBO]" ) {
//  {
//    SMTI grp =  SMTI::create_from_GRP("grp-test-small.instance");
//    REQUIRE( grp.agents_left() == 3 );
//    REQUIRE( grp.agents_right() == 3 );
//    for(int i = 0; i < 3; ++i) {
//      REQUIRE( grp.agent_left(i).num_prefs() == 3 );
//      REQUIRE( grp.agent_right(i).num_prefs() == 3 );
//    }
//    grp.preprocess(SMTI::PreprocessMode::Complete);
//    REQUIRE( grp.agents_left() == 3 );
//    REQUIRE( grp.agents_right() == 3 );
//    REQUIRE( grp.agent_left(0).num_prefs() == 1 );
//    REQUIRE( grp.agent_right(0).num_prefs() == 1 );
//    REQUIRE( grp.agent_left(1).num_prefs() == 2 );
//    REQUIRE( grp.agent_right(1).num_prefs() == 2 );
//    REQUIRE( grp.agent_left(2).num_prefs() == 2 );
//    REQUIRE( grp.agent_right(2).num_prefs() == 2 );
//  }
//}


TEST_CASE( "Encode instance with ties, no merged constraints", "[PBO]") {
  SMTI instance("test-ties.instance");
  // TODO Better way of testing encodings? One problem is that I want to have
  // comments explaining the expected encoding, which is easy with this method.
  std::string expected("* #variable= 24 #constraint= 25\n"
"* silly comment\n"
"* 4 with 3 is 1\n"
"* 4 with 4 is 2\n"
"* 4 with 1 is 3\n"
"* 4 with 2 is 4\n"
"* 3 with 3 is 5\n"
"* 3 with 4 is 6\n"
"* 3 with 1 is 7\n"
"* 3 with 2 is 8\n"
"* 1 with 1 is 9\n"
"* 1 with 2 is 10\n"
"* 1 with 3 is 11\n"
"* 1 with 4 is 12\n"
"* 2 with 1 is 13\n"
"* 2 with 2 is 14\n"
"* 2 with 3 is 15\n"
"* 2 with 4 is 16\n"
"* 3 with 4 is 6\n"
"* 4 with 4 is 2\n"
"* 1 with 4 is 12\n"
"* 2 with 4 is 16\n"
"* 3 with 3 is 5\n"
"* 4 with 3 is 1\n"
"* 1 with 3 is 11\n"
"* 2 with 3 is 15\n"
"* 1 with 1 is 9\n"
"* 2 with 1 is 13\n"
"* 3 with 1 is 7\n"
"* 4 with 1 is 3\n"
"* 1 with 2 is 10\n"
"* 2 with 2 is 14\n"
"* 3 with 2 is 8\n"
"* 4 with 2 is 4\n"
"min: 1 x17 1 x18 1 x19 1 x20 1 x21 1 x22 1 x23 1 x24 ;\n" // Minimise unassigned agents
"1 x1 1 x2 1 x3 1 x4 1 x17  = 1;\n" // Agent L4 capacity
"1 x5 1 x6 1 x7 1 x8 1 x18  = 1;\n" // Agent L3 capacity
"1 x9 1 x10 1 x11 1 x12 1 x19  = 1;\n" // Agent L2 capacity
"1 x13 1 x14 1 x15 1 x16 1 x20  = 1;\n" // Agent L1 capacity
"1 x6 1 x2 1 x12 1 x16 1 x21  = 1;\n" // Agent R4 capacity
"1 x5 1 x1 1 x11 1 x15 1 x22  = 1;\n" // Agent R3 capacity
"1 x9 1 x13 1 x7 1 x3 1 x23  = 1;\n" // Agent R2 capacity
"1 x10 1 x14 1 x8 1 x4 1 x24  = 1;\n" // Agent R1 capacity
"1 x1 1 x2 1 x5 >= 1;\n" // If L4 doesn't get R3 or R4, then R3 better have L3 (at least as good as L4 to R3)
"1 x1 1 x2 1 x6 >= 1;\n" // If L4 doesn't get R3 or R4, then R4 better have L3 (at least as good as L4 to R4)
"1 x1 1 x2 1 x3 1 x4 1 x7 1 x9 1 x13 >= 1;\n" // If L4 not assigned, then R1 must get something
"1 x1 1 x2 1 x3 1 x4 1 x8 1 x10 1 x14 >= 1;\n" // If L4 not assigned, then R2 better get something
"1 x1 1 x5 1 x6 >= 1;\n" // If L3 doesn't get R3 or R4, then R3 better have L4 (at least as good as L3 to R3)
"1 x2 1 x5 1 x6 >= 1;\n" // If L3 doesn't get R3 or R4, then R4 better have L4 (at least as good as L3 to R4)
"1 x3 1 x5 1 x6 1 x7 1 x8 1 x9 1 x13 >= 1;\n" // If L3 not assigned, then R1 better get something
"1 x4 1 x5 1 x6 1 x7 1 x8 1 x10 1 x14 >= 1;\n" // If L3 not assigned, then R2 better get something
"1 x9 1 x10 1 x13 >= 1;\n" // If L1 doesn't get R1 or R2, then R1 better have L2 (at least as good as L1 to R1)
"1 x9 1 x10 1 x14 >= 1;\n" // If L1 doesn't get R1 or R2, then R2 better have L2 (at least as good as L1 to R2)
"1 x1 1 x5 1 x9 1 x10 1 x11 1 x12 1 x15 >= 1;\n" // If L1 not assigned, then R3 must get something
"1 x2 1 x6 1 x9 1 x10 1 x11 1 x12 1 x16 >= 1;\n" // If L1 not assigned, then R4 must get something
"1 x9 1 x13 1 x14 >= 1;\n" // If L2 doesn't get R1 or R2, then R1 better get L1 (at least as good as L2 to R1)
"1 x10 1 x13 1 x14 >= 1;\n" // If L2 doesn't get R1 or R2, then R2 better get L1 (at least as good as L2 to R2)
"1 x1 1 x5 1 x11 1 x13 1 x14 1 x15 1 x16 >= 1;\n" // If L2 not assigned, then R3 must get something
"1 x2 1 x6 1 x12 1 x13 1 x14 1 x15 1 x16 >= 1;\n" // If L2 not assigned, then R4 must get something
"1 x17 1 x18 1 x19 1 x20 -1 x21 -1 x22 -1 x23 -1 x24 = 0;\n"); // For each L not assigned, an R must also be unassigned
  REQUIRE( expected == instance.encodePBO2(false));
}

//TEST_CASE( "Preprocess small unbalanced instance.", "[preprocess]" ) {
//  {
//    SMTI grp =  SMTI::create_from_GRP("grp-test-small-unbalanced.instance");
//    REQUIRE( grp.agents_left() == 3 );
//    REQUIRE( grp.agents_right() == 4 );
//    for(int i = 0; i < 3; ++i) {
//      REQUIRE( grp.agent_left(i).num_prefs() == 4 );
//      REQUIRE( grp.agent_right(i).num_prefs() == 3 );
//    }
//    REQUIRE( grp.agent_right(3).num_prefs() == 3 );
//    grp.preprocess(SMTI::PreprocessMode::Complete);
//    REQUIRE( grp.agents_left() == 3 );
//    REQUIRE( grp.agents_right() == 4 );
//    REQUIRE( grp.agent_left(0).num_prefs() == 1 );
//    REQUIRE( grp.agent_right(0).num_prefs() == 1 );
//    REQUIRE( grp.agent_left(1).num_prefs() == 2 );
//    REQUIRE( grp.agent_right(1).num_prefs() == 2 );
//    REQUIRE( grp.agent_left(2).num_prefs() == 2 );
//    REQUIRE( grp.agent_right(2).num_prefs() == 0 );
//    REQUIRE( grp.agent_right(3).num_prefs() == 2 );
//  }
//}

//TEST_CASE( "Preprocess medium instance.", "[preprocess]" ) {
//  {
//    SMTI grp =  SMTI::create_from_GRP("grp-test-medium.instance");
//    REQUIRE( grp.agents_left() == 10 );
//    REQUIRE( grp.agents_right() == 10 );
//    for(int i = 0; i < 10; ++i) {
//      REQUIRE( grp.agent_left(i).num_prefs() == 10 );
//      REQUIRE( grp.agent_right(i).num_prefs() == 10 );
//    }
//    grp.preprocess(SMTI::PreprocessMode::Complete);
//    REQUIRE( grp.agents_left() == 10 );
//    REQUIRE( grp.agents_right() == 10 );
//    REQUIRE( grp.agent_left(0).num_prefs() == 3 );
//    REQUIRE( grp.agent_right(0).num_prefs() == 3 );
//    REQUIRE( grp.agent_left(1).num_prefs() == 1 );
//    REQUIRE( grp.agent_right(1).num_prefs() == 2 );
//    REQUIRE( grp.agent_left(2).num_prefs() == 4 );
//    REQUIRE( grp.agent_right(2).num_prefs() == 6 );
//    REQUIRE( grp.agent_left(3).num_prefs() == 6 );
//    REQUIRE( grp.agent_right(3).num_prefs() == 1 );
//    REQUIRE( grp.agent_left(4).num_prefs() == 1 );
//    REQUIRE( grp.agent_right(4).num_prefs() == 6 );
//    REQUIRE( grp.agent_left(5).num_prefs() == 2 );
//    REQUIRE( grp.agent_right(5).num_prefs() == 6 );
//    REQUIRE( grp.agent_left(6).num_prefs() == 4 );
//    REQUIRE( grp.agent_right(6).num_prefs() == 3 );
//    REQUIRE( grp.agent_left(7).num_prefs() == 5 );
//    REQUIRE( grp.agent_right(7).num_prefs() == 4 );
//    REQUIRE( grp.agent_left(8).num_prefs() == 6 );
//    REQUIRE( grp.agent_right(8).num_prefs() == 1 );
//    REQUIRE( grp.agent_left(9).num_prefs() == 1 );
//    REQUIRE( grp.agent_right(9).num_prefs() == 1 );
//  }
//}
