CXX = g++
FLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PYBUILD = `python3-config --includes` -Iextern/pybind11/include
PYSUFFIX = `python3-config --extension-suffix`

.PHONY: package
package: package.cpp
	$(CXX) $(FLAGS) $(PYBUILD)  $< -o $@$(PYSUFFIX)

.PHONY: clean
clean: 
	rm -f package *.so

.PHONY: test
test:
	python3 test.py