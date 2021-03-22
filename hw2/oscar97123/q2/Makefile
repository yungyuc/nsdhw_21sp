.PHONY: test clean

_vector: _vector.cpp
	c++ -O3 -Wall -shared -std=c++11 -fPIC $$(python3 -m pybind11 --includes) $< -o $@$$(python3-config --extension-suffix)

test: _vector
	pytest

clean:
	rm -rf .pytest*  *.so __pycache__
