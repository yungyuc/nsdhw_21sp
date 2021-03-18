CXX=g++
CXXFLAGS=-std=c++11 -shared -O3 -Wall -Wextra -Werror -fPIC  
PYINCLUDE:=`python3-config --includes`
PYIEXTERN:=-Iextern/pybind11/include
PYSUFFIX:=`python3-config --extension-suffix`

.PHONY: test clean

_vector: _vector.cpp
	$(CXX) $(CXXFLAGS) $(PYINCLUDE) $(PYIEXTERN) $< -o $@$(PYSUFFIX)

test: _vector

clean:
	rm -f *.so
