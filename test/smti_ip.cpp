#include "catch.hpp"
#include "smti.h"
#include <iostream>

TEST_CASE( "Solve instance with ties, no merged constraints", "[IP]") {
  SMTI instance("test-ties.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  Matching matching = instance.solve();
  REQUIRE( matching.size() == 4 );
}

TEST_CASE( "Solve instance with ties, merged constraints", "[IP]") {
  SMTI instance("test-ties.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  Matching matching = instance.solve(true, true);
  REQUIRE( matching.size() == 4 );
}

TEST_CASE( "Solve small GRP instance, no merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-small.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  Matching matching = instance.solve();
  REQUIRE( matching.size() == 3 );
}

TEST_CASE( "Solve GRP instance, merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-small.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  Matching matching = instance.solve(true, true);
  REQUIRE( matching.size() == 3 );
}

TEST_CASE( "Solve unbalanced GRP instance, no merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-small-unbalanced.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  Matching matching = instance.solve();
  REQUIRE( matching.size() == 3 );
}

TEST_CASE( "Solve unbalanced GRP instance, merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-small-unbalanced.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  Matching matching = instance.solve(true, true);
  REQUIRE( matching.size() == 3 );
}

TEST_CASE( "Solve medium GRP instance with ties, no merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-medium.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  Matching matching = instance.solve();
  REQUIRE( matching.size() == 10 );
}

TEST_CASE( "Solve temdium GRP instance with ties, merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-medium.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  Matching matching = instance.solve(true, true);
  REQUIRE( matching.size() == 10 );
}
