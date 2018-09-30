CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC             := ../include
SRC             := ../src
CATCH           := ../lib
OBJ             := ${CATCH}/tests_main.o\
                   test_my_node.o test_my_queue.o test_my_stack.o\
				   test_bst_node.o

tests.out: ${OBJ}
	$(CXX) -o $@ $^

# catch main test define - COMPILE ONLY ONCE!
tests_main.o: tests_main.cpp\
	${CATCH}/catch.hpp
	$(CXX) -o ${CATCH}/$@ -c $<

# test my_node
test_my_node.out: ${CATCH}/tests_main.o test_my_node.o
	$(CXX) -o $@ $^

test_my_node.o: test_my_node.cpp\
	${INC}/my_node.h
	$(CXX) $(CXXFLAGS) -c $<

# test my_queue
test_my_queue.out: ${CATCH}/tests_main.o test_my_queue.o
	$(CXX) -o $@ $^

test_my_queue.o: test_my_queue.cpp\
	${INC}/my_node.h\
	${INC}/my_queue.h
	$(CXX) $(CXXFLAGS) -c $<

# test my_stack
test_my_stack.out: ${CATCH}/tests_main.o test_my_stack.o
	$(CXX) -o $@ $^

test_my_stack.o: test_my_stack.cpp\
	${INC}/my_node.h\
	${INC}/my_stack.h
	$(CXX) $(CXXFLAGS) -c $<

# test bst_node
test_bst_node.out: ${CATCH}/tests_main.o test_bst_node.o
	$(CXX) -o $@ $^

test_bst_node.o: test_bst_node.cpp\
	${INC}/bst_node.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
