CXX = g++

MODULE = _matrix
SOFILE = $(MODULE)$(shell python3-config --extension-suffix)

.PHONY: all test clean
all: $(SOFILE)

$(SOFILE): matrix.cc matrix.h
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3-config --includes) -Iextern/pybind11/include _matrix.cpp -o $(SOFILE) 

test: $(SOFILE)
	pytest

clean:
	rm -rf $(SOFILE) __pycache__ .pytest_cache performance.txt
	
