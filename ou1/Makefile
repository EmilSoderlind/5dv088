
CC=gcc
CFLAGS= -std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all: mpasswdsort

mpasswdsort: list.o mpasswdsort.o
	$(CC) $(CFLAGS) mpasswdsort.o list.o -o mpasswdsort

mpasswdsort.o: mpasswdsort.c list.h
	$(CC) $(CFLAGS) -c mpasswdsort.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

valgrind: mpasswdsort
	valgrind --leak-check=yes --show-reachable=yes ./mpasswdsort < /etc/passwd

clean:
	rm -rf *.o *.out mpasswdsort
