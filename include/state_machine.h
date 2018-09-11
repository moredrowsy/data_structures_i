/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : STATE_MACHINES
 * DESCRIPTION :
 ******************************************************************************/
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

namespace state_machine {

// GLOBAL CONSTANTS
const int MAX_COLUMNS = 256, MAX_ROWS = 4;

// fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]);

// mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state);

// mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state);

// true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state);

// mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to,
                int state);

// mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[],
                int state);

// mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state);

// this can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]);

// show string s and mark this position on the string:
// hello world   pos: 7
//      ^
void show_string(char s[], int _pos);

}  // namespace state_machine

#endif  // STATE_MACHINE_H