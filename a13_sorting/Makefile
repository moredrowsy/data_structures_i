CXX             := clang++
DEBUG_LEVEL     := #-g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
OPT             := -O2
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS) $(OPT)
LDLIBS          :=-lm -lstdc++ -pthread

INC             := ../include
SRC             := ../src
OBJ             := main.o

main.out: $(OBJ)
	$(CXX) -o $@ $^ $(LDLIBS)

main.o: main.cpp\
	${INC}/sort.h\
	${INC}/timer.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
