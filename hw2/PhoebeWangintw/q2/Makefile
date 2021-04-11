CPP=g++
VERSION=c++11
SRC=_vector.cpp
EXE=_vector$(shell python3-config --extension-suffix)

all: ${SRC}
	${CPP} -std=${VERSION} -O3 -Wall -shared -fPIC $(shell python3-config --includes) -Iextern/pybind11/include ${SRC} -o ${EXE}

test:
	python3 -m pytest

clean:
	rm ${EXE}
	rm -rf __pycache__ *.so .pytest* 
