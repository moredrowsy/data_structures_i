/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : STOKENIZER
 * DESCRIPTION : This header declares class STokenizer. This STokenizer will
 *      receive an input/buffer string (via constructor or set_string) and
 *      returns a valid token (via the insertion operator) from its starting
 *      position by its internal state machine, which follows the rules set in
 *      the adjacency matrix.
 ******************************************************************************/
#ifndef STOKENIZER_H
#define STOKENIZER_H

#include <iostream>
#include "state_machine.h"
#include "token.h"

namespace STokenizer {

const int MAX_BUFFER = 999;

class STokenizer {
public:
    // CONSTRUCTORS
    STokenizer();
    STokenizer(char str[]);  // cstring param assertion < MAX_BUFFER

    // BIG THREE

    // ACCESSORS
    bool done() const;  // true: there are no more tokens
    bool more() const;  // true: there are more tokens

    // MUTATORS
    // set a new string as the input string
    void set_string(char str[]);

    // FRIENDS
    // extract one token (very similar to the way cin >> works)
    friend STokenizer& operator>>(STokenizer& s, Token::Token& t);

private:
    // create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][state_machine::MAX_COLUMNS]);

    // extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int start_state, std::string& token);

    char _buffer[MAX_BUFFER];  // input string
    int _pos;                  // current position in the string
    static int _table[state_machine::MAX_ROWS][state_machine::MAX_COLUMNS];
};

}  // namespace STokenizer

#endif  // STOKENIZER_H
