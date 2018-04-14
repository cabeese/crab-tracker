/**
 * Read in a config file and parse options from it.
 * WIP/testing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "config.h"

int main(){
    FILE* fp;
    char* line = NULL;
    size_t linecap = 0;
    ssize_t len;
    int n, int_param = 0;

    fp = fopen(CONFIG_FILE_PATH, "r");
    if (fp == NULL){
        perror("fopen");
        return 1;
    }

    while ((len = getline(&line, &linecap, fp)) != -1) {
        // printf("Retrieved line of length %zu :\n", len);
        // printf("%s", line);

        for(int i=0; i<len; i++){
            /* Strip out comments (denoted by a hash (#)) */
            if(line[i] == '#'){
                line[i] = '\0';
                printf("Removed comment\n");
            }
        }

        /* Look for the "DISPLAY_PINGS" parameter as an example */
        n = sscanf(line, "DISPLAY_PINGS %d", &int_param);
        printf("scanf() -> %d\n", n);
        if(n > 0) printf("param = %d\n", int_param);
    }

    /* Clean up! */
    fclose(fp);
    if (line) free(line);

    return 0;
}
