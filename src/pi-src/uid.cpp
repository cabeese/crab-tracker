/******************************************************************************
Decodes unique identifiers from transmissions

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-28
******************************************************************************/

#include <math.h>
#include "uid.h"

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
int id_decode_ping(ping p){
    float duration_ms = (float)p.duration / 1000; /* convert us->ms */
    float raw_id = (duration_ms - MIN_PING_DUR_MS) / STEP_SIZE_MS;

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
    float delay_ms = ((float)(b.start - (a.start + a.duration))) / 1000; /* us->ms */
    float raw_id = (delay_ms - MIN_DELAY_DUR_MS) / STEP_SIZE_MS;

    return (int)roundf(raw_id);
}
