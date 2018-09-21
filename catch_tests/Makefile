CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC             := ../include
SRC             := ../src
CATCH           := ../lib
OBJ             := ${CATCH}/tests-main.o\
                   test_my_node.o

tests.out: ${OBJ}
	$(CXX) -o $@ $^

# catch main test define - COMPILE ONLY ONCE!
tests-main.o: tests-main.cpp\
	${CATCH}/catch.hpp
	$(CXX) -o ${CATCH}/$@ -c $<

# test for my_node
test_my_node.out: ${CATCH}/tests-main.o test_my_node.o
	$(CXX) -o $@ $^

test_my_node.o: test_my_node.cpp\
	${INC}/my_node.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
