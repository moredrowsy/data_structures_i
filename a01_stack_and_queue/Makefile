CC 				= g++
DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Wall
CXXFLAGS        = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INCLUDE		= ../include

main: main.o
	$(CC) main.o -o main.out

main.o: main.cpp ${INCLUDE}/my_node.h\
	${INCLUDE}/my_queue.h\
	${INCLUDE}/my_stack.h
	$(CC) $(CXXFLAGS) -c main.cpp

clean:
	rm -f *.o *.out
