#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
/* #include <sys/time.h> */

static int show_time(clockid_t clk_id) {
    struct timespec ts = { 0, 0 };
    if (clock_gettime(clk_id, &ts) == 0) {
        printf("%ld.%09ld\n", (long)ts.tv_sec, (long)ts.tv_nsec);
        return 1;
    }
    else {
        perror("clock_gettime");
        return 0;
    }
}

static void usage(void) {
    puts("Usage: clock_gettime [options]");
    puts("    -r CLOCK_REALTIME, system-wide real-time clock");
    puts("    -m CLOCK_MONOTONIC, monotonic time since some unspecified starting point");
#ifdef CLOCK_MONOTONIC_RAW
    puts("    -M CLOCK_MONOTONIC_RAW, hardware-based timer with no ntp adjustments");
#endif
    puts("    -p CLOCK_PROCESS_CPUTIME_ID, high-resolution per-process timer from the CPU");
    puts("    -t CLOCK_THREAD_CPUTIME_ID, thread-specific CPU-time clock");
    puts("(-p and -t are not particularly meaningful in this context.)");
    puts("Multiple arguments may be given");
    puts("With no arguments, show CLOCK_REALTIME");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    int ok = 1;

    if (argc > 1) {
        for (int i = 1; i < argc; i ++) {
            if (strlen(argv[i]) == 2 && argv[i][0] == '-') {
                switch (argv[i][1]) {
                    case 'r':
                        show_time(CLOCK_REALTIME);
                        break;
                    case 'm':
                        show_time(CLOCK_MONOTONIC);
                        break;
#ifdef CLOCK_MONOTONIC_RAW
                    case 'M':
                        show_time(CLOCK_MONOTONIC_RAW);
                        break;
#endif
                    case 'p':
                        show_time(CLOCK_PROCESS_CPUTIME_ID);
                        break;
                    case 't':
                        show_time(CLOCK_THREAD_CPUTIME_ID);
                        break;
                    default:
                        usage();
                        break;
                }
            }
            else {
                usage();
            }
        }
    }
    else {
        ok = ok && show_time(CLOCK_REALTIME);
    }

    exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
} /* main */
