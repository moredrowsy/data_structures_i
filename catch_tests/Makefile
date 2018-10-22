CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INC             := ../include
SRC             := ../src
LIB             := ../lib
OBJ             := ${LIB}/catch.o\
                   test_node.o test_list.o test_queue.o test_stack.o\
                   test_bst_node.o test_bst.o test_avl.o test_heap.o\
                   test_pqueue.o  test_hash.o test_fstream_sort.o

tests.out: ${OBJ}
	$(CXX) -o $@ $^

# catch2 framework - COMPILE ONLY ONCE!
${LIB}/catch.o: catch.cpp\
	${LIB}/catch.hpp
	$(CXX) -o $@ -c $<

# test node
test_node.out: ${LIB}/catch.o test_node.o
	$(CXX) -o $@ $^

test_node.o: test_node.cpp\
	${INC}/node.h
	$(CXX) $(CXXFLAGS) -c $<

# test queue
test_list.out: ${LIB}/catch.o test_list.o
	$(CXX) -o $@ $^

test_list.o: test_list.cpp\
	${INC}/node.h\
	${INC}/list.h
	$(CXX) $(CXXFLAGS) -c $<

# test queue
test_queue.out: ${LIB}/catch.o test_queue.o
	$(CXX) -o $@ $^

test_queue.o: test_queue.cpp\
	${INC}/node.h\
	${INC}/queue.h
	$(CXX) $(CXXFLAGS) -c $<

# test stack
test_stack.out: ${LIB}/catch.o test_stack.o
	$(CXX) -o $@ $^

test_stack.o: test_stack.cpp\
	${INC}/node.h\
	${INC}/stack.h
	$(CXX) $(CXXFLAGS) -c $<

# test bst_node
test_bst_node.out: ${LIB}/catch.o test_bst_node.o
	$(CXX) -o $@ $^

test_bst_node.o: test_bst_node.cpp\
	${INC}/bst_node.h
	$(CXX) $(CXXFLAGS) -c $<

# test bst
test_bst.out: ${LIB}/catch.o test_bst.o
	$(CXX) -o $@ $^

test_bst.o: test_bst.cpp\
	${INC}/bst_node.h\
    ${INC}/bst.h
	$(CXX) $(CXXFLAGS) -c $<

# test avl
test_avl.out: ${LIB}/catch.o test_avl.o
	$(CXX) -o $@ $^

test_avl.o: test_avl.cpp\
	${INC}/bst_node.h\
    ${INC}/avl.h
	$(CXX) $(CXXFLAGS) -c $<

# test heap
test_heap.out: ${LIB}/catch.o test_heap.o
	$(CXX) -o $@ $^

test_heap.o: test_heap.cpp\
	${INC}/heap.h
	$(CXX) $(CXXFLAGS) -c $<

# test pqueue
test_pqueue.out: ${LIB}/catch.o test_pqueue.o
	$(CXX) -o $@ $^

test_pqueue.o: test_pqueue.cpp\
	${INC}/heap.h\
	${INC}/pqueue.h
	$(CXX) $(CXXFLAGS) -c $<

# test hash
test_hash.out: ${LIB}/catch.o test_hash.o
	$(CXX) -o $@ $^

test_hash.o: test_hash.cpp\
	${INC}/record.h\
	${INC}/node.h\
	${INC}/list.h\
	${INC}/bst_node.h\
	${INC}/avl.h\
	${INC}/open_hash.h\
	${INC}/double_hash.h\
	${INC}/chained_avl_hash.h\
	${INC}/chained_list_hash.h
	$(CXX) $(CXXFLAGS) -c $<

# test file_sort
test_fstream_sort.out: ${LIB}/catch.o test_fstream_sort.o
	$(CXX) -o $@ $^

test_fstream_sort.o: test_fstream_sort.cpp\
	${INC}/fstream_sort.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
