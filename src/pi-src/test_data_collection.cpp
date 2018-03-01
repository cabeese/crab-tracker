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
    SECTION("Stress test - lots of in/out."){
        data = {0, 0};
        res = proc_block(data, &storage[0]); /* Reset */
        REQUIRE(res == 0);

        /* Set pins high. Two individually, then last two together */
        data = {100, 0b0001}; /* Pin 0 HIGH */
        res = proc_block(data, &storage[0]);
        REQUIRE(res == 0);
        data = {101, 0b0011}; /* Pin 1 HIGH */
        res = proc_block(data, &storage[0]);
        REQUIRE(res == 0);
        data = {102, 0b1111}; /* Pins 2,3 HIGH */
        res = proc_block(data, &storage[0]);
        REQUIRE(res == 0);

        /* Now make them low in the same pattern */
        data = {200, 0b1110}; /* Pin 0 LOW */
        res = proc_block(data, &storage[0]);
        REQUIRE(res == 1);
        data = {201, 0b0100}; /* Pins 1,3 LOW */
        res = proc_block(data, &storage[1]);
        REQUIRE(res == 2);
        data = {210, 0b0000}; /* Pin 2 LOW */
        res = proc_block(data, &storage[3]);
        REQUIRE(res == 1);

        REQUIRE(storage[0].pin == 0);
        REQUIRE(storage[0].start == 100);
        REQUIRE(storage[0].duration == 100);
        REQUIRE(storage[1].pin == 1);
        REQUIRE(storage[1].start == 101);
        REQUIRE(storage[1].duration == 100);
        REQUIRE(storage[2].pin == 3);
        REQUIRE(storage[2].start == 102);
        REQUIRE(storage[2].duration == 99);
        REQUIRE(storage[3].pin == 2);
        REQUIRE(storage[3].start == 102);
        REQUIRE(storage[3].duration == 108);
    }
}
