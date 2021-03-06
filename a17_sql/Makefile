CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
OPT             := -O0
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS) $(OPT)
LDLIBS          :=-lm -lstdc++

INC             := ../include
SRC             := ../src
OBJ             := state_machine.o token.o sql_parser.o sql_record.o\
                   sql_states.o sql_table.o sql_tokenizer.o sql.o	

main.out: $(OBJ) main.o
	$(CXX) -o $@ $^ $(LDLIBS)

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
	${INC}/smart_ptr_utils.h\
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

batch.out: $(OBJ) batch.o
	$(CXX) -o $@ $^ $(LDLIBS)

batch.o: batch.cpp\
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

session.out: $(OBJ) session.o
	$(CXX) -o $@ $^ $(LDLIBS)

session.o: session.cpp\
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

# ftokenizer_map.out entries

stokenizer.o: ${SRC}/stokenizer.cpp\
	${INC}/stokenizer.h
	$(CXX) $(CXXFLAGS) -c $<

ftokenizer.o: ${SRC}/ftokenizer.cpp\
	${INC}/ftokenizer.h
	$(CXX) $(CXXFLAGS) -c $<

ftokenizer_map.out: $(OBJ) stokenizer.o ftokenizer.o ftokenizer_map.o
	$(CXX) -o $@ $^ $(LDLIBS)

ftokenizer_map.o: ftokenizer_map.cpp\
	${INC}/array_utils.h\
	${INC}/sort.h\
	${INC}/vector_utils.h\
	${INC}/bptree.h\
	${INC}/pair.h\
	${INC}/bpt_map.h\
	${INC}/smart_ptr_utils.h\
	${INC}/state_machine.h\
	${INC}/token.h\
	${INC}/ftokenizer.h
	$(CXX) $(CXXFLAGS) -c $<


.PHONY: clean

clean:
	rm -f *.o *.out *.sql *.tbl
