#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static void usage_error(char *program_name) {
    printf("Usage: %s [-m]\n", program_name);
    printf("Without options, print Julian Date\n");
    printf("With \"-m\", print Modified Julian Date\n");
    printf("http://en.wikipedia.org/wiki/Julian_day#Alternatives\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int modified = 0; /* use Modified Julian Date */
    for (int i = 1; i < argc; i ++) {
        if (strcmp(argv[i], "-m") == 0) {
            modified = 1;
        }
        else {
            usage_error(argv[0]);
        }
    }
    struct timeval tp;
    if (gettimeofday(&tp, NULL) != 0) {
        perror("gettimeofday");
        exit(EXIT_FAILURE);
    }
    const long double unix_time = tp.tv_sec + tp.tv_usec/1000000.0;
    const long double jd = unix_time / 86400.0 + (modified ? 40587.0 : 2440587.5);
    printf("%.9Lf\n", jd);
    return 0;
}
