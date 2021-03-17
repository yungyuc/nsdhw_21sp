CXX = g++
cflag = -Wall -Wextra -Werror

.PHONY: package
package: package.cpp
	$(CXX) -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` $< -o $@`python3-config --extension-suffix`

.PHONY: clean
clean: 
	rm -f package *.so

.PHONY: test
test:
	python3 test.py