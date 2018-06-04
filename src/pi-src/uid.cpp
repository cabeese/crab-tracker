/******************************************************************************
Decodes unique identifiers from transmissions

The details of UID encoding and decoding can be found in the iCRAB Protocol
documentation (TransmissionProtocol.pdf).

Note that we expect some amount of noise or variance in our measured signals,
so we can't rely on the measured durations or delays matching perfectly with
our mathematical expectations. To compensate for this, we simply round each
decoded ID (as a float) to the nearest integer. This way, if a given duration
is 0.074ms longer than it "should be" based on our documentation, the small
error is thrown away.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-28
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "uid.h"

#define period_us 1.0/16.0

/**
 * Determines the ID encoded in a single ping's duration.
 *
 * Will calculate ID based on
 *     p.duration
 * while also accounting for slight discrepancies in timing values.
 *
 * @param p - The ping to decode
 * @returns The ID encoded in 'p'
 */
int counter = 0;
int id_decode_ping(ping p){
    
    //float period_us = 1.0/16.0; 
    float period_ns = period_us * 1000;
    float duration_ns = (float)p.duration * period_ns;
    float duration_us = duration_ns / 1000;
    float duration_ms = duration_us / 1000;
    //printf("p.duration %lu duration_ns %f duration_us %f duration_ms %f counter %d\n", p.duration, duration_ns, duration_us, duration_ms, counter%64);
    float raw_id = (duration_ms - MIN_PING_DUR_MS) / STEP_SIZE_MS;
    counter++;
    return (int)roundf(raw_id);
}

/**
 * Determines the ID encoded in the delay between two consecutive pings.
 *
 * Will calculate ID based on
 *     b.start - (a.start + a.duration)
 * while also accounting for slight discrepancies in timing values.
 *
 * @param a - The first ping (in chronological order)
 * @param b - The second ping (in chronological order)
 * returns The ID encoded by the time between 'a' and 'b'.
 */
int id_decode_delay(ping a, ping b){
    
    //float delta = (float)b.start - (float)a.start;
    //printf("delta %f ", delta);
    //fflush(stdout);
    float delay_ms = ((float)(b.start - (a.start + a.duration))) / 1000; /* us->ms */
    float raw_id = (delay_ms - MIN_DELAY_DUR_MS) / STEP_SIZE_MS;

    return (int)roundf(raw_id);
}
