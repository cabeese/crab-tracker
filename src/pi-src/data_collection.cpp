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

#include <stdio.h>
#include <string.h>
#include "data_collection.h"
#include "spi.h"
#include "util.h"

#define NUM_PINS 5 /* TODO: Define this elsewhere, globally */

int DISPLAY_PINGS = 0;

/* The previous raw data block. We only need the 1 most recent. */
spi_rawblock prev = {
    0x0, // timestamp
    0x0  // pinvals
};
/* Each time we read from SPI, store the data here */
spi_rawblock raw_data = {0, 0};

/* Store half of a 'ping'. Specifically, store the starting timestamp of
 * a ping. The timestamp at index `i` corresponds to the timestamp of the rising
 * edge of a ping detected from pin/hydrophone `i`. */
unsigned long partials[NUM_PINS];

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
 * @param storage - Where the processed `ping`s should be stored. Ensure that
 *     there is enough space to store up to 5 pings in this array.
 * @returns The number of `ping`s stored after the data is processed. Will be no
 *     more than 5.
 */
int proc_block(spi_rawblock data, ping *storage){
    int i,
        mask,
        changed = data.pinvals ^ prev.pinvals,
        count = 0;
    ping tmp = {0, 0, 0};

    for(i=0; i<5; i++){
        mask = 1 << i;
        if(changed & mask){
            /* Pin `i` changed states */
            if(data.pinvals & mask){
                /* Pin `i` changed LOW to HIGH */
                partials[i] = data.timestamp;
            } else {
                /* Pin `i` changed HIGH to LOW */
                tmp.pin = i;
                tmp.start = partials[i];
                tmp.duration = data.timestamp - partials[i];

                if(DISPLAY_PINGS) disp_ping(tmp);

                // Do we need to do this?
                memcpy(&storage[count], &tmp, sizeof(ping));
                count++;
            }
        }
    }
    /* Is memcpy necessary? Just use assignment? */
    memcpy(&prev, &data, sizeof(spi_rawblock));
    return count;
}

/**
 * Print a ping to stdout.
 * @param p The ping to print
 */
void disp_ping(ping p){
    printf("== PING == pin: %d\t start: %lu (0x%lx)\tduration: %lu (0x%lx)\n",
        p.pin, p.start, p.start, p.duration, p.duration);
}

/**
 * Highest level function for grabbing new data. Checks SPI for new data and
 *     then processes what it gets back.
 *
 * This function is currently unused.
 * @TODO: determine how to correctly pass the 'storage' array to the callee.
 *
 * @param storage - Where the processed `ping`s should be stored. Ensure that
 *     there is enough space to store up to 5 pings in this array.
 * @returns The number of `ping`s stored after the data is processed. Will be no
 *     more than 5.
 */
int poll(ping *storage){
    spi_getblock(&raw_data);
    return proc_block(raw_data, storage);
}

int initialize_dc(){
    return get_param((char*)"DISPLAY_PINGS", &DISPLAY_PINGS);
}
