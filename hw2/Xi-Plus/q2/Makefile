PYTHON = python3

# Ref: https://pybind11.readthedocs.io/en/stable/compiling.html#building-manually
_vector*.so: _vector.cpp
	# Hack: It's not pre-installed.
	$(PYTHON) -m pip install pybind11
	g++ -O3 -Wall -shared -std=c++11 -fPIC $$( $(PYTHON) -m pybind11 --includes) _vector.cpp -o _vector$$( $(PYTHON)-config --extension-suffix)

test:
	python3 -m pytest -v

clean:
	rm -f _vector *.so
