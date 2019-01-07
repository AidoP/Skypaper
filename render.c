/*
    render - Display the wallpaper
*/
#include "render.h"
#include "shader.h"

// Panel data
static GLuint square_vertex_array;

void skypaper_start_rendering() {
    prepare_panel();

    // Load in the shader
    GLuint shader_program_object = load_shaders();
    // Tell the shader program how to interpret the vertex buffer
    // Everything is still bound/linked from prepare_panel(), load_shaders()
    GLint vertex_buffer_attributes_object = glGetAttribLocation(shader_program_object, "vertexPosition");
    glEnableVertexAttribArray(vertex_buffer_attributes_object);
    glVertexAttribPointer(vertex_buffer_attributes_object, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Start render loop
    while (!glfwWindowShouldClose(skypaper_settings.skypaper_window)) {
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the panel
        glUseProgram(shader_program_object);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw the buffer and check for interrupts
        glfwSwapBuffers(skypaper_settings.skypaper_window);
        glfwPollEvents();
    }

    // Free memory
    glDeleteProgram(shader_program_object);

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