This is a collection of small POSIX-specific C programs that deal with time.

POSIX systems represent the current time as the number of seconds
since 1970-01-01 00:00:00 UTC, a time referred to as "the epoch".

The displayed precision for any of these programs does not imply that
it's actually that accurate.  The time to run the program and display
the output is likely to exceed one microsecond.

`gettimeofday` is a simple wrapper around the
[`gettimeofday`](http://linuxmanpages.com/man2/gettimeofday.2.php)
function.  It prints the current time, in seconds since the epoch,
to microsecond precision.

`clock_gettime` is a wrapper around the
[`clock_gettime`](http://linuxmanpages.com/man3/clock_gettime.3.php)
function.  With no arguments, it shows the result of calling
`clock_gettime`, called with `CLOCK_REALTIME`; this is similar to
`gettimeofday`, but it prints the current time to nanosecond precision.
Other options are as shown with `-h`:

```
Usage: clock_gettime [options]
  -r    CLOCK_REALTIME, system-wide real-time clock (default)
  -T    CLOCK_TAI, International Atomic Time (no leap seconds)
  -m    CLOCK_MONOTONIC, monotonic time since some unspecified starting point
  -M    CLOCK_MONOTONIC_RAW, hardware-based timer with no ntp adjustments
  -p    CLOCK_PROCESS_CPUTIME_ID, high-resolution per-process timer from the CPU
  -t    CLOCK_THREAD_CPUTIME_ID, thread-specific CPU-time clock
(-p and -t are not particularly meaningful in this context.)
  -R    Show resolution (calls clock_getres), may be followed by other options
Multiple arguments may be given
```

`jd` uses the `gettimeofday` function, but it displays the current
[Julian date](http://en.wikipedia.org/wiki/Julian_day).
`jd -m` prints the [Modified Julian
Date](http://en.wikipedia.org/wiki/Julian_day#Alternatives).
The value is printed to 9 decimal places, which gives a precision
of 1 billionth of a day or 86.4 microseconds.  (This doesn't imply
*accuracy* to that level.)

`timer` is more complex.  `timer -h` shows the following help message:
```
Usage: ./timer [options]
    -a : Absolute time (default is zero-based)
    -A : Absolute local time, human-readable
    -U : Absolute UTC time, human-readable
    -r : Show time interval relative to previous line
    -b : Buffered output (default is unbuffered)
    -h : Help (show this message and exit)
    -z : Zero-based time (redundant, this is the default)
    -u : Unbuffered output (redundant, this is the default)
    -o file : Print output to specified file
```

It reads from stdin and writes to stdout, prepending a timestamp to
each line.  Like the `time` command, it lets you see how long a progmam
takes to execute, but unlike `time` it shows timing information for
each line of ouput (and doesn't show CPU time).

`usleep-until` sleeps until the next multiple (in seconds since
the UNIX epoch) of the specified number of seconds.  It uses
`gettimeofday()` and `usleep()` to provide microsecond resolution
(subject to the overhead of running the program).  Note that
these functions are considered obsolete.  Consider using the
`nanosleep-until` command instead.
Optionally, it executes a command
given on the command line after the `seconds` argument.

```
Usage: usleep-until seconds [command [args...]]
```

`nanosleep-until` sleeps until the next multiple (in seconds
since the UNIX epoch) of the specified number of seconds.  It uses
`clock_gettime()` and `nanosleep()` to provide nanosecond resolution
(subject to the overhead of running the program).  Optionally, it
executes a command given on the command line after the `seconds`
argument.

The `seconds` argument is a positive integer.  Since `nanosleep-until`
uses `atoi` to parse its argument, any fractional part is quietly
ignored; `nanosleep-until 1.5` acts like `nanosleep 1`.  (This is a
bug, which I'll fix eventually.)

```
Usage: nanosleep-until seconds [command [args...]]
```
