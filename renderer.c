#include <string.h>

#include <glad/glad.h>
#include <sys/time.h>
#include <unistd.h>
#include "renderer.h"
#include "util.c"

GLuint quadVertexBuffer;

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

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

    int contextAttrs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        None
    };


    // Get config
    GLXFBConfig* fbc;
    int fbcSize, samp = -1, best = -1;
    if(!(fbc = glXChooseFBConfig(r->display, DefaultScreen(r->display), r->attributes, &fbcSize)))
        fatalError("Unable to get FBConfig");
    for (int t = 0; t < fbcSize; ++t) {
        XVisualInfo* xvi = glXGetVisualFromFBConfig(r->display, fbc[t]);
        if (xvi) {
            int samp_buf, samples;
            glXGetFBConfigAttrib(r->display, fbc[t], GLX_SAMPLE_BUFFERS, &samp_buf);
            glXGetFBConfigAttrib(r->display, fbc[t], GLX_SAMPLES,        &samples );
            XRenderPictFormat* fmt = XRenderFindVisualFormat(r->display, xvi->visual);
            
            if (!fmt || fmt->direct.alphaMask == 0)
                continue;
            
            if (best < 0 || (samp_buf && samples > samp)) {
                best = t;
                samp = samples;
            }
            XFree(xvi);
        }
    }

    if (best == -1) {
        fprintf(stderr, "Could not find suitable format for FBConfig\n");
        abort();
    }

    r->config = fbc[best];
    XFree(fbc);



    if(!(r->visualInfo = glXGetVisualFromFBConfig(r->display, r->config)))
        fatalError("Unable to get visual info");


    // load function
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB((const GLubyte*) "glXCreateContextAttribsARB");
    if (!glXCreateContextAttribsARB)
        fatalError("Unable to load glXCreateContextAttribsARB");

    if (!(r->context = glXCreateContextAttribsARB(r->display, r->config, 0, True, contextAttrs)))
        fatalError("Unable to create GL context");

    // Create the context
    glXMakeCurrent(r->display, r->root, r->context);

    XSetWindowAttributes winAttributes;
    winAttributes.colormap          = XCreateColormap(r->display, r->root, r->visualInfo->visual, AllocNone);
    winAttributes.event_mask        = ExposureMask | KeyPressMask;
    winAttributes.background_pixmap = None;
    winAttributes.border_pixel      = 0;


    if (!(r->window = XCreateWindow(r->display, r->root,
                               0, 0, 1920, 1080, 0,
                               r->visualInfo->depth, InputOutput, r->visualInfo->visual,
                               CWColormap | CWEventMask | CWBackPixmap | CWBorderPixel,
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
 
    // load OpenGL functions
    gladLoadGL();


    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glGenVertexArrays(1, &(r->vertexArray));
    glBindVertexArray(r->vertexArray);


    // Create quad data
    glGenBuffers(1, &quadVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);

    static const GLfloat quadVertexData[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        1.0f,  -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);

    return r;
}

void render(renderer* r, GLuint shaderProgram) {
    XWindowAttributes winAttributes;
    XEvent event;

    float res[2] = {1920.0f, 1080.0f};

    while(true) {
        if(XPending(r->display) > 0){
            XNextEvent(r->display, &event);
            if(event.type == KeyPress) {
            
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        struct timeval tv;
        gettimeofday(&tv, 0);
        GLint uTimeLoc = glGetUniformLocation(shaderProgram, "time");
        if (uTimeLoc != -1) 
            glUniform1f(uTimeLoc, (float)(((float)(tv.tv_sec % 1000)) + (((float)tv.tv_usec) / 1000000)));

        GLint uResLoc = glGetUniformLocation(shaderProgram, "resolution");
        if (uResLoc != -1) 
            glUniform2fv(uResLoc, 1, res); 

        // Draw the quad
        glBindVertexArray(r->vertexArray);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        glUseProgram(0);

        glXSwapBuffers(r->display, r->window);

        // Limit framerate (temp)
        usleep(1000000/60);
    }
}
