/*
    uniforms - Functions to update shader uniform values

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
#include "uniforms.h"
#include <GLFW/glfw3.h>
#include <time.h>

float program_start_time_seconds = 0;
float last_loop_time_seconds = 0;

void update_uniform_iTime(GLint uniform_location) {
    // Set initial time
    if (program_start_time_seconds == 0) {
        struct timespec program_start_time;
        clock_gettime(CLOCK_MONOTONIC, &program_start_time);
        program_start_time_seconds = program_start_time.tv_sec + (program_start_time.tv_nsec / 1E9f);
    }

    // Get current time
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    float time_seconds_current = current_time.tv_sec + (current_time.tv_nsec / 1E9f);

    // Get the time since program execution
    float time_seconds = time_seconds_current - program_start_time_seconds;

    // Set the uniform
    glUniform1f(uniform_location, time_seconds);
}

void update_uniform_iTimeDelta(GLint uniform_location) {
    // Initialise last loop time
    if (last_loop_time_seconds == 0) {
        struct timespec last_loop_time;
        clock_gettime(CLOCK_MONOTONIC, &last_loop_time);
        last_loop_time_seconds = last_loop_time.tv_sec + (last_loop_time.tv_nsec / 1E9);
    }

    // Get current time
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    float time_seconds_current = current_time.tv_sec + (current_time.tv_nsec / 1E9);

    // Get the delta time
    float delta_time_seconds = time_seconds_current - last_loop_time_seconds;
    
    // Set the uniform
    glUniform1f(uniform_location, delta_time_seconds);

    // Update the last loop time
    last_loop_time_seconds = time_seconds_current;
}

void update_uniform_iResolution(GLint uniform_location) {
    // Get the window resolution
    int width;
    int height;
    glfwGetFramebufferSize(skypaper_settings.skypaper_window, &width, &height);

    // Set the uniform
    glUniform2f(uniform_location, (float)width, (float)height);
}

void update_uniform_iMouse(GLint uniform_location) {
    // Get the window resolution
    double width;
    double height;
    glfwGetCursorPos(skypaper_settings.skypaper_window, &width, &height);
    
    // Set the uniform
    glUniform2f(uniform_location, (float)width, (float)height);
}