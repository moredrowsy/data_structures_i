#include "../include/token.h"
#include <iomanip>
#include <iostream>
#include "../include/state_machine.h"

namespace Token {

int Token::type() const { return _type; }

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
        default:
            type_string = "ERROR";
    }

    return type_string;
}

std::string Token::token_str() const { return _token; }

std::ostream& operator<<(std::ostream& outs, const Token& t) {
    outs << "|" + t.token_str() + "|";

    return outs;
}

}  // namespace Token
