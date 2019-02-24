# bigfib: Partial bignum package with demo apps

This software implements "big" unsigned integers. These
integers use an array of "buckets" holding pieces of an
integer, so that the integer can contain an arbitrary number
of bits. (This is a standard technique: see for example
[`libgmp`](https://gmplib.org/) for a full-featured fancy
implementation.)

### bigint

The bigint code is in `bigint.h` and `bigint.c`. The only
operations currently implemented are conversion from
hexadecimal strings, printing as a hexidecimal string, and
addition.

### bigadd 

The demo app `bigadd` takes two hexadecimal integers of
arbitrary length as arguments and prints the sum of these
integers in hexadecimal on standard output. For example:

    $ ./bigadd 123456789abcdef 23456789abcde0
    1468acf13579bcf

A Python 3 version of this app is available in `bigadd.py`
for testing and comparison purposes.

    $ python3 bigadd.py 123456789abcdef 23456789abcde0
    1468acf13579bcf

### bigfib

The demo app `bigfib` takes a positive decimal argument *n*
on the command line and prints the *n*-th
[Fibonacci Number](https://en.wikipedia.org/wiki/Fibonacci_number)
in hexadecimal on standard output. The code follows the
convention documented in the link above:

    F[1] = 1
    F[2] = 1
    F[3] = 2
    F[4] = 3
    F[5] = 5
    …

For example:

    $ ./bigfib 200
    338864a5c1caeb07d0ef067cb83df17e395

A Python 3 version of this app is available in `bigfib.py`
for testing and comparison purposes.

    $ python3 bigfib.py 200
    338864a5c1caeb07d0ef067cb83df17e395

