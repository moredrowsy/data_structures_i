#include "../include/state_machine.h"  // state_machine declarations
#include <cassert>                     // provides assert
#include <cmath>                       // round numbers
#include <iomanip>                     // stream formatting
#include <iostream>                    // output stream

namespace state_machine {

void init_table(int _table[][MAX_COLUMNS]) {
    // fill array with -1
    for(int row = 0; row < MAX_ROWS; ++row) {
        for(int col = 0; col < MAX_COLUMNS; ++col) {
            _table[row][col] = -1;
        }
    }
}

void mark_success(int _table[][MAX_COLUMNS], int state) {
    // assert state is not greater or equal to MAX ROWS
    assert(state < MAX_ROWS);

    // assign [row as state][column 0] to 1 as success state
    _table[state][0] = 1;
}

void mark_fail(int _table[][MAX_COLUMNS], int state) {
    // assert state is not greater or equal to MAX ROWS
    assert(state < MAX_ROWS);

    // assign [row as state][column 0] to 0 as fail
    _table[state][0] = 0;
}

bool is_success(const int _table[][MAX_COLUMNS], int state) {
    return _table[state][0];
}

void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to,
                int state) {
    for(int col = from; col <= to; ++col) {
        _table[row][col] = state;
    }
}

void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[],
                int state) {
    for(int i = 0; columns[i] != '\0'; ++i) {
        _table[row][columns[i]] = state;
    }
}

void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state) {
    _table[row][column] = state;
}

void mark_table_double(int _table[][MAX_COLUMNS], int start_state) {
    // MARK SUCCESS/FAILURE
    // state[+0] ---> fail
    // state[+1] ---> success
    // state[+2] ---> success
    // state[+3] ---> success
    // state[+4] ---> fail
    // state[+5] ---> fail
    // state[+6] ---> fail
    // state[+7] ---> success
    // state[+8] ---> fail
    // state[+9] ---> success
    mark_fail(_table, STATE_DOUBLE + 0);
    mark_success(_table, STATE_DOUBLE + 1);
    mark_success(_table, STATE_DOUBLE + 2);
    mark_success(_table, STATE_DOUBLE + 3);
    mark_fail(_table, STATE_DOUBLE + 4);
    mark_fail(_table, STATE_DOUBLE + 5);
    mark_fail(_table, STATE_DOUBLE + 6);
    mark_success(_table, STATE_DOUBLE + 7);
    mark_fail(_table, STATE_DOUBLE + 8);
    mark_success(_table, STATE_DOUBLE + 9);

    // MARK CELLS
    // state [+0] --- DIGIT ---> [+1]
    // state [+0] --- '.' -----> [+8]
    // state [+1] --- DIGIT ---> [+2]
    // state [+1] --- ',' -----> [+4]
    // state [+1] --- '.' -----> [+8]
    // state [+2] --- DIGIT ---> [+3]
    // state [+2] --- ',' -----> [+4]
    // state [+2] --- '.' -----> [+8]
    // state [+3] --- DIGIT ---> [+3]
    // state [+3] --- ',' -----> [+4]
    // state [+3] --- '.' -----> [+8]
    // state [+4] --- DIGIT ---> [+5]
    // state [+5] --- DIGIT ---> [+6]
    // state [+6] --- DIGIT ---> [+7]
    // state [+7] --- ',' -----> [+4]
    // state [+7] --- '.' -----> [+8]
    // state [+8] --- DIGIT ---> [+9]
    // state [+9] --- DIGIT ---> [+9]
    mark_cells(STATE_DOUBLE + 0, _table, DIGIT, STATE_DOUBLE + 1);
    mark_cells(STATE_DOUBLE + 0, _table, '.', '.', STATE_DOUBLE + 8);
    mark_cells(STATE_DOUBLE + 1, _table, DIGIT, STATE_DOUBLE + 2);
    mark_cells(STATE_DOUBLE + 1, _table, ',', ',', STATE_DOUBLE + 4);
    mark_cells(STATE_DOUBLE + 1, _table, '.', '.', STATE_DOUBLE + 8);
    mark_cells(STATE_DOUBLE + 2, _table, DIGIT, STATE_DOUBLE + 3);
    mark_cells(STATE_DOUBLE + 2, _table, ',', ',', STATE_DOUBLE + 4);
    mark_cells(STATE_DOUBLE + 2, _table, '.', '.', STATE_DOUBLE + 8);
    mark_cells(STATE_DOUBLE + 3, _table, DIGIT, STATE_DOUBLE + 3);
    mark_cells(STATE_DOUBLE + 3, _table, ',', ',', STATE_DOUBLE + 4);
    mark_cells(STATE_DOUBLE + 3, _table, '.', '.', STATE_DOUBLE + 8);
    mark_cells(STATE_DOUBLE + 4, _table, DIGIT, STATE_DOUBLE + 5);
    mark_cells(STATE_DOUBLE + 5, _table, DIGIT, STATE_DOUBLE + 6);
    mark_cells(STATE_DOUBLE + 6, _table, DIGIT, STATE_DOUBLE + 7);
    mark_cells(STATE_DOUBLE + 7, _table, ',', ',', STATE_DOUBLE + 4);
    mark_cells(STATE_DOUBLE + 7, _table, '.', '.', STATE_DOUBLE + 8);
    mark_cells(STATE_DOUBLE + 8, _table, DIGIT, STATE_DOUBLE + 9);
    mark_cells(STATE_DOUBLE + 9, _table, DIGIT, STATE_DOUBLE + 9);
}

