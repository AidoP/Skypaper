#ifndef RENDERER_H
#define RENDERER_H

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <X11/Xatom.h>

// Renderer.h - Handles OpenGL Rendering code

typedef struct renderer {
    Display* display;
    Window root;
    Window window;
    GLint attributes[5];
    XVisualInfo* visualInfo;
    GLXContext context;
} renderer;




#endif