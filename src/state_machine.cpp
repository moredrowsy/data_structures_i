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
    bool debug = false;

    bool peek_success = false,     // success state at peeking at next state
        success_state = false;     // success state
    char peek_char = '\0';         // peek at next character
    int last_successful_pos = -1,  // last successful position
        original_pos = _pos,       // store original position
        peek_state = -1;           // peek at next state
    std::string new_token;         // token for last full success
    if(debug) {
        print_table(_table);
        std::cout << std::endl
                  << "----" << std::endl
                  << "string is = " << input << std::endl
                  << "----" << std::endl;
        std::cout << "original state = " << state << ", current pos = " << _pos
                  << ", current char = '" << input[_pos] << "'"
                  << ", is success = " << is_success(_table, state)
                  << std::endl;
    }

    // loop until end of string and when state is not -1
    while(input[_pos] != '\0' && _table[state][(input[_pos])] != -1) {
        if(debug) {
            std::cout << "-----" << std::endl;
            std::cout << "original state = " << state
                      << ", current pos = " << _pos << ", current char = '"
                      << input[_pos] << "'" << std::endl;
        }

        // get new state
        state = _table[state][input[_pos]];

        // get peek character, then get peek state, and then get peek success
        peek_char = input[_pos + 1];
        peek_state = _table[state][peek_char];
        peek_success = _table[peek_state][0];

        if(debug) {
            std::cout << "new state = " << state << ", peek char = ";
            if(peek_char == '\0') {
                std::cout << "'NUL'";
            } else {
                std::cout << "'" << peek_char << "'";
            }
            std::cout << ", success = " << is_success(_table, state);
            std::cout << ", peek state = " << peek_state;

            std::cout << ", peek success = " << peek_success << std::endl;
        }

        // udpate new token when peeking at next success is fail or when NUL
        if(is_success(_table, state) &&
           (peek_char == '\0' || peek_state == -1 || !peek_success)) {
            new_token = "";
            last_successful_pos = _pos;
            success_state = true;

            if(debug) {
                std::cout << "new token creation" << std::endl;
            }
        }

        ++_pos;
    }

    // create new token when last successful pos (default = -1) is >=0
    for(int i = original_pos; i < last_successful_pos + 1; ++i) {
        new_token += input[i];
    }

    if(debug) {
        std::cout << "new token = " << new_token << std::endl;
    }

    // if new_token is empty, return original position via reference
    // else, return new token and pos stays updated via reference
    if(new_token.empty()) {
        _pos = original_pos;
    } else {
        token = new_token;
        _pos = last_successful_pos + 1;
    }

    return success_state;
}

}  // namespace state_machine
