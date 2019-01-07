#pragma once
/*
    settings - provides the data structure for storing global values and includes common headers
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