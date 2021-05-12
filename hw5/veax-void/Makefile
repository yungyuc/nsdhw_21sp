FLAGS ?=

MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=

MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed

NUMPYROOT := $(shell python -c "import numpy; print(numpy.get_include())")

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	-shared \
	-I${MKLROOT}/include \
	-I${NUMPYROOT} \
	#${MKLLINKLINE} \
	${FLAGS}

CXXGCOMPILE := -std=c++17 -O3 \
	-I${NUMPYROOT}

CXX = g++
PYBINDFLAGS := \
	-fPIC `python3 -m pybind11 --includes`

.PHONY: _matlink
_matlink: matrix mod.o matrix.o
	$(CXX) -shared -o _matrix`python3-config --extension-suffix` mod.o matrix.o
matrix: mod.cpp
	${CXX} $< ${CXXFLAGS} $(PYBINDFLAGS) -o $@`python3-config --extension-suffix`

mod.o: mod.cpp
	$(CXX) -c $< ${CXXGCOMPILE} $(PYBINDFLAGS) -o $@ -fPIC

matrix.o: matrix.cpp matrix.hpp Makefile
	$(CXX) -c $< ${CXXGCOMPILE} -I${MKLROOT}/include -o $@ -fPIC

.PHONY: clean
clean: 
	rm -rf matrix *.so *.o performance.txt __pycache__/ .pytest_cache/ .cache/

.PHONY: test
test: clean _matlink Makefile
	py.test -v -l