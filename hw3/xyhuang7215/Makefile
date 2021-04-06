PYBIND:=`python3 -m pybind11 --includes`
SO:=`python3-config --extension-suffix`
MKL:=-lblas

_matrix: _matrix.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(PYBIND) _matrix.cpp -o _matrix$(SO) $(MKL)

test:
	python -m pytest

clean:
	rm _matrix$(SO) __pycache__ .pytest_cache -r 