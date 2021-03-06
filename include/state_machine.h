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

// WARNING: MAKE SURE EACH STATE DO NOT OVERLAP IN ROWS!!!
enum STATES {
    STATE_ERROR = -1,             // unknown state
    STATE_DOUBLE = 5,             // uses 10 rows
    STATE_ALPHA = 20,             // uses 2 rows
    STATE_SPACE = 25,             // uses 2 rows
    STATE_PUNCT = 30,             // uses 2 rows
    STATE_IDENT = 34,             // identifier states
    STATE_IDENT_NORM = 35,        // uses 2 rows
    STATE_IDENT_QUOTE = 39,       // ident in quotes
    STATE_IN_QUOTE_S_IDENT = 40,  // uses 4 rows
    STATE_IN_QUOTE_D_IDENT = 45,  // uses 4 rows
    STATE_VALUE = 49,             // enclosed quote states
    STATE_IN_QUOTE_S = 50,        // use 3 rows
    STATE_IN_QUOTE_D = 55,        // use 3 rows
    STATE_R_OP = 60,              // uses 3 rows - relational operator states
    STATE_SIZE = 65               // end size
};

// GLOBAL CONSTANTS
const int MAX_COLUMNS = 256, MAX_ROWS = STATE_SIZE;
const char ALPHA[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char DIGIT[] = "0123456789";
const char SPACE[] = " \n\r\t\v";
const char COMMA[] = ",";
const char STAR[] = "*";
const char R_OPS[] = "<=>";
const char PUNCT[] = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";

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

// mark table for generic states
void unmark_table_generic(int _table[][MAX_COLUMNS], int state,
                          const char columns[]);

void mark_table_single_char(int _table[][MAX_COLUMNS], int state,
                            const char character);

void mark_table_single_char(int _table[][MAX_COLUMNS], int state,
                            const char columns[]);

// mark table for two char relations
void mark_table_duo_chars(int _table[][MAX_COLUMNS], int state, const char a,
                          const char b);

// mark table for enclosure by delimiters
void mark_table_enclosed_delim(int _table[][MAX_COLUMNS], int state,
                               const char delim);

// mark table for enclosure by delimiters
void mark_table_enclosed_delim_ident(int _table[][MAX_COLUMNS], int state,
                                     const char delim);

// mark table for STATE_DOUBLE
void mark_table_double(int _table[][MAX_COLUMNS], int state);

// mark table for STATE_IDENTIFIER
void mark_table_identifier(int _table[][MAX_COLUMNS], int state);

void mark_table_r_ops(int _table[][MAX_COLUMNS], int state);

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
