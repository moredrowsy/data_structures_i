CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC             := ../include
SRC             := ../src
OBJ             := test_all.o my_washer.o my_airport.o state_machine.o\
                   stokenizer.o ftokenizer.o

# tests for al files
tests.out: $(OBJ)
	$(CXX) -o $@ $^

tests_all.o: test_all.cpp\
	${INC}/*.h
	$(CXX) $(CXXFLAGS) -c $<

# test for my_node
test_my_node.out: test_my_node.o
	$(CXX) -o $@ $^

test_my_node.o: test_my_node.cpp\
	test_my_node.h\
	${INC}/my_node.h
	$(CXX) $(CXXFLAGS) -c $<

# test for my_queue
test_my_queue.out: test_my_queue.o
	$(CXX) -o $@ $^

test_my_queue.o: test_my_queue.cpp\
	test_my_queue.h\
	${INC}/my_queue.h\
	${INC}/my_node.h
	$(CXX) $(CXXFLAGS) -c $<

# test for my_stack
test_my_stack.out: test_my_stack.o
	$(CXX) -o $@ $^

test_my_stack.o: test_my_stack.cpp\
	test_my_stack.h\
	${INC}/my_stack.h\
	${INC}/my_node.h
	$(CXX) $(CXXFLAGS) -c $<

# test for my_washer
test_my_washer.out: test_my_washer.o\
	my_washer.o
	$(CXX) -o $@ $^

test_my_washer.o: test_my_washer.cpp\
	test_my_washer.h
	$(CXX) $(CXXFLAGS) -c $<

my_washer.o: ${SRC}/my_washer.cpp\
	${INC}/my_washer.h\
	${INC}/my_queue.h\
	${INC}/my_node.h
	$(CXX) $(CXXFLAGS) -c $<

# test for my_airport
test_my_airport.out: test_my_airport.o\
	my_airport.o
	$(CXX) -o $@ $^

test_my_airport.o: test_my_airport.cpp\
	test_my_airport.h
	$(CXX) $(CXXFLAGS) -c $<

my_airport.o: ${SRC}/my_airport.cpp\
	${INC}/my_airport.h\
	${INC}/my_queue.h\
	${INC}/my_node.h
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
