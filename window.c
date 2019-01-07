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

    // We want an undecorated window
    glfwWindowHint(GLFW_DECORATED, false);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create the window
    skypaper_settings.skypaper_window = glfwCreateWindow(video_mode->width, video_mode->height, SKYPAPER_WINDOW_TITLE, NULL, NULL);
    if(!skypaper_settings.skypaper_window)
        fatal_error("Unable to create window");
    
    glfwMakeContextCurrent(skypaper_settings.skypaper_window);

    {
        Window x11_window = glfwGetX11Window(skypaper_settings.skypaper_window);
        Display* x11_display = glfwGetX11Display();

        // Some of these properties take no effect unless you remap the window
        XUnmapWindow(x11_display, x11_window);

        // Tell the WM that this is a desktop window
        Atom property_window_type = XInternAtom(x11_display, "_NET_WM_WINDOW_TYPE", false);
        Atom property_window_type_value = XInternAtom(x11_display, "_NET_WM_WINDOW_TYPE_DESKTOP", false);
        XChangeProperty(x11_display, x11_window, property_window_type, XA_ATOM, 32, PropModeReplace, (unsigned char*) &property_window_type_value, 1);

        // Tell the WM that this window should be below others
        Atom property_state = XInternAtom(x11_display, "_NET_WM_STATE", false);
        Atom property_state_value = XInternAtom(x11_display, "_NET_WM_STATE_BELOW", false);
        XChangeProperty(x11_display, x11_window, property_state, XA_ATOM, 32, PropModeAppend, (unsigned char*) &property_state_value, 1);

        // Tell the WM to bind this window to all desktops
        Atom property_desktop = XInternAtom(x11_display, "_NET_WM_DESKTOP", false);
        long int property_desktop_value = 0xFFFFFFFF;
        XChangeProperty(x11_display, x11_window, property_desktop, XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &property_desktop_value, 1);

        // Map the window, updates for the previous property
        XMapWindow(x11_display, x11_window);
    }
}