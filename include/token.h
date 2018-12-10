/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : token
 * DESCRIPTION : This header declares class Token, which represents a string
 *      and a type associated with that string. It facilitates the STokenizer
 *      for interaction and reporting the type of tokens.
 ******************************************************************************/
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>         // stream objects
#include <string>           // string objects
#include "state_machine.h"  // state_machine functions

namespace token {

class Token {
public:
    // CONSTRUCTORS
    Token(std::string str = "", int type = -1)
        : _token(str), _type(type), _sub_type(_type) {}
    Token(std::string str, int type, int sub_type)
        : _token(str), _type(type), _sub_type(sub_type) {}

    // ACCESSORS
    int type() const;                         // return type in INT
    int sub_type() const;                     // return sub type in INT
    virtual std::string type_string() const;  // return type of token in string
    std::string string() const;               // return string of token

    // MUTATORS
    void set_type(int type);
    void set_sub_type(int sub_type);
    void set_string(std::string str);

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const Token& t);
    friend bool operator==(const Token& lhs, const Token& rhs);
    friend bool operator!=(const Token& lhs, const Token& rhs);
    friend Token& operator+=(Token& lhs, const Token& rhs);
    friend Token& operator+(Token& lhs, const Token& rhs);

protected:
    std::string _token;  // token string
    int _type;           // type of token
    int _sub_type;       // sub type
};

}  // namespace token

#endif  // TOKEN_H
