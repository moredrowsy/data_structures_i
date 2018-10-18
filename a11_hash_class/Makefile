CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC             := ../include
SRC             := ../src
OBJ             := main.o

main.out: $(OBJ)
	$(CXX) -o $@ $^

main.o: main.cpp\
	${INC}/record.h\
	${INC}/node.h\
	${INC}/list.h\
	${INC}/chained_hash.h\
	${INC}/double_hash.h\
	${INC}/open_hash.h\
	${INC}/chained_hash.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
