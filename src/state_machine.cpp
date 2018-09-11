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
    // assign [row as state][column 0] to 1 as success
    _table[state][0] = 1;
}

void mark_fail(int _table[][MAX_COLUMNS], int state) {
    // assign [row as state][column 0] to 0 as fail
    _table[state][0] = 0;
}

bool is_success(int _table[][MAX_COLUMNS], int state) {
    return _table[state][0];
}

void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to,
                int state) {
    for(int col = from; col < to; ++col) {
        _table[row][col] = state;
    }
}

void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[],
                int state) {
    for(int i = 0; columns[i] != '\0'; ++i) {
        _table[row][static_cast<int>(columns[i])] = state;
    }
}

void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state) {
    _table[row][column] = state;
}

void print_table(int _table[][MAX_COLUMNS]) {
    int cols_per_row = 12, count = 1, value_len;

    while(count < MAX_COLUMNS) {
        // print header
        std::cout << " S ";
        for(int col = count; col < count + cols_per_row; ++col) {
            if(col < MAX_COLUMNS) {
                value_len = std::to_string(col).length();
                if(value_len == 1) {
                    std::cout << "|  " << col << "  ";
                } else if(value_len == 2) {
                    std::cout << "|  " << col << " ";
                } else if(value_len == 3) {
                    std::cout << "| " << col << " ";
                }
            }
        }
        std::cout << "|" << std::endl;

        // print bar
        std::cout << "---";
        for(int col = count; col < count + cols_per_row; ++col) {
            if(col < MAX_COLUMNS) {
                std::cout << " -----";
            }
        }
        std::cout << std::endl;

        // print values in array
        for(int row = 0; row < MAX_ROWS; ++row) {
            std::cout << std::setw(2) << std::right << row << " ";
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
void show_string(char s[], int _pos) {
    // assert pos is not negative
    assert(_pos > 0);

    // print the string and carat as pointing to the position in string
    int index = 0;
    std::cout << s << std::endl;
    for(index = 0; s[index] != '\0'; ++index) {
        if(index == _pos - 1) {
            std::cout << '^';
        } else {
            std::cout << ' ';
        }
    }
    std::cout << std::endl;

    // assert that pos is less than index
    if(_pos > index) {
        assert(_pos < index);
    }
}

}  // namespace state_machine
