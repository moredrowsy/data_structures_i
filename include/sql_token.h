#ifndef SQL_TOKEN_H
#define SQL_TOKEN_H

#include <string>        // string
#include "sql_states.h"  // SQL constants
#include "token.h"       // Token class

namespace sql {

class SQLToken : public token::Token {
public:
    SQLToken(std::string str = "", int type = -1) : token::Token(str, type) {}

    std::string type_string() const {
        std::string type_string;

        switch(type()) {
            case ERROR:
                type_string = "ERROR";
                break;
            case SELECT:
                type_string = "SELECT";
                break;
            case FROM:
                type_string = "FROM";
                break;
            case COMMA:
                type_string = "COMMA";
                break;
            case ASTERISK:
                type_string = "ASTERISK";
                break;
            case QUOTE_S:
                type_string = "QUOTE_S";
                break;
            case QUOTE_D:
                type_string = "QUOTE_D";
                break;
            case STRING:
                type_string = "STRING";
                break;
            default:
                type_string = "UNKNOWN";
        }

        return type_string;
    }
};

}  // namespace sql

#endif  // SQL_TOKEN_H
