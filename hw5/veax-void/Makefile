# By veax-void
# Libs
# old blas
#BLAS=-I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

# new blas
MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=$(MKL_LIB_DIR)/libmkl_def.so \
    $(MKL_LIB_DIR)/libmkl_avx2.so \
    $(MKL_LIB_DIR)/libmkl_core.so \
    $(MKL_LIB_DIR)/libmkl_intel_lp64.so \
    $(MKL_LIB_DIR)/libmkl_sequential.so

BLAS=-I /usr/include/mkl $(MKL_LIBS) -lpthread -lm -ldl

# numpy
NUMPYROOT=-I $(shell python -c "import numpy; print(numpy.get_include())")

# pybuild11 
PYBUILD=$(shell python3-config --includes) -Iextern/pybind11/include
#PYBUILD=$$(python3 -m pybind11 --includes) # this one for local machine

# In
SOURCE = _matrix.cpp mod.cpp
# Out
MATRIX=_matrix$(shell python3-config --extension-suffix)


$(MATRIX): ${SOURCE}
	g++ -O3 -Wall -shared -std=c++17 -fPIC $(NUMPYROOT) $(PYBUILD) $(SOURCE) -o $(MATRIX) $(BLAS)

test: $(MATRIX)
	python3 -m pytest -v

clean:
	rm -rf __pycache__ *.so .ipynb*