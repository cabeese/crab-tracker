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

full_set active = {0,0,0,0,0,0,0};

/**
 * Initialize all 'modules' in the program.
 * @return (currently unused)
 */
int initialize(){
    initialize_util();
    initialize_spi();
    initialize_dc();
    return 1;
}

/**
 * Initialize settings and start listening for and processing data.
 * @return  (unused)
 */
int main (void) {
    int id;
    initialize();

    while (1){
        if(poll()){
            if((id = get_set(&active)) > -1){
                /* We have a set of 8 pings */
                printf("---------------------- ");
                printf("Got a full set for crab %d\n", id);
                disp_buffers();
                printf("---------------------- %d\n", id);
                // TODO: call direction algorithm
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
