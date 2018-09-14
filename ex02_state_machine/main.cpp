#include <iostream>
#include "../include/state_machine.h"

using namespace state_machine;

int main() {
    int _table[MAX_ROWS][MAX_COLUMNS];

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

    print_table(_table);

    bool success = false;
    char c[] = "0.314 1.248.";
    std::string token;
    int pos = 0;

    std::cout << "Input string is with starting position:" << std::endl;
    show_string(c, pos);
    std::cout << std::endl;

    std::cout << "First call:" << std::endl;
    std::cout << "current pos = " << pos;
    if(get_token(_table, c, pos, STATE_DOUBLE, token)) {
        std::cout << ", token returned = |" << token << "| ";
        std::cout << ", next pos = " << pos << std::endl;
    }

    std::cout << "\nSecond call:" << std::endl;
    std::cout << "current pos = " << pos;
    if(get_token(_table, c, pos, STATE_SPACE, token)) {
        std::cout << ", token returned = |" << token << "| ";
        std::cout << ", next pos = " << pos << std::endl;
    }

    std::cout << "\nThird call:" << std::endl;
    std::cout << "current pos = " << pos;
    if(get_token(_table, c, pos, STATE_DOUBLE, token)) {
        std::cout << ", token returned = |" << token << "| ";
        std::cout << ", next pos = " << pos << std::endl;
    }

    std::cout << "\nInput string with ending position: " << std::endl;
    show_string(c, pos);

    return 0;
}
