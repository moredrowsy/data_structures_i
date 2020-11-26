CXX             := clang++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
OPT             := -O0
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS) $(OPT)
LDLIBS          :=-lm -lstdc++

INC             := ../include
SRC             := ../src
OBJ             := main.o

main.out: $(OBJ)
	$(CXX) -o $@ $^ $(LDLIBS)

main.o: main.cpp\
	${INC}/sort.h\
	${INC}/array_utils.h\
	${INC}/btree.h\
	${INC}/vector_utils.h\
	${INC}/pair.h\
	${INC}/bt_map.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
