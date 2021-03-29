CXX = g++
cflag = -Wall -Wextra -Werror

.PHONY: main
main: main.o line.o
	$(CXX) $^ -o $@ $(cflag)

.PHONY: run
run: main
	./main

.PHONY: check
check: main
	./main >result.txt 2>&1

.PHONY: clean
clean: 
	rm -f main *.o result.txt

%.o: %.cpp line.hpp
	$(CXX) -c $< -o $@ $(cflag)