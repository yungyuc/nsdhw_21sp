main: main.o line.o
	g++ main.o line.o -o main -std=c++11

run: main
	./main

check: main
	./main > result.txt

clean:
	rm -f main *.o result.txt

main.o: main.cpp line.hpp
	g++ -c main.cpp -o main.o -std=c++11

line.o: line.cpp line.hpp
	g++ -c line.cpp -o line.o -std=c++11
