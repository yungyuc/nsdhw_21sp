CXXFLAGS=--std=c++11

build: line

line: line.o
	g++ -o line line.o

run: build
	./line

check: build
	./line > ./result.txt

clean:
	rm -f line result.txt line.o
