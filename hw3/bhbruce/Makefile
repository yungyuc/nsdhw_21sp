SRC=_matrix
BLAS=-I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

test:
	# g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3 -m pybind11 --includes) $(SRC).cpp -o _matrix$(shell python3-config --extension-suffix)
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3-config --includes) -Iextern/pybind11/include $(SRC).cpp -o $(SRC)$(shell python3-config --extension-suffix) $(BLAS)
	python3 -m pytest -q test_matrix.py

clean:
	rm -rdf *.so __pycache__/ performance.txt
