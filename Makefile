CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

OBJS = bigfib.o bigint.o

bigfib: $(OBJS)
	$(CC) $(CFLAGS) -o bigfib $(OBJS)

clean:
	-rm -f $(OBJS) bigfib
