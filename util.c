/*
    util - Helper functions for a few things
*/

#include "util.h"
#include <string.h>

void fatal_error(char* error_message) {
    fprintf(stderr, "Fatal Error: %s\n", error_message);
    exit(EXIT_FAILURE);
}

void warning(char* warning_message) {
    fprintf(stderr, "Warning: %s\n", warning_message);
}