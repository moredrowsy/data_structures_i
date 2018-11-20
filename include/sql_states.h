#ifndef SQL_CONSTS_H
#define SQL_CONSTS_H

#include <cassert>   //assert()
#include <iomanip>   // setw()
#include <iostream>  // stream objects
#include <string>    // string

namespace sql {

enum COMMANDS {
    CMD_START = 0,
    CMD_CREATE = 10,
    CMD_SELECT = 20,
    CMD_SIZE = 30
};

enum SELECT_STATES {
    SELECT_START = CMD_SELECT + 0,
    SELECT_ASTERISK = CMD_SELECT + 1,
    SELECT_FROM = CMD_SELECT + 2,
    SELECT_TABLE = CMD_SELECT + 3,
    SELECT_STRING = CMD_SELECT + 4,
    SELECT_COMMA = CMD_SELECT + 5
};

enum ROWS { MAX_ROWS = CMD_SIZE };

enum COLUMNS {
    ERROR = -1,
    SUCCESS,
    SELECT,
    FROM,
    COMMA,
    ASTERISK,
    IDENT,
    VALUE,
    SPACE,
    MAX_COLS
};

enum PARSE_KEYS { COMMAND, FIELDS, TABLE, MAX_KEYS };

// fill keys with SQL keys
void init_keys(std::string* _keys);

// fill types with SQL type strings
void init_types(std::string* _types);

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
void mark_table_command(int _table[][MAX_COLS]);

// mark table for STATE_DOUBLE
void mark_table_select(int _table[][MAX_COLS]);

void print_table(const int _table[][MAX_COLS]);

}  // namespace sql

#endif  // SQL_CONSTS_H