PYTHON = python3
BLAS=-I/opt/intel/mkl/include -L/opt/intel/mkl/lib/intel64 -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -lpthread
MATRIX=_matrix$$( $(PYTHON)-config --extension-suffix)

# Ref: https://pybind11.readthedocs.io/en/stable/compiling.html#building-manually
$(MATRIX): _matrix.cpp
	# Ref: https://pybind11.readthedocs.io/en/stable/basics.html#creating-bindings-for-a-simple-function
	# If you used Include as a submodule to get the pybind11 source, then use $(python3-config --includes) -Iextern/pybind11/include
	g++ -O3 -Wall -shared -std=c++11 -fPIC $$( $(PYTHON)-config --includes) -Iextern/pybind11/include _matrix.cpp -o $(MATRIX) $(BLAS)

test: $(MATRIX)
	python3 -m pytest -v

clean:
	rm -rf __pycache__ *.so
