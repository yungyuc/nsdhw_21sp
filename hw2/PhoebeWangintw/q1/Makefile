all: line_exec.cpp
	g++ -std=c++11 line_exec.cpp -o exec 
run: exec
	./exec
check: exec
	./exec > output.txt
clean:
	rm ./exec output.txt
