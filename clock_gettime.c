#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
/* #include <sys/time.h> */

static int show_res(clockid_t clk_id) {
    struct timespec ts = { 0, 0 };
    if (clock_getres(clk_id, &ts) == 0) {
        printf("%ld.%09ld\n", (long)ts.tv_sec, (long)ts.tv_nsec);
        return 1;
    }
    else {
        perror("clock_getres");
        return 0;
    }
}

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

static int show(clockid_t clk_id, bool res) {
    (res ? show_res : show_time)(clk_id);
}

static void usage(void) {
    puts("Usage: clock_gettime [options]");
    puts("  -r    CLOCK_REALTIME, system-wide real-time clock (default)");
    puts("  -m    CLOCK_MONOTONIC, monotonic time since some unspecified starting point");
#ifdef CLOCK_MONOTONIC_RAW
    puts("  -M    CLOCK_MONOTONIC_RAW, hardware-based timer with no ntp adjustments");
#endif
    puts("  -p    CLOCK_PROCESS_CPUTIME_ID, high-resolution per-process timer from the CPU");
    puts("  -t    CLOCK_THREAD_CPUTIME_ID, thread-specific CPU-time clock");
    puts("(-p and -t are not particularly meaningful in this context.)");
    puts("Multiple arguments may be given");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    bool ok = 1;
    int res = 0;

    if (argc > 1) {
        for (int i = 1; i < argc; i ++) {
            if (strlen(argv[i]) == 2 && argv[i][0] == '-') {
                switch (argv[i][1]) {
                    case 'R':
                        res = 1;
                        break;
                    case 'r':
                        show(CLOCK_REALTIME, res);
                        break;
                    case 'm':
                        show(CLOCK_MONOTONIC, res);
                        break;
#ifdef CLOCK_MONOTONIC_RAW
                    case 'M':
                        show(CLOCK_MONOTONIC_RAW, res);
                        break;
#endif
                    case 'p':
                        show(CLOCK_PROCESS_CPUTIME_ID, res);
                        break;
                    case 't':
                        show(CLOCK_THREAD_CPUTIME_ID, res);
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
        ok = ok && show(CLOCK_REALTIME, res);
    }

    exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
} /* main */
