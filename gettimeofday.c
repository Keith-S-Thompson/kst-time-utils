#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>

int main(void) {
    struct timeval tv = { 0, 0 };

    if (gettimeofday(&tv, NULL) == 0) {
        printf("%ld.%06ld\n", (long)tv.tv_sec, (long)tv.tv_usec);
        exit(EXIT_SUCCESS);
    }
    else {
        perror("gettimeofday");
        exit(EXIT_FAILURE);
    }
} /* main */
