/******************************************************************************
Helper methods, including configuration management.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-19
******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "config.h"


/******************************************************************************
 CONFIGURATION FUNCTIONS AND VARIABLES

 Configuration works as follows: on program startup, the initalize() function
 should be called. This will look for and load the `.conf` file whose location
 is specificed in CONFIG.H (generally `/etc/crab-tracker.conf`). The .conf file
 will be parsed and any key/value pairs whose keys we expect to see will be
 read in and saved. The keys we "expect to see" are those that are listed in the
 entries[] array below. Initially, entries[] contains the default values for all
 parameters we expect to come across; unless new values for those parameters are
 found in the .conf file, those values will be used.
 ******************************************************************************/

/* Basically a key/value store for configuration options */
struct config_entry{
    const char* param;
    int value;
    int isset;
};

/* An exhaustive set of all configuration options. Default values given here */
config_entry entries[] = { {"DISPLAY_PINGS", 1, 0},
                           {"DISPLAY_RAW_SPI", 0, 0},
                           {"HPHONE_ADJ_DIST_CM", 300, 0}};

int num_entries = sizeof(entries) / sizeof(config_entry);

/**
 * "Public" way for getting a configuration. Looks through entries and returns
 *     (via an out param) the value of that config option.
 * @param  param  The name of the parameter to look for
 * @param  result The value of the parameter, if found [OUT PARAM]
 * @return        1 if param was found, else 0
 */
int get_param(char* param, int* result){
    for(int i=0; i<num_entries; i++){
        if(!strcmp(param, entries[i].param)){
            *result = entries[i].value;
            return 1;
        }
    }
    return 0;
}

/* Rather than include math.h, here's a quick integer min() */
int min(int a, int b){ return a > b ? b : a; }

/**
 * Loads the .conf file on the machine and then stores the values of any known
 *     configuration parameters.
 * @return 0 if config loaded successfully, else 1.
 */
int initialize_util(){
    printf("Loading configuration... ");
    FILE* fp;
    char* line = NULL;
    char buf[120];
    size_t linecap = 0;
    ssize_t len;
    int n, m, n_found = 0, int_param = 0;

    /* Load the `.conf` file, if it exists */
    fp = fopen(CONFIG_FILE_PATH, "r");
    if (fp == NULL){
        perror("fopen");
        fprintf(stderr, "Unable to load configuration file\n");
        return 1;
    }

    /* Now we go through every line in the config file */
    while ((len = getline(&line, &linecap, fp)) != -1) {
        for(int i=0; i<len; i++){
            /* Strip out comments (denoted by a hash (#)) */
            if(line[i] == '#'){ line[i] = '\0'; i=len; };
        }

        /* See if any of our parameters are found in this line */
        for(int j=0; j<num_entries; j++){
            /* Set `buf = entries[j].param + " %d";` */
            m = min(strlen(entries[j].param), 115);
            strncpy(buf, entries[j].param, m);
            strcpy(&buf[m], " %d\0");

            n = sscanf(line, buf, &int_param);
            if(n > 0){
                /* Found the parameter! Store it */
                n_found++;
                entries[j].value = int_param;
                entries[j].isset = 1;
                j = num_entries; /* Break out of loop early */
            }
        }
    }

    /* Clean up! */
    fclose(fp);
    if (line) free(line);
    printf("successfully read %d configuration parameters\n", n_found);
    return 0;
}

/******************************************************************************
 DISPLAY AND OTHER HELPER FUNCTIONS

 These are mostly simple helper functions, such as binary print functions.
 ******************************************************************************/

/**
 * Print a 32-bit number as binary.
 * Written by Zach McGrew (WWU Comptuer Science Graduate Student)
 * @param number The number to print
 */
void print_bin(int32_t number) {
  uint32_t bit_set = (uint32_t) 1 << 31;
  int32_t bit_pos;

  for (bit_pos = 31; bit_pos >= 0; --bit_pos, bit_set >>= 1) {
    printf("%c", ((uint32_t)number & bit_set ? '1' : '0'));
  }
}

/**
 * Print an 8-bit unsigned integer in binary to stdout.
 * @param number The number (UNSIGNED) to print
 */
void print_bin_8(uint8_t number){
    uint8_t bit_set = (uint8_t) 1 << 7;
    int bit_pos;

    for(bit_pos = 7; bit_pos >= 0; --bit_pos){
        printf("%c", ((uint8_t)number & bit_set ? '1' : '0'));
        bit_set >>= 1;
    }
}

/**
 * Print a long unsigned integer in binary to stdout.
 * @param number The number (UNSIGNED) to print
 */
void print_bin_ulong(unsigned long number){
    unsigned long bit_set = (unsigned long) 1 << 31;
    int bit_pos;

    for(bit_pos = 31; bit_pos >= 0; --bit_pos){
        printf("%c", (number & bit_set ? '1' : '0'));
        bit_set >>= 1;
    }
}
