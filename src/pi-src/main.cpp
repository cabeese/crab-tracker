/******************************************************************************
Main Crab Tracker loop.
This file continuously checks for new data over SPI, and then processes batches
of data when they become available. The `full_set` object, `active`, will
contain 8 related pings when a single transmission is received. These pings can
be used to calculate the direction of the given crab, which will then be
displayed to the user.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-03
******************************************************************************/

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "util.h"
#include "config.h"
#include "spi.h"
#include "data_collection.h"
#include "direction.h"

full_set active = {0,0,0,0,0,0,0};

/**
 * Initialize all 'modules' in the program.
 * @return (currently unused)
 */
int initialize(){
    initialize_util();
    initialize_spi();
    initialize_dc();
    initialize_dir();
    return 1;
}

/**
 * Initialize settings and start listening for and processing data.
 * @return  (unused)
 */
int main (void) {
    int id, n;
    crab_event result;
    initialize();

    while (1){
        if(poll()){
            if((id = get_set(&active)) > -1){
                /* We have a set of 8 pings */
                printf("----------------------\n");
                printf("Crab ID: %d\n", id);
                // disp_buffers();
                // printf("---------------------- %d\n", id);
                n = triangulation(
                    *active.pings_a[0],
                    *active.pings_b[0],
                    *active.pings_c[0],
                    *active.pings_d[0],
                    &result);
                if(n > 0){
                    disp_direction(result);
                } else {
                    printf("Unable to determine direction\n");
                }
                printf("\n");

                // TODO: update GUI
                clear_set(&active);
            } else {
                /* nop */
            }
        } else {
            sleep(0.5);
        }
        usleep(100);
    }
}
