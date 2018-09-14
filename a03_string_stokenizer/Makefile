CC 				= g++
DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Wall -Werror=return-type
CXXFLAGS        = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INCLUDE		= ../include
SRC			= ../src

main: main.o state_machine.o stokenizer.o
	$(CC) main.o state_machine.o stokenizer.o -o main.out

main.o: main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp

state_machine.o: ${SRC}/state_machine.cpp\
	${INCLUDE}/state_machine.h
	$(CC) $(CXXFLAGS) -c ${SRC}/state_machine.cpp

stokenizer.o: ${SRC}/stokenizer.cpp\
	${INCLUDE}/stokenizer.h
	$(CC) $(CXXFLAGS) -c ${SRC}/stokenizer.cpp

clean:
	rm -f *.o *.out
