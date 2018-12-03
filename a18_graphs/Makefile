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
	${INC}/smart_ptr_utils.h\
	${INC}/sort.h\
	${INC}/bptree.h\
	${INC}/vector_utils.h\
	${INC}/pair.h\
	${INC}/bpt_map.h\
	${INC}/node.h\
	${INC}/queue.h\
	${INC}/set.h\
	${INC}/graph.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
