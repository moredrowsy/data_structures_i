/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * EXERCISE 02 : State Machine
 * DESCRIPTION : This application tests the state machine functions to generate
 *      an adjacency table rules for the get_token to extract a token from a
 *      a string with a given starting state and position in the string.
 ******************************************************************************/
#include <iostream>                    // stream objects
#include <string>                      // string objects
#include "../include/state_machine.h"  // state_machine functions

using namespace state_machine;

int main() {
    int _table[MAX_ROWS][MAX_COLUMNS];

    // initialize table with -1
    init_table(_table);

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
