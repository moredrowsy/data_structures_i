CXX 			= g++
DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Werror=return-type #-Wall
CXXFLAGS        = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC				= ../include
SRC				= ../src
OBJ				= main.o state_machine.o stokenizer.o ftokenizer.o

main.out: $(OBJ)
	$(CXX) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $<

state_machine.o: ${SRC}/state_machine.cpp\
	${INC}/state_machine.h
	$(CXX) $(CXXFLAGS) -c $<

stokenizer.o: ${SRC}/stokenizer.cpp\
	${INC}/stokenizer.h
	$(CXX) $(CXXFLAGS) -c $<

ftokenizer.o: ${SRC}/ftokenizer.cpp\
	${INC}/ftokenizer.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out