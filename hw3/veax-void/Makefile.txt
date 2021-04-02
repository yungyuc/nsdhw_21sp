CXX = g++

ifneq ($(wildcard /opt/intel/mkl/.),)
	INCLUDE_DIRS = /opt/intel/mkl/include
	MKL_DIR = /opt/intel/mkl/lib/intel64
else
	INCLUDE_DIRS = ${HOME}/opt/conda/include
	MKL_DIR = ${HOME}/opt/conda/lib
endif

CXXFLAGS = -O3 -Wl,--no-as-needed -shared -std=c++11 -fPIC -I$(INCLUDE_DIRS) -ldl -lpthread -lm
CXXFLAGS += $(MKL_DIR)/libmkl_def.so
CXXFLAGS += $(MKL_DIR)/libmkl_avx2.so
CXXFLAGS += $(MKL_DIR)/libmkl_core.so
CXXFLAGS += $(MKL_DIR)/libmkl_intel_lp64.so
CXXFLAGS += $(MKL_DIR)/libmkl_sequential.so

MODULE = _matrix
SOFILE = $(MODULE)$(shell python3-config --extension-suffix)

.PHONY: all test clean
all: $(SOFILE)

$(SOFILE): matrix.cc matrix.h
	$(CXX) `python3 -m pybind11 --includes` $< -o $@ $(CXXFLAGS)

test: $(SOFILE)
	pytest

clean:
	@rm -rf $(SOFILE) __pycache__ .pytest_cache performance.txt
