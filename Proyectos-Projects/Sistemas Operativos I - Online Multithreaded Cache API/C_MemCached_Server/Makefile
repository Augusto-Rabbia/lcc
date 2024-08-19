CC = gcc -g -ggdb3
CFLAGS += -O3
LDFLAGS += -pthread

all: memcached 

memcached: memcached.o sock.o common.o parser.o tablahash.o dlist.o log.o

clean:
	rm -f memcached *.o

run: all
	./memcached

.deps.mk:
	$(CC) -MM *.c > .deps.mk

.PHONY: all clean run

include .deps.mk
