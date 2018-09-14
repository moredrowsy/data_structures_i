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

// test boolean, token, and pos is correctly returned for PUNCTUATION
void test_get_token_punct();

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

    // mark table for STATE_DOUBLE
    mark_table_double(_table, STATE_DOUBLE);

    // test variables
    const int TEST_SIZE = 19;
    bool success = false;
    int state = STATE_DOUBLE;
    std::string token;

    const char *test_strings[] = {"",      ".",       "A",         "1",
                                  ".1",    ".1.",     "0.1",       "0.1.",
                                  "1.",    "1.23",    "1.23.",     "1.23..4",
                                  "1,000", "10,000",  "100,000",   "1,000,000",
                                  "10,0",  "1,000,0", "0123456789"};
    const char *test_tokens[] = {
        "",        "",          "",     "1",     ".1",        ".1",    "0.1",
        "0.1",     "1",         "1.23", "1.23",  "1.23",      "1,000", "10,000",
        "100,000", "1,000,000", "10",   "1,000", "0123456789"};
    bool test_success[TEST_SIZE] = {false, false, false, true, true, true, true,
                                    true,  true,  true,  true, true, true, true,
                                    true,  true,  true,  true, true};
    int test_start_pos[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0};
    int test_after_pos[TEST_SIZE] = {0, 0, 0, 1, 2, 2, 3, 3, 1, 4,
                                     4, 4, 5, 6, 7, 9, 2, 5, 10};

    for(int i = 0; i < TEST_SIZE; ++i) {
        token = "";
        success =
            get_token(_table, test_strings[i], test_start_pos[i], state, token);
        assert(success == test_success[i]);
        assert(token == test_tokens[i]);
        assert(test_start_pos[i] == test_after_pos[i]);
    }

    std::cout << "passed." << std::endl;
}

void test_get_token_space() {
    using namespace state_machine;

    std::cout << "test_get_token_space: ";

    int _table[MAX_ROWS][MAX_COLUMNS];

    // initialize table with -1
    init_table(_table);

    // mark table for STATE_SPACE
    mark_table_generic(_table, STATE_SPACE, SPACE);

    // test variables
    const int TEST_SIZE = 7;
    bool success = false;
    int state = STATE_SPACE;
    std::string token;

    const char *test_strings[] = {"", ".", "A", "1", " ", "   A", ".   "};
    const char *test_tokens[] = {"", "", "", "", " ", "   ", "   "};
    bool test_success[TEST_SIZE] = {false, false, false, false,
                                    true,  true,  true};
    int test_start_pos[] = {0, 0, 0, 0, 0, 0, 1};
    int test_after_pos[TEST_SIZE] = {0, 0, 0, 0, 1, 3, 4};

    for(int i = 0; i < TEST_SIZE; ++i) {
        token = "";
        success =
            get_token(_table, test_strings[i], test_start_pos[i], state, token);
        assert(success == test_success[i]);
        assert(token == test_tokens[i]);
        assert(test_start_pos[i] == test_after_pos[i]);
    }

    std::cout << "passed." << std::endl;
}

void test_get_token_alpha() {
    using namespace state_machine;

    std::cout << "test_get_token_alpha: ";

    int _table[MAX_ROWS][MAX_COLUMNS];

    // initialize table with -1
    init_table(_table);

    // mark table for STATE_ALPHA
    mark_table_generic(_table, STATE_ALPHA, ALPHA);

    // test variables
    const int TEST_SIZE = 8;
    bool success = false;
    int state = STATE_ALPHA;
    std::string token;

    const char *test_strings[] = {"",
                                  ".",
                                  "A",
                                  "1",
                                  " ",
                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                  "abcdefghijklmnopqrstuvwxyz",
                                  " This!"};
    const char *test_tokens[] = {"",
                                 "",
                                 "A",
                                 "",
                                 "",
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                 "abcdefghijklmnopqrstuvwxyz",
                                 "This"};
    bool test_success[TEST_SIZE] = {false, false, true, false,
                                    false, true,  true, true};
    int test_start_pos[] = {0, 0, 0, 0, 0, 0, 0, 1};
    int test_after_pos[TEST_SIZE] = {0, 0, 1, 0, 0, 26, 26, 5};

    for(int i = 0; i < TEST_SIZE; ++i) {
        token = "";
        success =
            get_token(_table, test_strings[i], test_start_pos[i], state, token);
        assert(success == test_success[i]);
        assert(token == test_tokens[i]);
        assert(test_start_pos[i] == test_after_pos[i]);
    }

    std::cout << "passed." << std::endl;
}

void test_get_token_punct() {
    using namespace state_machine;

    std::cout << "test_get_token_punct: ";

    int _table[MAX_ROWS][MAX_COLUMNS];

    // initialize table with -1
    init_table(_table);

    // mark table for STATE_PUNCT
    mark_table_generic(_table, STATE_PUNCT, PUNCT);

    // test variables
    const int TEST_SIZE = 6;
    bool success = false;
    int state = STATE_PUNCT;
    std::string token;

    const char *test_strings[] = {"", ".", "A", "1", " ", "??!! "};
    const char *test_tokens[] = {"", ".", "", "", "", "??!!"};
    bool test_success[TEST_SIZE] = {false, true, false, false, false, true};
    int test_start_pos[] = {0, 0, 0, 0, 0, 0};
    int test_after_pos[TEST_SIZE] = {0, 1, 0, 0, 0, 4};

    for(int i = 0; i < TEST_SIZE; ++i) {
        token = "";
        success =
            get_token(_table, test_strings[i], test_start_pos[i], state, token);
        assert(success == test_success[i]);
        assert(token == test_tokens[i]);
        assert(test_start_pos[i] == test_after_pos[i]);
    }

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
    test_get_token_punct();
}

}  // namespace test_state_machine

#endif  // TEST_STATE_MACHINE_H
