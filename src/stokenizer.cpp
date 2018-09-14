#include "../include/stokenizer.h"  // stokenizer declarations
#include <cassert>                  // provides assert

namespace stokenizer {

// STATIC VARIABLES
int STokenizer::_table[state_machine::MAX_ROWS][state_machine::MAX_COLUMNS];

int Token::type() const { return _type; }

std::string Token::type_string() const {
    std::string type_string;

    switch(_type) {
        case state_machine::STATE_UNKNOWN:
            type_string = "UNKNOWN";
            break;
        case state_machine::STATE_FRACTION:
            type_string = "FRACTION";
            break;
        case state_machine::STATE_DOUBLE:
            type_string = "DOUBLE";
            break;
        case state_machine::STATE_SPACE:
            type_string = "SPACE";
            break;
        case state_machine::STATE_ALPHA:
            type_string = "ALPHA";
            break;
        case state_machine::STATE_PUNCT:
            type_string = "PUNCT";
            break;
        default:
            type_string = "ERROR";
    }

    return type_string;
}

std::string Token::token_str() const { return _token; }

std::ostream& operator<<(std::ostream& outs, const Token& t) {
    outs << "|" + t.token_str() + "|";

    return outs;
}

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

    // STATE_FRACTION
    mark_fail(_table, STATE_FRACTION + 0);  // fail states: 0,1,2,3
    mark_fail(_table, STATE_FRACTION + 1);
    mark_fail(_table, STATE_FRACTION + 2);
    mark_fail(_table, STATE_FRACTION + 3);
    mark_success(_table, STATE_FRACTION + 4);  // success states: 4

    // MARK CELLS
    // state [0] --- DIGIT ---> [1]
    // state [1] --- DIGIT ---> [1]
    // state [1] ---- '/' ----> [2]
    // state [1] --- SPACE ---> [3]
    // state [2] --- DIGIT ---> [4]
    // state [3] --- DIGIT ---> [1]
    // state [4] --- DIGIT ---> [4]
    mark_cells(STATE_FRACTION + 0, _table, DIGIT, STATE_FRACTION + 1);
    mark_cells(STATE_FRACTION + 1, _table, DIGIT, STATE_FRACTION + 1);
    mark_cells(STATE_FRACTION + 1, _table, '/', '/', STATE_FRACTION + 2);
    mark_cells(STATE_FRACTION + 1, _table, SPACE, STATE_FRACTION + 3);
    mark_cells(STATE_FRACTION + 2, _table, DIGIT, STATE_FRACTION + 4);
    mark_cells(STATE_FRACTION + 3, _table, DIGIT, STATE_FRACTION + 1);
    mark_cells(STATE_FRACTION + 4, _table, DIGIT, STATE_FRACTION + 4);

    // mark table for STATE_DOUBLE
    mark_table_double(_table, STATE_DOUBLE);

    // mark table for STATE_FRACTION
    mark_table_fraction(_table, STATE_FRACTION);

    // mark table for STATE_SPACE
    mark_table_space(_table, STATE_SPACE);

    // mark table for STATE_ALPHA
    mark_table_alpha(_table, STATE_ALPHA);

    // mark table for STATE_PUNCT
    mark_table_punct(_table, STATE_PUNCT);
}

bool STokenizer::get_token(int start_state, std::string& token) {
    return state_machine::get_token(_table, _buffer, _pos, start_state, token);
}

STokenizer& operator>>(STokenizer& s, Token& t) {
    std::string token;

    // process tokens one state at a time until unknown token
    if(s.get_token(state_machine::STATE_FRACTION, token)) {
        t = Token(token, state_machine::STATE_FRACTION);
    } else if(s.get_token(state_machine::STATE_DOUBLE, token)) {
        t = Token(token, state_machine::STATE_DOUBLE);
    } else if(s.get_token(state_machine::STATE_SPACE, token)) {
        t = Token(token, state_machine::STATE_SPACE);
    } else if(s.get_token(state_machine::STATE_ALPHA, token)) {
        t = Token(token, state_machine::STATE_ALPHA);
    } else if(s.get_token(state_machine::STATE_PUNCT, token)) {
        t = Token(token, state_machine::STATE_PUNCT);
    } else {
        t = Token(std::string(1, s._buffer[s._pos]));  // create unknown
        ++s._pos;  // when unknown token, go to next position
    }

    return s;
}

}  // namespace stokenizer
