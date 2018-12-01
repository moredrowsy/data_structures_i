CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
OPT             := -O0
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS) $(OPT)
LDFLAGS         := -lm -lstdc++

INC             := ../include
SRC             := ../src
OBJ             := main.o state_machine.o token.o sql_parser.o sql_record.o\
                   sql_states.o sql_table.o sql_tokenizer.o sql.o

main.out: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main.cpp\
	${INC}/array_utils.h\
	${INC}/sort.h\
	${INC}/vector_utils.h\
	${INC}/node.h\
	${INC}/queue.h\
	${INC}/stack.h\
	${INC}/set.h\
	${INC}/bptree.h\
	${INC}/pair.h\
	${INC}/bpt_map.h\
	${INC}/state_machine.h\
	${INC}/token.h\
	${INC}/sql_parser.h\
	${INC}/sql_record.h\
	${INC}/sql_states.h\
	${INC}/sql_table.h\
	${INC}/sql_token.h\
	${INC}/sql_tokenizer.h\
	${INC}/sql_typedefs.h\
	${INC}/sql.h
	$(CXX) $(CXXFLAGS) -c $<

state_machine.o: ${SRC}/state_machine.cpp\
	${INC}/state_machine.h
	$(CXX) $(CXXFLAGS) -c $<

token.o: ${SRC}/token.cpp\
	${INC}/token.h
	$(CXX) $(CXXFLAGS) -c $<

sql_parser.o: ${SRC}/sql_parser.cpp\
	${INC}/sql_parser.h
	$(CXX) $(CXXFLAGS) -c $<

sql_record.o: ${SRC}/sql_record.cpp\
	${INC}/sql_record.h
	$(CXX) $(CXXFLAGS) -c $<

sql_states.o: ${SRC}/sql_states.cpp\
	${INC}/sql_states.h
	$(CXX) $(CXXFLAGS) -c $<

sql_table.o: ${SRC}/sql_table.cpp\
	${INC}/sql_table.h
	$(CXX) $(CXXFLAGS) -c $<

sql_tokenizer.o: ${SRC}/sql_tokenizer.cpp\
	${INC}/sql_tokenizer.h
	$(CXX) $(CXXFLAGS) -c $<

sql.o: ${SRC}/sql.cpp\
	${INC}/sql.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *.out
