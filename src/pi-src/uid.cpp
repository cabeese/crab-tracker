/******************************************************************************
Decodes unique identifiers from transmissions

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-28
******************************************************************************/

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
    long duration_ms = p.duration / 1000; /* convert us->ms */
    float raw_id = (duration - MIN_PING_DUR_MS) / STEP_SIZE_MS;

    /* TODO: correctly handle variability.
     * for example, do
     *   return Math.round(raw_id)
     * to round up or down appropriately.
     */
    return (int)raw_id;
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
    long delay_ms = (b.start - (a.start + a.duration)) / 1000; /* us->ms */
    float raw_id = (delay - MIN_DELAY_DUR_MS) / STEP_SIZE_MS;

    /* TODO: correctly handle variability.
     * for example, do
     *   return Math.round(raw_id)
     * to round up or down appropriately.
     */
    return (int)raw_id;
}
