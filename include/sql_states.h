#ifndef SQL_CONSTS_H
#define SQL_CONSTS_H

#include <cassert>   //assert()
#include <iomanip>   // setw()
#include <iostream>  // stream objects

namespace sql {

enum STATES { STATE_COMMAND = 0, STATE_SELECT = 10 };

enum SELECT_STATES {
    SELECT_START = STATE_SELECT + 0,
    SELECT_ASTERISK = STATE_SELECT + 1,
    SELECT_FROM = STATE_SELECT + 2,
    SELECT_STRING = STATE_SELECT + 4,
    SELECT_COMMA = STATE_SELECT + 5,
    SELECT_END = STATE_SELECT + 3
};

enum ROWS { MAX_ROWS = 50 };

enum COLUMNS {
    ERROR = -1,
    SUCCESS,
    SELECT,
    FROM,
    COMMA,
    ASTERISK,
    QUOTE,
    STRING,
    MAX_COLS
};

// fill all cells of the array with -1
void init_table(int _table[][MAX_COLS]);

// mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLS], int state);

// mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLS], int state);

// true if state is a success state
bool is_success(const int _table[][MAX_COLS], int state);

// mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLS], int from, int to, int state);

// mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLS], const int columns[],
                int state);

// mark this row and column
void mark_cell(int row, int _table[][MAX_COLS], int column, int state);

// mark table for STATE_DOUBLE
void mark_table_command(int _table[][MAX_COLS], int state);

// mark table for STATE_DOUBLE
void mark_table_select(int _table[][MAX_COLS], int state);

void print_table(const int _table[][MAX_COLS]);

}  // namespace sql

#endif  // SQL_CONSTS_H
