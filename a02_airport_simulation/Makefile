CXX 			:= g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC				:= ../include
SRC				:= ../src
OBJ				:= main.o my_airport.o

main.out: $(OBJ)
	$(CXX) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $<

my_airport.o: ${SRC}/my_airport.cpp\
	${INC}/my_airport.h\
	${INC}/my_queue.h\
	${INC}/my_node.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
