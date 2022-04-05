#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
/* #include <sys/time.h> */

static bool show_res(clockid_t clk_id) {
    // puts(">>> show_res");
    struct timespec ts = { 0, 0 };
    if (clock_getres(clk_id, &ts) == 0) {
        printf("%ld.%09ld\n", (long)ts.tv_sec, (long)ts.tv_nsec);
        return true;
    }
    else {
        perror("clock_getres");
        return false;
    }
}

static bool show_time(clockid_t clk_id) {
    // puts(">>> show_time");
    struct timespec ts = { 0, 0 };
    if (clock_gettime(clk_id, &ts) == 0) {
        printf("%ld.%09ld\n", (long)ts.tv_sec, (long)ts.tv_nsec);
        return true;
    }
    else {
        perror("clock_gettime");
        return false;
    }
}

static bool show(clockid_t clk_id, bool res) {
    // puts(">>> show");
    return (res ? show_res : show_time)(clk_id);
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
    puts("  -R    Show resolution (calls clock_getres), may be followed by other options");
    puts("Multiple arguments may be given");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    bool ok = true;
    bool show_res = false;
    bool shown = false;

    for (int i = 1; i < argc; i ++) {
        if (strlen(argv[i]) == 2 && argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'R':
                    // puts(">>> show_res = true");
                    show_res = true;
                    break;
                case 'r':
                    ok = ok && show(CLOCK_REALTIME, show_res);
                    shown = true;
                    break;
                case 'm':
                    ok = ok && show(CLOCK_MONOTONIC, show_res);
                    shown = true;
                    break;
#ifdef CLOCK_MONOTONIC_RAW
                case 'M':
                    ok = ok && show(CLOCK_MONOTONIC_RAW, show_res);
                    shown = true;
                    break;
#endif
                case 'p':
                    ok = ok && show(CLOCK_PROCESS_CPUTIME_ID, show_res);
                    shown = true;
                    break;
                case 't':
                    ok = ok && show(CLOCK_THREAD_CPUTIME_ID, show_res);
                    shown = true;
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

    if (!shown) {
        ok = ok && show(CLOCK_REALTIME, show_res);
    }

    exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
} /* main */
