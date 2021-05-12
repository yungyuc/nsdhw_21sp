.PHONY: clean test _matrix

ifneq ("$(wildcard /opt/intel)","")
    INCLUDE_DIRS = /opt/intel/compilers_and_libraries_2020.4.304/linux/mkl/include/	
	MKLROOT = /opt/intel/compilers_and_libraries_2020.4.304/linux/mkl/lib/intel64_lin/
else
    INCLUDE_DIRS = ${HOME}/opt/conda/include	
	MKLROOT = ${HOME}/opt/conda/lib
endif

MKLLINKLINE := \
	${MKLROOT}/libmkl_def.so \
	${MKLROOT}/libmkl_intel_lp64.so \
	${MKLROOT}/libmkl_sequential.so \
	${MKLROOT}/libmkl_core.so \
	${MKLROOT}/libmkl_avx2.so 

SOURCE = matrix.cpp mod.cpp
BINDLIBRARY = _matrix`python3-config --extension-suffix` 

CXX = g++
CXXFLAGS := -Wl,--no-as-needed -std=c++17 -O3 -g -m64 -Wall -Wextra -shared -lpthread -lm -ldl -fPIC
PYBIND := $(shell python3-config --includes) -Iextern/pybind11/include

${BINDLIBRARY}: ${SOURCE}
	${CXX} ${CXXFLAGS} -I${INCLUDE_DIRS} -I./ ${MKLLINKLINE} ${PYBIND} ${SOURCE} -o ${BINDLIBRARY} 

clean:
	rm -rf *.o *.dSYM/ ${BINS} *.so .pytest_cache __pycache__/ performance.txt .cache/

test: _matrix
	python3 -m pytest -v