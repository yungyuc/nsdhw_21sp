PYTHON = python3
CXX = g++
FLAGS = -O3 -Wall -shared -std=c++11 -fPIC

_vector*.so: _vector.cpp
	$(PYTHON) -m pip install pybind11
	$(CXX) $(FLAGS) $$( $(PYTHON) -m pybind11 --includes) _vector.cpp -o _vector$$( $(PYTHON)-config --extension-suffix)

test:
	python3 test.py

clean:
	rm -f _vector *.so
