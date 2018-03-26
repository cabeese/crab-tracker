/**
 * Read in a config file and parse options from it.
 * WIP/testing.
 */

#include <stdio.h>
#include <sys/stat.h>
#include <fnctl.h>
#include "CONFIG.h"

int fd;

fd = open(CONFIG_FILE, O_RDONLY);
