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
 *  Initialize array of keys.
 *
 * PRE-CONDITIONS:
 *  std::string *_keys
 *  array size == MAX_KEYS
 *
 * POST-CONDITIONS:
 *  All cells' value are -1.
 *
 * RETURN:
 *  none
 ******************************************************************************/
void init_keys(std::string* _keys) {
    _keys[COMMAND] = "COMMAND";
    _keys[FIELDS] = "FIELDS";
    _keys[TABLE] = "TABLE";
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

void mark_table_command(int _table[][MAX_COLS], int state) {
    // MARK SUCCESS/FAILURE
    mark_fail(_table, state);

    // MARK GO TO STATE
    mark_cell(state, _table, SELECT, STATE_SELECT);
}

void mark_table_select(int _table[][MAX_COLS], int state) {
    // MARK SUCCESS/FAILURE
    // state [+0] ---> fail
    // state [+1] ---> fail
    // state [+2] ---> fail
    // state [+3] ---> fail
    // state [+4] ---> success
    // state [+5] ---> fail
    // state [+6] ---> fail
    mark_fail(_table, SELECT_START);
    mark_fail(_table, SELECT_ASTERISK);
    mark_fail(_table, SELECT_FROM);
    mark_success(_table, SELECT_END);
    mark_fail(_table, SELECT_STRING);
    mark_fail(_table, SELECT_COMMA);

    // MARK CELLS
    // state [0] ---- SELECT ---> [+0] <-- COMMAND STATE
    // state [+0] --- ASTERISK -> [+2]
    // state [+1] --- STRING ---> [+5]
    // state [+2] --- FROM -----> [+3]
    // state [+3] --- STRING ---> [+4]
    // state [+5] --- FROM -----> [+3]
    // state [+5] --- COMMA ----> [+6]
    // state [+6] --- STRING ---> [+5]
    mark_cell(state + 0, _table, ASTERISK, SELECT_ASTERISK);
    mark_cell(state + 0, _table, STRING, SELECT_STRING);
    mark_cell(state + 1, _table, FROM, SELECT_FROM);
    mark_cell(state + 2, _table, STRING, SELECT_END);
    mark_cell(state + 4, _table, FROM, SELECT_FROM);
    mark_cell(state + 4, _table, COMMA, SELECT_COMMA);
    mark_cell(state + 5, _table, STRING, SELECT_STRING);
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
