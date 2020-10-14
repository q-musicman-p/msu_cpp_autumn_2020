CC = g++ $(CFLAGS)
CFLAGS = -g -O2 -std=c++17

all: allocator

allocator: main.o
	$(CC) -o Allocator main.o -lm

main.o: main.cpp allocator.h
	$(CC) -c main.cpp

test: main.o
	$(CC) -o test main.o -lm
	./test

clean:
	rm -rf *.o allocator test
