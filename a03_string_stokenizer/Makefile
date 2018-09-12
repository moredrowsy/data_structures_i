CC 				= g++
DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Wall
CXXFLAGS        = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INCLUDE		= ../include
SRC			= ../src

main: main.o state_machine.o token.o stokenizer.o
	$(CC) main.o state_machine.o token.o stokenizer.o -o main.out

main.o: main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp

state_machine.o: ${SRC}/state_machine.cpp\
	${INCLUDE}/state_machine.h
	$(CC) $(CXXFLAGS) -c ${SRC}/state_machine.cpp

token.o: ${SRC}/token.cpp\
	${INCLUDE}/token.h
	$(CC) $(CXXFLAGS) -c ${SRC}/token.cpp

stokenizer.o: ${SRC}/stokenizer.cpp\
	${INCLUDE}/stokenizer.h
	$(CC) $(CXXFLAGS) -c ${SRC}/stokenizer.cpp

clean:
	rm -f *.o *.out
