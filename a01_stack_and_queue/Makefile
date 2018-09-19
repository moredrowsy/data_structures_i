CXX 			:= g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC				:= ../include
SRC				:= ../src
OBJ				:= main.o

main.out: $(OBJ)
	$(CXX) -o $@ $^

main.o: main.cpp\
	${INC}/my_node.h\
	${INC}/my_queue.h\
	${INC}/my_stack.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
