CC = g++ $(CFLAGS)
CFLAGS = -O2 -std=c++17

all: main

test: main
	./main.out

main: main.cpp serializer.hpp
	$(CC) main.cpp -o main.out

clean:
	rm -rf *.o main
