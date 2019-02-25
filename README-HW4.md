# PSU CS 201 HW 4: Inline Assembly
Bart Massey  
Winter 2019

In this assignment you will learn to:

* Represent large numbers as a data structure.
* Profile and benchmark a long-running program.
* Read, write, debug and test inline assembly code in C
  programs.

## Background: Big Numbers

The *n*-th
[Fibonacci Number](https://en.wikipedia.org/wiki/Fibonacci_number)
needs *2^n* bits for its representation. These kind of large
integers come up all the time, so there's a standard plan
for representing them: use an array of normal-sized
integers. We'll call those normal-sized integers "buckets".

So imagine that we want to represent a 256-bit number. Let's
represent it as an array of 32 `uint8_t`s. We will put the
least significant bits closest to the start of the array,
and just pack the bits in from there. To represent 0x85 in
this way we could say

    uint8_t a[32] = {0x85, 0, ..};

We could represent 0x583 as

    uint8_t b[32] = {0x83, 0x5, 0, ..};

To get the sum of these numbers, we just sum up the
individual buckets pairwise — but remember to carry the 1
from the previous bucket when needed!

    uint8_t c[32] = {0x08, 0x06, 0, ..};

Hey presto, big number addition!

We probably want to allow the size to be flexible, and
remember what it is separately. We also probably want to use
the biggest buckets the CPU can handle: this will minimize
the number of bucket adds in a big-number add.

## The Codebase

I've written part of a bigint package: it lives in
`bigint.c` and `bigint.h`. Look at `bucket_t` and `struct
bigint` in `bigint.h` for the definitions we are using.

Note that `bigint_read()` and `bigint_print()` work in hex,
not decimal. This makes things easier for me to implement
and you to work with.

This is a "function-style" interface: no bigint will be
freed or modified unless you do it yourself. `bigint_add()`
creates a new bigint for the sum.

Examine `bigadd.c` and `bigfib.c` carefully to understand
how the interface works. Try them out on sample inputs;
compare with the Python 3 versions to see that they give the
same answers.

Now examine `bigint.c` carefully. In particular, take a look
at `bigint_add()`, which is the most complicated piece. Note
that it uses `ADDC()`, which is a macro for one of several
functions that add two buckets and a carry to produce a sum
and a new carry.

Examine the `Makefile` carefully. It has a bunch of options
you can choose for how to compile the program. We'll be
using most of them.

## Benchmarking and Profiling

Please take notes as you do these steps, and turn in them in
in the `WRITEUP` described below.

1. Try

    time ./bigfib 100000 >/dev/null

   You should get a time somewhere in the 0.25-2 second
   range.

2. Now try

    time python3 bigfib.py 100000 >/dev/null

   You should get a similar but faster time.

   But this is bad!  Why should C be slower than Python — it's
   normally about 40 times faster. Well, a couple of
   reasons. Most importantly, the part of Python that adds big
   numbers is written in C, so it's as fast as C. But also, our
   C code has a performance problem.

3. Turn on the profiling flags in the `Makefile`, `make clean`,
   `make`, and run `bigfib` as before. Now say `gprof
   bigfib gmon.out`.

   Most of the program has been optimized away, but note
   that we spend a lot of time in `addc_c()`. It turns out
   that it's hard to know when a carry out of a bucket
   occurs: the way we are doing it from C code has a bunch
   of jumps in it, and as we have learned jumps are really
   expensive.

4. Let's start by turning off profiling in the `Makefile`
   and turning on `ADDC_ASM_JMP`. This enable a hand-written
   assembly version of `ADDC` that basically does the same
   thing as the C code. Benchmark this as above.

   You should find that this version is almost exactly the
   same speed as the C version. What a surprise:
   hand-written assembly doesn't magically make programs
   faster.

5. I wonder if the assembly version of `ADDC` is even
   correct? Run `./unit`, which should indicated that
   several tests of `ADDC` have all passed. You might want
   to look at `unit.c` and consider adding more tests.

6. Let's write some assembly code for a faster version of
   `ADDC`! This is the next part of the assignment.

## Write Some Inline Assembly

The big problem with the assembly version of `ADDC` is that
it has jumps in it still. You're going to get rid of the
jumps using the x86-64 `adc` instruction.

Make a copy of `addc_asm_jmp()` in `bigint.c` and call it
`addc_asm_adc()`. Wire it up like the other two `ADDC`
functions.

Now rip out the machine instructions in the `asm()` in
`addc_asm_adc()` and replace them with your own
instructions.

Oof. That sounds hard. Let's talk about it. Our goal is to
get the passed-in carry-in to set the x86-64 carry flag, do
an `adc`, and pass the resulting carry flag back out to do
the carry-out.

Here's the instructions I used:

* [`adcq`](https://www.felixcloutier.com/x86/adc): The add
  with carry instruction. It adds its operands and the carry
  flag, and sets the carry flag if and only if there is a
  carry out of the addition.

* [`cmc`](https://www.felixcloutier.com/x86/cmc): Funny
  little machine instruction that negates the carry flag.

* [`cmpq`](https://www.felixcloutier.com/x86/cmp): A
  standard compare (fake-subtract) instruction, which sets
  flags. Note that it sets the carry flag when it tries to
  "borrow" a one bit.

* [`movq`](https://www.felixcloutier.com/x86/movq): The
  standard "move" (copy) instruction. Note that it does not
  touch the flags.

* [`setc`](https://www.felixcloutier.com/x86/setcc): Writes
  the current value of the carry flag into a *byte* register
  or *byte* of memory.

These five instructions, used properly in the right order,
are enough to do an add with carry. (You could also use
`pushf` and `popf`, but this turns out to be slower and
harder.)

Write your add-with-carry inline asm. Use the `unit` test
program to check it. When it passes unit tests, run `bigfib`
to see if it gives the same answer as the Python version for
a large Fibonacci Number. Use `gdb` for machine-level
debugging if you have trouble getting your code right.

Above all, ask on `#help` on the Slack as you run into hard
problems. There may be some.

When you have everything working, compare the run time of
`bigfib` using your new `ADDC` against `bigfib` using the
old `ADDC` and with `Python`. I found that my new one was
substantially faster.

## Hints

* The documentation for GCC inline assembly starts
  [here](https://gcc.gnu.org/onlinedocs/gcc-4.4.4/gcc/Extended-Asm.html#Extended-Asm).

* There are several tutorials linked from the lecture notes
  on using GDB for machine-level debugging.

* The only thing you might have to change about the C code
  of your `addc_asm_adc()` is the types of some of the
  variables. (Hint: which instruction listed above requires
  an 8-bit operand?)

## Writeup

Your submission *must* include a writeup in a text file
named `WRITEUP.txt` or `WRITEUP.md`. The writeup should
contain your name and a description of anything interesting
about your program or its development.

## Source Control

For this assignment, you are not required to use source
control. However, I strongly suggest putting all your
project files under Git control: it will avoid
backup-file messes and give you a good history of work.

## Submission

You should submit your materials (program, Makefile,
writeup) to the course website as a ZIP archive called
`bigfib.zip`. The ZIP archive should unzip into a directory
named `bigfib`: use `zip -r bigfib.zip bigfib` to make the
archive and `unzip -t bigfib.zip` to check it. Use the
assignment submission interface on the course website to
upload your archive.
