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
#include "shader.h"
#include "shader_read.h"

char* get_default_shader(GLenum shader_type) {
    if (shader_type == GL_FRAGMENT_SHADER)
        return SKYPAPER_DEFAULT_FRAGMENT_SHADER;
    else
        return SKYPAPER_DEFAULT_VERTEX_SHADER;
}

GLuint compile_shader(GLenum shader_type, char** shader_source) {
    GLuint shader_object = glCreateShader(shader_type);

    // Compile the shader
    glShaderSource(shader_object, 1, (const char* const*)shader_source, NULL);
    glCompileShader(shader_object);

    // Get log output
    GLint compile_status, error_message_length;
    glGetShaderiv(shader_object, GL_COMPILE_STATUS, &compile_status);
	glGetShaderiv(shader_object, GL_INFO_LOG_LENGTH, &error_message_length);
    if (compile_status != GL_TRUE) {
        char error_message[error_message_length + 1];
        error_message[0] = '\0';
        glGetShaderInfoLog(shader_object, error_message_length, NULL, error_message);
        warning("Unable to compile shader: ");
        printf("------ %s Shader ------\n%s%s------------\n\n", shader_type == GL_FRAGMENT_SHADER ? "Fragment" : "Vertex", *shader_source == NULL ? "No shader provided\n" : "", error_message);
    }

    return shader_object;
}

GLuint load_shaders(char** fragment_shader_path, char** vertex_shader_path) {
    char* fragment_shader_source = NULL;
    char* vertex_shader_source = NULL;

    if (*fragment_shader_path)
        read_file(fragment_shader_path, &fragment_shader_source);
    else {
        fragment_shader_source = get_default_shader(GL_FRAGMENT_SHADER);
        fprintf(stderr, "Path = %s", skypaper_settings.fragment_shader_path);
    }

    if (*vertex_shader_path)
        read_file(vertex_shader_path, &vertex_shader_source);
    else
        vertex_shader_source = get_default_shader(GL_VERTEX_SHADER);

    // Compile the shaders into a shader program
    GLuint shader_program_object = glCreateProgram();
    glAttachShader(shader_program_object, compile_shader(GL_FRAGMENT_SHADER, &fragment_shader_source));
    glAttachShader(shader_program_object, compile_shader(GL_VERTEX_SHADER, &vertex_shader_source));

    // Bind output and link
    glBindFragDataLocation(shader_program_object, 0, "fragColor");
    glLinkProgram(shader_program_object);

    if (*fragment_shader_path)
        free(fragment_shader_source);
    if (*vertex_shader_path)
        free(vertex_shader_source);

    // Get log output
    GLint program_link_status, error_message_length;
    glGetProgramiv(shader_program_object, GL_LINK_STATUS, &program_link_status);
	glGetProgramiv(shader_program_object, GL_INFO_LOG_LENGTH, &error_message_length);
    if (program_link_status != GL_TRUE) {
        char error_message[error_message_length + 1];
        error_message[0] = '\0';
        glGetProgramInfoLog(shader_program_object, error_message_length, NULL, error_message);
        warning("Shader program linking failed:");
        printf("------------\n%s\n------------\n\n", error_message);
    }

    return shader_program_object;
}
