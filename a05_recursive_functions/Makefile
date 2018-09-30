CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC             := ../include
SRC             := ../src
OBJ             := main.o my_recursions.o

main.out: $(OBJ)
	$(CXX) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $<

my_recursions.o: ${SRC}/my_recursions.cpp\
	${INC}/my_recursions.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
