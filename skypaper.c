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
#include <string.h>

#include "settings.h"
#include "shader_read.h"
#include "window.h"
#include "render.h"

int main (int argc, char** argv) {
    /* initialise skypaper_settings */
    {
        skypaper_settings.is_windowed   = false;

        skypaper_settings.fragment_shader_path = NULL;
        skypaper_settings.vertex_shader_path = NULL;
    }

    /* parse options */
    {
        char* option_layout = "hVf:v:s:w";
        static struct option option_list[] = {
            {"fragment",    required_argument,  NULL,                                   'f'},
            {"vertex",      required_argument,  NULL,                                   'v'},
            {"shader",      required_argument,  NULL,                                   's'},
            {"windowed",    no_argument,        (int*) &skypaper_settings.is_windowed,  true},
            {"version",     no_argument,        NULL,                                   'V'},
            {"help",        no_argument,        NULL,                                   'h'},
            {0, 0, 0, 0}
        };

        // Get next option until none left
        char opt;
        while ((opt = getopt_long(argc, argv, option_layout, option_list, NULL)) != -1) {
            // Handle each option
            switch (opt) {
                case 'f':
                    skypaper_settings.fragment_shader_path = optarg;
                    break;
                case 'v':
                    skypaper_settings.vertex_shader_path = optarg;
                    break;
                case 's':
                    if (!check_for_shader_folder())
                        continue;

                    char* data_directory_path;
                    get_data_directory(&data_directory_path);

                    char* frag_name = "frag.glsl";
                    char* vertex_name = "vertex.glsl";

                    // Set the paths
                    get_from_shader_data_path(&data_directory_path, &optarg, &frag_name, &skypaper_settings.fragment_shader_path);
                    get_from_shader_data_path(&data_directory_path, &optarg, &vertex_name, &skypaper_settings.vertex_shader_path);
                    
                    break;
                case 'V':
                    // Print version string
                    printf("Skypaper %s\n%s\n", VERSION, LICENCE_SHORT);
                    exit(EXIT_SUCCESS);
                    break;
                case 'h':
                    // Print help text
                    printf(HELP_STRING, argv[0]);
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