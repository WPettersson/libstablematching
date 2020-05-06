#include "catch.hpp"
#include "matching.h"
#include <iostream>

TEST_CASE( "Test equality", "[Matching]") {
  Matching one{ {1, 2}, {3, 4} };
  Matching two{ {1, 2}, {3, 4} };
  Matching three{ {1, 2}, {3, 5} };
  Matching four{ {1, 2}, {3, 5} };
  REQUIRE( one == two );
  REQUIRE( two == one );
  REQUIRE( one != three );
  REQUIRE( two != four );
  REQUIRE( three == four );
}

