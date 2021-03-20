all: line_exec.cpp
	g++ -std=c++11 line_exec.cpp -o exec 
run: exec
	./exec
check: exec
	./exec > result.txt
clean:
	rm ./exec result.txt