void mark_table_fraction(int _table[][MAX_COLUMNS], int start_state) {
    // MARK SUCCESS/FAILURE
    // state[+0] ---> fail
    // state[+1] ---> fail
    // state[+2] ---> fail
    // state[+3] ---> fail
    // state[+4] ---> fail
    // state[+5] ---> fail
    // state[+6] ---> fail
    // state[+7] ---> fail
    // state[+8] ---> fail
    // state[+9] ---> fail
    // state[+10] --> fail
    // state[+11] --> fail
    // state[+12] --> fail
    // state[+13] --> fail
    // state[+14] --> fail
    // state[+15] --> success
    // state[+16] --> fail
    // state[+17] --> success
    mark_fail(_table, STATE_FRACTION + 0);
    mark_fail(_table, STATE_FRACTION + 1);
    mark_fail(_table, STATE_FRACTION + 2);
    mark_fail(_table, STATE_FRACTION + 3);
    mark_fail(_table, STATE_FRACTION + 4);
    mark_fail(_table, STATE_FRACTION + 5);
    mark_fail(_table, STATE_FRACTION + 6);
    mark_fail(_table, STATE_FRACTION + 7);
    mark_fail(_table, STATE_FRACTION + 8);
    mark_fail(_table, STATE_FRACTION + 9);
    mark_fail(_table, STATE_FRACTION + 10);
    mark_fail(_table, STATE_FRACTION + 11);
    mark_fail(_table, STATE_FRACTION + 12);
    mark_fail(_table, STATE_FRACTION + 13);
    mark_fail(_table, STATE_FRACTION + 14);
    mark_success(_table, STATE_FRACTION + 15);
    mark_fail(_table, STATE_FRACTION + 16);
    mark_success(_table, STATE_FRACTION + 17);

    // MARK CELLS
    // state [+0] --- DIGIT ---> [+1]
    // state [+0] --- '.' -----> [+8]
    // state [+1] --- DIGIT ---> [+2]
    // state [+1] --- ',' -----> [+4]
    // state [+1] --- '.' -----> [+8]
    // state [+1] --- ' ' -----> [+10]
    // state [+1] --- '/' -----> [+12]
    // state [+2] --- DIGIT ---> [+2]
    // state [+2] --- ',' -----> [+4]
    // state [+2] --- '.' -----> [+8]
    // state [+2] --- ' ' -----> [+10]
    // state [+2] --- '/' -----> [+12]
    // state [+3] --- DIGIT ---> [+3]
    // state [+3] --- ',' -----> [+4]
    // state [+3] --- '.' -----> [+8]
    // state [+3] --- ' ' -----> [+10]
    // state [+3] --- '/' -----> [+12]
    // state [+4] --- DIGIT ---> [+5]
    // state [+5] --- DIGIT ---> [+6]
    // state [+6] --- DIGIT ---> [+7]
    // state [+7] --- ',' -----> [+4]
    // state [+7] --- '.' -----> [+8]
    // state [+7] --- ' ' -----> [+10]
    // state [+8] --- DIGIT ---> [+9]
    // state [+9] --- DIGIT ---> [+9]
    // state [+9] --- '/' ----> [+12]
    // state [+10] --- DIGIT --> [+11]
    // state [+11] --- DIGIT --> [+11]
    // state [+11] --- '/' ----> [+12]
    // state [+11] --- '.' ----> [+13]
    // state [+12] --- DIGIT --> [+15]
    // state [+13] --- DIGIT --> [+14]
    // state [+14] --- '/' ----> [+12]
    // state [+14] --- DIGIT --> [+14]
    // state [+15] --- DIGIT --> [+15]
    // state [+15] --- '.' ----> [+16]
    // state [+16] --- DIGIT --> [+17]
    // state [+17] --- DIGIT --> [+17]
    mark_cells(STATE_FRACTION + 0, _table, DIGIT, STATE_FRACTION + 1);
    mark_cells(STATE_FRACTION + 0, _table, '.', '.', STATE_FRACTION + 8);
    mark_cells(STATE_FRACTION + 1, _table, DIGIT, STATE_FRACTION + 2);
    mark_cells(STATE_FRACTION + 1, _table, ',', ',', STATE_FRACTION + 4);
    mark_cells(STATE_FRACTION + 1, _table, '.', '.', STATE_FRACTION + 8);
    mark_cells(STATE_FRACTION + 1, _table, ' ', ' ', STATE_FRACTION + 10);
    mark_cells(STATE_FRACTION + 1, _table, '/', '/', STATE_FRACTION + 12);
    mark_cells(STATE_FRACTION + 2, _table, DIGIT, STATE_FRACTION + 3);
    mark_cells(STATE_FRACTION + 2, _table, ',', ',', STATE_FRACTION + 4);
    mark_cells(STATE_FRACTION + 2, _table, '.', '.', STATE_FRACTION + 8);
    mark_cells(STATE_FRACTION + 2, _table, ' ', ' ', STATE_FRACTION + 10);
    mark_cells(STATE_FRACTION + 2, _table, '/', '/', STATE_FRACTION + 12);
    mark_cells(STATE_FRACTION + 3, _table, DIGIT, STATE_FRACTION + 3);
    mark_cells(STATE_FRACTION + 3, _table, ',', ',', STATE_FRACTION + 4);
    mark_cells(STATE_FRACTION + 3, _table, '.', '.', STATE_FRACTION + 8);
    mark_cells(STATE_FRACTION + 3, _table, ' ', ' ', STATE_FRACTION + 10);
    mark_cells(STATE_FRACTION + 3, _table, '/', '/', STATE_FRACTION + 12);
    mark_cells(STATE_FRACTION + 4, _table, DIGIT, STATE_FRACTION + 5);
    mark_cells(STATE_FRACTION + 5, _table, DIGIT, STATE_FRACTION + 6);
    mark_cells(STATE_FRACTION + 6, _table, DIGIT, STATE_FRACTION + 7);
    mark_cells(STATE_FRACTION + 7, _table, ',', ',', STATE_FRACTION + 4);
    mark_cells(STATE_FRACTION + 7, _table, '.', '.', STATE_FRACTION + 8);
    mark_cells(STATE_FRACTION + 7, _table, ' ', ' ', STATE_FRACTION + 10);
    mark_cells(STATE_FRACTION + 8, _table, DIGIT, STATE_FRACTION + 9);
    mark_cells(STATE_FRACTION + 9, _table, DIGIT, STATE_FRACTION + 9);
    mark_cells(STATE_FRACTION + 9, _table, ' ', ' ', STATE_FRACTION + 10);
    mark_cells(STATE_FRACTION + 9, _table, '/', '/', STATE_FRACTION + 12);
    mark_cells(STATE_FRACTION + 10, _table, DIGIT, STATE_FRACTION + 11);
    mark_cells(STATE_FRACTION + 11, _table, DIGIT, STATE_FRACTION + 11);
    mark_cells(STATE_FRACTION + 11, _table, '/', '/', STATE_FRACTION + 12);
    mark_cells(STATE_FRACTION + 11, _table, '.', '.', STATE_FRACTION + 13);
    mark_cells(STATE_FRACTION + 12, _table, DIGIT, STATE_FRACTION + 15);
    mark_cells(STATE_FRACTION + 13, _table, DIGIT, STATE_FRACTION + 14);
    mark_cells(STATE_FRACTION + 14, _table, '/', '/', STATE_FRACTION + 12);
    mark_cells(STATE_FRACTION + 14, _table, DIGIT, STATE_FRACTION + 14);
    mark_cells(STATE_FRACTION + 15, _table, DIGIT, STATE_FRACTION + 15);
    mark_cells(STATE_FRACTION + 15, _table, '.', '.', STATE_FRACTION + 16);
    mark_cells(STATE_FRACTION + 16, _table, DIGIT, STATE_FRACTION + 17);
    mark_cells(STATE_FRACTION + 17, _table, DIGIT, STATE_FRACTION + 17);
}

