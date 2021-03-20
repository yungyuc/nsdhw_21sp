CPP=g++
VERSION=c++11
SRC=_vector.cpp
EXE=_vector$(shell python3-config --extension-suffix)

all: ${SRC}
	${CPP} -std=${VERSION} -O3 -Wall -shared -fPIC $(shell python3 -m pybind11 --includes) ${SRC} -o ${EXE}

test:
	pytest

clean:
	rm ${EXE}
	rm -rf __pycache__ 
