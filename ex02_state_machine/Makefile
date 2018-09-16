CXX 			= g++
DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Werror=return-type #-Wall
CXXFLAGS        = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC				= ../include
SRC				= ../src
OBJ				= main.o state_machine.o

main.out: $(OBJ)
	$(CXX) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $<

state_machine.o: ${SRC}/state_machine.cpp\
	${INC}/state_machine.h\
	${INC}/my_queue.h\
	${INC}/my_node.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
