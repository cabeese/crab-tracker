/******************************************************************************
Data collection and initial processing

Polls the SPI slave for new data. The data comes in as raw "blocks" containing
a timestamp and a set of pin values. As these raw blocks are collected, they
are turned into "ping" structs that can be consumed by the higher-level
functions that will determine location and UID.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-10
******************************************************************************/

#include "data_collection.h"
#include "spi.h"

#define NUM_PINS 5 /* TODO: Define this elsewhere, globally */

/* The previous raw data block. We only need the 1 most recent. */
spi_rawblock prev;

/* Store half of a 'ping'. Specifically, store the starting timestamp of
 * a ping. The timestamp at index `i` corresponds to the timestamp of the rising
 * edge of a ping detected from pin/hydrophone `i`. */
unsigned long partials[NUM_PINS];

ping tmp; /* TODO: remove. should be stored in array */

/**
 * Given a fresh, new timestamp, parse out any data we can possibly get. For
 * example, `ping` structs may be calculated based on the new data.
 *
 * This function checks to see if each pin value has changed. If it has changed
 * from LOW to HIGH, this means that a `ping` has started to register on that
 * pin, so the timestamp of this rising edge is stored. If it instead changes
 * from HIGH to LOW, a new `ping` instance is created with the duration measured
 * as the time between the timestamp of the current block of data (`data`) and
 * the timestamp stored in the `partials` array (aka the rising edge).
 *
 * @param data - The latest block of data from SPI
 */
void proc_block(spi_rawblock data){
    int changed = data.pinvals ^ prev.pinvals;
    int i, mask;

    for(i=0; i<5; i++){
        mask = 1 << i;
        if(changed & i){
            /* Pin `i` changed states */
            if(data.pinvals & mask){
                /* Pin `i` changed LOW to HIGH */
                partials[i] = data.timestamp;
            } else {
                /* Pin `i` changed HIGH to LOW */
                /* TODO: Share this new `ping` where `main` can see it */
                tmp.pin = i;
                tmp.start = partials[i];
                tmp.duration = data.timestamp - partials[i];
            }
        }
    }
}
