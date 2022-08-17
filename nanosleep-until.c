#define _XOPEN_SOURCE 500

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ONE_BILLION 1000000000

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

    struct timespec ts;
    int result = clock_gettime(CLOCK_REALTIME, &ts);
    if (result != 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    time_t sleep_sec = seconds - ts.tv_sec % seconds - 1;
    long sleep_nsec = ONE_BILLION - ts.tv_nsec;
    if (sleep_nsec == ONE_BILLION) {
        sleep_nsec = 0;
        sleep_sec ++;
    }

    const struct timespec how_long = {
        .tv_sec  = sleep_sec,
        .tv_nsec = sleep_nsec
    };

    nanosleep(&how_long, NULL);

    if (argc > 2) {
        execvp(argv[2], argv+2);
    }
}
