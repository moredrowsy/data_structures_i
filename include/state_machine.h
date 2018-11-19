/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : state_machine
 * DESCRIPTION : This header declares lower level functions to handle the state
 *      machine's adjacency table: initializes the table, mark success/fail
 *      to the table, and mark table's cells to given state.
 ******************************************************************************/
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <cassert>   // assertions
#include <iomanip>   // stream formatting
#include <iostream>  // stream objects
#include <string>    // string

namespace state_machine {

// GLOBAL CONSTANTS
const int MAX_COLUMNS = 256, MAX_ROWS = 50;
const char ALPHA[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char DIGIT[] = "0123456789";
const char SPACE[] = " \n\r\t\v";
const char COMMA[] = ",";
const char STAR[] = "*";
const char QUOTE_S[] = "'";
const char QUOTE_D[] = "\"";
const char PUNCT[] = "!\"#$%&\'()+-./:;<=>?@[\\]^_`{|}~";

// WARNING: MAKE SURE EACH STATE DO NOT OVERLAP IN ROWS!!!
const int STATE_UNKNOWN = -1;  // unknown state
const int STATE_DOUBLE = 5;    // allocate 10 rows
const int STATE_ALPHA = 20;    // allocate 2 rows
const int STATE_SPACE = 32;    // allocate 2 rows
const int STATE_COMMA = 22;    // allocate 2 rows
const int STATE_STAR = 24;     // allocate 2 rows
const int STATE_QUOTE_S = 26;  // allocate 2 rows
const int STATE_QUOTE_D = 28;  // allocate 2 rows
const int STATE_PUNCT = 30;    // allocate 2 rows

// fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]);

// mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state);

// mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state);

// true if state is a success state
bool is_success(const int _table[][MAX_COLUMNS], int state);

// mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to,
                int state);

// mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[],
                int state);

// mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state);

// mark table for generic states
void mark_table_generic(int _table[][MAX_COLUMNS], int state,
                        const char columns[]);

// mark table for STATE_DOUBLE
void mark_table_double(int _table[][MAX_COLUMNS], int state);

// this can realistically be used on a small table
void print_table(const int _table[][MAX_COLUMNS]);

// show string s and mark this position on the string:
// hello world   pos: 7
//      ^
void show_string(const char s[], int _pos);

// get a token from string, return boolean on success
// on return true, by reference, gives next pos and good token
// on return false, by reference, gives original pos, and last good token
bool get_token(const int _table[][MAX_COLUMNS], const char input[], int &_pos,
               int state, std::string &token);

}  // namespace state_machine

#endif  // STATE_MACHINE_H
