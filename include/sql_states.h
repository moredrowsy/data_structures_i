/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : sql_states
 * NAMESPACE   : sql
 * DESCRIPTION : This header defines the various functions to initialize the
 *          grammar adjacency table from state_machine.h for the SQLParser.
 ******************************************************************************/
#ifndef SQL_STATES_H
#define SQL_STATES_H

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
    SELECT_VALUE,
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

enum OP_STRINGS {
    STR_ASSIGN,
    STR_LESS,
    STR_LESS_THAN,
    STR_GREATER,
    STR_GREATER_THAN,
    STR_OP_OR,
    STR_OP_AND,
    STR_OPS_SIZE
};

enum PARSE_KEYS {
    KEY_COMMAND,
    KEY_FIELDS,
    KEY_R_FIELDS,
    KEY_WHERE,
    KEY_TABLE,
    KEY_VALUES,
    MAX_KEYS
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

#endif  // SQL_STATES_H
