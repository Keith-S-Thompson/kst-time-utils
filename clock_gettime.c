#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
/* #include <sys/time.h> */

int main(void) {
    struct timespec ts = { 0, 0 };

    if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
	printf("%ld.%09ld\n", (long)ts.tv_sec, (long)ts.tv_nsec);
	exit(EXIT_SUCCESS);
    }
    else {
	perror("clock_gettime");
	exit(EXIT_FAILURE);
    }
} /* main */
