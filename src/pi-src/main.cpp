/******************************************************************************
Project: Crab Tracker

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

#define STORAGE_SIZE 32

// int DISPLAY_RAW_SPI;
unsigned int result;
full_set active = {0,0,0,0,0,0,0};

/**
 * Initialize all 'modules' in the program.
 * @return (currently unused)
 */
int initialize(){
    initialize_util();
    initialize_spi();
    initialize_dc();
    // get_param((char*)"DISPLAY_RAW_SPI", &DISPLAY_RAW_SPI);

    // for(int i=0; i<STORAGE_SIZE; i++){ storage[i] = {0, 0, 0}; }

    return 1;
}

/**
 * Initialize settings and start listening for and processing data.
 * @return  (unused)
 */
int main (void) {
    int id;
    // int n_unused_pings = 0;
    initialize();

    while (1){
        if(poll()){
            if((id = get_set(&active)) > -1){
                /* We have a set of 8 pings */
                disp_buffers();
                printf("----------------------\n");
                printf("Got a full set. Direction = ...\n");
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
