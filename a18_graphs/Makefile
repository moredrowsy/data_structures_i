CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
OPT             := -O0
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS) $(OPT)
LDFLAGS         := -lm -lstdc++

INC             := ../include
SRC             := ../src
OBJ             := main.o

main.out: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main.cpp\
	${INC}/graph.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
