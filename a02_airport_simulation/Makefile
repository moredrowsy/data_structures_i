CC 				= g++
DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Wall
CXXFLAGS        = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INCLUDE		= ../include
SRC			= ../src

main: main.o my_airport.o
	$(CC) main.o my_airport.o -o main.out

main.o: main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp

my_airport.o: ${SRC}/my_airport.cpp\
	${INCLUDE}/my_airport.h\
	${INCLUDE}/my_queue.h\
	${INCLUDE}/my_node.h
	$(CC) $(CXXFLAGS) -c ${SRC}/my_airport.cpp

clean:
	rm -f *.o *.out
