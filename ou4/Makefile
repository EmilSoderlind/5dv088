
CC=gcc
CFLAGS= -pthread -g -std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all: mfind

mfind: queue.o mfind.o
	$(CC) $(CFLAGS) mfind.o queue.o -o mfind

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) queue.c -c

mfind.o: mfind.c mfind.h
	$(CC) $(CFLAGS) mfind.c -c

valgrind: all
	valgrind --leak-check=yes --show-reachable=yes ./mfind .. i

clean:
	rm -rf *.o *.out mfind