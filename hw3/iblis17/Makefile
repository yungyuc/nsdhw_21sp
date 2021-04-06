PIP=./venv/bin/pip

CXX=g++

build: _matrix.so

_matrix.so: matrix.o
	$(CXX) -shared matrix.o -o _matrix.so

matrix.o: matrix.cpp venv
	$(CXX) -std=c++17 -fPIC $$(./venv/bin/pybind11-config --includes) -c matrix.cpp

venv:
	python3 -m venv venv
	$(PIP) install wheel
	$(PIP) install pybind11==2.6.2
	$(PIP) install pytest==6.2.2
	$(PIP) install mkl-devel==2021.2.0

clean:
	rm -rf venv *.so __pycache__ *.o

test: build
	./venv/bin/pytest test_matrix.py
