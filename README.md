This is a small collection of small POSIX-specific C programs that
deal with time.

POSIX systems represent the current time as the number of seconds
since 1970-01-01 00:00:00 UTC, a time referred to as "the epoch".

The displayed precision for any of these programs does not imply that
it's actually that accurate.  The time to run the program and display
the output is likely to exceed one microsecond.

`gettimeofday` is a simple wrapper around the
[gettimeofday](http://linuxmanpages.com/man2/gettimeofday.2.php)
function.  It prints the current time, in seconds since the epoch,
to microsecond precision.

`clock_gettime` is a simple wrapper around the
[clock_gettime](http://linuxmanpages.com/man3/clock_gettime.3.php)
function, called with `CLOCK_REALTIME`.  It's similar to
`gettimeofday`, but it prints the current time to nanosecond precision.

`jd` uses the `gettimeofday` function, but it displays the current
[Julian day](http://en.wikipedia.org/wiki/Julian_day),  The value is
printed to 9 decimal places, which gives a precision of 1 billionth
of a day or 86.4 microseconds.

`timer` is more complex.  `timer -h` shows the following help message:

    Usage: timer [options]
        -a : Absolute time (default is zero-based)
        -b : Buffered output (default is unbuffered)
        -h : Help (show this message and exit)
        -z (zero-based) and -u (unbuffered) options are accepted
           for compatibility with older versions.
        -o file : Print output to specified file

It reads from stdin and write to stdout, prepending a timestamp to
each line.  Like the `time` command, it lets you see how long a progmam
takes to execute, but unlike `time` it shows timing information for
each line of ouput.
