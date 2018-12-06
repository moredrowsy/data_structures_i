#include "../include/sql_states.h"

namespace sql {

/*******************************************************************************
 * DESCRIPTION:
 *  Initialize the entire table with -1.
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLS]: adjacency table
 *
 * POST-CONDITIONS:
 *  All cells' value are -1.
 *
 * RETURN:
 *  none
 ******************************************************************************/
void init_table(int _table[][MAX_COLS]) {
    for(int row = 0; row < MAX_ROWS; ++row)
        for(int col = 0; col < MAX_COLS; ++col) _table[row][col] = -1;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the cell in row 'state' at column 0 with 1 (as true)
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLS]: adjacency table
 *  int state            : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Target cell's value is 1
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_success(int _table[][MAX_COLS], int state) {
    assert(state < MAX_ROWS);
    _table[state][0] = 1;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the cell in row 'state' at column 0 with 0 (as false)
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLS]: adjacency table
 *  int state             : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Target cell's value is 0
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_fail(int _table[][MAX_COLS], int state) {
    assert(state < MAX_ROWS);
    _table[state][0] = 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return a boolean value of cell in row 'state' and column 0.
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLS]: adjacency table
 *  int state             : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
bool is_success(const int _table[][MAX_COLS], int state) {
    return _table[state][0];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cell in a row with column range with value of 'state'.
 *
 * PRE-CONDITIONS:
 *  int row               : 0 to MAX_ROWS - 1
 *  int _table[][MAX_COLS]: integer array
 *  int from              : 0 to MAX_COLS - 1
 *  int to                : 0 to MAX_COLS - 1
 *  int state             : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Cells are marked with state
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_cells(int row, int _table[][MAX_COLS], int from, int to, int state) {
    for(int col = from; col <= to; ++col) _table[row][col] = state;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cell in a row with char array with value of 'state'.
 *
 * PRE-CONDITIONS:
 *  int row               : 0 to MAX_ROWS - 1
 *  int _table[][MAX_COLS]: integer array
 *  const char columns[]  : 0 to MAX_COLS - 1
 *  int state             : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Cells are marked with state
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_cells(int row, int _table[][MAX_COLS], const int columns[],
                int state) {
    for(int i = 0; columns[i] != '\0'; ++i) _table[row][columns[i]] = state;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cell in a row and column with value of 'state'.
 *
 * PRE-CONDITIONS:
 *  int row               : 0 to MAX_ROWS - 1
 *  int _table[][MAX_COLS]: integer array
 *  int column            : 0 to MAX_COLS - 1
 *  int state             : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Cell is marked with state
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_cell(int row, int _table[][MAX_COLS], int column, int state) {
    _table[row][column] = state;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cells for STATE_COMMAND. This state is where it branches
 *  off to different command states.
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLS]: integer array
 *  int state             : STATE_COMMAND
 *
 * POST-CONDITIONS:
 *  Cells are marked with command states
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_table_command(int _table[][MAX_COLS]) {
    // MARK SUCCESS/FAILURE
    mark_fail(_table, CMD_START);

    // MARK GO TO STATE
    mark_cell(CMD_START, _table, CREATE, CMD_CREATE);
    mark_cell(CMD_START, _table, INSERT, CMD_INSERT);
    mark_cell(CMD_START, _table, SELECT, CMD_SELECT);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cells for CMD_CREATE. This is the pathway for the
 *  CREATE command.
 *
 * PRE-CONDITIONS:
 *  REQUIRE ROWS: 6
 *  int _table[][MAX_COLS]: integer array
 *  int state             : CMD_CREATE
 *
 * POST-CONDITIONS:
 *  Cells are marked with states
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_table_create(int _table[][MAX_COLS]) {
    // MARK SUCCESS/FAILURE
    // state [+0] ---> fail
    // state [+1] ---> fail
    // state [+2] ---> fail
    // state [+3] ---> fail
    // state [+4] ---> success
    // state [+5] ---> fail
    mark_fail(_table, CREATE_START);
    mark_fail(_table, CREATE_TABLE_KEY);
    mark_fail(_table, CREATE_TABLE);
    mark_fail(_table, CREATE_FIELDS_KEY);
    mark_success(_table, CREATE_FIELDS);
    mark_fail(_table, CREATE_COMMA);

    // MARK CELLS
    // state [0] ---- CREATE ---> [+0] <-- COMMAND STATE
    // state [+0] --- IDENT ----> [+1]
    // state [+1] --- IDENT ----> [+2]
    // state [+2] --- COMMA ----> [+3]
    // state [+3] --- IDENT ----> [+2]
    mark_cell(CREATE_START, _table, TABLE, CREATE_TABLE_KEY);
    mark_cell(CREATE_TABLE_KEY, _table, IDENT, CREATE_TABLE);
    mark_cell(CREATE_TABLE, _table, FIELDS, CREATE_FIELDS_KEY);
    mark_cell(CREATE_FIELDS_KEY, _table, IDENT, CREATE_FIELDS);
    mark_cell(CREATE_FIELDS, _table, COMMA, CREATE_COMMA);
    mark_cell(CREATE_COMMA, _table, IDENT, CREATE_FIELDS);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cells for CMD_INSERT. This is the pathway for the
 *  INSERT command.
 *
 * PRE-CONDITIONS:
 *  REQUIRE ROWS: 6
 *  int _table[][MAX_COLS]: integer array
 *  int state             : CMD_INSERT
 *
 * POST-CONDITIONS:
 *  Cells are marked with states
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_table_insert(int _table[][MAX_COLS]) {
    // MARK SUCCESS/FAILURE
    // state [+0] ---> fail
    // state [+1] ---> fail
    // state [+2] ---> fail
    // state [+3] ---> fail
    // state [+4] ---> success
    // state [+5] ---> fail
    mark_fail(_table, INSERT_START);
    mark_fail(_table, INSERT_INTO);
    mark_fail(_table, INSERT_TABLE);
    mark_fail(_table, INSERT_VALUES);
    mark_success(_table, INSERT_VALUE);
    mark_fail(_table, INSERT_COMMA);

    // MARK CELLS
    // state [0] ---- INSERT ---> [+0] <-- COMMAND STATE
    // state [+0] --- INTO -----> [+1]
    // state [+1] --- IDENT ----> [+2]
    // state [+2] --- VALUES ---> [+3]
    // state [+3] --- IDENT ----> [+4]
    // state [+3] --- VALUE ----> [+4]
    // state [+4] --- COMMA ----> [+5]
    // state [+5] --- IDENT ----> [+4]
    // state [+5] --- VALUE ----> [+4]
    mark_cell(INSERT_START, _table, INTO, INSERT_INTO);
    mark_cell(INSERT_INTO, _table, IDENT, INSERT_TABLE);
    mark_cell(INSERT_TABLE, _table, VALUES, INSERT_VALUES);
    mark_cell(INSERT_VALUES, _table, IDENT, INSERT_VALUE);
    mark_cell(INSERT_VALUES, _table, VALUE, INSERT_VALUE);
    mark_cell(INSERT_VALUE, _table, COMMA, INSERT_COMMA);
    mark_cell(INSERT_COMMA, _table, IDENT, INSERT_VALUE);
    mark_cell(INSERT_COMMA, _table, VALUE, INSERT_VALUE);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cells for CMD_SELECT. This is the pathway for the
 *  SELECT command.
 *
 * PRE-CONDITIONS:
 *  REQUIRE ROWS: 7
 *  int _table[][MAX_COLS]: integer array
 *  int state             : CMD_SELECT
 *
 * POST-CONDITIONS:
 *  Cells are marked with states
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_table_select(int _table[][MAX_COLS]) {
    // MARK SUCCESS/FAILURE
    // state [+0] ---> fail
    // state [+1] ---> fail
    // state [+2] ---> fail
    // state [+3] ---> success
    // state [+4] ---> fail
    // state [+5] ---> fail
    mark_fail(_table, SELECT_START);
    mark_fail(_table, SELECT_ASTERISK);
    mark_fail(_table, SELECT_FROM);
    mark_success(_table, SELECT_TABLE);
    mark_fail(_table, SELECT_FIELDS);
    mark_fail(_table, SELECT_COMMA);
    mark_fail(_table, SELECT_WHERE);
    mark_fail(_table, SELECT_R_FIELDS);
    mark_fail(_table, SELECT_R_OPS);
    mark_success(_table, SELECT_VALUE);
    mark_fail(_table, SELECT_L_OPS);

    // MARK CELLS
    // state [0] ---- SELECT ---> [+0] <-- COMMAND STATE
    // state [+0] --- ASTERISK -> [+1]
    // state [+1] --- IDENT ----> [+4]
    // state [+2] --- FROM -----> [+2]
    // state [+3] --- IDENT ----> [+3]
    // state [+4] --- FROM -----> [+4]
    // state [+4] --- COMMA ----> [+5]
    // state [+5] --- IDENT ----> [+4]
    mark_cell(SELECT_START, _table, ASTERISK, SELECT_ASTERISK);
    mark_cell(SELECT_START, _table, IDENT, SELECT_FIELDS);
    mark_cell(SELECT_ASTERISK, _table, FROM, SELECT_FROM);
    mark_cell(SELECT_FROM, _table, IDENT, SELECT_TABLE);
    mark_cell(SELECT_FIELDS, _table, FROM, SELECT_FROM);
    mark_cell(SELECT_FIELDS, _table, COMMA, SELECT_COMMA);
    mark_cell(SELECT_COMMA, _table, IDENT, SELECT_FIELDS);

    mark_cell(SELECT_TABLE, _table, WHERE, SELECT_WHERE);
    mark_cell(SELECT_WHERE, _table, IDENT, SELECT_R_FIELDS);
    mark_cell(SELECT_R_FIELDS, _table, R_OPS, SELECT_R_OPS);
    mark_cell(SELECT_R_OPS, _table, IDENT, SELECT_VALUE);
    mark_cell(SELECT_R_OPS, _table, VALUE, SELECT_VALUE);
    mark_cell(SELECT_VALUE, _table, L_OPS, SELECT_L_OPS);
    mark_cell(SELECT_L_OPS, _table, IDENT, SELECT_R_FIELDS);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints the table to console.
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLS]: adjacency table
 *
 * POST-CONDITIONS:
 *  Output of table values.
 *
 * RETURN:
 *  none
 ******************************************************************************/
void print_table(const int _table[][MAX_COLS]) {
    int cols_per_row = 11, count = 1, value_len;

    while(count < MAX_COLS) {
        // print header
        std::cout << " S ";
        std::cout << "|  " << 0 << "  ";
        for(int col = count; col < count + cols_per_row; ++col) {
            if(col < MAX_COLS && (col < 32 || col > 126)) {
                value_len = std::to_string(col).length();
                if(value_len == 1)
                    std::cout << "|  " << col << "  ";
                else if(value_len == 2)
                    std::cout << "|  " << col << " ";
                else if(value_len == 3)
                    std::cout << "| " << col << " ";

            } else if(col < MAX_COLS && col > 31 && col < 127)
                std::cout << "| '" << static_cast<char>(col) << "' ";
        }
        std::cout << "|" << std::endl;

        // print bar
        std::cout << "--- -----";
        for(int col = count; col < count + cols_per_row; ++col)
            if(col < MAX_COLS) std::cout << " -----";

        std::cout << std::endl;

        // print values in array
        for(int row = 0; row < MAX_ROWS; ++row) {
            // print row number
            std::cout << std::setw(2) << std::right << row << " ";

            // print column 0
            value_len = std::to_string(_table[row][0]).length();
            if(value_len == 1)
                std::cout << "|  " << _table[row][0] << "  ";
            else if(value_len == 2)
                std::cout << "|  " << _table[row][0] << " ";
            else if(value_len == 3)
                std::cout << "| " << _table[row][0] << " ";

            for(int col = count; col < count + cols_per_row; ++col) {
                if(col < MAX_COLS) {
                    value_len = std::to_string(_table[row][col]).length();
                    if(value_len == 1)
                        std::cout << "|  " << _table[row][col] << "  ";
                    else if(value_len == 2)
                        std::cout << "|  " << _table[row][col] << " ";
                    else if(value_len == 3)
                        std::cout << "| " << _table[row][col] << " ";
                }
            }
            std::cout << "|" << std::endl;
        }
        std::cout << std::endl;

        count += cols_per_row;  // udpate count
    }
}

}  // namespace sql
