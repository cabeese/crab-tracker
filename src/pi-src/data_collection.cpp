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
#include "uid.h"
#include "spi.h"
#include "util.h"

#define NUM_PINS 5 /* TODO: Define this elsewhere, globally */
#define PING_BUF_LEN 16 /* TODO: Define as config param */
// extern int DISPLAY_PINGS;

struct ping_collector_t {
    ping pings[PING_BUF_LEN]; /* Recent pings on this hydrophone */
    int index;                /* Insert new pings here */
};

struct ping_collector_t ping_collectors[NUM_PINS];
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
int proc_block(spi_rawblock data){
    int i,
        idx,
        mask,
        count = 0;
    /* Ignore high-order 4 bits. May need to adjust if NUM_PINS increased */
    uint8_t changed = (data.pinvals ^ prev.pinvals) & 0xf;
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
                idx = ping_collectors[i].index;

                // Do we need to do this?
                // memcpy(&storage[count], &tmp, sizeof(ping));
                memcpy(&(ping_collectors[i].pings[idx]), &tmp, sizeof(ping));
                ping_collectors[i].index = (idx + 1) % PING_BUF_LEN;
                count++;

                if(DISPLAY_PINGS){
                    disp_ping(ping_collectors[i].pings[idx]);
                }
            }
        }
    }
    /* data = prev (compare next block with this most recent one) */
    memcpy(&prev, &data, sizeof(spi_rawblock));
    return count;
}

/**
 * Print a ping to stdout.
 * @param p The ping to print
 */
void disp_ping(ping p){
    printf("== PING == pin: %d\tstart: %lu (0x%lx)\tduration: %lu (0x%lx)\n",
        p.pin, p.start, p.start, p.duration, p.duration);
    fflush(stdout);
}

void disp_buffers(){
    for(int p=0; p<NUM_PINS; p++){
        printf("Pings on pin %d:", p);
        for(int i=0; i<PING_BUF_LEN; i++){
            printf(" %d;", ping_collectors[p].pings[i].duration);
        }
        printf("\n");
    }
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
    return proc_block(raw_data);
}

int initialize_dc(){
    for(int i=0; i<NUM_PINS; i++){
        ping_collectors[i] = { {0, 0, 0}, 0};
    }
    return get_param((char*)"DISPLAY_PINGS", &DISPLAY_PINGS);
}

/**
 * Determines if two pings are from the same transmission; that is, checks to
 * see that they both encode the same ID and that the delay between them
 * matches that ID. (Order doesn't matter - checks both ways)
 */
int pingsMatch(ping a, ping b){
    int id = id_decode_ping(a);
    return id == id_decode_ping(b) &&
           (id_decode_delay(a, b) == id || id_decode_delay(b,a) == id);
}

/**
 * Finds two matching pins on a given pin where both pings encode a given ID.
 */
int find_match_on_pin_with_id(int pin, int id, ping **first, ping **second){
    ping *fst;
    ping *snd;
    for(int i=0; i<PING_BUF_LEN; i++){
        fst = &(ping_collectors[pin].pings[i]);
        if(id_decode_ping(*fst) == id){
            /* First ping matches given ID. Find a second */
            for(int j=i+1; j<PING_BUF_LEN; j++){
                snd = &(ping_collectors[pin].pings[j]);
                /* Pings may be in reverse order, so need to check
                   delay with each in front */
                if(pingsMatch(*fst, *snd)){
                    /* Pings found! Return in chronological order */
                    int in_order = fst->start < snd->start;
                    *first  = in_order ? fst : snd;
                    *second = in_order ? snd : fst;
                    return 1;
                }
            }
        }
    }
    return 0; /* failure */
}

int find_match_on_pin(int pin, ping **first, ping **second){
    ping *fst;
    ping *snd;
    int candidate_id = -1;
    for(int i=0; i<PING_BUF_LEN; i++){
        // TODO: skip if id < 0
        /* Check each ping, one at a time */
        fst = &(ping_collectors[pin].pings[i]);
        candidate_id = id_decode_ping(*fst);

        /* Compare this ping with one that comes afterwards */
        for(int j=i+1; j<PING_BUF_LEN; j++){
            snd = &(ping_collectors[pin].pings[j]);
            int t_id = id_decode_ping(*snd);

            /* If pings have same ID and delay is correct, we found a match! */
            if(pingsMatch(*fst, *snd)){
                int in_order = fst->start < snd->start;
                *first  = in_order ? fst : snd;
                *second = in_order ? snd : fst;
                return 1;
            }
        }
    }
    return 0;
    /*
    for each ping on pin 0:
        if duration != 0:
            look for match in remainder of pings
            if no match, return 0;
            else
                add two matches from pin 0 to return
                for each of the other pins 1..NUM_PINS:
                    if duration matches current duration within threshold
                        add it
                        ............ ehh........?
                        */
}

/**
 * When a ping has been used, zero it out so that it is not used twice.
 */
int clear_ping(ping *p){
    int pin = p->pin;
    int i = 0;
    while(i<PING_BUF_LEN && &(ping_collectors[pin].pings[i]) != p){ i++; }
    if(i<PING_BUF_LEN){
        memset(p, 0, sizeof(ping));
        return 1;
    }
    return 0;
}

int main(void){
    spi_rawblock d;
    initialize_dc();

    d = {0, 0x2};
    proc_block(d);
    d = {3200, 0x0};
    proc_block(d);

    d = {15400, 0x2};
    proc_block(d);
    d = {18600, 0x0};
    proc_block(d);

    disp_buffers();

    ping *f;
    ping *s;
    int found;
    found = find_match_on_pin(1, &f, &s);
    printf("(1) Match found? %d\n", found);
    printf("f=0x%x; s=0x%x\n", f, s);
    disp_ping(*f);
    disp_ping(*s);

    found = find_match_on_pin_with_id(1, 22, &f, &s);
    printf("(2) Match found? %d\n", found);
    printf("f=0x%x; s=0x%x\n", f, s);
    disp_ping(*f);
    disp_ping(*s);

    printf("clearing...");
    printf(" %s\n", clear_ping(f) ? "success" : "failed");
    disp_ping(*f);

    found = find_match_on_pin(1, &f, &s);
    printf("(3) Match found? %d\n", found);
}
