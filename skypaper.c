/*  Skypaper.
     An OpenGL based live wallpaper for Linux. 

  Copyright (C) 2018  Aidan Prangnell

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>

#include "renderer.c"

#define SKYPAPER_VERSION "Developer 0\n"


static struct option options[] = {
    {"shader", required_argument, NULL, 's'},
    {"windowed", no_argument, NULL, 'w'},
    {"version", no_argument, NULL, 'V'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0}
};
char* optionsString = "hVws:";

static const char* helpString = "Usage: skypaper -[hVw] -s <shader>\n"
                                "Skypaper is an OpenGL based live wallpaper\n"
                                "   Arguments:\n"
                                "       -s, --shader        The shader to draw\n"
                                "       -w, --windowed      If the shader should be drawn\n"
                                "                            in a window instead\n"
                                "       -V, --version       Prints the verion\n"
                                "       -h, --help          Prints this help\n";

int main(int argc, char** argv) {
    char* shader = "rainbow";
    bool windowMode = false;

    int c;
    // Get all set options
    while ((c = getopt_long(argc, argv, optionsString, options, NULL)) != -1) {
        switch (c) {
            case 's':
                shader = optarg;
                break;
            case 'w':
                windowMode = true;
                break;
            case 'h':
                printf(helpString);
                exit(EXIT_SUCCESS);
                break;
            case 'V':
                printf("Skypaper %s", SKYPAPER_VERSION);
                exit(EXIT_SUCCESS);
                break;
        }
    }

    renderer* rend = init_renderer();
    render(rend);
}