CC = g++ $(CFLAGS)
CFLAGS = -g -O2 -std=c++17

all: matrix

matrix: main.o
	$(CC) -o matrix main.o -lm

main.o: main.cpp matrix.hpp
	$(CC) -c main.cpp

test: matrix
	./matrix

clean:
	rm -rf *.o matrix test