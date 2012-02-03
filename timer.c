#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/time.h>

static int absolute = 0;
static int buffered = 0;
static struct timeval t0;
static int t0_initialized = 0;

/* ------------------------------------------------------------------ */

static struct timeval diff(struct timeval left, struct timeval right)
{
    struct timeval result;

    result.tv_sec = left.tv_sec - right.tv_sec;
    result.tv_usec = left.tv_usec - right.tv_usec;
    if (result.tv_usec < 0) {
        result.tv_sec --;
        result.tv_usec += 1000000;
    }
    return result;
}

/* ------------------------------------------------------------------ */

static void show_timeval(FILE *stream, struct timeval tv)
{
    fprintf(stream, absolute ? "%10ld.%06ld" : "%8ld.%06ld",
                    (long)tv.tv_sec,
                    (long)tv.tv_usec);
}

/* ------------------------------------------------------------------ */

static void show_time(FILE *stream, int real_time_too)
{
    struct timeval tv;
    int result = gettimeofday(&tv, NULL);
    if (result == 0) {
        if (! t0_initialized) {
            t0 = tv;
            t0_initialized = 1;
        }
        if (absolute) {
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
    }
    else {
        fprintf(stream, "---------.------");
    }
} /* show_time */

/* ------------------------------------------------------------------ */

static void usage(char *prog_name, char *message)
{
    if (message != NULL) {
        puts(message);
    }
    printf("Usage: %s [options]\n", prog_name);
    puts("    -a : Absolute time (default is zero-based)");
    puts("    -b : Buffered output (default is unbuffered)");
    puts("    -h : Help (show this message and exit)");
    puts("    -z (zero-based) and -u (unbuffered) options are accepted");
    puts("       for compatibility with older versions.");
    puts("    -o file : Print output to specified file");
    exit(EXIT_SUCCESS);
} /* usage */

/* ------------------------------------------------------------------ */

int main(int argc, char **argv)
{
#define LEN 8192

    char line[LEN];
    int i;
    char *output_file_name = NULL;
    FILE *output_file = stdout;

    for (i = 1; i < argc; i ++) {
        if (strcmp(argv[i], "-a") == 0) {
            absolute = 1;
        }
        else if (strcmp(argv[i], "-z") == 0) {
            absolute = 0;
        }
        else if (strcmp(argv[i], "-b") == 0) {
            buffered = 1;
        }
        else if (strcmp(argv[i], "-u") == 0) {
            buffered = 0;
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
