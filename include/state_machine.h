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

#include <string>  // provides string

namespace state_machine {

// GLOBAL CONSTANTS
const int MAX_COLUMNS = 256, MAX_ROWS = 60;
const char DIGIT[] = "0123456789";
const char SPACE[] = " \n\r\t\v";
const char ALPHA[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char PUNCT[] = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";

// WARNING: MAKE SURE EACH STATE DO NOT OVERLAP IN ROWS!!!
const int STATE_UNKNOWN = -1;  // unknown state
const int STATE_FRACTION = 0;  // allocate 20 rows
const int STATE_DOUBLE = 20;   // allocate 10 rows
const int STATE_SPACE = 30;    // allocate 10 rows
const int STATE_ALPHA = 40;    // allocate 10 rows
const int STATE_PUNCT = 50;    // allocate 10 rows

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
void mark_table_generic(int _table[][MAX_COLUMNS], int start_state,
                        const char columns[]);

// mark table for STATE_DOUBLE
void mark_table_double(int _table[][MAX_COLUMNS], int start_state);

// mark table for STATE_FRACTION
void mark_table_fraction(int _table[][MAX_COLUMNS], int start_state);

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
