#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SAMPLE_RATE_P1 8000
#define SAMPLE_RATE_P2 8000
#define AUDIO_BUF_SIZE 1024

#ifdef _WIN32
    #include <io.h>
    #include <windows.h>
    #define POPEN _popen
    #define PCLOSE _pclose
    #define WRITE_MODE "wb"
    #define SLEEP(s) Sleep((s) * 1000) 
    #define USLEEP(us) Sleep((us) / 1000)
    
    #define DISPLAY_HANDLE HDC
    #define WINDOW_HANDLE  HWND
    #define GC_HANDLE      int 
    #define M_RGB(r, g, b) RGB(r, g, b)

    static inline DISPLAY_HANDLE getScreenDisplay(WINDOW_HANDLE hwnd) { return GetDC(hwnd); }
    static inline void releaseScreen(WINDOW_HANDLE hwnd, DISPLAY_HANDLE hdc, GC_HANDLE gc) { 
        (void)gc; ReleaseDC(hwnd, hdc); 
    }
    static inline int getWindowWidth(DISPLAY_HANDLE hdc, WINDOW_HANDLE window) {
        (void)window; return GetDeviceCaps(hdc, HORZRES);
    }
    static inline int getWindowHeight(DISPLAY_HANDLE hdc, WINDOW_HANDLE window) {
        (void)window; return GetDeviceCaps(hdc, VERTRES);
    }
    static inline void drawPixel(DISPLAY_HANDLE hdc, WINDOW_HANDLE win, GC_HANDLE gc, int x, int y, COLORREF color) {
        (void)win; (void)gc; SetPixel(hdc, x, y, color);
    }
    static inline void copyRect(DISPLAY_HANDLE hdc, WINDOW_HANDLE src_win, WINDOW_HANDLE dest_win, GC_HANDLE gc,
                         int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
        (void)src_win; (void)dest_win; (void)gc;
        BitBlt(hdc, dest_x, dest_y, width, height, hdc, src_x, src_y, SRCCOPY);
    }
#else
    #include <unistd.h>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #define POPEN popen
    #define PCLOSE pclose
    #define WRITE_MODE "w"
    #define SLEEP(s) sleep(s)
    #define USLEEP(us) usleep(us)
    
    #define DISPLAY_HANDLE Display*
    #define WINDOW_HANDLE  Window
    #define GC_HANDLE      GC
    #define M_RGB(r, g, b) (((unsigned long)(r) << 16) | ((unsigned long)(g) << 8) | (b))

    static inline DISPLAY_HANDLE getScreenDisplay(WINDOW_HANDLE win) { (void)win; return XOpenDisplay(NULL); }
    static inline void releaseScreen(DISPLAY_HANDLE display, WINDOW_HANDLE win, GC_HANDLE gc) { 
        if (gc) XFreeGC(display, gc);
        if (win) XDestroyWindow(display, win);
        XCloseDisplay(display); 
    }
    static inline int getWindowWidth(DISPLAY_HANDLE display, WINDOW_HANDLE window) {
        XWindowAttributes attributes; XGetWindowAttributes(display, window, &attributes); return attributes.width;
    }
    static inline int getWindowHeight(DISPLAY_HANDLE display, WINDOW_HANDLE window) {
        XWindowAttributes attributes; XGetWindowAttributes(display, window, &attributes); return attributes.height;
    }
    static inline void drawPixel(DISPLAY_HANDLE display, WINDOW_HANDLE win, GC_HANDLE gc, int x, int y, unsigned long color) {
        XSetForeground(display, gc, color); XDrawPoint(display, win, gc, x, y);
    }
    static inline void copyRect(DISPLAY_HANDLE display, WINDOW_HANDLE src_win, WINDOW_HANDLE dest_win, GC_HANDLE gc,
                         int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
        XCopyArea(display, src_win, dest_win, gc, src_x, src_y, width, height, dest_x, dest_y);
    }
#endif

// Sync Shared Architecture Definitions
typedef struct {
    volatile unsigned int current_t;
    volatile bool audio_running;
    unsigned char (*current_bytebeat)(unsigned int);
} AudioVisualBridge;

#endif // PLATFORM_H
