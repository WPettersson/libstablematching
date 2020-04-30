#include "catch.hpp"
#include "smti.h"
#include <iostream>

TEST_CASE( "Solve instance with ties, no merged constraints", "[IP]") {
  SMTI instance("test-ties.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  SMTI::IP_Model model = SMTI::IP_Model(&instance);
  model.merge(false);
  Matching matching = model.solve();
  REQUIRE( matching.size() == 4 );
}

TEST_CASE( "Solve instance with ties, merged constraints", "[IP]") {
  SMTI instance("test-ties.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  SMTI::IP_Model model = SMTI::IP_Model(&instance);
  Matching matching = model.solve();
  REQUIRE( matching.size() == 4 );
}

TEST_CASE( "Solve small GRP instance, no merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-small.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  SMTI::IP_Model model = SMTI::IP_Model(&instance);
  model.merge(false);
  Matching matching = model.solve();
  REQUIRE( matching.size() == 3 );
}

TEST_CASE( "Solve GRP instance, merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-small.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  SMTI::IP_Model model = SMTI::IP_Model(&instance);
  Matching matching = model.solve();
  REQUIRE( matching.size() == 3 );
}

TEST_CASE( "Solve unbalanced GRP instance, no merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-small-unbalanced.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  SMTI::IP_Model model = SMTI::IP_Model(&instance);
  model.merge(false);
  Matching matching = model.solve();
  REQUIRE( matching.size() == 3 );
}

TEST_CASE( "Solve unbalanced GRP instance, merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-small-unbalanced.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  SMTI::IP_Model model = SMTI::IP_Model(&instance);
  Matching matching = model.solve();
  REQUIRE( matching.size() == 3 );
}

TEST_CASE( "Solve medium GRP instance with ties, no merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-medium.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  SMTI::IP_Model model = SMTI::IP_Model(&instance);
  model.merge(false);
  Matching matching = model.solve();
  REQUIRE( matching.size() == 10 );
}

TEST_CASE( "Solve medium GRP instance with ties, merged constraints", "[IP]") {
  SMTI instance =  SMTI::create_from_GRP("grp-test-medium.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  SMTI::IP_Model model = SMTI::IP_Model(&instance);
  Matching matching = model.solve();
  REQUIRE( matching.size() == 10 );
}

TEST_CASE( "Solve instance with ties, forced pairings", "[IP]") {
  SMTI instance("test-ties.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  SMTI::IP_Model model = SMTI::IP_Model(&instance);
  Matching forced;
  forced.emplace_back(1, 2);
  forced.emplace_back(3, 4);
  model.force(forced);
  Matching matching = model.solve();
  REQUIRE( matching.size() == 4 );
  // find 2,1 and 4, 3 in matching
  bool find21 = false;
  bool find43 = false;
  for(auto [left, right]: matching) {
    if ((left == 2) && (right == 1)) {
      find21 = true;
    }
    if ((left == 4) && (right == 3)) {
      find43 = true;
    }
  }
  REQUIRE( find21 );
  REQUIRE( find43 );
}

TEST_CASE( "Solve instance with ties, avoided pairings", "[IP]") {
  SMTI instance("test-ties.instance");
  // Recall, parameters to solve() are optimise, merged
  // if optimise is true, optimise size of matching, else assume every agent
  // must be matched (COM-SMTI)
  // merged is for merging constraints
  SMTI::IP_Model model = SMTI::IP_Model(&instance);
  Matching avoid;
  avoid.emplace_back(1, 2);
  avoid.emplace_back(3, 4);
  model.avoid(avoid);
  Matching matching = model.solve();
  REQUIRE( matching.size() == 4 );
  // find 2,2 and 3, 3 in matching
  bool find22 = false;
  bool find33 = false;
  for(auto [left, right]: matching) {
    if ((left == 2) && (right == 2)) {
      find22 = true;
    }
    if ((left == 3) && (right == 3)) {
      find33 = true;
    }
  }
  REQUIRE( find22 );
  REQUIRE( find33 );
}
