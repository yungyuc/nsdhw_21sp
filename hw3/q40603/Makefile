FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	${MKLROOT}/lib/libmkl_avx2.${MKLEXT} \
	${MKLROOT}/lib/libmkl_avx512.${MKLEXT} \
	${MKLROOT}/lib/libmkl_def.${MKLEXT} \
	-lpthread -lm -ldl

CXX=g++
CXXFLAGS=-std=c++11 -shared -O3 -Wall -Wextra -Werror -fPIC  
PYINCLUDE:=`python3-config --includes`
PYIEXTERN:=-Iextern/pybind11/include
PYSUFFIX:=`python3-config --extension-suffix`
	

.PHONY: module test clean
_matrix: _matrix.cpp
	$(CXX) $(CXXFLAGS) $(PYINCLUDE) $(PYIEXTERN) $< -o $@$(PYSUFFIX)

clean:
	rm -rf *.so __pycache__ .pytest_cache performance.txt

test: _matrix
	python3 -m pytest -v