.PHONY: test clean

_vector: _vector.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $$(python3-config --includes) -Iextern/pybind11/include $< -o $@$$(python3-config --extension-suffix)

test: _vector
	pytest

clean:
	rm -rf .pytest*  *.so __pycache__
