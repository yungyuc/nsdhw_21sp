PYBIND:=`python3 -m pybind11 --includes`
SO:=`python3-config --extension-suffix`

_vector: _vector.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(PYBIND) _vector.cpp -o _vector$(SO)

test:
	pytest

clean:
	rm _vector$(SO) __pycache__ .pytest_cache -r