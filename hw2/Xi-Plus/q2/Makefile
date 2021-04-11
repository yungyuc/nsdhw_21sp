PYTHON = python3

# Ref: https://pybind11.readthedocs.io/en/stable/compiling.html#building-manually
_vector*.so: _vector.cpp
	# Ref: https://pybind11.readthedocs.io/en/stable/basics.html#creating-bindings-for-a-simple-function
	# If you used Include as a submodule to get the pybind11 source, then use $(python3-config --includes) -Iextern/pybind11/include
	g++ -O3 -Wall -shared -std=c++11 -fPIC $$( $(PYTHON)-config --includes) -Iextern/pybind11/include _vector.cpp -o _vector$$( $(PYTHON)-config --extension-suffix)

test:
	python3 -m pytest -v

clean:
	rm -f _vector *.so
