/*
    util - Helper functions for a few things

    Copyright (C) 2019  Aidan Prangnell (AidoP)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or 
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    A full copy of the license should be included with the source. If not see https://www.gnu.org/licenses/.
*/

#include "util.h"

void fatal_error(char* error_message) {
    fprintf(stderr, "Fatal Error: %s\n", error_message);
    exit(EXIT_FAILURE);
}

void warning(char* warning_message) {
    fprintf(stderr, "Warning: %s\n", warning_message);
}