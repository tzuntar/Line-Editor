# Line Editor Makefile
CXX         := g++
CXXFLAGS    := -std=c++11 -Wall
LDFLAGS     := -lpanel -lncurses
RM          := rm -rf
PROGRAMNAME := lineedit
MANPAGENAME := lineedit.1
SOURCES     := $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard **/*.cpp)
OBJECTS     := $(patsubst %.cpp, %.o, $(SOURCES))
ifeq ($(PREFIX),)
	PREFIX  := /usr/local
endif
ifeq ($(MANPATH),)
	MANPATH := $(PREFIX)/share/man/man1
endif

.PHONY: run clean $(PROGRAMNAME) all install uninstall

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

install: $(PROGRAMNAME)
	install -d $(PREFIX)/bin/
	install -m +x ./$(PROGRAMNAME) $(PREFIX)/bin/
	install -d $(MANPATH)/
	install ./$(MANPAGENAME) $(MANPATH)/

uninstall:
	rm -f $(PREFIX)/bin/$(PROGRAMNAME)
	rm -f $(MANPATH)/$(MANPAGENAME)
