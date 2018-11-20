#ifndef SQL_CONSTS_H
#define SQL_CONSTS_H

#include <cassert>   //assert()
#include <iomanip>   // setw()
#include <iostream>  // stream objects
#include <string>    // string

namespace sql {

enum COMMANDS {
    CMD_START = 0,
    CMD_CREATE = 10,  // uses 4 rows
    CMD_INSERT = 20,  // uses 6 rows
    CMD_SELECT = 30,  // uses 11 rows
    CMD_SIZE = 45
};

enum CREATE_STATES {
    CREATE_START = CMD_CREATE,
    CREATE_TABLE_KEY,
    CREATE_TABLE,
    CREATE_FIELDS_KEY,
    CREATE_FIELDS,
    CREATE_COMMA
};

enum INSERT_STATES {
    INSERT_START = CMD_INSERT,
    INSERT_INTO,
    INSERT_TABLE,
    INSERT_VALUES,
    INSERT_VALUE,
    INSERT_COMMA,
};

enum SELECT_STATES {
    SELECT_START = CMD_SELECT,
    SELECT_ASTERISK,
    SELECT_FROM,
    SELECT_TABLE,
    SELECT_FIELDS,
    SELECT_COMMA,
    SELECT_WHERE,
    SELECT_R_FIELDS,
    SELECT_R_OPS,
    SELECT_VALUES,
    SELECT_L_OPS,
};

enum ROWS { MAX_ROWS = CMD_SIZE };

enum COLUMNS {
    ERROR = -1,
    SUCCESS,
    CREATE,
    INSERT,
    SELECT,
    TABLE,
    INTO,
    FROM,
    WHERE,
    FIELDS,
    VALUES,
    COMMA,
    ASTERISK,
    IDENT,
    VALUE,
    R_OPS,
    L_OPS,
    AND,
    OR,
    SPACE,
    MAX_COLS
};

enum RELATIONAL_OPS {
    ASSIGN,
    LESS,
    LESS_THAN,
    GREATER,
    GREATER_THAN,
    R_OPS_SIZE
};

enum PARSE_KEYS {
    COMMAND,
    FIELDS_KEY,
    R_FIELDS,
    R_OPS_KEY,
    L_OPS_KEY,
    TABLE_KEY,
    VALUE_KEY,
    MAX_KEYS
};

// fill keys with SQL keys
void init_keys(std::string* _keys);

// fill types with SQL type strings
void init_types(std::string* _types);

// fill types with SQL type strings
void init_r_ops(std::string* _r_ops);

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

// mark table for CMD_START to jump to other COMMANDS
void mark_table_command(int _table[][MAX_COLS]);

// mark table for CMD_CREATE
void mark_table_create(int _table[][MAX_COLS]);

// mark table for CMD_INSERT
void mark_table_insert(int _table[][MAX_COLS]);

// mark table for CMD_SELECT
void mark_table_select(int _table[][MAX_COLS]);

void print_table(const int _table[][MAX_COLS]);

}  // namespace sql

#endif  // SQL_CONSTS_H
