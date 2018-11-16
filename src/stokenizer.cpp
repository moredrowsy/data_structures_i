#include "../include/stokenizer.h"  // stokenizer declarations

namespace stokenizer {

// STATIC VARIABLES
int STokenizer::_table[state_machine::MAX_ROWS][state_machine::MAX_COLUMNS];
bool STokenizer::_made_table = false;

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor initializes _buffer and _pos  to 0 and calls make_table
 *  to initialize all values in table to -1.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  initialized member variables
 *
 * RETURN:
 *  none
 ******************************************************************************/
STokenizer::STokenizer() : _buffer(), _buffer_size(0), _pos(0) {
    _buffer[0] = '\0';
    if(!_made_table) make_table(_table);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor initializes calls set_string to copy cstring into
 *  _buffer, set _buffer_size to cstring len, reset _pos to 0 and calls
 *  make_table to initialize all values in table to -1.
 *
 * PRE-CONDITIONS:
 *  char str[]: cstring for input buffer
 *
 * POST-CONDITIONS:
 *  initialized member variables via set_string() and make_table()
 *
 * RETURN:
 *  none
 ******************************************************************************/
STokenizer::STokenizer(char str[]) : _buffer(), _buffer_size(0), _pos(0) {
    set_string(str);
    if(!_made_table) make_table(_table);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor initializes calls set_string to copy const cstring into
 *  _buffer, set _buffer_size to cstring len, reset _pos to 0 and calls
 *  make_table to initialize all values in table to -1.
 *
 * PRE-CONDITIONS:
 *  const char str[]: cstring for input buffer
 *
 * POST-CONDITIONS:
 *  initialized member variables via set_string() and make_table()
 *
 * RETURN:
 *  none
 ******************************************************************************/
STokenizer::STokenizer(const char str[]) : _buffer(), _buffer_size(0), _pos(0) {
    set_string(str);
    if(!_made_table) make_table(_table);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if there no tokens left in buffer cstring from current _pos.
 *  Condition: a) if cstring's len is 0, then true.
 *             b) if cstring's len > 0, evaluate prev pos is NUL terminate
 *                to ensure done() only returns true once past NUL position.
 *
 * PRE-CONDITIONS:
 *  char _buffer[MAX_BUFFER]: cstring
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
bool STokenizer::done() const {
    return _buffer_size > 0 ? _pos > _buffer_size : true;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if there more tokens left in buffer cstring from current _pos.
 *  Condition: a) if cstring's len is 0, then false.
 *             b) if cstring's len > 0, evaluate prev pos is not NUL terminate
 *                to ensure more() only returns false once past NUL position.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
bool STokenizer::more() const {
    return _buffer_size > 0 ? _pos <= _buffer_size : false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Explicit operator bool() overloading, such as in the case of using
 *  extraction operators. Ex: while(stokenizer >> token) statements.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
STokenizer::operator bool() const { return more(); }

/*******************************************************************************
 * DESCRIPTION:
 *  Assign cstring param to buffer and reset _pos to 0.
 *
 * PRE-CONDITIONS:
 *  char str[]: cstring
 *
 * POST-CONDITIONS:
 *  char _buffer[]: assigned to str[]
 *  _buffer_size  : assigned to cstring size
 *  int _pos      : assigned to 0
 *
 * RETURN:
 *  none
 ******************************************************************************/
void STokenizer::set_string(char str[]) {
    // copy string into buffer
    int index = 0;
    while(str[index] != '\0') {
        assert(index < MAX_BUFFER - 1);
        _buffer[index] = str[index];
        ++index;
    }

    _buffer[index] = '\0';  // NUL terminate cstring
    _buffer_size = index;   // set buffer size
    _pos = 0;               // reset cstring pos
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assign const cstring param to buffer and reset _pos to 0.
 *
 * PRE-CONDITIONS:
 *  const char str[]: cstring
 *
 * POST-CONDITIONS:
 *  char _buffer[]: assigned to str[]
 *  _buffer_size  : assigned to cstring size
 *  int _pos      : assigned to 0
 *
 * RETURN:
 *  none
 ******************************************************************************/
void STokenizer::set_string(const char str[]) {
    // copy string into buffer
    int index = 0;
    while(str[index] != '\0') {
        assert(index < MAX_BUFFER - 1);
        _buffer[index] = str[index];
        ++index;
    }

    _buffer[index] = '\0';  // NUL terminate cstring
    _buffer_size = index;   // set buffer size
    _pos = 0;               // reset cstring pos
}

/*******************************************************************************
 * DESCRIPTION:
 *  Initialize all values in table to -1.
 *
 * PRE-CONDITIONS:
 *  int _table[][state_machine::MAX_COLUMNS]: integer array
 *
 * POST-CONDITIONS:
 *  all cells in table initialized to -1
 *
 * RETURN:
 *  none
 ******************************************************************************/
void STokenizer::make_table(int _table[][state_machine::MAX_COLUMNS]) {
    using namespace state_machine;

    init_table(_table);  // initialize table with -1

    // mark table with adjacency values
    mark_table_double(_table, STATE_DOUBLE);
    mark_table_generic(_table, STATE_SPACE, SPACE);
    mark_table_generic(_table, STATE_ALPHA, ALPHA);
    mark_table_generic(_table, STATE_PUNCT, PUNCT);

    _made_table = true;  // disable further make_table() calls in CTOR
}

/*******************************************************************************
 * DESCRIPTION:
 *  Calls get_token version from state_machine with params and return success
 *  state of get_token.
 *
 * PRE-CONDITIONS:
 *  int start_state: 0 to MAX_ROWS - 1
 *  string& token  : token string via reference
 *
 * POST-CONDITIONS:
 *  int _pos    : old pos if fail, else new pos after token changed, refernce
 *  string token: string unchanged if fail, else change to valid token
 *
 * RETURN:
 *  boolean when state_machine's get_token success/fail
 ******************************************************************************/
bool STokenizer::get_token(int start_state, std::string& token) {
    return state_machine::get_token(_table, _buffer, _pos, start_state, token);
}

STokenizer& operator>>(STokenizer& s, token::Token& t) {
    std::string token;

    // process tokens one state at a time
    if(s._pos > s._buffer_size)  // bound check if call w/o more() or done()
        t = token::Token();
    else if(s.get_token(state_machine::STATE_DOUBLE, token))
        t = token::Token(token, state_machine::STATE_DOUBLE);
    else if(s.get_token(state_machine::STATE_ALPHA, token))
        t = token::Token(token, state_machine::STATE_ALPHA);
    else if(s.get_token(state_machine::STATE_SPACE, token))
        t = token::Token(token, state_machine::STATE_SPACE);
    else if(s.get_token(state_machine::STATE_PUNCT, token))
        t = token::Token(token, state_machine::STATE_PUNCT);
    else {
        if(s._pos == s._buffer_size)  // create empty token on NUL char
            t = token::Token();
        else  // create token for UNKNOWN char
            t = token::Token(std::string(1, s._buffer[s._pos]));

        ++s._pos;  // when fail to get token, go to next position
    }

    return s;
}

}  // namespace stokenizer
