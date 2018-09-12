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
    Token() : _token(""), _type(-1) {}
    Token(std::string str, int type) : _token(str), _type(type) {}

    // ACCESSORS
    int type() const;
    std::string type_string() const;
    std::string token_str() const;

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const Token& t);

private:
    std::string _token;
    int _type;
};

}  // namespace Token

#endif  // TOKEN_H
