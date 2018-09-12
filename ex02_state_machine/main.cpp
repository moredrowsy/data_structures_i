#include <iostream>
#include "../include/state_machine.h"

using namespace state_machine;

int main() {
    int table[MAX_ROWS][MAX_COLUMNS];

    // init table with -1
    init_table(table);

    // doubles:
    mark_fail(table, 0);     // Mark states 0 and 2 as fail states
    mark_success(table, 1);  // Mark states 1 and 3 as success states
    mark_fail(table, 2);
    mark_success(table, 3);

    mark_cells(0, table, DIGITS, 1);    // state [0] --- DIGITS ---> [1]
    mark_cells(0, table, '.', '.', 2);  // state [0] --- '.' ------> [2]
    mark_cells(1, table, DIGITS, 1);    // state [1] --- DIGITS ---> [1]
    mark_cells(1, table, '.', '.', 2);  // state [1] --- '.' ------> [2]
    mark_cells(2, table, DIGITS, 3);    // state [2] --- DIGITS ---> [3]
    mark_cells(3, table, DIGITS, 3);    // state [3] --- DIGITS ---> [3]

    print_table(table);

    bool success = false;
    char c[] = ".3.";
    std::string token;
    int pos = 0;

    show_string(c, pos);

    success = get_token(table, c, pos, STATE_DOUBLE, token);
    std::cout << "next pos: " << pos << std::endl;
    std::cout << "token returned: " << token << std::endl;
    std::cout << "success state: " << success << std::endl;

    show_string(c, pos);

    return 0;
}
