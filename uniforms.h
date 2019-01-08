#pragma once

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
#include "settings.h"

/*
    All functions must be of type
        void (GLint)
*/


void update_uniform_iTime(GLint uniform_location);
void update_uniform_iTimeDelta(GLint uniform_location);
void update_uniform_iResolution(GLint uniform_location);
void update_uniform_iMouse(GLint uniform_location);