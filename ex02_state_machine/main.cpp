#include <iostream>
#include "../include/state_machine.h"

int main() {
    int table[state_machine::MAX_ROWS][state_machine::MAX_COLUMNS];

    // init table with -1
    state_machine::init_table(table);

    // mark success/fail states in column 0
    state_machine::mark_fail(table, 0);
    state_machine::mark_success(table, 1);
    state_machine::mark_fail(table, 2);
    state_machine::mark_success(table, 3);

    state_machine::print_table(table);

    return 0;
}
