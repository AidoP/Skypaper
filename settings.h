#pragma once
/*
    settings - provides the data structure for storing global values and includes common headers

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

#include <stdbool.h>
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "util.h"
#include "build_config.h"

struct {
    char* target_shader;
    char* skypaper_data_directory;
    bool use_vertex_shader;
    bool enable_textures;
    bool transparent_window;
    GLFWwindow* skypaper_window;
} skypaper_settings;