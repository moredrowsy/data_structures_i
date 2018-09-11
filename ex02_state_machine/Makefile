CC 				= g++
DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Wall
CXXFLAGS        = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INCLUDE		= ../include
SRC			= ../src

main: main.o state_machine.o
	$(CC) main.o state_machine.o -o main.out

main.o: main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp

state_machine.o: ${SRC}/state_machine.cpp\
	${INCLUDE}/state_machine.h\
	${INCLUDE}/my_queue.h\
	${INCLUDE}/my_node.h
	$(CC) $(CXXFLAGS) -c ${SRC}/state_machine.cpp

clean:
	rm -f *.o *.out
