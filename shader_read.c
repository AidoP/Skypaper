/*
    shader_read - Read in shaders from files

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
#include "shader_read.h"

void read_file(char** path, char** file_data) {

    // Allocate memory for the error return
    *file_data = malloc(1);
    **file_data = '\0';
    if (!*file_data)
        fatal_error("Unable to allocate memory");

    // Open file for reading only
    FILE* file = fopen(*path, "r");
    if (!file) {
        fprintf(stderr, "Unable to open %s\n", *path);
        return;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long int file_size = ftell(file);
    rewind(file);


    // Reallocate memory for the read file data
    free(*file_data);
    *file_data = malloc(file_size + 1);
    if (!*file_data) {
        fclose(file);
        fatal_error("Unable to allocate memory");
    }

    // Read from the file
    int read_status = fread(*file_data, file_size, 1, file);
    if (read_status != 1) {
        fprintf(stderr, "Unable to read %s\n", *path);
        **file_data = '\0';
        return;
    }
    (*file_data)[file_size] = '\0';


    fprintf(stderr, "Shader source=\n%s", *file_data);
    
    // Close file and return read data
    fclose(file);
}

void get_data_directory(char** data_directory_path) {
    // Set the data directory to $XDG_DATA_HOME/skypaper
    // String manipulation
    char* xdg_home = getenv("XDG_DATA_HOME");
    char* home_path = getenv("HOME");
    if (!home_path)
        fatal_error("$HOME must be set");
    if (!strcmp(home_path, ""))
        warning("$HOME is empty. Assuming CWD is home.");

    bool found_xdg_home = true;
    if (!xdg_home)
        found_xdg_home = false;
    else if (!strcmp(xdg_home, ""))
        found_xdg_home = false;

    if (!found_xdg_home) {
        char* subpath = "/.local/share";
        free(xdg_home);
        xdg_home = malloc(strlen(home_path) + strlen(subpath) + 1);
        xdg_home[0] = '\0';
        strcat(xdg_home, home_path);
        strcat(xdg_home, subpath);
    }
    
    char* skypaper_home_directory = "/skypaper";
    *data_directory_path = malloc(strlen(xdg_home) + strlen(skypaper_home_directory) + 1);
    *data_directory_path[0] = '\0';
    strcat(*data_directory_path, xdg_home);
    strcat(*data_directory_path, skypaper_home_directory);
}

bool check_for_shader_folder() {
    // Ensure that our shader folders exists
    char* shader_folder_path;
    get_data_directory(&shader_folder_path);
    bool folder_exists = true;

    DIR* directory = opendir(shader_folder_path);
    if (!directory) {
        fprintf(stderr, "Folder missing. Could not open %s\n", shader_folder_path);
        folder_exists = false;
    }
    closedir(directory);

    free(shader_folder_path);
    return folder_exists;
}

void get_from_shader_data_path(char** data_directory, char** shader_name, char** file_name, char** path) {
    // Handle path strings
    char* shader_folder_path = "/shaders/";

    *path = malloc(strlen(*data_directory)+ strlen(shader_folder_path) + strlen(*shader_name) + 1 + strlen(*file_name) + 1);

    if (!*path)
        fatal_error("Unable to allocate memory");

    **path = '\0';
    strcat(*path, *data_directory);
    strcat(*path, shader_folder_path);
    strcat(*path, *shader_name);
    strcat(*path, "/");
    strcat(*path, *file_name);
}

#include <unistd.h>
bool try_read_stdin(char** read_data) {
    // Allocate a 1MiB buffer
    int buffer_size = 1048576;
    *read_data = malloc(buffer_size);
    if (!*read_data)
        return false;
    // Read in to a max of one less than the buffer size
    int read_size = read(0, *read_data, buffer_size - 1);

    // Return if failed
    if (read_size < 0)
        return false;

    // Ensure string is null terminating
    (*read_data)[read_size + 1] = '\0';
    return true;
}