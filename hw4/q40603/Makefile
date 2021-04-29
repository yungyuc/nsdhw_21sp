BLAS=-I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

CXX=g++
CXXFLAGS=-std=c++11 -shared -O3 -Wall -Wextra -Werror -fPIC 
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