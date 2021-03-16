TRASH = __pycache__

TEST_FILE = $(wildcard test_*.py)

SOURCES = $(wildcard *.cpp)
OBJS = $(addsuffix $(shell python3-config --extension-suffix), $(basename $(notdir $(SOURCES))))

CXXFLAGS = -std=c++17 -Wall -O2 -shared -fPIC $(shell python3 -m pybind11 --includes)

all: $(OBJS)
	@echo Compile Success

$(OBJS): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $<

test:
	pytest -q $(TEST_FILE)

clean:
	rm -rf $(OBJS) $(TRASH)
