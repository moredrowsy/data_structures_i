CXX             := clang++
DEBUG_LEVEL     := #-g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
OPT             := -O2
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS) $(OPT)
LDLIBS          := -lm -lstdc++

INC             := ../include
SRC             := ../src
OBJ             := main.o

main.out: $(OBJ)
	$(CXX) -o $@ $^

main.o: main.cpp\
	${INC}/timer.h\
	${INC}/fstream_utils.h\
	${INC}/fstream_handler.h\
	${INC}/fstream_sort.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
