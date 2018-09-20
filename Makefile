
CC=gcc
CFLAGS=

all:a2
rogram:a2.o
a2.o: a2.c

clean:
	rm -f a2 a2.o
run: a2
	./a2
