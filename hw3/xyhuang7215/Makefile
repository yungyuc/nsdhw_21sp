PYBIND:=`python3-config --includes`
SO:=`python3-config --extension-suffix`
MKL:=-lblas

_matrix: _matrix.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(PYBIND) _matrix.cpp -o _matrix$(SO) $(MKL)

test:
	python -m pytest

clean:
	if [ -d __pycache__ ]; then \
		rm __pycache__  -r; \
	fi
	if [ -d .pytest_cache ]; then \
		rm .pytest_cache -r; \
	fi
	if [ -f _matrix$(SO) ]; then \
		rm _matrix$(SO); \
	fi
