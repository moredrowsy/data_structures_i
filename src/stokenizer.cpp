#include "../include/stokenizer.h"

namespace STokenizer {

STokenizer::STokenizer() {
    _buffer[0] = '\0';
    _pos = 0;
    // make_table(_table);
}

STokenizer::STokenizer(char str[]) {
    // copy string into buffer
    int index = 0;
    while(str[index] != '\0') {
        _buffer[index] = str[index];
        ++index;
    }
    _buffer[index] = '\0';

    _pos = 0;
    // make_table(_table);
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

void STokenizer::make_table(int _table[][MAX_COLUMNS]) {
    using namespace state_machine;

    // set STATE_DOUBLE
    mark_fail(_table, 0);     // Mark states 0 and 2 as fail states
    mark_success(_table, 1);  // Mark states 1 and 3 as success states
    mark_fail(_table, 2);
    mark_success(_table, 3);
    mark_cells(0, _table, DIGITS, 1);    // state [0] --- DIGITS ---> [1]
    mark_cells(0, _table, '.', '.', 2);  // state [0] --- '.' ------> [2]
    mark_cells(1, _table, DIGITS, 1);    // state [1] --- DIGITS ---> [1]
    mark_cells(1, _table, '.', '.', 2);  // state [1] --- '.' ------> [2]
    mark_cells(2, _table, DIGITS, 3);    // state [2] --- DIGITS ---> [3]
    mark_cells(3, _table, DIGITS, 3);    // state [3] --- DIGITS ---> [3]
}

bool STokenizer::get_token(int start_state, std::string& token) {
    return state_machine::get_token(_table, _buffer, _pos, start_state, token);
}

STokenizer& operator>>(STokenizer& s, Token::Token& t) {
    bool success = false;
    std::string token;

    // loop until no more tokens to extract
    if(STATE_DOUBLE) {
        success = s.get_token(STATE_DOUBLE, token);

        t = Token::Token(token, STATE_DOUBLE);
    }
}

}  // namespace STokenizer
