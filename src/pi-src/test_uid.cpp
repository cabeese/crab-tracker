/******************************************************************************
Test cases for UID decoding.

Author:  Noah Strong
Created: 2018-02-28
******************************************************************************/

#include "catch.hpp"

#include "uid.h"
#include "data_collection.h"

float calc_ping_dur_us(int id){
    return ((id * STEP_SIZE_MS) + MIN_PING_DUR_MS) * 1000;
}

float calc_delay_dur_us(int id){
    return ((id * STEP_SIZE_MS) + MIN_DELAY_DUR_MS) * 1000;
}

TEST_CASE("UID decoding", "[uid_decoding]"){

    SECTION("Simple PING decoding"){
        int id = 200;
        ping a;
        float duration_us = calc_ping_dur_us(id);

        a.pin = 0;
        a.start = 0;
        a.duration = duration_us;

        REQUIRE(id_decode_ping(a) == id);
    }

    SECTION("Simple DELAY decoding"){
        int id = 320;
        int dur = 10;
        ping a, b;
        float delay = calc_delay_dur_us(id);

        a.pin = 0;
        a.start = 0;
        a.duration = dur;
        b.pin = 0;
        b.start = dur + delay;

        b.duration = dur;

        REQUIRE(id_decode_delay(a, b) == id);
    }

    /*
     * Test how the algorithm handles data points that are not perfectly
     * (mathematically) matched with our expected formula. We expect some
     * noise or inconsistency in our measurements, so we must be able to
     * handle this.
     *
     * For example, the ping for crab 22 is supposed to be 3.2ms long (by v1
     * of the iCRAB protocol). However, it's quite likely that it could be
     * measured as being 3.287ms long.
     *
     * We will test a range of values between two IDs to ensure that all
     * rounding is done as expected.
     */
    SECTION("Variance testing - PING"){
        ping p;
        int id = 22;
        float dur;
        float dur_0 = calc_ping_dur_us(id);
        float dur_1 = calc_ping_dur_us(id+1);
        float diff = dur_1-dur_0;
        p.pin = 0; p.start = 0;

        /* Test up to the midpoint between two durations */
        for(dur = dur_0; dur < dur_0 + (diff/2); dur++){
            p.duration = dur;
            REQUIRE(id_decode_ping(p) == id);
        }
        /* Test from the midpoint to the next expected duration */
        for(; dur <= dur_1; dur++){
            p.duration = dur;
            REQUIRE(id_decode_ping(p) == id+1);
        }
    }
    SECTION("Variance testing - DELAY"){
        ping a, b;
        int id = 22;
        float dur;
        float dur_0 = calc_delay_dur_us(id);
        float dur_1 = calc_delay_dur_us(id+1);
        float diff = dur_1-dur_0;
        a.pin = 0; a.duration = calc_ping_dur_us(id); a.start = 0;
        b.pin = 0; b.duration = calc_ping_dur_us(id);

        /* Test up to the midpoint between two durations */
        for(dur = dur_0; dur < dur_0 + (diff/2); dur++){
            b.start = (a.duration) + dur;
            REQUIRE(id_decode_delay(a,b) == id);
        }
        /* Test from the midpoint to the next expected duration */
        for(; dur <= dur_1; dur++){
            b.start = (a.duration) + dur;
            REQUIRE(id_decode_delay(a,b) == id+1);
        }
    }
}
