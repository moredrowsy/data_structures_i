#ifndef TEST_STATE_MACHINE_H
#define TEST_STATE_MACHINE_H

#include <cassert>
#include <iostream>
#include "../include/state_machine.h"

namespace test_state_machine {

// test init table with all values init to -1
void test_init_table();

// test row = state and col = 0 is marked with success 1
void test_mark_success();

// test row = state and col = 0 is marked with fail 0
void test_mark_fail();

// test if boolean returns as expected
void test_is_success();

// test a range of cells in row is marked with state
void test_mark_cells();

// test boolean, token, and pos is correctly returned for DOUBLE
void test_get_token_double();

// test boolean, token, and pos is correctly returned for SPACE
void test_get_token_space();

// test boolean, token, and pos is correctly returned for ALPHABET
void test_get_token_alpha();

// call all test functions
void test_all();

void test_init_table() {
    using namespace state_machine;

    std::cout << "test_init_table: ";

    int table[MAX_ROWS][MAX_COLUMNS];

    // assert that all cells are -1
    init_table(table);
    for(int row = 0; row < MAX_ROWS; ++row) {
        for(int col = 0; col < MAX_COLUMNS; ++col) {
            assert(table[row][col] == -1);
        }
    }

    std::cout << "passed." << std::endl;
}

void test_mark_success() {
    using namespace state_machine;

    std::cout << "test_mark_success: ";

    int table[MAX_ROWS][MAX_COLUMNS];
    int state = 0;

    // assert that row = state and col = 0 is -1
    init_table(table);
    assert(table[0][0] == -1);

    // assert that row = state and col = 0 is now 1
    mark_success(table, state);
    assert(table[state][0] == 1);

    std::cout << "passed." << std::endl;
}

void test_mark_fail() {
    using namespace state_machine;

    std::cout << "test_mark_fail: ";

    int table[MAX_ROWS][MAX_COLUMNS];
    int state = 0;

    // assert that row = state and col = 0 is -1
    init_table(table);
    assert(table[0][0] == -1);

    // assert that row = state and col = 0 is now 1
    mark_fail(table, state);
    assert(table[state][0] == 0);

    std::cout << "passed." << std::endl;
}

void test_is_success() {
    using namespace state_machine;

    std::cout << "test_is_success: ";

    int table[MAX_ROWS][MAX_COLUMNS];
    int state1 = 0, state2 = 1;

    // assert that row = state and col = 0 is -1
    init_table(table);
    assert(table[0][0] == -1);

    // assert that row = state and col = 0 is now 1
    mark_success(table, state1);
    assert(table[state1][0] == 1);
    assert(is_success(table, state1) == true);

    // assert that row = state and col = 0 is now 1
    mark_fail(table, state2);
    assert(table[state2][0] == 0);
    assert(is_success(table, state2) == false);

    std::cout << "passed." << std::endl;
}

void test_mark_cells() {
    using namespace state_machine;

    std::cout << "test_mark_cells: ";

    int table[MAX_ROWS][MAX_COLUMNS];
    char columns[] = " .,19AZaz";

    // assert that row = state and col = 0 is -1
    init_table(table);
    assert(table[0][0] == -1);

    // assertion on mark_cells range function for single column
    // assert that row 0, column 10 is marked with state 0
    mark_cells(0, table, 10, 10, 0);
    assert(table[0][10] == 0);

    // assert that on mark_cell function for single column
    // assert that row 0, column 11 is marked with state 0
    mark_cell(0, table, 11, 0);
    assert(table[0][11] == 0);

    // assert row 1, column 20 to 29 is marked with state 1
    mark_cells(1, table, 20, 29, 1);
    for(int i = 20; i <= 29; ++i) {
        assert(table[1][i] == 1);
    }

    // assert that row 2 with columns array is marked with stated 2
    mark_cells(2, table, columns, 2);
    for(int i = 0; columns[i] != '\0'; ++i) {
        assert(table[2][columns[i]] == 2);
    }

    std::cout << "passed." << std::endl;
}

void test_get_token_double() {
    using namespace state_machine;

    std::cout << "test_get_token_double: ";

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

    // test variables
    bool success_state = false;
    std::string token;
    int pos = 0, state = STATE_DOUBLE;

    // test string01
    char string01[] = "";
    pos = 0;
    token = "";
    success_state = get_token(_table, string01, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string02
    char string02[] = ".";
    pos = 0;
    token = "";
    success_state = get_token(_table, string02, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string03
    char string03[] = "A";
    pos = 0;
    token = "";
    success_state = get_token(_table, string03, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string04
    char string04[] = "3";
    pos = 0;
    token = "";
    success_state = get_token(_table, string04, pos, state, token);
    assert(success_state == true);
    assert(token == "3");
    assert(pos == 1);

    // test string05
    char string05[] = ".3";
    pos = 0;
    token = "";
    success_state = get_token(_table, string05, pos, state, token);
    assert(success_state == true);
    assert(token == ".3");
    assert(pos == 2);

    // test string06
    char string06[] = ".3.";
    pos = 0;
    token = "";
    success_state = get_token(_table, string06, pos, state, token);

    assert(success_state == true);
    assert(token == ".3");
    assert(pos == 2);

    // test string07
    char string07[] = "0.3";
    pos = 0;
    token = "";
    success_state = get_token(_table, string07, pos, state, token);
    assert(success_state == true);
    assert(token == "0.3");
    assert(pos == 3);

    // test string08
    char string08[] = "0.3.";
    pos = 0;
    token = "";
    success_state = get_token(_table, string08, pos, state, token);
    assert(success_state == true);
    assert(token == "0.3");
    assert(pos == 3);

    // test string09
    char string09[] = "3.";
    pos = 0;
    token = "";
    success_state = get_token(_table, string09, pos, state, token);
    assert(success_state == true);
    assert(token == "3");
    assert(pos == 1);

    // test string10
    char string10[] = "3.14";
    pos = 0;
    token = "";
    success_state = get_token(_table, string10, pos, state, token);
    assert(success_state == true);
    assert(token == "3.14");
    assert(pos == 4);

    // test string11
    char string11[] = "3.14.";
    pos = 0;
    token = "";
    success_state = get_token(_table, string11, pos, state, token);
    assert(success_state == true);
    assert(token == "3.14");
    assert(pos == 4);

    // test string12
    char string12[] = "3.14..14";
    pos = 0;
    token = "";
    success_state = get_token(_table, string12, pos, state, token);
    assert(success_state == true);
    assert(token == "3.14");
    assert(pos == 4);

    // test string11
    char string13[] = "0.31.14..14";
    pos = 0;
    token = "";
    success_state = get_token(_table, string13, pos, state, token);
    assert(success_state == true);
    assert(token == "0.31");
    assert(pos == 4);

    std::cout << "passed." << std::endl;
}

void test_get_token_space() {
    using namespace state_machine;

    std::cout << "test_get_token_space: ";

    int _table[MAX_ROWS][MAX_COLUMNS];

    // initialize table with -1
    init_table(_table);

    // STATE_SPACE
    mark_fail(_table, STATE_SPACE + 0);     // fail states: 10
    mark_success(_table, STATE_SPACE + 1);  // success states: 11
    // MARK CELLS
    // state [10] --- SPACE ---> [11]
    // state [11] --- SPACE ---> [11]
    mark_cells(STATE_SPACE + 0, _table, SPACE, STATE_SPACE + 1);
    mark_cells(STATE_SPACE + 1, _table, SPACE, STATE_SPACE + 1);

    // test variables
    bool success_state = false;
    std::string token;
    int pos = 0, state = STATE_SPACE;

    // test string01
    char string01[] = "";
    pos = 0;
    token = "";
    success_state = get_token(_table, string01, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string02
    char string02[] = ".";
    pos = 0;
    token = "";
    success_state = get_token(_table, string02, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string03
    char string03[] = "A";
    pos = 0;
    token = "";
    success_state = get_token(_table, string03, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string04
    char string04[] = "3";
    pos = 0;
    token = "";
    success_state = get_token(_table, string04, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string05
    char string05[] = " ";
    pos = 0;
    token = "";
    success_state = get_token(_table, string05, pos, state, token);
    assert(success_state == true);
    assert(token == " ");
    assert(pos == 1);

    // test string06
    char string06[] = "     ";
    pos = 0;
    token = "";
    success_state = get_token(_table, string06, pos, state, token);
    assert(success_state == true);
    assert(token == "     ");
    assert(pos == 5);

    // test string07
    char string07[] = "     .";
    pos = 0;
    token = "";
    success_state = get_token(_table, string07, pos, state, token);
    assert(success_state == true);
    assert(token == "     ");
    assert(pos == 5);

    // test string08
    char string08[] = "     1";
    pos = 0;
    token = "";
    success_state = get_token(_table, string08, pos, state, token);
    assert(success_state == true);
    assert(token == "     ");
    assert(pos == 5);

    // test string09
    char string09[] = "     A";
    pos = 0;
    token = "";
    success_state = get_token(_table, string09, pos, state, token);
    assert(success_state == true);
    assert(token == "     ");
    assert(pos == 5);

    // test string10
    char string10[] = "     z";
    pos = 0;
    token = "";
    success_state = get_token(_table, string10, pos, state, token);
    assert(success_state == true);
    assert(token == "     ");
    assert(pos == 5);

    // test string11
    char string11[] = ".    z";
    pos = 0;
    token = "";
    success_state = get_token(_table, string11, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string12
    char string12[] = " .014 A";
    pos = 5;
    token = "";
    success_state = get_token(_table, string12, pos, state, token);
    assert(success_state == true);
    assert(token == " ");
    assert(pos == 6);

    std::cout << "passed." << std::endl;
}

void test_get_token_alpha() {
    using namespace state_machine;

    std::cout << "test_get_token_alpha: ";

    int _table[MAX_ROWS][MAX_COLUMNS];

    // initialize table with -1
    init_table(_table);

    // STATE_ALPHA
    mark_fail(_table, STATE_ALPHA + 0);     // fail states: 20
    mark_success(_table, STATE_ALPHA + 1);  // success states: 21
    // MARK CELLS
    // state [20] --- ALPHA ---> [21]
    // state [21] --- ALPHA ---> [21]
    mark_cells(STATE_ALPHA + 0, _table, ALPHA, STATE_ALPHA + 1);
    mark_cells(STATE_ALPHA + 1, _table, ALPHA, STATE_ALPHA + 1);

    // test variables
    bool success_state = false;
    std::string token;
    int pos = 0, state = STATE_ALPHA;

    // test string01
    char string01[] = "";
    pos = 0;
    token = "";
    success_state = get_token(_table, string01, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string02
    char string02[] = ".";
    pos = 0;
    token = "";
    success_state = get_token(_table, string02, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string03
    char string03[] = "A";
    pos = 0;
    token = "";
    success_state = get_token(_table, string03, pos, state, token);
    assert(success_state == true);
    assert(token == "A");
    assert(pos == 1);

    // test string04
    char string04[] = "3";
    pos = 0;
    token = "";
    success_state = get_token(_table, string04, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string05
    char string05[] = " ";
    pos = 0;
    token = "";
    success_state = get_token(_table, string05, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string06
    char string06[] = " .014 A";
    pos = 6;
    token = "";
    success_state = get_token(_table, string06, pos, state, token);
    assert(success_state == true);
    assert(token == "A");
    assert(pos == 7);

    // test string07
    char string07[] = ".A";
    pos = 0;
    token = "";
    success_state = get_token(_table, string07, pos, state, token);
    assert(success_state == false);
    assert(token == "");
    assert(pos == 0);

    // test string08
    char string08[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    pos = 0;
    token = "";
    success_state = get_token(_table, string08, pos, state, token);
    assert(success_state == true);
    assert(token == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    assert(pos == 26);

    // test string09
    char string09[] = "abcdefghijklmnopqrstuvwxyz";
    pos = 0;
    token = "";
    success_state = get_token(_table, string09, pos, state, token);
    assert(success_state == true);
    assert(token == "abcdefghijklmnopqrstuvwxyz");
    assert(pos == 26);

    // test string10
    char string10[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    pos = 0;
    token = "";
    success_state = get_token(_table, string10, pos, state, token);
    assert(success_state == true);
    assert(token == "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    assert(pos == 52);

    // test string11
    char string11[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";
    pos = 0;
    token = "";
    success_state = get_token(_table, string11, pos, state, token);
    assert(success_state == true);
    assert(token == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    assert(pos == 26);

    std::cout << "passed." << std::endl;
}

void test_all() {
    test_init_table();
    test_mark_success();
    test_mark_fail();
    test_is_success();
    test_mark_cells();
    test_get_token_double();
    test_get_token_space();
    test_get_token_alpha();
}

}  // namespace test_state_machine

#endif  // TEST_STATE_MACHINE_H
