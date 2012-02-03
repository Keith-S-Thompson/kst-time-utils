#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(void)
{
    struct timeval tp;
    long double unix_time;
    long double jd;
    if (gettimeofday(&tp, NULL) != 0) {
	perror("gettimeofday");
	exit(EXIT_FAILURE);
    }
    unix_time = tp.tv_sec + tp.tv_usec/1000000.0;
    jd = unix_time / 86400.0 + 2440587.5;
    printf("%.9Lf\n", jd);
    return 0;
}
