/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : TOKEN
 * DESCRIPTION : This header declares class Token. Token is a class where
 *      it represents a token as a string and stores a type associated with it.
 *      It facilitates the STokenizer class for interaction and reporting the
 *      type of tokens.
 ******************************************************************************/
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

namespace Token {

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

}  // namespace Token

#endif  // TOKEN_H
