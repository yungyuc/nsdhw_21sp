EXE = _matrix$(shell python3-config --extension-suffix)
OBJ_DIR = obj

TEST_FILE = $(wildcard test_*.py)

TRASH = .cache __pycache__ .pytest_cache performance.txt

SOURCES = $(wildcard *.cpp)
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CXXFLAGS = -std=c++17 -Wall -O2 -shared -fPIC $(shell python3-config --includes) -Iextern/pybind11/include

LIBS =

all: create_object_directory $(EXE)
	@echo Compile Success

create_object_directory:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

test: all
	python3 -m pytest -q $(TEST_FILE)

clean:
	rm -rf $(EXE) $(OBJ_DIR) $(TRASH)
