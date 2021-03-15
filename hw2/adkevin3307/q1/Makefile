EXE = main
OBJ_DIR = obj
RESULT_FILE = result.txt

SOURCES = Line.cpp main.cpp
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CXXFLAGS = -std=c++17 -Wall -O2

LIBS =

all: create_object_directory $(EXE)
	@echo Compile Success

create_object_directory:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

run:
	./$(EXE)

check:
	./$(EXE) > $(RESULT_FILE)

clean:
	rm -rf $(EXE) $(OBJ_DIR) $(RESULT_FILE)
