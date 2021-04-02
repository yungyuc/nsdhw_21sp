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

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	-ftree-vectorize -funroll-loops -ffast-math \
	-Wall -Wextra -Werror -shared -fPIC \
	`python3 -m pybind11 --includes` _matrix.cpp \
	-o _matrix`python3-config --extension-suffix` \
	-I${MKLROOT}/include\
	${MKLLINKLINE}

.PHONY: module test clean
module:
	$(CXX) $(CXXFLAGS)

clean:
	rm -rf *.so __pycache__ .pytest_cache performance.txt

test: module
	pytest