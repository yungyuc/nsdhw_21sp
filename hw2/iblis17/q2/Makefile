PIP=./venv/bin/pip

CXX=g++

build: _vector.so

_vector.so: vector.o
	$(CXX) -shared vector.o -o _vector.so

vector.o: vector.cpp venv
	$(CXX) -std=c++11 -fPIC $$(./venv/bin/pybind11-config --includes) -c vector.cpp

venv:
	python3 -m venv venv
	$(PIP) install wheel
	$(PIP) install pybind11==2.6.2
	$(PIP) install pytest==6.2.2

clean:
	rm -rf venv *.so __pycache__ *.o

test: build
	./venv/bin/pytest test_vector.py
