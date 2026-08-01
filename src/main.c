#include "project_audio.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/extensions/shape.h>  
    #include <X11/extensions/Xfixes.h> 
#endif

int main(void) {
#ifdef _WIN32
    WINDOW_HANDLE window = GetDesktopWindow(); 
    DISPLAY_HANDLE display = getScreenDisplay(window);
    GC_HANDLE gc = 0;
    int width = getWindowWidth(display, window);
    int height = getWindowHeight(display, window);
#else
    Display* dpy = XOpenDisplay(NULL);
    if (!dpy) return 1;
    int scr = DefaultScreen(dpy);
    Window root = RootWindow(dpy, scr);

    XVisualInfo vinfo;
    if (!XMatchVisualInfo(dpy, scr, 32, TrueColor, &vinfo)) {
        XCloseDisplay(dpy);
        return 1;
    }
    int width = DisplayWidth(dpy, scr);
    int height = DisplayHeight(dpy, scr);

    XSetWindowAttributes attr = {
        .colormap = XCreateColormap(dpy, root, vinfo.visual, AllocNone),
        .background_pixel = 0, .border_pixel = 0, .override_redirect = True,
        .event_mask = ExposureMask
    };
    Window window = XCreateWindow(dpy, root, 0, 0, width, height, 0, vinfo.depth, 
                                  InputOutput, vinfo.visual, 
                                  CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect | CWEventMask, &attr);
    XFixesSelectCursorInput(dpy, window, 0);
    XFixesSetWindowShapeRegion(dpy, window, ShapeInput, 0, 0, None);
    XMapWindow(dpy, window);

    DISPLAY_HANDLE display = dpy;
    GC_HANDLE gc = XCreateGC(display, window, 0, NULL);
    Visual *visual = vinfo.visual;
#endif

    printf("Render Overlay Canvas Initialized: %dx%d.\n", width, height);
    int box_size = 1200 * width / height;

#ifndef _WIN32
    unsigned long *image_data = malloc(box_size * box_size * sizeof(unsigned long));
    XImage *x_image = XCreateImage(display, visual, vinfo.depth, ZPixmap, 0, (char *)image_data, box_size, box_size, 32, 0);
#else
    void* x_image = NULL;
#endif

    // Synchronized track execution blocks
    play_track_1(display, window, gc, x_image, box_size);
    play_track_2(display, window, gc, x_image, box_size);
    play_track_3(display, window, gc, x_image, box_size);

#ifndef _WIN32
    XDestroyImage(x_image); XFreeGC(display, gc);
    XDestroyWindow(display, window); XCloseDisplay(display);
#else
    releaseScreen(window, display, gc);
#endif
    return 0;
}
