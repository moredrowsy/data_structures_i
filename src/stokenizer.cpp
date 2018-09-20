#include "../include/stokenizer.h"  // stokenizer declarations

namespace stokenizer {

// STATIC VARIABLES
int STokenizer::_table[state_machine::MAX_ROWS][state_machine::MAX_COLUMNS];

/*******************************************************************************
 * DESCRIPTION:
 *  Returns an integer corresponding to the string type, such as the
 *  state_machine's state constants.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  int of string type (or state constants in state_machine)
 ******************************************************************************/
int Token::type() const { return _type; }

/*******************************************************************************
 * DESCRIPTION:
 *  Returns a readable string corresponding to the string type, such as the
 *  state_machine's state constants, for human interpretation.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  string type for human readable interpretation
 ******************************************************************************/
std::string Token::type_string() const {
    std::string type_string;

    switch(_type) {
        case state_machine::STATE_UNKNOWN:
            type_string = "UNKNOWN";
            break;
        case state_machine::STATE_DOUBLE:
            type_string = "DOUBLE";
            break;
        case state_machine::STATE_SPACE:
            type_string = "SPACE";
            break;
        case state_machine::STATE_ALPHA:
            type_string = "ALPHA";
            break;
        case state_machine::STATE_PUNCT:
            type_string = "PUNCT";
            break;
        default:
            type_string = "ERROR";
    }

    return type_string;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the string value, such as the input buffer string from STokenizer.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  string
 ******************************************************************************/
std::string Token::token_str() const { return _token; }

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts a formatted Token's string value to the outstream.
 *
 * PRE-CONDITIONS:
 *  ostream& outs : out stream
 *  const Token& t: Token object with string value
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  ostream by reference
 ******************************************************************************/
std::ostream& operator<<(std::ostream& outs, const Token& t) {
    return outs << "|" + t.token_str() + "|";
}

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
STokenizer::STokenizer() {
    _buffer[0] = '\0';
    _pos = 0;
    make_table(_table);  // initialize table with adjacency rules
}

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor initializes _buffer to cstring and _pos and calls
 *  make_table to initialize all values in table to -1.
 *
 * PRE-CONDITIONS:
 *  char str[]: cstring for input buffer
 *
 * POST-CONDITIONS:
 *  initialized member variables
 *
 * RETURN:
 *  none
 ******************************************************************************/
STokenizer::STokenizer(char str[]) {
    // copy string into buffer
    int index = 0;
    while(str[index] != '\0') {
        assert(index < MAX_BUFFER - 1);  // assert index < MAX_BUFFER-1 in loop
        _buffer[index] = str[index];
        ++index;
    }
    _buffer[index] = '\0';  // NUL terminate cstring

    _pos = 0;
    make_table(_table);  // initialize table with adjacency rules
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if there no tokens left in buffer cstring from current _pos.
 *  Condition check: a) if length is zero and terminate, then true
 *                   b) if length is greater than zero, next pos is past last
 *                      pos is NUL, then true
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
bool STokenizer::done() const {
    return _buffer[0] == '\0' ? true : _buffer[_pos - 1] == '\0';
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if there more tokens left in buffer cstring from current _pos.
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
    return _buffer[0] == '\0' ? false : _buffer[_pos - 1] != '\0';
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
 *  int _pos      : assigned to 0
 *
 * RETURN:
 *  none
 ******************************************************************************/
void STokenizer::set_string(char str[]) {
    // copy string into buffer
    int index = 0;
    while(str[index] != '\0') {
        assert(index < MAX_BUFFER - 1);  // assert index < MAX_BUFFER-1 in loop
        _buffer[index] = str[index];
        ++index;
    }
    _buffer[index] = '\0';  // NUL terminate cstring
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

    // initialize table with -1
    init_table(_table);

    // mark table for STATE_DOUBLE
    mark_table_double(_table, STATE_DOUBLE);

    // mark table for STATE_SPACE
    mark_table_generic(_table, STATE_SPACE, SPACE);

    // mark table for STATE_ALPHA
    mark_table_generic(_table, STATE_ALPHA, ALPHA);

    // mark table for STATE_PUNCT
    mark_table_generic(_table, STATE_PUNCT, PUNCT);
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

STokenizer& operator>>(STokenizer& s, Token& t) {
    std::string token;

    // process tokens one state at a time until unknown token
    if(s.get_token(state_machine::STATE_DOUBLE, token)) {
        t = Token(token, state_machine::STATE_DOUBLE);
    } else if(s.get_token(state_machine::STATE_ALPHA, token)) {
        t = Token(token, state_machine::STATE_ALPHA);
    } else if(s.get_token(state_machine::STATE_SPACE, token)) {
        t = Token(token, state_machine::STATE_SPACE);
    } else if(s.get_token(state_machine::STATE_PUNCT, token)) {
        t = Token(token, state_machine::STATE_PUNCT);
    } else {
        if(s._buffer[s._pos] == '\0')
            t = Token();
        else
            t = Token(std::string(1, s._buffer[s._pos]));

        ++s._pos;  // when fail to get token, go to next position
    }

    // std::cout << "stk extract called" << std::endl;

    return s;
}

}  // namespace stokenizer
