#include <string.h>
#include "renderer.h"
#include "util.c"

void DrawAQuad() {
 glClearColor(1.0, 1.0, 1.0, 1.0);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-1., 1., -1., 1., 1., 20.);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 //gluLookAt(0., 0., 10., 0., 0., 0., 0., 1.1, 0.);

 glBegin(GL_QUADS);
  glColor3f(1., 0., 0.); glVertex3f(-1., -1., 0.);
  glColor3f(0., 1., 0.); glVertex3f( 1., -1., 0.);
  glColor3f(0., 0., 1.); glVertex3f( 1.,  1., 0.);
  glColor3f(1., 1., 0.); glVertex3f(-1.,  1., 0.);
 glEnd();
} 

renderer* init_renderer() {
    // Create the structure in memory
    renderer* r = malloc(sizeof(struct renderer));

    /* open display */
    if (!(r->display = XOpenDisplay(NULL)) )
        fatalError("Unable to connect to X");

    // Get the root window
    r->root = DefaultRootWindow(r->display);

    {
        GLint attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
        memcpy(r->attributes, attributes, sizeof(attributes));
    }

    if(!(r->visualInfo = glXChooseVisual(r->display, 0, r->attributes)))
        fatalError("Unable to get visual info");
    
    // Create the context
    if (!(r->context = glXCreateContext(r->display, r->visualInfo, NULL, GL_TRUE)))
        fatalError("Unable to create GL context");
    glXMakeCurrent(r->display, r->root, r->context);

    XSetWindowAttributes winAttributes;
    winAttributes.colormap          = XCreateColormap(r->display, r->root, r->visualInfo->visual, AllocNone);
    winAttributes.event_mask        = ExposureMask | KeyPressMask;
    winAttributes.background_pixmap = None;


    if (!(r->window = XCreateWindow(r->display, r->root,
                               0, 0, 1920, 1080, 0,
                               r->visualInfo->depth, InputOutput, r->visualInfo->visual,
                               CWColormap | CWEventMask,
                               &winAttributes)))
                               fatalError("Failed to create window");

    XMapWindow(r->display, r->window);


    // Set as desktop window - Big thanks to Glava here
    Atom wtype = XInternAtom(r->display, "_NET_WM_WINDOW_TYPE", false);
    Atom desk = XInternAtom(r->display, "_NET_WM_WINDOW_TYPE_DESKTOP", false);
    XChangeProperty(r->display, r->window, wtype, XA_ATOM, 32, PropModeReplace, (unsigned char*) &desk, 1);


    XMapWindow(r->display, r->window);
    XStoreName(r->display, r->window, "Yote");
 
    XSync(r->display, False);
    glXMakeCurrent(r->display, r->window, r->context);
 
    glEnable(GL_DEPTH_TEST); 
    return r;
}

void render(renderer* r) {
    XWindowAttributes winAttributes;
    XEvent event;

    while(1) {
        XNextEvent(r->display, &event);
        
        if(event.type == Expose) {
                XGetWindowAttributes(r->display, r->window, &winAttributes);
                glViewport(0, 0, winAttributes.width, winAttributes.height);
                DrawAQuad(); 
                glXSwapBuffers(r->display, r->window);
        }
                
        else if(event.type == KeyPress) {
                glXMakeCurrent(r->display, None, NULL);
                glXDestroyContext(r->display, r->context);
                XDestroyWindow(r->display, r->window);
                XCloseDisplay(r->display);
                exit(0);
        }
    }
}