#include "../include/state_machine.h"  // state_machine declarations

namespace state_machine {

/*******************************************************************************
 * DESCRIPTION:
 *  Initialize the entire table with -1.
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLUMNS]: an integer array
 *
 * POST-CONDITIONS:
 *  All cells' value are -1.
 *
 * RETURN:
 *  none
 ******************************************************************************/
void init_table(int _table[][MAX_COLUMNS]) {
    // fill array with -1
    for(int row = 0; row < MAX_ROWS; ++row) {
        for(int col = 0; col < MAX_COLUMNS; ++col) {
            _table[row][col] = -1;
        }
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the cell in row 'state' at column 0 with 1 (as true)
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLUMNS]: integer array
 *  int state                : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Target cell's value is 1
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_success(int _table[][MAX_COLUMNS], int state) {
    // assert state is not greater or equal to MAX ROWS
    assert(state < MAX_ROWS);

    // assign [row as state][column 0] to 1 as success state
    _table[state][0] = 1;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the cell in row 'state' at column 0 with 0 (as false)
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLUMNS]: integer array
 *  int state                : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Target cell's value is 0
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_fail(int _table[][MAX_COLUMNS], int state) {
    // assert state is not greater or equal to MAX ROWS
    assert(state < MAX_ROWS);

    // assign [row as state][column 0] to 0 as fail
    _table[state][0] = 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return a boolean value of cell in row 'state' and column 0.
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLUMNS]: integer array
 *  int state                : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
bool is_success(const int _table[][MAX_COLUMNS], int state) {
    return _table[state][0];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cell in a row with column range with value of 'state'.
 *
 * PRE-CONDITIONS:
 *  int row                  : 0 to MAX_ROWS - 1
 *  int _table[][MAX_COLUMNS]: integer array
 *  int from                 : 0 to MAX_COLUMNS - 1
 *  int to                   : 0 to MAX_COLUMNS - 1
 *  int state                : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Cells are marked with state
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to,
                int state) {
    for(int col = from; col <= to; ++col) {
        _table[row][col] = state;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cell in a row with char array with value of 'state'.
 *
 * PRE-CONDITIONS:
 *  int row                  : 0 to MAX_ROWS - 1
 *  int _table[][MAX_COLUMNS]: integer array
 *  const char columns[]     : 0 to MAX_COLUMNS - 1
 *  int state                : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Cells are marked with state
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[],
                int state) {
    for(int i = 0; columns[i] != '\0'; ++i) {
        _table[row][columns[i]] = state;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cell in a row and column with value of 'state'.
 *
 * PRE-CONDITIONS:
 *  int row                  : 0 to MAX_ROWS - 1
 *  int _table[][MAX_COLUMNS]: integer array
 *  int column               : 0 to MAX_COLUMNS - 1
 *  int state                : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Cell is marked with state
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state) {
    _table[row][column] = state;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cells with fail/success states and the adjacency values
 *  for a generic two states rules for given values in char array.
 *
 * ILLUSTRATION:
 *  MARK SUCCESS/FAILURE
 *  state [+0] ---> fail
 *  state [+1] ---> success
 *
 *  MARK CELLS
 *  state [+0] --- VALUES --> [+1]
 *  state [+1] --- VALUES --> [+1] ---> loop at state +1 for VALUES
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLUMNS]: integer array
 *  int start_state          : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Cells are marked with state
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_table_generic(int _table[][MAX_COLUMNS], int start_state,
                        const char columns[]) {
    // MARK SUCCESS/FAILURE
    // state [+0] ---> fail
    // state [+1] ---> success
    mark_fail(_table, start_state + 0);
    mark_success(_table, start_state + 1);

    // MARK CELLS
    // state [+0] --- VALUES --> [+1]
    // state [+1] --- VALUES --> [+1]
    mark_cells(start_state + 0, _table, columns, start_state + 1);
    mark_cells(start_state + 1, _table, columns, start_state + 1);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Mark the table's cells with fail/success states and the adjacency values
 *  for searching for DOUBLE token: formatted and unformatted numbers.
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLUMNS]: integer array
 *  int start_state          : 0 to MAX_ROWS - 1
 *
 * POST-CONDITIONS:
 *  Cells are marked with state
 *
 * RETURN:
 *  none
 ******************************************************************************/
void mark_table_double(int _table[][MAX_COLUMNS], int start_state) {
    // MARK SUCCESS/FAILURE
    // state [+0] ---> fail
    // state [+1] ---> success
    // state [+2] ---> success
    // state [+3] ---> success
    // state [+4] ---> fail
    // state [+5] ---> fail
    // state [+6] ---> fail
    // state [+7] ---> success
    // state [+8] ---> fail
    // state [+9] ---> success
    mark_fail(_table, STATE_DOUBLE + 0);
    mark_success(_table, STATE_DOUBLE + 1);
    mark_success(_table, STATE_DOUBLE + 2);
    mark_success(_table, STATE_DOUBLE + 3);
    mark_fail(_table, STATE_DOUBLE + 4);
    mark_fail(_table, STATE_DOUBLE + 5);
    mark_fail(_table, STATE_DOUBLE + 6);
    mark_success(_table, STATE_DOUBLE + 7);
    mark_fail(_table, STATE_DOUBLE + 8);
    mark_success(_table, STATE_DOUBLE + 9);

    // MARK CELLS
    // state [+0] --- DIGIT ---> [+1]
    // state [+0] --- '.' -----> [+8]
    // state [+1] --- DIGIT ---> [+2]
    // state [+1] --- ',' -----> [+4]
    // state [+1] --- '.' -----> [+8]
    // state [+2] --- DIGIT ---> [+3]
    // state [+2] --- ',' -----> [+4]
    // state [+2] --- '.' -----> [+8]
    // state [+3] --- DIGIT ---> [+3]
    // state [+3] --- ',' -----> [+4]
    // state [+3] --- '.' -----> [+8]
    // state [+4] --- DIGIT ---> [+5]
    // state [+5] --- DIGIT ---> [+6]
    // state [+6] --- DIGIT ---> [+7]
    // state [+7] --- ',' -----> [+4]
    // state [+7] --- '.' -----> [+8]
    // state [+8] --- DIGIT ---> [+9]
    // state [+9] --- DIGIT ---> [+9]
    mark_cells(STATE_DOUBLE + 0, _table, DIGIT, STATE_DOUBLE + 1);
    mark_cells(STATE_DOUBLE + 0, _table, '.', '.', STATE_DOUBLE + 8);
    mark_cells(STATE_DOUBLE + 1, _table, DIGIT, STATE_DOUBLE + 2);
    mark_cells(STATE_DOUBLE + 1, _table, ',', ',', STATE_DOUBLE + 4);
    mark_cells(STATE_DOUBLE + 1, _table, '.', '.', STATE_DOUBLE + 8);
    mark_cells(STATE_DOUBLE + 2, _table, DIGIT, STATE_DOUBLE + 3);
    mark_cells(STATE_DOUBLE + 2, _table, ',', ',', STATE_DOUBLE + 4);
    mark_cells(STATE_DOUBLE + 2, _table, '.', '.', STATE_DOUBLE + 8);
    mark_cells(STATE_DOUBLE + 3, _table, DIGIT, STATE_DOUBLE + 3);
    mark_cells(STATE_DOUBLE + 3, _table, ',', ',', STATE_DOUBLE + 4);
    mark_cells(STATE_DOUBLE + 3, _table, '.', '.', STATE_DOUBLE + 8);
    mark_cells(STATE_DOUBLE + 4, _table, DIGIT, STATE_DOUBLE + 5);
    mark_cells(STATE_DOUBLE + 5, _table, DIGIT, STATE_DOUBLE + 6);
    mark_cells(STATE_DOUBLE + 6, _table, DIGIT, STATE_DOUBLE + 7);
    mark_cells(STATE_DOUBLE + 7, _table, ',', ',', STATE_DOUBLE + 4);
    mark_cells(STATE_DOUBLE + 7, _table, '.', '.', STATE_DOUBLE + 8);
    mark_cells(STATE_DOUBLE + 8, _table, DIGIT, STATE_DOUBLE + 9);
    mark_cells(STATE_DOUBLE + 9, _table, DIGIT, STATE_DOUBLE + 9);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints the table to console.
 *
 * PRE-CONDITIONS:
 *  int _table[][MAX_COLUMNS]: integer array
 *
 * POST-CONDITIONS:
 *  Output of table values.
 *
 * RETURN:
 *  none
 ******************************************************************************/
void print_table(const int _table[][MAX_COLUMNS]) {
    int cols_per_row = 11, count = 1, value_len;

    while(count < MAX_COLUMNS) {
        // print header
        std::cout << " S ";
        std::cout << "|  " << 0 << "  ";
        for(int col = count; col < count + cols_per_row; ++col) {
            if(col < MAX_COLUMNS && (col < 32 || col > 126)) {
                value_len = std::to_string(col).length();
                if(value_len == 1) {
                    std::cout << "|  " << col << "  ";
                } else if(value_len == 2) {
                    std::cout << "|  " << col << " ";
                } else if(value_len == 3) {
                    std::cout << "| " << col << " ";
                }
            } else if(col < MAX_COLUMNS && col > 31 && col < 127) {
                std::cout << "| '" << static_cast<char>(col) << "' ";
            }
        }
        std::cout << "|" << std::endl;

        // print bar
        std::cout << "--- -----";
        for(int col = count; col < count + cols_per_row; ++col) {
            if(col < MAX_COLUMNS) {
                std::cout << " -----";
            }
        }
        std::cout << std::endl;

        // print values in array
        for(int row = 0; row < MAX_ROWS; ++row) {
            // print row number
            std::cout << std::setw(2) << std::right << row << " ";

            // print column 0
            value_len = std::to_string(_table[row][0]).length();
            if(value_len == 1) {
                std::cout << "|  " << _table[row][0] << "  ";
            } else if(value_len == 2) {
                std::cout << "|  " << _table[row][0] << " ";
            } else if(value_len == 3) {
                std::cout << "| " << _table[row][0] << " ";
            }

            for(int col = count; col < count + cols_per_row; ++col) {
                if(col < MAX_COLUMNS) {
                    value_len = std::to_string(_table[row][col]).length();
                    if(value_len == 1) {
                        std::cout << "|  " << _table[row][col] << "  ";
                    } else if(value_len == 2) {
                        std::cout << "|  " << _table[row][col] << " ";
                    } else if(value_len == 3) {
                        std::cout << "| " << _table[row][col] << " ";
                    }
                }
            }
            std::cout << "|" << std::endl;
        }
        std::cout << std::endl;

        count += cols_per_row;  // udpate count
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints the given cstring with the caret character pointing to the position
 *  of the string.
 *
 * PRE-CONDITIONS:
 *  const char s[]: cstring
 *  int _pos      : position (0 to len - 1)
 *
 * POST-CONDITIONS:
 *  Output of table values.
 *
 * RETURN:
 *  none
 ******************************************************************************/
// _posstarts from indexing from 1, not 0?
void show_string(const char s[], int _pos) {
    // assert positive position
    assert(_pos > -1);

    // print the string and carat as pointing to the position in string
    std::cout << s << std::endl;
    int index = 0;
    while(s[index] != '\0') {
        if(index == _pos) {
            std::cout << '^';
        } else {
            std::cout << ' ';
        }
        ++index;
    }

    // print carat behind string when pos is pointing at NUL
    if(s[_pos] == '\0') {
        std::cout << '^';
    }
    std::cout << std::endl;

    // assert that pos is less than index
    assert(_pos <= index);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Gets a valid token from input string, starting position of string, and
 *  the state to start looking for in the adjacency table. If valid
 *  token is found, it returns success boolean with position after the valid
 *  token. If get token fails to find a valid token, returns fail boolean and
 *  original position.
 *
 * PSEUDO CODE:
 *  While loop until char is non-ascii, until end of cstring, until state -1.
 *  -> Peek at next char, next state, and next success.
 *  -> If current state is success and next char is '\0', or next state is -1
 *     or next peek success fails, then set success = true and set success pos
 *     to current pos.
 *  -> Increase pos by 1
 *  When while loop is done or exits
 *  -> If success state is true, produce string token from original pos to
 *     successful pos. and update position to after token.
 *  -> Else, return original position.
 *  Return success boolean.
 *
 * ILLUSTRATION:
 *  String with pos = 0 with S = success state, F = fail state. Ignore pipe!
 *      |888,888,88|
 *       FFSFFFSFFFF
 *
 *  @ Partial success/fail, recorded success pos at 2:
 *      |888,888,88|
 *         ^^ -> 8 is @ success state, ',' is @ fail state
 *         SF -> success & !fail = success -> record pos
 *
 *  @ Partial success/fail, recorded success pos at 6:
 *      |888,888,88|
 *             ^^ -> 8 is @ success state, ',' is @ fail state
 *             SF -> success & !fail = success -> record pos
 *
 *  @ Full fail, record is not activated.
 *      |888,888,88|
 *                ^^ -> 8 is @ fail state, '\0' is @ fail state
 *                FF -> fail & !fail = fail -> not recorded
 *
 *  @ Record string from original pos to success pos 6:
 *      |888,888,88|
 *       ^-----^ -> gives |888,888| and next pos at 7
 *
 * PRE-CONDITIONS:
 *  const int _table[][MAX_COLUMNS]: integer array
 *  const char input[]             : input string to process
 *  int &_pos                      : position of string by reference
 *  int state                      : starting state (row) in adjacency table
 *  string &token                  : valid token if found
 *
 * POST-CONDITIONS:
 *  Success/fail process
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
bool get_token(const int _table[][MAX_COLUMNS], const char input[], int &_pos,
               int state, std::string &token) {
    bool peek_success = false,  // success state at peeking at next state
        success = false;        // success state
    char peek_char = '\0';      // peek at next character
    int success_pos = -1,       // last successful position
        original_pos = _pos,    // store original position
        peek_state = -1;        // peek at next state

    // loop until state and char (ascii) is not -1 and until end of string
    while(input[_pos] > -1 && _table[state][input[_pos]] != -1 &&
          input[_pos] != '\0') {
        state = _table[state][input[_pos]];     // current state for char
        peek_char = input[_pos + 1];            // peek next char
        peek_state = _table[state][peek_char];  // peek next state
        peek_success = _table[peek_state][0];   // peek next success state

        // udpate new token when peeking at next success is fail or when NUL
        if(is_success(_table, state) &&
           (peek_char == '\0' || peek_state == -1 || !peek_success)) {
            success_pos = _pos;
            success = true;
        }

        ++_pos;
    }

    if(success) {
        // clear and create new token when success pos (default = -1)
        token.clear();
        for(int i = original_pos; i < success_pos + 1; ++i) {
            token += input[i];
        }

        _pos = success_pos + 1;  // return position after token
    } else {
        _pos = original_pos;
    }

    return success;
}

}  // namespace state_machine
