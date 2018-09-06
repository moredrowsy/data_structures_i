CC 				= g++
DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Wall
CXXFLAGS        = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INCLUDE		= ../include
SRC			= ../src

main: main.o my_washer.o
	$(CC) main.o my_washer.o -o main.out

main.o: main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp

my_washer.o: ${SRC}/my_washer.cpp\
	${INCLUDE}/my_washer.h\
	${INCLUDE}/my_queue.h\
	${INCLUDE}/my_node.h
	$(CC) $(CXXFLAGS) -c ${SRC}/my_washer.cpp

clean:
	rm -f *.o *.out
