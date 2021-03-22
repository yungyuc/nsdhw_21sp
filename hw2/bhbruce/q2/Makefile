SRC=_vector

all:
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3 -m pybind11 --includes) $(SRC).cpp -o $(SRC)$(shell python3-config --extension-suffix)
	echo "compile c++ source code"
test:
	python3 -m pytest -q test_vector.py
clean:
	rm -rdf *.so __pycache__/
