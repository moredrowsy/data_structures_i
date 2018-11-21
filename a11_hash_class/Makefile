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
	${INC}/hash_record.h\
	${INC}/node.h\
	${INC}/list.h\
	${INC}/bst_node.h\
	${INC}/avl.h\
	${INC}/open_hash.h\
	${INC}/double_hash.h\
	${INC}/chained_avl_hash.h\
	${INC}/chained_list_hash.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
