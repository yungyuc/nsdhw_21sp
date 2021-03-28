RESULT = result
DUMP_FILE = result.txt
SOURCES = $(wildcard *.cpp)
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)))
CXXFLAGS = -std=c++17

all: $(OBJECTS)
	g++ $^ -o $(RESULT) $(CXXFLAGS)

%.o: %.cpp
	echo $@
	g++ $< -c -o $@ $(CXXFLAGS)

run:
	./$(RESULT)

check:
	./$(RESULT) > $(DUMP_FILE)

clean:
	rm -rf $(RESULT) $(OBJECTS) $(DUMP_FILE)