void mark_table_space(int _table[][MAX_COLUMNS], int start_state) {
    // MARK SUCCESS/FAILURE
    mark_fail(_table, start_state + 0);     // fail states: +0
    mark_success(_table, start_state + 1);  // success states: +1

    // MARK CELLS
    // state [+0] --- SPACE ---> [+1]
    // state [+1] --- SPACE ---> [+1]
    mark_cells(start_state + 0, _table, SPACE, start_state + 1);
    mark_cells(start_state + 1, _table, SPACE, start_state + 1);
}

void mark_table_alpha(int _table[][MAX_COLUMNS], int start_state) {
    // MARK SUCCESS/FAILURE
    mark_fail(_table, start_state + 0);     // fail states: +0
    mark_success(_table, start_state + 1);  // success states: +1

    // MARK CELLS
    // state [+0] --- ALPHA ---> [+1]
    // state [+1] --- ALPHA ---> [+1]
    mark_cells(start_state + 0, _table, ALPHA, start_state + 1);
    mark_cells(start_state + 1, _table, ALPHA, start_state + 1);
}

void mark_table_punct(int _table[][MAX_COLUMNS], int start_state) {
    // MARK SUCCESS/FAILURE
    mark_fail(_table, start_state + 0);     // fail states: +0
    mark_success(_table, start_state + 1);  // success states: +1

    // MARK CELLS
    // state [+0] --- PUNCT ---> [+1]
    // state [+1] --- PUNCT ---> [+1]
    mark_cells(start_state + 0, _table, PUNCT, start_state + 1);
    mark_cells(start_state + 1, _table, PUNCT, start_state + 1);
}

