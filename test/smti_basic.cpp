#include "catch.hpp"
#include "smti.h"

TEST_CASE( "Test removing a pair.", "[basic]" ) {
  SMTI instance("test-tiny.instance");
  std::cout << instance.to_string() << std::endl;
  REQUIRE( instance.num_agents_left() == 2 );
  REQUIRE( instance.num_agents_right() == 2 );
  for(int i = 1; i <= 2; ++i) {
    REQUIRE( instance.agent_left(i).num_prefs() == 2 );
    REQUIRE( instance.agent_right(i).num_prefs() == 2 );
  }
  instance.remove_pair(1, 2);
  REQUIRE( instance.num_agents_left() == 2 );
  REQUIRE( instance.num_agents_right() == 2 );
  REQUIRE( instance.agent_left(1).num_prefs() == 1 );
  REQUIRE( instance.agent_right(1).num_prefs() == 2 );
  REQUIRE( instance.agent_left(2).num_prefs() == 2 );
  REQUIRE( instance.agent_right(2).num_prefs() == 1 );
  instance.remove_pair(2, 1);
  REQUIRE( instance.num_agents_left() == 2 );
  REQUIRE( instance.num_agents_right() == 2 );
  REQUIRE( instance.agent_left(1).num_prefs() == 1 );
  REQUIRE( instance.agent_right(1).num_prefs() == 1 );
  REQUIRE( instance.agent_left(2).num_prefs() == 1 );
  REQUIRE( instance.agent_right(2).num_prefs() == 1 );
}

TEST_CASE( "Test removing multiple preferences", "[basic]" ) {
  std::vector<int> group_1 = {1,2};
  std::vector<int> group_2 = {3};
  std::vector<int> group_3 = {4,5};
  std::vector<int> group_4 = {6};
  std::vector<std::vector<int>> preferences { group_1, group_2, group_3, group_4};
  Agent a(1, preferences);
  REQUIRE( a.num_prefs() == 6 );
  std::cout << a.pref_list_string() << std::endl;
  a.remove_after(2);
  std::cout << a.pref_list_string() << std::endl;
  REQUIRE( a.num_prefs() == 5 );
  a.remove_after(0);
  std::cout << a.pref_list_string() << std::endl;
  REQUIRE( a.num_prefs() == 2 );
}
