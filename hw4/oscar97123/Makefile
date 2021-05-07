.PHONY: test clean

_matrix: _matrix.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $$(python3-config --includes) -Iextern/pybind11/include $< -o $@$$(python3-config --extension-suffix) -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

test: _matrix
	python3 -m pytest

clean:
	rm -rf .pytest*  *.so __pycache__ performance.txt