void print_table(const int _table[][MAX_COLUMNS]) {
    int cols_per_row = 11, count = 1, value_len;

    while(count < MAX_COLUMNS) {
        // print header
        std::cout << " S ";
        std::cout << "|  " << 0 << "  ";
        for(int col = count; col < count + cols_per_row; ++col) {
            if(col < MAX_COLUMNS && (col < 32 || col > 126)) {
                value_len = std::to_string(col).length();
                if(value_len == 1) {
                    std::cout << "|  " << col << "  ";
                } else if(value_len == 2) {
                    std::cout << "|  " << col << " ";
                } else if(value_len == 3) {
                    std::cout << "| " << col << " ";
                }
            } else if(col < MAX_COLUMNS && col > 31 && col < 127) {
                std::cout << "| '" << static_cast<char>(col) << "' ";
            }
        }
        std::cout << "|" << std::endl;

        // print bar
        std::cout << "--- -----";
        for(int col = count; col < count + cols_per_row; ++col) {
            if(col < MAX_COLUMNS) {
                std::cout << " -----";
            }
        }
        std::cout << std::endl;

        // print values in array
        for(int row = 0; row < MAX_ROWS; ++row) {
            // print row number
            std::cout << std::setw(2) << std::right << row << " ";

            // print column 0
            value_len = std::to_string(_table[row][0]).length();
            if(value_len == 1) {
                std::cout << "|  " << _table[row][0] << "  ";
            } else if(value_len == 2) {
                std::cout << "|  " << _table[row][0] << " ";
            } else if(value_len == 3) {
                std::cout << "| " << _table[row][0] << " ";
            }

            for(int col = count; col < count + cols_per_row; ++col) {
                if(col < MAX_COLUMNS) {
                    value_len = std::to_string(_table[row][col]).length();
                    if(value_len == 1) {
                        std::cout << "|  " << _table[row][col] << "  ";
                    } else if(value_len == 2) {
                        std::cout << "|  " << _table[row][col] << " ";
                    } else if(value_len == 3) {
                        std::cout << "| " << _table[row][col] << " ";
                    }
                }
            }
            std::cout << "|" << std::endl;
        }
        std::cout << std::endl;

        count += cols_per_row;  // udpate count
    }
}

