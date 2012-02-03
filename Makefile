PROGRAMS = clock_gettime gettimeofday jd timer

CC = gcc -O3 -lrt
# "-lrt" is needed by clock_gettime

all:	        $(PROGRAMS)

test:	all
	./clock_gettime
	./gettimeofday
	./jd
	( echo hello ; sleep 1 ; echo good-bye ) | ./timer

clean:
	- rm -f *.o $(PROGRAMS)
