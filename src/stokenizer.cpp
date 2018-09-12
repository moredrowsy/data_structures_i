#include "../include/stokenizer.h"
#include <cassert>

namespace STokenizer {

// STATIC VARIABLES
int STokenizer::_table[state_machine::MAX_ROWS][state_machine::MAX_COLUMNS];

STokenizer::STokenizer() {
    _buffer[0] = '\0';
    _pos = 0;
    make_table(_table);  // initialize table with adjacency rules
}

STokenizer::STokenizer(char str[]) {
    // copy string into buffer
    int index = 0;
    while(str[index] != '\0') {
        assert(index < MAX_BUFFER - 1);  // assert index < MAX_BUFFER-1 in loop
        _buffer[index] = str[index];
        ++index;
    }
    _buffer[index] = '\0';

    _pos = 0;
    make_table(_table);  // initialize table with adjacency rules
}

bool STokenizer::done() const { return _buffer[_pos] == '\0'; }

bool STokenizer::more() const { return _buffer[_pos] != '\0'; }

void STokenizer::set_string(char str[]) {
    // copy string into buffer
    int index = 0;
    while(str[index] != '\0') {
        _buffer[index] = str[index];
        ++index;
    }
    _buffer[index] = '\0';
}

void STokenizer::make_table(int _table[][state_machine::MAX_COLUMNS]) {
    using namespace state_machine;

    // initialize table with -1
    init_table(_table);

    // STATE_DOUBLE
    mark_fail(_table, STATE_DOUBLE + 0);     // fail states: 0 and 2
    mark_success(_table, STATE_DOUBLE + 1);  // success states: 1 and 3
    mark_fail(_table, STATE_DOUBLE + 2);
    mark_success(_table, STATE_DOUBLE + 3);

    // MARK CELLS
    // state [0] --- DIGITS ---> [1]
    // state [0] --- '.' ------> [2]
    // state [1] --- DIGITS ---> [1]
    // state [1] --- '.' ------> [2]
    // state [2] --- DIGITS ---> [3]
    // state [3] --- DIGITS ---> [3]
    mark_cells(STATE_DOUBLE + 0, _table, DIGITS, STATE_DOUBLE + 1);
    mark_cells(STATE_DOUBLE + 0, _table, '.', '.', STATE_DOUBLE + 2);
    mark_cells(STATE_DOUBLE + 1, _table, DIGITS, STATE_DOUBLE + 1);
    mark_cells(STATE_DOUBLE + 1, _table, '.', '.', STATE_DOUBLE + 2);
    mark_cells(STATE_DOUBLE + 2, _table, DIGITS, STATE_DOUBLE + 3);
    mark_cells(STATE_DOUBLE + 3, _table, DIGITS, STATE_DOUBLE + 3);

    // STATE_SPACE
    mark_fail(_table, STATE_SPACE + 0);     // fail states: 10
    mark_success(_table, STATE_SPACE + 1);  // success states: 11

    // MARK CELLS
    // state [10] --- SPACE ---> [11]
    // state [11] --- SPACE ---> [11]
    mark_cells(STATE_SPACE + 0, _table, SPACE, STATE_SPACE + 1);
    mark_cells(STATE_SPACE + 1, _table, SPACE, STATE_SPACE + 1);

    // STATE_ALPHA
    mark_fail(_table, STATE_ALPHA + 0);     // fail states: 20
    mark_success(_table, STATE_ALPHA + 1);  // success states: 21

    // MARK CELLS
    // state [20] --- ALPHA ---> [21]
    // state [21] --- ALPHA ---> [21]
    mark_cells(STATE_ALPHA + 0, _table, ALPHA, STATE_ALPHA + 1);
    mark_cells(STATE_ALPHA + 1, _table, ALPHA, STATE_ALPHA + 1);
}

bool STokenizer::get_token(int start_state, std::string& token) {
    return state_machine::get_token(_table, _buffer, _pos, start_state, token);
}

STokenizer& operator>>(STokenizer& s, Token::Token& t) {
    std::string token;

    // process tokens one state at a time until unknown token
    if(s.get_token(state_machine::STATE_DOUBLE, token)) {
        t = Token::Token(token, state_machine::STATE_DOUBLE);
    } else if(s.get_token(state_machine::STATE_SPACE, token)) {
        t = Token::Token(token, state_machine::STATE_SPACE);
    } else if(s.get_token(state_machine::STATE_ALPHA, token)) {
        t = Token::Token(token, state_machine::STATE_ALPHA);
    } else {
        t = Token::Token(std::string(1, s._buffer[s._pos]));  // create unknown
        ++s._pos;  // when unknown token, go to next position
    }

    return s;
}

}  // namespace STokenizer
