#include "project_audio.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Link internal state routing headers defined in audio.c
void init_track_bridge(unsigned char (*beat_func)(unsigned int));
void stop_track_bridge(void);
unsigned int get_track_time(void);

void play_track_1(DISPLAY_HANDLE display, WINDOW_HANDLE window, GC_HANDLE gc, IMAGE_PTR img, int box_size) {
    AudioConfig config = { SAMPLE_RATE_P1 };
    init_track_bridge(bytebeat_p1);

    pthread_t thread;
    if (pthread_create(&thread, NULL, audio_thread_worker, &config) != 0) return;
    printf("Simultaneous Direct Hardware AV Streaming Enabled (Track 1)...\n");

    for (int frame = 0; frame < 100; frame++) {
        unsigned int current_sound_sample = get_track_time();
    #ifndef _WIN32
        XClearWindow(display, window);
    #endif
        for (int y = 0; y < box_size; y++) {
            for (int x = 0; x < box_size; x++) {
                unsigned char r = (current_sound_sample ^ x) & 0xFF;
                unsigned char g = (y + (current_sound_sample >> 4)) & 0xFF;
                unsigned long color = 0xFF000000 | (M_RGB(r, g, 180) & 0x00FFFFFF);
            #ifdef _WIN32
                drawPixel(display, window, gc, x, y, color);
            #else
                XPutPixel((XImage*)img, x, y, color);
            #endif
            }
        }
    #ifndef _WIN32
        XPutImage(display, window, gc, (XImage*)img, 0, 0, 0, 0, box_size, box_size);
        XFlush(display);
    #endif
        USLEEP(16666);
    }
    stop_track_bridge();
    pthread_join(thread, NULL);
}

void play_track_2(DISPLAY_HANDLE display, WINDOW_HANDLE window, GC_HANDLE gc, IMAGE_PTR img, int box_size) {
    AudioConfig config = { SAMPLE_RATE_P2 };
    init_track_bridge(bytebeat_p2);

    pthread_t thread;
    if (pthread_create(&thread, NULL, audio_thread_worker, &config) != 0) return;
    printf("\nTransitioning Stream Channels. Launching Track 2...\n");

    for (int frame = 0; frame < 600; frame++) {
        unsigned int current_sound_sample = get_track_time();
    #ifndef _WIN32
        XClearWindow(display, window);
    #endif
        for (int y = 0; y < box_size; y++) {
            for (int x = 0; x < box_size; x++) {
                unsigned char b = (x * current_sound_sample) % 255;
                unsigned long color = 0xFF000000 | (M_RGB(30, 200, b) & 0x00FFFFFF);
            #ifdef _WIN32
                drawPixel(display, window, gc, x, y, color);
            #else
                XPutPixel((XImage*)img, x, y, color);
            #endif
            }
        }
    #ifndef _WIN32
        XPutImage(display, window, gc, (XImage*)img, 0, 0, 0, 0, box_size, box_size);
        XFlush(display);
    #endif
        USLEEP(33333);
    }
    stop_track_bridge();
    pthread_join(thread, NULL);
}

void play_track_3(DISPLAY_HANDLE display, WINDOW_HANDLE window, GC_HANDLE gc, IMAGE_PTR img, int box_size) {
    AudioConfig config = { SAMPLE_RATE_P1 };
    init_track_bridge(bytebeat_p3);

    pthread_t thread;
    if (pthread_create(&thread, NULL, audio_thread_worker, &config) != 0) return;
    printf("\nTransitioning Stream Channels. Launching Track 3 (Noise Equation)...\n");

    for (int frame = 0; frame < 600; frame++) {
        unsigned int current_sound_sample = get_track_time();
    #ifndef _WIN32
        XClearWindow(display, window);
    #endif
        for (int y = 0; y < box_size; y++) {
            for (int x = 0; x < box_size; x++) {
                unsigned char noise1 = (current_sound_sample ^ rand()) & 0xFF;
                unsigned char noise2 = (current_sound_sample ^ rand()) & 0xFF;
                unsigned char noise3 = (current_sound_sample ^ rand()) & 0xFF;
                unsigned long color = 0xFF000000 | (M_RGB(noise1, noise2, noise3) & 0x00FFFFFF);
            #ifdef _WIN32
                drawPixel(display, window, gc, x, y, color);
            #else
                XPutPixel((XImage*)img, x, y, color);
            #endif
            }
        }
    #ifndef _WIN32
        XPutImage(display, window, gc, (XImage*)img, 0, 0, 0, 0, box_size, box_size);
        XFlush(display);
    #endif
        USLEEP(16666);
    }
    stop_track_bridge();
    pthread_join(thread, NULL);
}
