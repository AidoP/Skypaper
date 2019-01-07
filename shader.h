#pragma once

/*
    shader - Open and interact with shaders

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

#include "settings.h"
#include <string.h>
#include <dirent.h>

// Defines for errors
#define SKYPAPER_FILE_OPEN_ERROR "__SKYPAPER_FILE_OPEN_ERROR__"
#define SKYPAPER_FILE_READ_ERROR "__SKYPAPER_FILE_READ_ERROR__"

void read_file(const char* path, char** file_data);


void get_default_shader(GLenum shader_type, char** shader_source);

GLuint compile_shader(GLenum shader_type, const char* path);

GLuint load_shaders();


// Default shaders
#define SKYPAPER_DEFAULT_FRAGMENT_SHADER "#version 330 core\n" \
                                         "out vec4 fragColor;"\
                                         "in vec2 fragCoord;"\
                                         "void main() {\n"\
                                         "    fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"\
                                         "}"

#define SKYPAPER_DEFAULT_VERTEX_SHADER "#version 330 core\n"\
                                        "in vec2 vertexPosition;\n"\
                                        "out vec2 fragCoord;"\
                                        "void main() {\n"\
                                        "    fragCoord = vertexPosition;\n"\
                                        "    gl_Position = vec4(vertexPosition, 0.0f, 1.0f);\n"\
                                        "}"
