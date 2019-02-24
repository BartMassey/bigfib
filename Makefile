CC = gcc
CDEBUG = -g
CFLAGS = -Wall -Wextra -Werror $(CDEBUG)

OBJS = bigfib.o bigint.o bigadd.o
BINS = bigfib bigadd

all: $(BINS)

bigfib: bigfib.o bigint.o
	$(CC) $(CFLAGS) -o bigfib bigfib.o bigint.o

bigadd: bigadd.o bigint.o
	$(CC) $(CFLAGS) -o bigadd bigadd.o bigint.o

$(OBJS): bigint.h

clean:
	-rm -f $(OBJS) $(BINS) *.s gmon.out

.c.s:
	$(CC) $(CFLAGS) -S $*.c
