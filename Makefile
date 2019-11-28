# Line Editor Makefile
CXX         := g++
CXXFLAGS    := -std=c++11 -Wall
LDFLAGS     := -lpanel -lncurses
RM          := rm -rf
PROGRAMNAME := lineedit
SOURCES     := editing.cpp display.cpp lineedit.cpp
OBJECTS     := $(patsubst %.cpp, %.o, $(SOURCES))

.PHONY: run clean $(PROGRAMNAME) all

all: $(PROGRAMNAME)

$(PROGRAMNAME): $(OBJECTS)
	$(CXX) -o $(PROGRAMNAME) $(OBJECTS) $(LDFLAGS)
	@echo "Done. Run the program with ./$(PROGRAMNAME)"

clean:
	-$(RM) $(OBJECTS)
	-$(RM) $(PROGRAMNAME)
	@echo "Done."

run: $(PROGRAMNAME)
	./$(PROGRAMNAME)
