/******************************************************************************
Test cases for data collection and formatting (not SPI itself).

Tests built for the Catch2 framework: https://github.com/catchorg/Catch2

Author:  Noah Strong
Created: 2018-02-12
******************************************************************************/


/* This tells Catch to provide a main() - only do this in one cpp file */
#define CATCH_CONFIG_MAIN

#include "catch.hpp"

TEST_CASE("1: All tests in other .cpp files (empty)", "[multi-file:1]"){}
