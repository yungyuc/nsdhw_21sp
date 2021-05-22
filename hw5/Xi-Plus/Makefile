# Ref: https://stackoverflow.com/a/37866377/13509181
BLAS=-I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas
MATRIX=_matrix$(shell python3-config --extension-suffix)

# Ref: https://pybind11.readthedocs.io/en/stable/compiling.html#building-manually
# Ref: https://pybind11.readthedocs.io/en/stable/basics.html#creating-bindings-for-a-simple-function
$(MATRIX): matrix.cpp matrix.hpp mod.cpp
	g++ -O3 -Wall -shared -std=c++17 -fPIC $(shell python3-config --includes) -Iextern/pybind11/include matrix.cpp mod.cpp -o $(MATRIX) $(BLAS)

test: $(MATRIX)
	python3 -m pytest -v

clean:
	rm -rf __pycache__ *.so
