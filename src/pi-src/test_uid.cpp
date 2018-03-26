/******************************************************************************
Test cases for UID decoding.

Author:  Noah Strong
Created: 2018-02-28
******************************************************************************/

#include "catch.hpp"

#include "uid.h"
#include "data_collection.h"

TEST_CASE("UID decoding", "[uid_decoding]"){

    SECTION("PING decoding"){
        int id = 200;
        ping a;
        long duration = ((id * STEP_SIZE_MS) + MIN_PING_DUR_MS) * 1000;

        a.pin = 0;
        a.start = 0;
        a.duration = duration;

        REQUIRE(id_decode_ping(a) == id);
    }

    SECTION("DELAY decoding"){
        int id = 320;
        int dur = 10;
        ping a, b;
        long delay = ((id * STEP_SIZE_MS) + MIN_DELAY_DUR_MS) * 1000;

        a.pin = 0;
        a.start = 0;
        a.duration = dur;
        b.pin = 0;
        b.start = dur + delay;
        b.duration = dur;

        REQUIRE(id_decode_delay(a, b) == id);
    }
}
