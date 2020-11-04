CC = g++ $(CFLAGS)
CFLAGS = -O2 -std=c++17

all: main

test: main
	./main.out

main: main.o bignumber.o
	$(CC) main.o bignumber.o -o main.out

main.o: main.cpp bignumber.hpp
	$(CC) -c main.cpp

bignumber.o: bignumber.cpp bignumber.hpp
	$(CC) -c bignumber.cpp

clean:
	rm -rf main *.o
