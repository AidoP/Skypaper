/*
    Skypaper - Live glsl wallpaper for Linux
    See the readme.

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

// Use GLX backend
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <string.h>

#include "settings.h"
#include "window.h"
#include "render.h"

int main (int argc, char** argv) {
    /* initialise skypaper_settings */
    {
        skypaper_settings.enable_textures = true;
        skypaper_settings.transparent_window = true;
        skypaper_settings.use_vertex_shader = false;
        skypaper_settings.target_shader = "";

        // Set the data directory to $XDG_DATA_HOME/skypaper
        char* xdg_home = getenv("XDG_DATA_HOME");
        char* home_path = getenv("HOME");
        if (!home_path)
            fatal_error("$HOME must be set");
        if (!strcmp(home_path, ""))
            warning("$HOME is empty. Assuming CWD is home.");

        if (!xdg_home) {
            char* subpath = "/.local/share";
            xdg_home = malloc(strlen(home_path) + strlen(subpath) + 1);
            xdg_home[0] = '\0';
            strcat(xdg_home, home_path);
            strcat(xdg_home, subpath);
        } else if (!strcmp(xdg_home, "")) {
            char* subpath = "/.local/share";
            xdg_home = malloc(strlen(home_path) + strlen(subpath) + 1);
            xdg_home[0] = '\0';
            strcat(xdg_home, home_path);
            strcat(xdg_home, subpath);
        }
        char* skypaper_home_directory = "/skypaper";
        skypaper_settings.skypaper_data_directory = malloc(strlen(xdg_home) + strlen(skypaper_home_directory) + 1);
        skypaper_settings.skypaper_data_directory[0] = '\0';
        strcat(skypaper_settings.skypaper_data_directory, xdg_home);
        strcat(skypaper_settings.skypaper_data_directory, skypaper_home_directory);
    }

    /* parse options */
    {
        char opt;

        char* option_layout = "hVvtas:";
        static struct option option_list[] = {
            {"shader",          required_argument,  NULL, 's'},
            {"use-vertex",      no_argument,        NULL, 'v'},
            {"version",         no_argument,        NULL, 'V'},
            {"help",            no_argument,        NULL, 'h'},
            {"no-texture",      no_argument,        NULL, 't'},
            {"no-transparency", no_argument,        NULL, 'a'},
            {0, 0, 0, 0}
        };

        // Get next option until none left
        while ((opt = getopt_long(argc, argv, option_layout, option_list, NULL)) != -1) {
            // Handle each option
            switch (opt) {
                case 's':
                    skypaper_settings.target_shader = optarg;
                    break;
                case 'v':
                    skypaper_settings.use_vertex_shader = true;
                    break;
                case 't':
                    skypaper_settings.enable_textures = false;
                    break;
                case 'a':
                    skypaper_settings.transparent_window = false;
                    break;
                case 'h':
                    {
                        // Print help text
                        static const char* help_string =
                            "Usage: %s -[hV] -s <shader>\n"
                            "Skypaper - Live GLSL wallpaper for Linux\n"
                            "   Arguments:\n"
                            "       -s [name], --shader[=name]      The shader to draw\n"
                            "       -v, --use-vertex                Try using a custom vertex shader\n"
                            "       -t, --no-texture                Disable texture support\n"
                            "       -a, --no-transparency           Don't use a transparent window\n"
                            "       -V, --version                   Prints the verion\n"
                            "       -h, --help                      Prints this help\n";
                        printf(help_string, argv[0]);
                        exit(EXIT_SUCCESS);
                    }
                    break;
                case 'V':
                    // Print version string
                    printf("Skypaper %s\n%s\n", VERSION, LICENCE_SHORT);
                    exit(EXIT_SUCCESS);
                    break;
            }
        }
    }

    // Initialise glfw
    if(!glfwInit())
        fatal_error("Failed to initialise glfw");

    create_skypaper_window();

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fatal_error("Unable to load OpenGL extensions");
    }

    skypaper_start_rendering();
}