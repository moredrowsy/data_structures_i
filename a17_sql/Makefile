CXX             := g++
DEBUG_LEVEL     := -g
EXTRA_CCFLAGS   := -Wall -Werror=return-type -Wextra -pedantic
OPT             := -O0
CXXFLAGS        := $(DEBUG_LEVEL) $(EXTRA_CCFLAGS) $(OPT)
LDFLAGS         := -lm -lstdc++

INC             := ../include
SRC             := ../src
OBJ             := main.o state_machine.o token.o stokenizer.o sql_states.o\
                   sql_tokenizer.o

main.out: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main.cpp\
	${INC}/array_utils.h\
	${INC}/sort.h\
	${INC}/vector_utils.h\
	${INC}/bptree.h\
	${INC}/pair.h\
	${INC}/bpt_map.h\
	${INC}/state_machine.h\
	${INC}/token.h\
	${INC}/stokenizer.h\
	${INC}/sql_states.h\
	${INC}/sql_token.h\
	${INC}/sql_tokenizer.h\
	${INC}/sql.h
	$(CXX) $(CXXFLAGS) -c $<

state_machine.o: ${SRC}/state_machine.cpp\
	${INC}/state_machine.h
	$(CXX) $(CXXFLAGS) -c $<

token.o: ${SRC}/token.cpp\
	${INC}/token.h
	$(CXX) $(CXXFLAGS) -c $<

stokenizer.o: ${SRC}/stokenizer.cpp\
	${INC}/stokenizer.h
	$(CXX) $(CXXFLAGS) -c $<

sql_states.o: ${SRC}/sql_states.cpp\
	${INC}/sql_states.h
	$(CXX) $(CXXFLAGS) -c $<

sql_tokenizer.o: ${SRC}/sql_tokenizer.cpp\
	${INC}/sql_tokenizer.h
	$(CXX) $(CXXFLAGS) -c $<


.PHONY: clean

clean:
	rm -f *.o *.out
