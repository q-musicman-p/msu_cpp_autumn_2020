CC = g++ $(CFLAGS)
CFLAGS = -g -O2 -std=c++17

all: parser

parser: main.o
	$(CC) -o parser main.o -lm

main.o: main.cpp parser.hpp
	$(CC) -c main.cpp

test: parser
	./parser

clean:
	rm -rf *.o parser test