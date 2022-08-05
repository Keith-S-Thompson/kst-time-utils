#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/time.h>

#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef enum { false, true } bool;
#endif

static bool absolute = false;
static bool absolute_readable = false;
static bool utc = false;
static bool relative = false;
static bool buffered = false;
static struct timeval t0;
static int t0_initialized = false;

static struct timeval diff(struct timeval left, struct timeval right) {
    struct timeval result;

    result.tv_sec = left.tv_sec - right.tv_sec;
    result.tv_usec = left.tv_usec - right.tv_usec;
    if (result.tv_usec < 0) {
        result.tv_sec --;
        result.tv_usec += 1000000;
    }
    return result;
}

static void show_timeval(FILE *stream, struct timeval tv) {
    if (absolute_readable) {
        // YYYY-MM-DD-HH:MM:SS-mmmmmm
        struct tm *ltime = (utc ? gmtime : localtime)(&tv.tv_sec);
        char format[100];
        sprintf(format, "%%F-%%T.%06ld", tv.tv_usec);
        char buf[100];
        strftime(buf, sizeof buf, format, ltime);
        fputs(buf, stream);
    }
    else {
        fprintf(stream, absolute ? "%10ld.%06ld" : "%8ld.%06ld",
                        (long)tv.tv_sec,
                        (long)tv.tv_usec);
    }
}

static void show_time(FILE *stream, int real_time_too) {
    struct timeval tv;
    int result = gettimeofday(&tv, NULL);
    if (result == 0) {
        if (! t0_initialized) {
            t0 = tv;
            t0_initialized = 1;
        }
        if (absolute || absolute_readable) {
            show_timeval(stream, tv);
        }
        else {
            show_timeval(stream, diff(tv, t0));
            if (real_time_too) {
                fprintf(stream, " [");
                show_timeval(stream, tv);
                fprintf(stream, "]");
            }
        }
        if (relative) {
            t0 = tv;
        }
    }
    else {
        fprintf(stream, "---------.------");
    }
} /* show_time */

static void usage(char *prog_name, char *message) {
    if (message != NULL) {
        puts(message);
    }
    printf("Usage: %s [options]\n", prog_name);
    puts("    -a : Absolute time (default is zero-based)");
    puts("    -A : Absolute local time, human-readable");
    puts("    -U : Absolute UTC time, human-readable");
    puts("    -r : Show time interval relative to previous line");
    puts("    -b : Buffered output (default is unbuffered)");
    puts("    -h : Help (show this message and exit)");
    puts("    -z : Zero-based time (redundant, this is the default)");
    puts("    -u : Unbuffered output (redundant, this is the default)");
    puts("    -o file : Print output to specified file");
    exit(EXIT_SUCCESS);
} /* usage */

int main(int argc, char **argv) {
#define LEN 8192

    char line[LEN];
    int i;
    char *output_file_name = NULL;
    FILE *output_file = stdout;

    for (i = 1; i < argc; i ++) {
        if (strcmp(argv[i], "-a") == 0) {
            absolute = true;
        }
        else if (strcmp(argv[i], "-A") == 0) {
            absolute_readable = true;
            utc = false;
        }
        else if (strcmp(argv[i], "-U") == 0) {
            absolute_readable = true;
            utc = true;
        }
        else if (strcmp(argv[i], "-r") == 0) {
            relative = true;
        }
        else if (strcmp(argv[i], "-z") == 0) {
            absolute = false;
        }
        else if (strcmp(argv[i], "-b") == 0) {
            buffered = true;
        }
        else if (strcmp(argv[i], "-u") == 0) {
            buffered = false;
        }
        else if (strcmp(argv[i], "-o") == 0) {
            if (i < argc-1) {
                output_file_name = argv[i+1];
                i ++;
            }
            else {
                usage(argv[0], "Option -o requires a file name argument");
            }
        }
        else if (strcmp(argv[i], "-h") == 0) {
            usage(argv[0], NULL);
        }
        else {
            char message[100];
            sprintf(message, "Unrecognized argument: \"%s\"", argv[i]);
            usage(argv[0], message);
        }
    }

    if (absolute && relative) {
        usage(argv[0], "Options -a and -r are incompatible");
    }

    if (output_file_name != NULL) {
        output_file = fopen(output_file_name, "w");
        if (output_file == NULL) {
            perror(output_file_name);
            exit(EXIT_FAILURE);
        }
    }


    if (! buffered) {
        setbuf(output_file, NULL);
    }

    show_time(output_file, 1);
    putc('\n', output_file);

    while (fgets(line, LEN, stdin)) {
        show_time(output_file, 0);
        fprintf(output_file, " %s", line);
    }

    show_time(output_file, 1);
    putc('\n', output_file);
    
    if (output_file_name != NULL) {
        fclose(output_file);
    }

    exit(EXIT_SUCCESS);
} /* main */