// _posstarts from indexing from 1, not 0?
void show_string(const char s[], int _pos) {
    // assert positive position
    assert(_pos > -1);

    // print the string and carat as pointing to the position in string
    int index = 0;
    std::cout << s << std::endl;
    for(index = 0; s[index] != '\0'; ++index) {
        if(index == _pos) {
            std::cout << '^';
        } else {
            std::cout << ' ';
        }
    }

    // print carat behind string when pos is pointing at NUL
    if(s[_pos] == '\0') {
        std::cout << '^';
    }
    std::cout << std::endl;

    // assert that pos is less than index
    assert(_pos <= index);
}

bool get_token(const int _table[][MAX_COLUMNS], const char input[], int &_pos,
               int state, std::string &token) {
    bool peek_success = false,     // success state at peeking at next state
        success = false;           // success state
    char peek_char = '\0';         // peek at next character
    int last_successful_pos = -1,  // last successful position
        original_pos = _pos,       // store original position
        peek_state = -1;           // peek at next state
    std::string new_token;         // token for last full success

    // loop until end of string and when state is not -1
    while(input[_pos] > -1 && input[_pos] != '\0' &&
          _table[state][(input[_pos])] != -1) {
        // get new state
        state = _table[state][input[_pos]];

        // get peek character, then get peek state, and then get peek success
        peek_char = input[_pos + 1];
        peek_state = _table[state][peek_char];
        peek_success = _table[peek_state][0];

        // udpate new token when peeking at next success is fail or when NUL
        if(is_success(_table, state) &&
           (peek_char == '\0' || peek_state == -1 || !peek_success)) {
            new_token.clear();
            last_successful_pos = _pos;
            success = true;
        }

        ++_pos;
    }

    // create new token when last successful pos (default = -1) is >=0
    for(int i = original_pos; i < last_successful_pos + 1; ++i) {
        new_token += input[i];
    }

    // if success, return new token and new position after last successful pos
    // else, return original position via reference
    if(success) {
        token = new_token;
        _pos = last_successful_pos + 1;
    } else {
        _pos = original_pos;
    }

    return success;
}

}  // namespace state_machine
