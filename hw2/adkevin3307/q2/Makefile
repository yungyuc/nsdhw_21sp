TRASH = __pycache__ .pytest*

TEST_FILE = $(wildcard test_*.py)

SOURCES = $(wildcard *.cpp)
OBJS = $(addsuffix $(shell python3-config --extension-suffix), $(basename $(notdir $(SOURCES))))

CXXFLAGS = -std=c++17 -Wall -O2 -shared -fPIC $(shell python3-config --includes) -Iextern/pybind11/include

all: $(OBJS)
	@echo Compile Success

$(OBJS): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $<

test:
	python3 -m pytest -q $(TEST_FILE)

clean:
	rm -rf $(OBJS) $(TRASH)
