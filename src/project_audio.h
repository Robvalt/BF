#ifndef PROJECT_AUDIO_H
#define PROJECT_AUDIO_H

#include "platform.h"

// Forward-declare XImage for platform safety across non-X11 targets
#ifndef _WIN32
    #include <X11/Xlib.h>
    typedef XImage* IMAGE_PTR;
#else
    typedef void* IMAGE_PTR;
#endif

// Shared hardware configurations
typedef struct {
    int sample_rate;
} AudioConfig;

// Signal generators
unsigned char bytebeat_p1(unsigned int t);
unsigned char bytebeat_p2(unsigned int t);
unsigned char bytebeat_p3(unsigned int t);

// Background threaded execution worker
void* audio_thread_worker(void* arg);

// Segment tracking runners
void play_track_1(DISPLAY_HANDLE display, WINDOW_HANDLE window, GC_HANDLE gc, IMAGE_PTR img, int box_size);
void play_track_2(DISPLAY_HANDLE display, WINDOW_HANDLE window, GC_HANDLE gc, IMAGE_PTR img, int box_size);
void play_track_3(DISPLAY_HANDLE display, WINDOW_HANDLE window, GC_HANDLE gc, IMAGE_PTR img, int box_size);

#endif
