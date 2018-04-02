/*  Skypaper.
 *      Written by AidoP for Trifuse.

 *      See http://trifuse.biz/skypaper for info
*/

// Currently only supports Solus 3.999 and Gnome

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
                                "Skypaper is draws a GLSL shader as the wallpaper\n"
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