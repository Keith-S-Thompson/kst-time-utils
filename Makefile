PROGRAMS = clock_gettime gettimeofday jd timer usleep-until

CC = gcc -O3 -std=c99

all:	        $(PROGRAMS)

clock_gettime: clock_gettime.c
	$(CC) clock_gettime.c -lrt -o clock_gettime

test:	all
	./clock_gettime
	./gettimeofday
	./jd
	( echo hello ; sleep 1 ; echo good-bye ) | ./timer
	./usleep-until 1

clean:
	- rm -f *.o $(PROGRAMS)
