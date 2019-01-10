#pragma once
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
#include "settings.h"

void read_file(char** path, char** file_data);
bool check_for_shader_folder();
void get_data_directory(char** data_directory_path);
void get_from_shader_data_path(char** data_directory, char** shader_name, char** file_name, char** path);
