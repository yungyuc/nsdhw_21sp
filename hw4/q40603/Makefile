BLAS=-I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

CXX = g++

ifneq ($(wildcard /opt/intel/mkl/.),)
	INCLUDE_DIRS = /opt/intel/mkl/include
	MKL_DIR = /opt/intel/mkl/lib/intel64
else
	INCLUDE_DIRS = ${HOME}/opt/conda/include
	MKL_DIR = ${HOME}/opt/conda/lib
endif

CXXFLAGS = -O3 -Wl,--no-as-needed -shared -std=c++11 -fPIC -I$(INCLUDE_DIRS) -ldl -lpthread -lm
CXXFLAGS += $(MKL_DIR)/libmkl_def.so
CXXFLAGS += $(MKL_DIR)/libmkl_avx2.so
CXXFLAGS += $(MKL_DIR)/libmkl_core.so
CXXFLAGS += $(MKL_DIR)/libmkl_intel_lp64.so
CXXFLAGS += $(MKL_DIR)/libmkl_sequential.so

PYINCLUDE:=`python3-config --includes`
PYIEXTERN:=-Iextern/pybind11/include
PYSUFFIX:=`python3-config --extension-suffix`
	

.PHONY: module test clean
_matrix: _matrix.cpp
	$(CXX) $(CXXFLAGS) $(PYINCLUDE) $(PYIEXTERN) $< -o $@$(PYSUFFIX) $(BLAS)

clean:
	rm -rf *.so __pycache__ .pytest_cache

test: _matrix
	../validate.py