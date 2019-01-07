/*
    window - Create and manage the desktop window

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
#include "window.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <stdio.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

void create_skypaper_window() {
    // Get our current video mode and set to match it. We can make a fullscreen window without changing modes
    const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create the window
    skypaper_settings.skypaper_window = glfwCreateWindow(video_mode->width, video_mode->height, SKYPAPER_WINDOW_TITLE, glfwGetPrimaryMonitor(), NULL);
    if(!skypaper_settings.skypaper_window)
        fatal_error("Unable to create window");
    
    glfwMakeContextCurrent(skypaper_settings.skypaper_window);

    {
        // Tell X11 that this is a desktop window
        Window x11_window = glfwGetX11Window(skypaper_settings.skypaper_window);
        Display* x11_display = glfwGetX11Display();
        Atom property_window_type = XInternAtom(x11_display, "_NET_WM_WINDOW_TYPE", false);
        Atom property_window_type_value = XInternAtom(x11_display, "_NET_WM_WINDOW_TYPE_DESKTOP", false);
        XChangeProperty(x11_display, x11_window, property_window_type, XA_ATOM, 32, PropModeReplace, (unsigned char*) &property_window_type_value, 1);
    }
}