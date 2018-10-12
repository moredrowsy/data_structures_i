/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : stokenizer
 * DESCRIPTION : This header declares class Token and STokenizer. Token
 *      represents a string and a type associated with that string. It
 *      facilitates the STokenizer for interaction and reporting the type of
 *      tokens. The STokenizer will receive an input/buffer string (via
 *      constructor or set_string) and returns a valid token (via the insertion
 *      operator) from its starting position by its internal state machine,
 *      which follows the rules set in the adjacency matrix. STokenizer also
 *      maintains position current position of the input string when extracting
 *      token.
 ******************************************************************************/
#ifndef STOKENIZER_H
#define STOKENIZER_H

#include <cassert>          // assertions
#include <iostream>         // stream objects
#include <string>           // string objects
#include "state_machine.h"  // state_machine functions

namespace stokenizer {

// GLOBAL CONSTANT
const int MAX_BUFFER = 500;

class Token {
public:
    // CONSTRUCTORS
    Token(std::string str = "", int type = -1) : _token(str), _type(type) {}

    // ACCESSORS
    int type() const;                 // return type of token in integer
    std::string type_string() const;  // return type of token in string
    std::string token_str() const;    // return string of token

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const Token& t);

private:
    std::string _token;  // token string
    int _type;           // type of token
};

class STokenizer {
public:
    // CONSTRUCTORS
    STokenizer();
    STokenizer(char str[]);        // cstring param assertion < MAX_BUFFER
    STokenizer(const char str[]);  // cstring param assertion < MAX_BUFFER

    // ACCESSORS
    bool done() const;               // true: there are no more tokens
    bool more() const;               // true: there are more tokens
    explicit operator bool() const;  // boolean conversion for extractor

    // MUTATORS
    void set_string(char str[]);        // set a new string as the input string
    void set_string(const char str[]);  // set a new string as the input string

    // FRIENDS
    // extract one token (very similar to the way cin >> works)
    friend STokenizer& operator>>(STokenizer& s, Token& t);

private:
    // create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][state_machine::MAX_COLUMNS]);

    // extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int start_state, std::string& token);

    char _buffer[MAX_BUFFER];  // input string
    int _buffer_size;          // input string size
    int _pos;                  // current position in the string
    static int _table[state_machine::MAX_ROWS][state_machine::MAX_COLUMNS];
    static bool _made_table;  // check if _table is initialized
};

}  // namespace stokenizer

#endif  // STOKENIZER_H
