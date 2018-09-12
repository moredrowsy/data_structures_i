#include <iostream>
#include "../include/state_machine.h"

using namespace state_machine;

int main() {
    int _table[MAX_ROWS][MAX_COLUMNS];

    // initialize table with -1
    init_table(_table);

    // STATE_DOUBLE
    mark_fail(_table, 0);     // fail states: 0 and 2
    mark_success(_table, 1);  // success states: 1 and 3
    mark_fail(_table, 2);
    mark_success(_table, 3);

    mark_cells(0, _table, DIGITS, 1);    // state [0] --- DIGITS ---> [1]
    mark_cells(0, _table, '.', '.', 2);  // state [0] --- '.' ------> [2]
    mark_cells(1, _table, DIGITS, 1);    // state [1] --- DIGITS ---> [1]
    mark_cells(1, _table, '.', '.', 2);  // state [1] --- '.' ------> [2]
    mark_cells(2, _table, DIGITS, 3);    // state [2] --- DIGITS ---> [3]
    mark_cells(3, _table, DIGITS, 3);    // state [3] --- DIGITS ---> [3]

    // STATE_SPACE
    mark_fail(_table, 10);     // fail states: 10
    mark_success(_table, 11);  // success states: 11

    mark_cells(10, _table, SPACE, 11);  // state [10] --- SPACE ---> [11]
    mark_cells(11, _table, SPACE, 11);  // state [11] --- SPACE ---> [11]

    // print_table(_table);

    bool success = false;
    char c[] = "0.314 1.248.";
    std::string token;
    int pos = 0;

    show_string(c, pos);

    if(get_token(_table, c, pos, STATE_DOUBLE, token)) {
        std::cout << "next pos: " << pos << std::endl;
        std::cout << "token returned: |" << token << "|" << std::endl;
    }

    if(get_token(_table, c, pos, STATE_SPACE, token)) {
        std::cout << "next pos: " << pos << std::endl;
        std::cout << "token returned: |" << token << "|" << std::endl;
    }

    show_string(c, pos);

    return 0;
}
