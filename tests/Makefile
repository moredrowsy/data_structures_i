CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC             := ../include
SRC             := ../src
OBJ             := test_all.o washer.o airport.o state_machine.o\
                   stokenizer.o ftokenizer.o

# tests for al files
tests.out: $(OBJ)
	$(CXX) -o $@ $^

tests_all.o: test_all.cpp\
	${INC}/*.h
	$(CXX) $(CXXFLAGS) -c $<

# test for node
test_node.out: test_node.o
	$(CXX) -o $@ $^

test_node.o: test_node.cpp\
	test_node.h\
	${INC}/node.h
	$(CXX) $(CXXFLAGS) -c $<

# test for queue
test_queue.out: test_queue.o
	$(CXX) -o $@ $^

test_queue.o: test_queue.cpp\
	test_queue.h\
	${INC}/queue.h\
	${INC}/node.h
	$(CXX) $(CXXFLAGS) -c $<

# test for stack
test_stack.out: test_stack.o
	$(CXX) -o $@ $^

test_stack.o: test_stack.cpp\
	test_stack.h\
	${INC}/stack.h\
	${INC}/node.h
	$(CXX) $(CXXFLAGS) -c $<

# test for washer
test_washer.out: test_washer.o\
	washer.o
	$(CXX) -o $@ $^

test_washer.o: test_washer.cpp\
	test_washer.h
	$(CXX) $(CXXFLAGS) -c $<

washer.o: ${SRC}/washer.cpp\
	${INC}/washer.h\
	${INC}/queue.h\
	${INC}/node.h
	$(CXX) $(CXXFLAGS) -c $<

# test for airport
test_airport.out: test_airport.o\
	airport.o
	$(CXX) -o $@ $^

test_airport.o: test_airport.cpp\
	test_airport.h
	$(CXX) $(CXXFLAGS) -c $<

airport.o: ${SRC}/airport.cpp\
	${INC}/airport.h\
	${INC}/queue.h\
	${INC}/node.h
	$(CXX) $(CXXFLAGS) -c $<

# test for state_machine
test_state_machine.out: test_state_machine.o\
	state_machine.o
	$(CXX) -o $@ $^

test_state_machine.o: test_state_machine.cpp\
	test_state_machine.h
	$(CXX) $(CXXFLAGS) -c $<

state_machine.o: ${SRC}/state_machine.cpp\
	${INC}/state_machine.h
	$(CXX) $(CXXFLAGS) -c $<

# test for stokenizer
test_stokenizer.out: test_stokenizer.o\
	state_machine.o\
	stokenizer.o
	$(CXX) -o $@ $^

test_stokenizer.o: test_stokenizer.cpp\
	test_stokenizer.h
	$(CXX) $(CXXFLAGS) -c $<

stokenizer.o: ${SRC}/stokenizer.cpp\
	${INC}/state_machine.h\
	${INC}/stokenizer.h
	$(CXX) $(CXXFLAGS) -c $<

# test for ftokenizer
test_ftokenizer.out: test_ftokenizer.o\
	state_machine.o\
	stokenizer.o\
    ftokenizer.o
	$(CXX) -o $@ $^

test_ftokenizer.o: test_ftokenizer.cpp\
	test_ftokenizer.h
	$(CXX) $(CXXFLAGS) -c $<

ftokenizer.o: ${SRC}/ftokenizer.cpp\
	${INC}/state_machine.h\
	${INC}/stokenizer.h\
    ${INC}/ftokenizer.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
