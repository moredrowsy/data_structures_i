#include "../include/token.h"
#include <iomanip>
#include <iostream>

namespace Token {

int Token::type() const { return _type; }

std::string Token::type_string() const {
    std::string type_string;

    switch(_type) {
        case 0:
            type_string = "NUMBER";
            break;
        case 1:
            type_string = "SPACE";
            break;
        case 2:
            type_string = "ALPHA";
            break;
        default:
            type_string = "UNKNOWN";
    }

    return type_string;
}

std::string Token::token_str() const { return _token; }

std::ostream& operator<<(std::ostream& outs, const Token& t) {
    outs << std::setw(10) << std::right << t.type_string() << std::setw(10)
         << "" << t.token_str();
}

}  // namespace Token
