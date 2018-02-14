/******************************************************************************
Test cases for data collection and formatting (not SPI itself).

Author:  Noah Strong
Created: 2018-02-12
******************************************************************************/

#include "catch.hpp"

#include "data_collection.h"
#include "spi.h"

TEST_CASE("proc_block", "[proc_block]"){
    ping storage[5];
    int res;

    storage[0] = {0,0};

    for(int i=0; i<5; i++){ storage[i] = {0, 0}; }

    SECTION("No change detected if only HIGH->LOW edges"){
        spi_rawblock data = {42, 0b1};
        res = proc_block(data, &storage[0]);

        REQUIRE(res == 0);
    }
}
