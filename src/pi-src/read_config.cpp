/**
 * Read in a config file and parse options from it.
 * WIP/testing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "config.h"

struct config_entry{
    const char* param;
    int value;
    int isset;
};

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

int initialize(){
    FILE* fp;
    char* line = NULL;
    char buf[120];
    size_t linecap = 0;
    ssize_t len;
    int n, m, int_param = 0;

    fp = fopen(CONFIG_FILE_PATH, "r");
    if (fp == NULL){
        perror("fopen");
        return 1;
    }

    while ((len = getline(&line, &linecap, fp)) != -1) {
        for(int i=0; i<len; i++){
            /* Strip out comments (denoted by a hash (#)) */
            if(line[i] == '#') line[i] = '\0';
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
                entries[j].value = int_param;
                entries[j].isset = 1;
                printf("set %s to %d\n", entries[j].param, entries[j].value);
                j = num_entries; /* Break out of loop early */
            }
        }
    }

    /* Clean up! */
    fclose(fp);
    if (line) free(line);

    return 0;
}

int main(){
    int res = 123;
    int n;
    initialize();

    n = get_param((char*)"DISPLAY_PINGS", &res);
    printf("get_param [%d] --> %d\n", n, res);
}
