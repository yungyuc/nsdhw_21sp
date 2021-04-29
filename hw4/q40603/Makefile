# Ref: https://stackoverflow.com/a/37866377/13509181
BLAS=-I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas
MATRIX=_matrix$(shell python3-config --extension-suffix)

# Ref: https://pybind11.readthedocs.io/en/stable/compiling.html#building-manually
# Ref: https://pybind11.readthedocs.io/en/stable/basics.html#creating-bindings-for-a-simple-function
$(MATRIX): _matrix.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3-config --includes) -Iextern/pybind11/include _matrix.cpp -o $(MATRIX) $(BLAS)

clean:
	rm -rf *.so __pycache__ .pytest_cache

test: _matrix
	../validate.py