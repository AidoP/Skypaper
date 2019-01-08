/*
    render - Display the wallpaper

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
#include "render.h"
#include "shader.h"
#include "uniforms.h"

// Panel data
static GLuint square_vertex_array;

// This structure allows for many uniforms to be added without excessive repetition
struct skypaper_uniform {
    GLint location;
    // This function should call all OpenGL functions needed to update the uniform
    // The shader program will be "used" before execution
    // Takes the uniform location as a parameter
    void (*update_uniform_function)(GLint);
};

struct skypaper_uniform* skypaper_shader_uniforms;
int skypaper_shader_uniforms_length;

void skypaper_start_rendering() {
    prepare_panel();

    // Load in the shader
    GLuint shader_program_object = load_shaders();
    // Tell the shader program how to interpret the vertex buffer
    // Everything is still bound/linked from prepare_panel(), load_shaders()
    GLint vertex_buffer_attributes_object = glGetAttribLocation(shader_program_object, "vertexPosition");
    glEnableVertexAttribArray(vertex_buffer_attributes_object);
    glVertexAttribPointer(vertex_buffer_attributes_object, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Populate the array of shader uniforms
    initialise_shader_uniforms(shader_program_object);

    // Start render loop
    while (!glfwWindowShouldClose(skypaper_settings.skypaper_window)) {
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Update shader uniforms
        glUseProgram(shader_program_object);
        update_shader_uniforms();

        // Draw the panel
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw the buffer and check for interrupts
        glfwSwapBuffers(skypaper_settings.skypaper_window);
        glfwPollEvents();
    }

    // Free memory
    glDeleteProgram(shader_program_object);
    free(skypaper_shader_uniforms);

    // Close glfw as we are exiting
    glfwTerminate();
}

// Draw the square that covers the screen
void prepare_panel() {
    GLuint square_vertex_buffer;
    const GLfloat square_vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        1.0f,  -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    // Create and bind the Vertex Array
    glGenVertexArrays(1, &square_vertex_array);
    glBindVertexArray(square_vertex_array);

    // Generate the buffer object
    glGenBuffers(1, &square_vertex_buffer);

    // Send the buffer to the graphics device
    glBindBuffer(GL_ARRAY_BUFFER, square_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);
}

typedef void(*update_uniform_function_pointer)(GLint);

void initialise_shader_uniforms(GLint shader_program_object) {
    // Get the objects to bind
    const char* uniform_names_array[]
        = {"iTime", "iTimeDelta", "iResolution", "iMouse"};
    update_uniform_function_pointer uniform_functions_array[]
        = {&update_uniform_iTime, &update_uniform_iTimeDelta, update_uniform_iResolution, &update_uniform_iMouse};

    // Get number of elements. Arrays MUST have an identical number of elements
    int array_size = sizeof(uniform_names_array) / sizeof(uniform_names_array[0]);

    // Define array
    skypaper_shader_uniforms_length = array_size;
    skypaper_shader_uniforms = malloc(sizeof(struct skypaper_uniform) * array_size);
    
    // Populate array of shader uniforms
    for (int iteration = 0; iteration < array_size; iteration++) {
        skypaper_shader_uniforms[iteration].location = glGetUniformLocation(shader_program_object, uniform_names_array[iteration]);
        skypaper_shader_uniforms[iteration].update_uniform_function = uniform_functions_array[iteration];
    }
}

void update_shader_uniforms() {
    // Iterate through skypaper_shader_uniforms and run each function
    for (int iteration = 0; iteration < skypaper_shader_uniforms_length; iteration++) {
        (*skypaper_shader_uniforms[iteration].update_uniform_function)(skypaper_shader_uniforms[iteration].location);
    }
}