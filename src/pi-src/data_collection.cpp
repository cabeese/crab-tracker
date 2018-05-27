/******************************************************************************
Data collection and initial processing

Polls the SPI slave for new data. The data comes in as raw "blocks" containing
a timestamp and a set of pin values. As these raw blocks are collected, they
are turned into "ping" structs and stored for future use. Once there are enough
pings (8, specifically) to run a direction calculation, a set of all 8 can be
returned and then later cleared.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-10
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "common.h"
#include "data_collection.h"
#include "uid.h"
#include "spi.h"
#include "util.h"

#define NUM_PINS 4 /* TODO: Define this elsewhere, globally */
#define PING_BUF_LEN 16 /* TODO: Define as config param */
int DISPLAY_PINGS = 0;

/**
 * As pings come in, we need to store them. We need at least 2 pings per pin
 * to run the direction algorithm, but we also need to account for the fact
 * that we may get some noise between two pings. Thus we have a small buffer
 * of size PING_BUF_LEN for each pin. As pings come in, they are stored in the
 * appropriate buffer. Later, we search through all these buffers to find
 * matching pings that we can run triangulation on.
 * We insert at the "end" of this circular buffer every time and erase pings
 * once they have been used. If a ping becomes "stale," meaning that it has
 * been in the buffer for a while without being used in a calculation, it will
 * eventually be overwritten with fresh data as the insert head of the buffer
 * wraps back around to it.
 */
struct ping_collector_t {
    ping pings[PING_BUF_LEN]; /* Recent pings on this hydrophone */
    int index;                /* Insert new pings here */
};
struct ping_collector_t ping_collectors[NUM_PINS];

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
    uint8_t changed = data.pinvals ^ prev.pinvals;
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
    int id = id_decode_ping(p);
    printf("== PING == pin: %d\tduration: %lu\tID: %d\n",
           p.pin, p.duration, id);
    fflush(stdout);
}

/**
 * Print the duration of every pin in all buffers
 */
void disp_buffers(){
    for(int p=0; p<NUM_PINS; p++){
        printf("Pings on pin %d:", p);
        for(int i=0; i<PING_BUF_LEN; i++){
            printf(" %lu;", ping_collectors[p].pings[i].duration);
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
 * @returns 1 if new pings were stored, else 0
 */
int poll(){
    spi_rawblock raw_data;
    if(spi_getblock(&raw_data)){
        return proc_block(raw_data) > 0;
    }
    return 0;
}

/**
 * Determines if two pings are from the same transmission; that is, checks to
 * see that they both encode the same ID and that the delay between them
 * matches that ID. (Order doesn't matter - checks both ways)
 * @param a - One ping
 * @param b - The other ping
 * @returns - 1 if 'a' and 'b' are from the same transmission; 0 otherwise
 */
int pings_match(ping a, ping b){
    int id = id_decode_ping(a);
    return id == id_decode_ping(b) &&
           (id_decode_delay(a, b) == id || id_decode_delay(b,a) == id);
}

/**
 * Finds two matching pins on a given pin where both pings encode a given ID.
 * @param pin    - The pin [0..3] to read from
 * @param id     - The ID to search for
 * @param first  - The chronologically-first ping will be pointed at by first
 * @param second - The chronologically-second ping will be pointed at by second
 * @returns      - The ID of the pings, if found, or -1 if no match found
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

                if(pings_match(*fst, *snd)){
                    /* Pings found! Return in chronological order */
                    int in_order = fst->start < snd->start;
                    *first  = in_order ? fst : snd;
                    *second = in_order ? snd : fst;
                    return id;
                }
            }
        }
    }
    return -1; /* failure */
}

/**
 * Finds two matching pins on a given pin where both pings encode some ID.
 * @param pin    - The pin [0..3] to read from
 * @param first  - The chronologically-first ping will be pointed at by first
 * @param second - The chronologically-second ping will be pointed at by second
 * @returns      - The ID of the pings, if found, or -1 if no match found
 */
int find_match_on_pin(int pin, ping **first, ping **second){
    ping *fst;
    ping *snd;
    int candidate_id = -1;
    for(int i=0; i<PING_BUF_LEN; i++){
        /* Check each ping, one at a time */
        fst = &(ping_collectors[pin].pings[i]);
        candidate_id = id_decode_ping(*fst);

        /* Find a matching ping in the remainder of the buffer */
        if(candidate_id >= 0){
            for(int j=i+1; j<PING_BUF_LEN; j++){
                snd = &(ping_collectors[pin].pings[j]);
                if(pings_match(*fst, *snd)){
                    int in_order = fst->start < snd->start;
                    *first  = in_order ? fst : snd;
                    *second = in_order ? snd : fst;
                    return candidate_id;
                }
            }
        }
    }
    return -1; /* No match found */
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

/**
 * Look for a total of 8 pings that can be matched to a single iCRAB broadcast
 * @param set - Pointers to the appropriate pings will be stored here
 * @returns   - The ID of the crab, if enough pings found, else -1
 */
int get_set(full_set *set){
    ping *f, *s;
    int id;
    /* Find pings on pin A */
    if((id = find_match_on_pin(0, &f, &s)) < 0) return -1;
    set->pings_a[0] = f;
    set->pings_a[1] = s;
    /* Find pings on pin B */
    if(find_match_on_pin_with_id(1, id, &f, &s) != id) return -1;
    set->pings_b[0] = f;
    set->pings_b[1] = s;
    /* Find pings on pin C */
    if(find_match_on_pin_with_id(2, id, &f, &s) != id) return -1;
    set->pings_c[0] = f;
    set->pings_c[1] = s;
    /* Find pings on pin D */
    if(find_match_on_pin_with_id(3, id, &f, &s) != id) return -1;
    set->pings_d[0] = f;
    set->pings_d[1] = s;
    return id;
}

/**
 * Deletes ping entries from their buffers.
 * Call once direction algorithm completes.
 * @param set - The set of pings to delete
 */
void clear_set(full_set *set){
    for(int i=0; i<2; i++){
        clear_ping(set->pings_a[i]);
        clear_ping(set->pings_b[i]);
        clear_ping(set->pings_c[i]);
        clear_ping(set->pings_d[i]);
    }
}

int initialize_dc(){
    for(int i=0; i<NUM_PINS; i++){
        ping_collectors[i] = { {0, 0, 0}, 0};
    }
    return get_param((char*)"DISPLAY_PINGS", &DISPLAY_PINGS);
}
