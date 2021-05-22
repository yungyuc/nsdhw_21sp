PIP=$(PWD)/venv/bin/pip

CXX=g++
MKLROOT=$(PWD)/venv

build: _matrix.so

_matrix.so: mod.o
	$(CXX) -shared mod.o -o _matrix.so \
	    -Wl,--start-group \
	    $(MKLROOT)/lib/libmkl_intel_ilp64.a \
	    $(MKLROOT)/lib/libmkl_sequential.a \
	    $(MKLROOT)/lib/libmkl_core.a \
	    $(MKLROOT)/lib/libmkl_blacs_intelmpi_ilp64.a \
	    -Wl,--end-group -lpthread -lm -ldl

mod.o: mod.cpp matrix.hpp venv
	$(CXX) -m64 -std=c++17 -fPIC $$(./venv/bin/pybind11-config --includes) \
	    -I$$(python -c 'import numpy; print(numpy.get_include())') \
	    -I./venv/include -DMKL_ILP64 -c mod.cpp

venv:
	python3 -m venv venv
	$(PIP) install wheel
	$(PIP) install pybind11==2.6.2
	$(PIP) install pytest==6.2.2
	$(PIP) install mkl-devel==2021.2.0 mkl-static==2021.2.0
	$(PIP) install numpy==1.19.5

clean:
	rm -rf venv *.so __pycache__ *.o

test: build
	./venv/bin/pytest test_matrix.py
	cat ./performance.txt
