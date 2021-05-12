.PHONY: clean test _matrix

INCLUDE_DIRS=${HOME}/opt/conda/include
MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu

LD_PRELOAD := \
	${MKL_LIB_DIR}/libmkl_def.so \
	${MKL_LIB_DIR}/libmkl_intel_lp64.so \
	${MKL_LIB_DIR}/libmkl_sequential.so \
	${MKL_LIB_DIR}/libmkl_core.so \
	${MKL_LIB_DIR}/libmkl_avx2.so 

SOURCE = matrix.cpp mod.cpp
BINDLIBRARY = _matrix`python3-config --extension-suffix` 

CXX = g++
CXXFLAGS := -Wl,--no-as-needed -std=c++11 -O3 -g -m64 -Wall -Wextra -shared -lpthread -lm -ldl -fPIC
PYBIND := $(shell python3-config --includes) -Iextern/pybind11/include
# PYBIND := $(python3 -m pybind11 --includes)

${BINDLIBRARY}: ${SOURCE}
	${CXX} ${CXXFLAGS} -I${INCLUDE_DIRS} -I./ ${LD_PRELOAD} ${PYBIND} ${SOURCE} -o ${BINDLIBRARY} 

clean:
	rm -rf *.o *.dSYM/ ${BINS} *.so .pytest_cache __pycache__/ performance.txt .cache/

test: _matrix
	python3 -m pytest -v