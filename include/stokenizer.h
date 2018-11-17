/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : stokenizer
 * DESCRIPTION : This header declares class STokenizer. The STokenizer will
 *      receive an input/buffer string (via constructor or set_string) and
 *      returns a valid token (via the insertion operator) from its starting
 *      position by its internal state machine, which follows the rules set in
 *      the adjacency matrix. STokenizer also maintains position current
 *      position of the input string when extracting token, from class Token.
 ******************************************************************************/
#ifndef STOKENIZER_H
#define STOKENIZER_H

#include <cassert>          // assertions
#include <iostream>         // stream objects
#include <string>           // string objects
#include "state_machine.h"  // state_machine functions
#include "token.h"          // Token class

namespace stokenizer {

enum { MAX_BUFFER = 1000 };

class STokenizer {
public:
    // CONSTRUCTORS
    STokenizer(std::size_t max_buf = MAX_BUFFER);
    STokenizer(char str[], std::size_t max_buf = MAX_BUFFER);
    STokenizer(const char str[], std::size_t max_buf = MAX_BUFFER);

    ~STokenizer();

    // ACCESSORS
    bool done() const;               // true: there are no more tokens
    bool more() const;               // true: there are more tokens
    explicit operator bool() const;  // boolean conversion for extractor

    // MUTATORS
    void set_string(char str[]);        // set a new string as the input string
    void set_string(const char str[]);  // set a new string as the input string

    // FRIENDS
    // extract one token (very similar to the way cin >> works)
    friend STokenizer& operator>>(STokenizer& s, token::Token& t);

private:
    // create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][state_machine::MAX_COLUMNS]);

    // extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int start_state, std::string& token);

    char* _buffer;         // input string
    std::size_t _max_buf;  // max buffer size
    int _buffer_size;      // input string size
    int _pos;              // current position in the string
    static int _table[state_machine::MAX_ROWS][state_machine::MAX_COLUMNS];
    static bool _made_table;  // check if _table is initialized
};

}  // namespace stokenizer

#endif  // STOKENIZER_H
