FLAGS ?=
BLAS=-I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas
MATRIX=_matrix$(shell python3-config --extension-suffix)

UNAME_S := $(shell uname -s)
NUMPY_PATH := $(shell python3 -c "import numpy;print(numpy.get_include())")
ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLROOT ?= ${HOME}/opt/conda
#MKLROOT ?= /home/kctsai/miniconda3/envs/demo
#MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	${MKLROOT}/lib/libmkl_avx2.${MKLEXT} \
	${MKLROOT}/lib/libmkl_def.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	-ftree-vectorize -funroll-loops -ffast-math \
	-fPIC $(shell python3-config --includes) \
	-Iextern/pybind11/include \
	-Wall -shared \
	-I${NUMPY_PATH} \
	-I ./ \
	# -I${MKLROOT}/include \
	# ${MKLLINKLINE}



.PHONY: default
_matrix: matrix.hpp
	${CXX} ./export.cpp ${CXXFLAGS} -o $(MATRIX) $(BLAS)

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS} *.so *.txt __pycache__

.PHONY: test
test:
	make;pytest -v