CC = g++ $(CFLAGS)
CFLAGS = -g -O2

all: allocator

allocator: main.o
	$(CC) -o Allocator main.o -lm

main.o: main.cpp allocator.h
	$(CC) -c main.cpp

clean:
	rm -rf *.o allocator
