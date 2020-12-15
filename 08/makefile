CFLAGS = -O2 -std=c++17 -pthread
CC = g++ $(CFLAGS)

all: main

test: main
	./thread_pool.out

main: main.cpp thread_pool.hpp
	$(CC) main.cpp -o thread_pool.out

clean:
	rm -rf *.out main