#include "project_audio.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #include <mmeapi.h>
#else
    #include <alsa/asoundlib.h>
#endif

// Anchor structural declarations safely using your project's bridge blueprint
static AudioVisualBridge av_sync = {0, false, NULL};

unsigned char bytebeat_p1(unsigned int t) {
    return ((((t>>2) + (t&32768?13:14)*t) / (1 + (t>>12)%4)) & (128 + (t>>12))) / 1;
}

unsigned char bytebeat_p2(unsigned int t) {
    return t * (t ^ t >> 20 * (t >> 11));
}

unsigned char bytebeat_p3(unsigned int t) {
    return t * rand();
}

void* audio_thread_worker(void* arg) {
    AudioConfig* config = (AudioConfig*)arg;
    int sample_rate = config->sample_rate;
    unsigned char buffer[AUDIO_BUF_SIZE];

#ifdef _WIN32
    WAVEFORMATEX wfx = {
        .wFormatTag = WAVE_FORMAT_PCM, .nChannels = 1, .nSamplesPerSec = sample_rate,
        .nAvgBytesPerSec = sample_rate, .nBlockAlign = 1, .wBitsPerSample = 8, .cbSize = 0
    };
    HWAVEOUT hWaveOut;
    if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
        fprintf(stderr, "Fatal: Windows waveOut initialization failed.\n");
        return NULL;
    }
    WAVEHDR header1 = { .lpData = (LPSTR)malloc(AUDIO_BUF_SIZE), .dwBufferLength = AUDIO_BUF_SIZE };
    WAVEHDR header2 = { .lpData = (LPSTR)malloc(AUDIO_BUF_SIZE), .dwBufferLength = AUDIO_BUF_SIZE };
    waveOutPrepareHeader(hWaveOut, &header1, sizeof(WAVEHDR));
    waveOutPrepareHeader(hWaveOut, &header2, sizeof(WAVEHDR));
    WAVEHDR* active_header = &header1;
#else
    snd_pcm_t *pcm_handle;
    if (snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        fprintf(stderr, "Fatal: ALSA device playback allocation error.\n");
        return NULL;
    }
    snd_pcm_set_params(pcm_handle, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED, 1, sample_rate, 1, 500000);
#endif

    while (av_sync.audio_running) {
        for (int i = 0; i < AUDIO_BUF_SIZE; i++) {
            buffer[i] = av_sync.current_bytebeat(av_sync.current_t + i);
        }
    #ifdef _WIN32
        while (!(active_header->dwFlags & WHDR_DONE) && av_sync.audio_running) { USLEEP(1000); }
        memcpy(active_header->lpData, buffer, AUDIO_BUF_SIZE);
        waveOutWrite(hWaveOut, active_header, sizeof(WAVEHDR));
        active_header = (active_header == &header1) ? &header2 : &header1;
    #else
        snd_pcm_sframes_t frames = snd_pcm_writei(pcm_handle, buffer, AUDIO_BUF_SIZE);
        if (frames < 0) { snd_pcm_prepare(pcm_handle); }
    #endif
        av_sync.current_t += AUDIO_BUF_SIZE;
    }

#ifdef _WIN32
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &header1, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header2, sizeof(WAVEHDR));
    free(header1.lpData); free(header2.lpData);
    waveOutClose(hWaveOut);
#else
    snd_pcm_drain(pcm_handle); snd_pcm_close(pcm_handle);
#endif
    return NULL;
}

// Internal bridge helper used to bootstrap separate track signals safely
void init_track_bridge(unsigned char (*beat_func)(unsigned int)) {
    av_sync.current_t = 0;
    av_sync.audio_running = true;
    av_sync.current_bytebeat = beat_func;
}

// Interface wrapper to stop processing queues
void stop_track_bridge(void) {
    av_sync.audio_running = false;
}

// Interface getter helper to capture synchronicity indexes
unsigned int get_track_time(void) {
    return av_sync.current_t;
}
