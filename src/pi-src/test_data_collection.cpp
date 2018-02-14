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
    spi_rawblock data;
    int res;

    storage[0] = {0,0};

    for(int i=0; i<5; i++){ storage[i] = {0, 0}; }

    SECTION("One pin goes HIGH. No ping created."){
        /* Make first pin HIGH. Shouldn't create a ping */
        data = {100, 0b1};
        res = proc_block(data, &storage[0]);
        REQUIRE(res == 0);
    }
    SECTION("First pin goes LOW. Single ping created."){
        /* Now make ping  1 LOW. Should create a new ping */
        data = {200, 0b0};
        res = proc_block(data, &storage[0]);
        REQUIRE(res == 1);
        REQUIRE(storage[0].pin == 0);
        REQUIRE(storage[0].start == 100);
        REQUIRE(storage[0].duration == 200-100);
    }
    SECTION("Two pins go HIGH then LOW. Two pings created."){
        data = {300, 0b11}; /* Two pins go high */
        res = proc_block(data, &storage[0]);
        REQUIRE(res == 0);

        data = {400, 0b0}; /* Both pins go low */
        res = proc_block(data, &storage[0]);
        REQUIRE(res == 2);
        REQUIRE(storage[0].pin == 0);
        REQUIRE(storage[0].start == 300);
        REQUIRE(storage[0].duration == 400-300);
        REQUIRE(storage[1].pin == 1);
        REQUIRE(storage[1].start == 300);
        REQUIRE(storage[1].duration == 400-300);
    }
}
