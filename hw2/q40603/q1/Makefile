CXX=clang++
CXXFLAGS=-std=c++11 -Wall -Wextra -Werror

.PHONY: run check clean

line: line.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

run: line
	./line

check: line
	./line >result.txt 2>&1

clean: 
	rm -f line result.txt
