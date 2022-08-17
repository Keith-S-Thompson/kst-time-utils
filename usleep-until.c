#define _XOPEN_SOURCE 500

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ONE_MILLION 1000000

static void usage_error(const char *program_name) {
    fprintf(stderr, "Usage: %s seconds [command [args...]]\n", program_name);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage_error(argv[0]);
    }
    const int seconds = atoi(argv[1]);
    if (seconds <= 0) {
        usage_error(argv[0]);
    }

    struct timeval tv;
    gettimeofday(&tv, NULL);

    time_t sleep_seconds = seconds - tv.tv_sec % seconds - 1;
    suseconds_t sleep_useconds = ONE_MILLION - tv.tv_usec;
    if (sleep_useconds == ONE_MILLION) {
        sleep_useconds = 0;
        sleep_seconds ++;
    }

    usleep(ONE_MILLION * (useconds_t)sleep_seconds + sleep_useconds);

    if (argc > 2) {
        execvp(argv[2], argv+2);
    }
}
