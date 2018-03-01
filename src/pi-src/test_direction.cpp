/******************************************************************************
Test cases for direction calculation algorithm.

Author:  Lizzy Schoen
Created: 2018-03-1
******************************************************************************/

#include "catch.hpp"

#include "direction.h"

TEST_CASE("direction", "[direction]"){
  SECTION("calcX"){
    double expected = 500001.5;
    double actual = calcX(200, 5);

    REQUIRE(actual > expected-1);
    REQUIRE(actual < expected+1);
  }
}
