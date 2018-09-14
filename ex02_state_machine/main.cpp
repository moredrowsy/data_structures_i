#include <iostream>
#include "../include/state_machine.h"

using namespace state_machine;

int main() {
    int _table[MAX_ROWS][MAX_COLUMNS];

    // initialize table with -1
    init_table(_table);

    // mark table for STATE_FRACTION
    mark_table_fraction(_table, STATE_FRACTION);

    // mark table for STATE_DOUBLE
    mark_table_double(_table, STATE_DOUBLE);

    // mark table for STATE_SPACE
    mark_table_generic(_table, STATE_SPACE, SPACE);

    // mark table for STATE_ALPHA
    mark_table_generic(_table, STATE_ALPHA, ALPHA);

    // mark table for STATE_PUNCT
    mark_table_generic(_table, STATE_PUNCT, PUNCT);

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
