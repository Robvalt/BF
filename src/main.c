#include <stdio.h>
#include <stdlib.h>

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 8000
#endif

#ifdef _WIN32
    #include <io.h>
    #define POPEN _popen
    #define PCLOSE _pclose
    #define WRITE_MODE "wb"
#else
    #define POPEN popen
    #define PCLOSE pclose
    #define WRITE_MODE "w"
#endif

unsigned char bytebeat(unsigned int t) {
    return t>>8 | t<<4;
}

int main() {
    char cmd[256];
    
    // Dynamically build the command string based on the OS and Sample Rate
#ifdef _WIN32
    snprintf(cmd, sizeof(cmd), "ffplay -f u8 -ar %d -ac 1 -nodisp -i -", SAMPLE_RATE);
#else
    snprintf(cmd, sizeof(cmd), "aplay -r %d -c 1 -t raw", SAMPLE_RATE);
#endif

    FILE *audioPipe = POPEN(cmd, WRITE_MODE);
    if (!audioPipe) {
        fprintf(stderr, "Fatal: Could not open audio pipe.\n");
        return 1;
    }

    unsigned int t = 0;
    while (1) {
        unsigned char sample = bytebeat(t++);
        if (fputc(sample, audioPipe) == EOF) break;
    }

    PCLOSE(audioPipe);
    return 0;
}
 