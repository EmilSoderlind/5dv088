
CC=gcc
CFLAGS= -std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all: mish

mish: parser.o mish.o execute.o sighant.o
	$(CC) $(CFLAGS) parser.o mish.o execute.o sighant.o -o mish

sighant.o: sighant.c sighant.h
	$(CC) $(CFLAGS) sighant.c -c

execute.o: execute.c execute.h
	$(CC) $(CFLAGS) -c execute.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

mish.o: mish.c mish.h
	$(CC) $(CFLAGS) mish.c -c

valgrind: mish
	valgrind --leak-check=yes --show-reachable=yes ./mish 

clean:
	rm -rf *.o *.out mish