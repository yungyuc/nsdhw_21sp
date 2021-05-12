BLAS=-I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

MATRIX=_matrix$(shell python3-config --extension-suffix)

$(MATRIX): _matrix.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3-config --includes) -Iextern/pybind11/include mod.cpp -o $(MATRIX) $(BLAS)

test: $(MATRIX)
	python3 -m pytest -v

clean:
	rm -rf __pycache__ *.so