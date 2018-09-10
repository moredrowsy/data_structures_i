CC 				= g++
DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Wall
CXXFLAGS        = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)

INCLUDE			= ../include
SRC				= ../src

test_my_node: test_my_node.o
	$(CC) test_my_node.o -o test_my_node.out

test_my_node.o: test_my_node.cpp test_my_node.h\
	${INCLUDE}/my_node.h
	$(CC) $(CXXFLAGS) -c test_my_node.cpp -o test_my_node.o

test_my_queue: test_my_queue.o
	$(CC) test_my_queue.o -o test_my_queue.out

test_my_queue.o: test_my_queue.cpp test_my_queue.h\
	${INCLUDE}/my_node.h\
	${INCLUDE}/my_queue.h
	$(CC) $(CXXFLAGS) -c test_my_queue.cpp -o test_my_queue.o

test_my_stack: test_my_stack.o
	$(CC) test_my_stack.o -o test_my_stack.out

test_my_stack.o: test_my_stack.cpp test_my_stack.h\
	${INCLUDE}/my_node.h\
	${INCLUDE}/my_stack.h
	$(CC) $(CXXFLAGS) -c test_my_stack.cpp -o test_my_stack.o

test_my_washer: test_my_washer.o my_washer.o
	$(CC) test_my_washer.o my_washer.o -o test_my_washer.out

test_my_washer.o: test_my_washer.cpp test_my_washer.h
	$(CC) $(CXXFLAGS) -c test_my_washer.cpp

my_washer.o: ${SRC}/my_washer.cpp ${INCLUDE}/my_washer.h\
	${INCLUDE}/my_queue.h
	$(CC) $(CXXFLAGS) -c ${SRC}/my_washer.cpp

test_my_airport: test_my_airport.o my_airport.o
	$(CC) test_my_airport.o my_airport.o -o test_my_airport.out

test_my_airport.o: test_my_airport.cpp test_my_airport.h
	$(CC) $(CXXFLAGS) -c test_my_airport.cpp

my_airport.o: ${SRC}/my_airport.cpp ${INCLUDE}/my_airport.h\
	${INCLUDE}/my_queue.h
	$(CC) $(CXXFLAGS) -c ${SRC}/my_airport.cpp

tests: tests.o my_washer.o my_airport.o
	$(CC) tests.o my_washer.o my_airport.o -o tests.out

tests.o: test_all.cpp\
	test_my_node.h\
	test_my_queue.h\
	test_my_stack.h\
	test_my_washer\
	test_my_airport.h
	$(CC) $(CXXFLAGS) -c test_all.cpp -o tests.o

clean:
	rm -f *.o *.out
