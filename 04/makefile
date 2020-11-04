all: main

test: main
	./main.out

main: main.o bignumber.o
	g++ main.o bignumber.o -o main.out

main.o: main.cpp bignumber.hpp
	g++ -c main.cpp

bignumber.o: bignumber.cpp bignumber.hpp
	g++ -c bignumber.cpp

clean:
	rm -rf main *.o
