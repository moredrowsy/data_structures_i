/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : sql_tokenizer
 * DESCRIPTION : This header declares class SQLTokenizer. The SQLTokenizer will
 *      receive an input/buffer string (via constructor or set_string) and
 *      returns a valid token (via the insertion operator) from its starting
 *      position by its internal state machine, which follows the rules set in
 *      the adjacency matrix. SQLTokenizer also maintains position current
 *      position of the input string when extracting token, from class Token.
 ******************************************************************************/
#ifndef STOKENIZER_H
#define STOKENIZER_H

#include <cassert>          // assertions
#include <iostream>         // stream objects
#include <string>           // string objects
#include "state_machine.h"  // state_machine functions
#include "token.h"          // Token class

namespace sql {

enum { MAX_BUFFER = 1000 };

class SQLTokenizer {
public:
    // CONSTRUCTORS
    SQLTokenizer(std::size_t max_buf = MAX_BUFFER);
    SQLTokenizer(char str[], std::size_t max_buf = MAX_BUFFER);
    SQLTokenizer(const char str[], std::size_t max_buf = MAX_BUFFER);

    ~SQLTokenizer();

    // ACCESSORS
    bool done() const;               // true: there are no more tokens
    bool more() const;               // true: there are more tokens
    explicit operator bool() const;  // boolean conversion for extractor

    // MUTATORS
    void set_string(char str[]);        // set a new string as the input string
    void set_string(const char str[]);  // set a new string as the input string

    // FRIENDS
    friend SQLTokenizer& operator>>(SQLTokenizer& s, token::Token& t);

private:
    static int _table[state_machine::MAX_ROWS][state_machine::MAX_COLUMNS];
    static bool _need_init;  // check if _table is initialized

    char* _buffer;         // input string
    std::size_t _max_buf;  // max buffer size
    int _buffer_size;      // input string size
    int _pos;              // current position in the string

    void make_table(int _table[][state_machine::MAX_COLUMNS]);
    bool get_token(int start_state, std::string& token);
};

}  // namespace sql

#endif  // STOKENIZER_H
